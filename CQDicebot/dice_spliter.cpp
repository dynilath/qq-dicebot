#include "stdafx.h"

#include "diceroller.h"
#include "dice_spliter.h"

std::regex regex_single_dice("(\\+|\\-)?((\\d*d\\d+((k|kl)\\d+)?)|(\\d+))");

#define PCAL_WITH_IS_DOUBLE_CHECK(_D_Mode,_I_Mode,_OPERATOR,_FACTOR) \
if (is_using_double) _D_Mode _OPERATOR _FACTOR;\
else _I_Mode _OPERATOR _FACTOR

#define SWITCH_IS_DOUBLE_MODE { d_dice_buffer_level_1 = i_dice_buffer_level_1;\
d_dice_summary = i_dice_summary;\
is_using_double = true;}

#define PCAL_TYPE_ADD 0
#define PCAL_TYPE_MUL 1
#define PCAL_TYPE_DVI 2
#define PCAL_TYPE_INIT -1

bool base_split_dice(const std::string & str_input, std::string & str_output, bool giving_detailed_message) {
	std::string str_input_copy(str_input);
	std::smatch matchList_single_dice;

	std::ostringstream ostrs_dice_stream(std::ostringstream::ate);
	ostrs_dice_stream << str_input;
	if (giving_detailed_message) ostrs_dice_stream << " = ";

	INT32 i_dice_summary = 0;
	double d_dice_summary = 0.0;

	bool is_this_line_output = true;

	int i_cal_level = 0;
	bool is_using_double = false;

	INT32 i_dice_buffer_level_1 = 0;
	double d_dice_buffer_level_1 = 0.0;

	INT32 i_dice_result = 0;

	int i_dice_cal_type = PCAL_TYPE_INIT;

	int i_max_unit_alert = 0;
	for (; i_max_unit_alert < MAX_DICE_UNIT_COUNT; i_max_unit_alert++) {


		if (!std::regex_search(str_input_copy, matchList_single_dice, regex_single_dice)) { break; }
		std::string str_single_dice = matchList_single_dice.str();
		dice_roller dr_single_dice(str_single_dice);
		if (dr_single_dice.status != dice_roller::ROLL_STATUS_FINISHED) { is_this_line_output = false; break; }
		i_dice_result = dr_single_dice.i_sum_result;


		if(giving_detailed_message) ostrs_dice_stream << *(dr_single_dice.pstr_detail_result);
		str_input_copy = matchList_single_dice.suffix().str();

		if (i_dice_cal_type == PCAL_TYPE_MUL) {
			if ((!is_using_double) && INT32_MAX / i_dice_result < i_dice_buffer_level_1)
				SWITCH_IS_DOUBLE_MODE;
			PCAL_WITH_IS_DOUBLE_CHECK(d_dice_buffer_level_1, i_dice_buffer_level_1, *=, i_dice_result);
		}
		else if (i_dice_cal_type == PCAL_TYPE_DVI) {
			if (i_dice_result == 0) {
				is_this_line_output = false;
				break;
			}
			if ((!is_using_double) && i_dice_buffer_level_1 % i_dice_result != 0)
				SWITCH_IS_DOUBLE_MODE;
			PCAL_WITH_IS_DOUBLE_CHECK(d_dice_buffer_level_1, i_dice_buffer_level_1, /=, i_dice_result);
		}

		if (str_input_copy.length() > 0 && (str_input_copy[0] == '*' || str_input_copy[0] == '/')) {
			if (giving_detailed_message) ostrs_dice_stream << ' ' << str_input_copy[0] << ' ';
			if (i_cal_level == 0) {
				PCAL_WITH_IS_DOUBLE_CHECK(d_dice_buffer_level_1, i_dice_buffer_level_1, =, i_dice_result);
				i_cal_level = 1;
			}
			if (str_input_copy[0] == '*') {
				i_dice_cal_type = PCAL_TYPE_MUL;
			}
			else if (str_input_copy[0] == '/') {
				i_dice_cal_type = PCAL_TYPE_DVI;
			}
		}
		else {

			if (i_cal_level > 0) {
				i_cal_level = 0;
				if (is_using_double) d_dice_summary += d_dice_buffer_level_1;
				else i_dice_summary += i_dice_buffer_level_1;
			}
			else PCAL_WITH_IS_DOUBLE_CHECK(d_dice_summary, i_dice_summary, +=, i_dice_result);
			if (str_input_copy.length() == 0) break;
			else i_dice_cal_type = PCAL_TYPE_ADD;
		}
	}

	if (i_max_unit_alert == MAX_DICE_UNIT_COUNT) is_this_line_output = false;

	if (is_this_line_output) {
		ostrs_dice_stream << " = ";
		if (is_using_double) {
			ostrs_dice_stream << d_dice_summary;
		}
		else {
			ostrs_dice_stream << i_dice_summary;
		}
		str_output.assign(ostrs_dice_stream.str());
	}
	return is_this_line_output;
}

bool binary_tree_split_dice(
	const std::string & str_input,
	std::string & str_roll_command,
	std::string & str_roll_detail,
	std::string & str_roll_result,
	std::string & message)
{
	std::regex regex_dice("^ *(\\d*[d|D]\\d+(?:[k|K][l|L]?\\d+)?)");
	std::regex regex_num("^ *(\\d*\\.?\\d+%?)");
	std::regex regex_operator("^ *(\\+|-|\\*|/)");
	std::regex regex_bracket("^ *(\\(|\\))");

	std::smatch smatch_split_dice;
	std::string source(str_input);
	std::list<std::string> strlist_equation;
	std::list<std::string> strlist_num_like;
	operation oper_target;
	while (true) {
		std::regex_search(source, smatch_split_dice, regex_dice);
		if (smatch_split_dice.begin() != smatch_split_dice.end()) {
			bool oper_state = oper_target.add_operation(
				operation_mode::DICE_OPERATION_MODE_DICE, 
				smatch_split_dice[1].str(), 
				smatch_split_dice.str());
			if (!oper_state) break;
			source.assign(smatch_split_dice.suffix().str());
			continue;
		}
		std::regex_search(source, smatch_split_dice, regex_num);
		if (smatch_split_dice.begin() != smatch_split_dice.end()) {
			bool oper_state = oper_target.add_operation(
				operation_mode::DICE_OPERATION_MODE_NUMBER,
				smatch_split_dice[1].str(),
				smatch_split_dice.str());
			if (!oper_state) break; 
			source.assign(smatch_split_dice.suffix().str());
			continue;
		}
		std::regex_search(source, smatch_split_dice, regex_operator);
		if (smatch_split_dice.begin() != smatch_split_dice.end()) {
			bool oper_state = oper_target.add_operation(
				operation_mode::DICE_OPERATION_MODE_OPERATOR,
				smatch_split_dice[1].str(),
				smatch_split_dice.str());
			if (!oper_state) break;
			source.assign(smatch_split_dice.suffix().str());
			continue;
		}
		std::regex_search(source, smatch_split_dice, regex_bracket);
		if (smatch_split_dice.begin() != smatch_split_dice.end()) {
			bool oper_state = oper_target.add_operation(
				operation_mode::DICE_OPERATION_MODE_BRACKET,
				smatch_split_dice[1].str(),
				smatch_split_dice.str());
			if (!oper_state) break;
			source.assign(smatch_split_dice.suffix().str());
			continue;
		}

		break;
	}

	bool oper_cal_successfule = false;
	if(oper_target.size() > 0){
		oper_cal_successfule = oper_target.calculate();
		oper_target.gen_tail();
	}
	if (oper_target.tail.size() > 0 || source.size() > 0) {
		std::string t_message = oper_target.tail + source;
		if (t_message.size() > 0) {
			int p = t_message.find_first_not_of(" \t");
			if (p == std::string::npos) {
				t_message.assign("");
			}
			else message.assign(t_message.substr(p));
		}
	}

	if (oper_cal_successfule) {
		str_roll_command.assign(oper_target.str_roll_command); 
		str_roll_result.assign(oper_target.ret_value.str());
		if (oper_target.no_dice) str_roll_detail.assign("");
		else str_roll_detail.assign(oper_target.str_roll_detail);
	}
	else { 
		str_roll_command.assign("");
		str_roll_detail.assign("");
		str_roll_result.assign(""); 
	}

	return true;
}

operation_item::operation_item(int mode, std::string oper, std::string source)
{
	this->i_operation_mode = mode;
	this->source.assign(source);
	switch (mode)
	{
	case operation_mode::DICE_OPERATION_MODE_DICE:
	case operation_mode::DICE_OPERATION_MODE_NUMBER:
		this->mode_num_like_source.assign(oper);
		break;
	case operation_mode::DICE_OPERATION_MODE_OPERATOR:
	case operation_mode::DICE_OPERATION_MODE_BRACKET:
		this->mode_oper_like_operator = oper[0];
		break;
	default:
		break;
	}

	if (mode == operation_mode::DICE_OPERATION_MODE_OPERATOR) {
		switch (this->mode_oper_like_operator)
		{
		case '-':
		case '+':
			this->mode_oper_priority = 0;
			break;
		case '*':
		case '/':
			this->mode_oper_priority = 1;
		default:
			break;
		}
	}
}

operation_item::operation_item(const operation_item & copy)
{
	this->i_operation_mode = copy.i_operation_mode;
	this->mode_num_like_source = copy.mode_num_like_source;
	this->mode_oper_like_operator = copy.mode_oper_like_operator;
	this->source.assign(copy.source);
	if (copy.first != nullptr) this->first = copy.first;
	if (copy.second != nullptr) this->first = copy.second;
}

operation_item::~operation_item()
{
}

bool operation_item::is_num_like()
{
	return this->i_operation_mode == operation_mode::DICE_OPERATION_MODE_NUMBER
		|| this->i_operation_mode == operation_mode::DICE_OPERATION_MODE_DICE;
}

bool operation_item::is_operation()
{
	return this->i_operation_mode == operation_mode::DICE_OPERATION_MODE_OPERATOR;
}

bool operation_item::is_right_bracket()
{
	if (this->i_operation_mode != operation_mode::DICE_OPERATION_MODE_BRACKET) return false;
	return this->mode_oper_like_operator == ')';
}

bool operation_item::is_left_bracket()
{
	if (this->i_operation_mode != operation_mode::DICE_OPERATION_MODE_BRACKET) return false;
	return this->mode_oper_like_operator == '(';
}

bool operation_item::calculate()
{
	switch (this->i_operation_mode)
	{
	case operation_mode::DICE_OPERATION_MODE_DICE:{
		dice_roller dice(this->mode_num_like_source);
		if (dice.status == dice_roller::ROLL_STATUS_FINISHED) {
			this->ret_value = dice.i_sum_result;
			this->str_cal_command.assign(this->mode_num_like_source);
			this->str_cal_detail.assign(*(dice.pstr_detail_result));
			return this->successful();
		}
		else return this->fail();
	}break;
	case operation_mode::DICE_OPERATION_MODE_NUMBER:{
		int pos_of_percent = this->mode_num_like_source.find_last_of('%');
		bool exist_percent = pos_of_percent != std::string::npos;

		std::string str_leave_percent;
		if (exist_percent) str_leave_percent = this->mode_num_like_source.substr(0, pos_of_percent);
		else  str_leave_percent = this->mode_num_like_source;

		this->ret_value = str_leave_percent;
		if (exist_percent) this->ret_value = this->ret_value * 0.01;

		this->str_cal_command.assign(this->mode_num_like_source);
		this->str_cal_detail.assign(this->mode_num_like_source);
		return this->successful();
	}break;
	case operation_mode::DICE_OPERATION_MODE_OPERATOR: {
		if (this->is_unary_operator) {
			bool right_available = false;
			if (this->second != nullptr)
				right_available = this->second->calculate();

			if(right_available){
				this->mode_oper_cal_value_from(this->second.get());
				return this->successful();
			}
			else return this->fail();
		}
		else{
			bool left_available = false;
			bool right_available = false;
			if (this->first != nullptr)
				left_available = this->first->calculate();

			if(!left_available)  return this->fail();

			if (this->second != nullptr)
				right_available = this->second->calculate();

			if (right_available && left_available) {
				this->mode_oper_cal_value_from(this->first.get(), this->second.get());
				return this->successful();
			}
			else {
				this->assign_value_from(this->first.get());
				return this->abandon_right_part();
			}
		}

	}break;
	default:
		return this->fail();
	}
	return this->fail();
}

void operation_item::assign_value_from(operation_item * item)
{
	this->str_cal_command.assign(item->str_cal_command);
	this->str_cal_detail.assign(item->str_cal_detail);
	this->ret_value = item->ret_value;
}

bool operation_item::mode_oper_cal_value_from(operation_item * item1, operation_item * item2)
{
	if (this->i_operation_mode == operation_mode::DICE_OPERATION_MODE_OPERATOR) {

		switch (this->mode_oper_like_operator)
		{
		case '+':
			this->ret_value = item1->ret_value + item2->ret_value;
			break;
		case '-':
			this->ret_value = item1->ret_value - item2->ret_value;
			break;
		case '*':
			this->ret_value = item1->ret_value * item2->ret_value;
			break;
		case '/':
			try{ this->ret_value = item1->ret_value / item2->ret_value; }
			catch (zero_devider_exception zde) {
				return this->is_cal_successful = false;
			}
			break;
		default:
			return this->is_cal_successful = false;
		}
		std::ostringstream cal_command(std::ostringstream::ate);
		cal_command << item1->str_cal_command << ' ';
		cal_command << this->mode_oper_like_operator << ' ';
		cal_command << item2->str_cal_command;
		this->str_cal_command.assign(cal_command.str());

		std::ostringstream cal_detail(std::ostringstream::ate);
		cal_detail << item1->str_cal_detail << ' ';
		cal_detail << this->mode_oper_like_operator << ' ';
		cal_detail << item2->str_cal_detail;
		this->str_cal_detail.assign(cal_detail.str());

		return this->is_cal_successful = true;
	}
	else return this->is_cal_successful = false;
}

bool operation_item::mode_oper_cal_value_from(operation_item * item)
{
	if (this->i_operation_mode == operation_mode::DICE_OPERATION_MODE_OPERATOR) {
		if (this->mode_oper_like_operator == '-') {
			std::ostringstream cal_oper(std::ostringstream::ate);
			cal_oper << this->mode_oper_like_operator;

			this->ret_value = number(0) - this->second->ret_value;
			this->str_cal_command.assign(cal_oper.str() + item->str_cal_command);
			this->str_cal_detail.assign(cal_oper.str() + item->str_cal_detail);
			return this->is_cal_successful = true;
		}
	}
	return this->is_cal_successful = false;
}

bool operation_item::successful()
{
	if (this->is_bracket_surrounded) {
		this->left_bracket->is_cal_successful = true;
		this->right_bracket->is_cal_successful = true;

		this->str_cal_command.assign("(" + this->str_cal_command + ")");
		this->str_cal_detail.assign("(" + this->str_cal_detail + ")");
	}
	return this->is_cal_successful = true;
}

bool operation_item::abandon_right_part()
{
	if (this->is_bracket_surrounded) return false;
	return true;
}

bool operation_item::fail()
{
	return false;
}

operation::operation()
{
	this->list_operations = new std::list<std::shared_ptr<operation_item>>();
	this->list_output = new std::list<std::shared_ptr<operation_item>>();
	this->stack_temp = new std::stack<std::shared_ptr<operation_item>>();
	this->left_bracket_location = new std::list<size_t>();
}

operation::~operation()
{
	if (list_operations != nullptr) delete list_operations;
	if (list_output != nullptr) delete list_output;
	if (stack_temp != nullptr) delete stack_temp;
	if (left_bracket_location != nullptr) delete left_bracket_location;
}

bool operation::add_operation(int i_oper_mode, std::string str_oper, std::string source)
{
	switch (i_oper_mode)
	{
	case operation_mode::DICE_OPERATION_MODE_DICE :{
		if (list_operations->size() > 0
			&& (list_operations->back()->is_num_like()
				|| list_operations->back()->is_right_bracket())) return false;
		operation_item * oper_item = new operation_item(
			operation_mode::DICE_OPERATION_MODE_DICE,
			str_oper,
			source);
		std::shared_ptr<operation_item> p_oper_item(oper_item);
		this->list_output->push_back(p_oper_item);
		this->list_operations->push_back(p_oper_item);
		this->no_dice = false;
		this->i_num_like_count++;
		return true;
	} break;
	case operation_mode::DICE_OPERATION_MODE_NUMBER: {
		if (list_operations->size() > 0 
			&& (list_operations->back()->is_num_like()
				|| list_operations->back()->is_right_bracket())) return false;
		operation_item * oper_item = new operation_item(
			operation_mode::DICE_OPERATION_MODE_NUMBER,
			str_oper,
			source);
		std::shared_ptr<operation_item> p_oper_item(oper_item);
		this->list_output->push_back(p_oper_item);
		this->list_operations->push_back(p_oper_item);

		this->i_num_like_count++;
		return true;
	} break;
	case operation_mode::DICE_OPERATION_MODE_OPERATOR: {
		operation_item * oper_item = new operation_item(
			operation_mode::DICE_OPERATION_MODE_OPERATOR,
			str_oper,
			source);
		if (list_operations->size() > 0 && list_operations->back()->is_operation() && oper_item->mode_oper_like_operator != '-') return false;
		
		if (list_operations->size() == 0 || list_operations->back()->is_operation()){
			oper_item->is_unary_operator = true; 
			oper_item->mode_oper_priority = 2;
		}
		
		while (stack_temp->size() > 0
			&& stack_temp->top()->is_operation()
			&& stack_temp->top()->mode_oper_priority >= oper_item->mode_oper_priority) {
			this->list_output->push_back(stack_temp->top());
			this->stack_temp->pop();
		}
		std::shared_ptr<operation_item> p_oper_item(oper_item);
		this->stack_temp->push(p_oper_item);
		this->list_operations->push_back(p_oper_item);

		if (!oper_item->is_unary_operator) this->i_binary_operator_count++;
		return true;
	}break;
	case operation_mode::DICE_OPERATION_MODE_BRACKET: {
		operation_item * oper_item = new operation_item(
			operation_mode::DICE_OPERATION_MODE_BRACKET,
			str_oper,
			source);
		if (oper_item->is_left_bracket()) {
			if (list_operations->size() > 0
				&& (list_operations->back()->is_num_like()
					|| list_operations->back()->is_right_bracket())) return false;

			this->left_bracket_location->push_back(this->list_output->size());
			std::shared_ptr<operation_item> p_oper_item(oper_item);
			this->stack_temp->push(p_oper_item);
			this->list_operations->push_back(p_oper_item);
			return true;
		}
		else { 
			if (this->left_bracket_location->size() == 0) return false;
			this->left_bracket_location->pop_back();
			bool is_empty_bracket = true;
			while (!this->stack_temp->top()->is_left_bracket()) {
				this->list_output->push_back(this->stack_temp->top());
				this->stack_temp->pop();
				is_empty_bracket = false;
			}
			std::shared_ptr<operation_item> p_oper_item(oper_item);
			if (!is_empty_bracket) {
				this->list_output->back()->is_bracket_surrounded = true;
				this->list_output->back()->left_bracket = this->stack_temp->top();
				this->list_output->back()->right_bracket = p_oper_item;
			}
			this->stack_temp->pop();
			this->list_operations->push_back(p_oper_item);
			return true;
		}
	}break;
	default:
		return false;
	}
}

void operation::gen_tail()
{
	std::ostringstream ostrs_stream(std::ostringstream::ate);
	for each (std::shared_ptr<operation_item> var in *list_operations)
	{
		if (var->is_cal_successful) continue;
		else ostrs_stream << var->source;
	}
	this->tail.assign(ostrs_stream.str());
}

bool operation::calculate()
{
	if (this->left_bracket_location->size() > 0) {
		size_t pos_of_ire_blacket = this->left_bracket_location->front();
		while (this->list_output->size() > pos_of_ire_blacket) {
			if (this->list_output->back()->is_num_like()) this->i_num_like_count--;
			else if (this->list_output->back()->is_operation() 
				&& !this->list_output->back()->is_unary_operator) this->i_binary_operator_count--;
			this->list_output->pop_back();
		}

		while (this->left_bracket_location->size() > 0) {
			if (this->stack_temp->top()->is_left_bracket())
				this->left_bracket_location->pop_back();

			if (this->stack_temp->size() > 0 && this->stack_temp->top()->is_operation()
				&& !this->stack_temp->top()->is_unary_operator) this->i_binary_operator_count--;
			this->stack_temp->pop();
		}
	}

	if (this->stack_temp->size() > 0 && this->i_binary_operator_count > (this->i_num_like_count - 1)) {
		this->stack_temp->pop();
	}

	while (this->stack_temp->size() > 0) {
		this->list_output->push_back(stack_temp->top());
		this->stack_temp->pop();
	}

	if (this->list_output->size() == 0) return false;

	std::list<std::shared_ptr<operation_item>> list_vals;

	while (this->list_output->size() > 0) {
		if (this->list_output->front()->is_num_like()) {
			list_vals.push_back(list_output->front());
			list_output->pop_front();
		}
		else if (this->list_output->front()->is_operation()) {

			if (list_vals.size() >= 1 && list_output->front()->is_unary_operator) {
				this->list_output->front()->second = list_vals.back();
				list_vals.pop_back();
			}
			else if(list_vals.size() >= 2){
				this->list_output->front()->second = list_vals.back();
				list_vals.pop_back();
				this->list_output->front()->first = list_vals.back();
				list_vals.pop_back();
			}
			else return false;


			list_vals.push_back(list_output->front());
			list_output->pop_front();
		}
	}

	if (list_vals.size() > 0) {
		if(list_vals.front()->calculate()){
			this->str_roll_command.assign(list_vals.front()->str_cal_command);
			this->str_roll_detail.assign(list_vals.front()->str_cal_detail);
			this->ret_value = list_vals.front()->ret_value;
			return true;
		}
	}

	return false;
}

size_t operation::size()
{
	return this->list_operations->size();
}
