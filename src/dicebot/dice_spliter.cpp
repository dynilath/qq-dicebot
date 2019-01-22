#include "./dice_spliter.h"

#include "./dice_roller.h"

using namespace dicebot;

static std::regex const regex_single_dice("(\\+|\\-)?((\\d*d\\d+((k|kl)\\d+)?)|(\\d+))");
static std::regex const regex_dice("^ *(\\d*d\\d+(?:kl?\\d+)?)",std::regex_constants::icase);
static std::regex const regex_num("^ *(\\d*\\.?\\d+%?)");
static std::regex const regex_operator("^ *(\\+|-|\\*|/)");
static std::regex const regex_bracket("^ *(\\(|\\))");

bool dicebot::binary_tree_split_dice(
    std::string const & str_input,
    std::string & str_roll_command,
    std::string & str_roll_detail,
    std::string & str_roll_result,
    std::string & message){
    std::smatch smatch_split_dice;
    std::string source(str_input);
    std::list<std::string> strlist_equation;
    std::list<std::string> strlist_num_like;
    operation oper_target;
    while (true){
        std::regex_search(source, smatch_split_dice, regex_dice);
        if(smatch_split_dice.size() > 0){
            bool oper_state = oper_target.add_operation(
                operation_mode::DICE, 
                smatch_split_dice[1].str(), 
                smatch_split_dice.str());
            if(!oper_state) break;
            source.assign(smatch_split_dice.suffix().str());
            continue;
        }
        std::regex_search(source, smatch_split_dice, regex_num);
        if(smatch_split_dice.size() > 0){
            bool oper_state = oper_target.add_operation(
                operation_mode::NUMBER,
                smatch_split_dice[1].str(),
                smatch_split_dice.str());
            if(!oper_state) break; 
            source.assign(smatch_split_dice.suffix().str());
            continue;
        }
        std::regex_search(source, smatch_split_dice, regex_operator);
        if(smatch_split_dice.size() > 0){
            bool oper_state = oper_target.add_operation(
                operation_mode::OPERATOR,
                smatch_split_dice[1].str(),
                smatch_split_dice.str());
            if(!oper_state) break;
            source.assign(smatch_split_dice.suffix().str());
            continue;
        }
        std::regex_search(source, smatch_split_dice, regex_bracket);
        if(smatch_split_dice.size() > 0){
            bool oper_state = oper_target.add_operation(
                operation_mode::BRACKET,
                smatch_split_dice[1].str(),
                smatch_split_dice.str());
            if(!oper_state) break;
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
    if(oper_target.tail.size() > 0 || source.size() > 0){
        std::string t_message = oper_target.tail + source;
        if(t_message.size() > 0){
            int p = t_message.find_first_not_of(" \t");
            if(p == std::string::npos){
                t_message.assign("");
            }
            else message.assign(t_message.substr(p));
        }
    }

    if(oper_cal_successfule){
        str_roll_command.assign(oper_target.str_roll_command); 
        str_roll_result.assign(oper_target.ret_value.str());
        if(oper_target.no_dice) str_roll_detail.assign("");
        else str_roll_detail.assign(oper_target.str_roll_detail);
    }
    else{ 
        str_roll_command.assign("");
        str_roll_detail.assign("");
        str_roll_result.assign(""); 
    }

    return true;
}

operation_item::operation_item(operation_mode mode, std::string oper, std::string source){
    this->mode = mode;
    this->source.assign(source);
    switch (mode){
    case operation_mode::DICE:
    case operation_mode::NUMBER:
        this->mode_num_like_source.assign(oper);
        break;
    case operation_mode::OPERATOR:
    case operation_mode::BRACKET:
        this->mode_oper_like_operator = oper[0];
        break;
    default:
        break;
    }

    if(mode == operation_mode::OPERATOR){
        switch (this->mode_oper_like_operator){
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

operation_item::operation_item(const operation_item & copy){
    this->mode = copy.mode;
    this->mode_num_like_source = copy.mode_num_like_source;
    this->mode_oper_like_operator = copy.mode_oper_like_operator;
    this->source.assign(copy.source);
    if(copy.first != nullptr) this->first = copy.first;
    if(copy.second != nullptr) this->first = copy.second;
}

operation_item::~operation_item(){
}

bool operation_item::is_num_like(){
    return this->mode == operation_mode::NUMBER
        || this->mode == operation_mode::DICE;
}

bool operation_item::is_operation(){
    return this->mode == operation_mode::OPERATOR;
}

bool operation_item::is_right_bracket(){
    if(this->mode != operation_mode::BRACKET) return false;
    return this->mode_oper_like_operator == ')';
}

bool operation_item::is_left_bracket(){
    if(this->mode != operation_mode::BRACKET) return false;
    return this->mode_oper_like_operator == '(';
}

bool operation_item::calculate(){
    switch (this->mode){
    case operation_mode::DICE:{
        roll::dice_roll dr;
        roll::roll_rdk(dr,this->mode_num_like_source);
        if(dr){
            this->ret_value = dr.summary;;
            this->str_cal_command.assign(this->mode_num_like_source);
            this->str_cal_detail = dr.detail();
            return this->successful();
        }
        else return this->fail();
    }break;
    case operation_mode::NUMBER:{
        int pos_of_percent = this->mode_num_like_source.find_last_of('%');
        bool exist_percent = pos_of_percent != std::string::npos;

        std::string str_leave_percent;
        if(exist_percent) str_leave_percent = this->mode_num_like_source.substr(0, pos_of_percent);
        else  str_leave_percent = this->mode_num_like_source;

        this->ret_value = str_leave_percent;
        if(exist_percent) this->ret_value = this->ret_value * 0.01;

        this->str_cal_command.assign(this->mode_num_like_source);
        this->str_cal_detail.assign(this->mode_num_like_source);
        return this->successful();
    }break;
    case operation_mode::OPERATOR:{
        if(this->is_unary_operator){
            bool right_available = false;
            if(this->second != nullptr)
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
            if(this->first != nullptr)
                left_available = this->first->calculate();

            if(!left_available)  return this->fail();

            if(this->second != nullptr)
                right_available = this->second->calculate();

            if(right_available && left_available){
                this->mode_oper_cal_value_from(this->first.get(), this->second.get());
                return this->successful();
            }
            else{
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

void operation_item::assign_value_from(operation_item * item){
    this->str_cal_command.assign(item->str_cal_command);
    this->str_cal_detail.assign(item->str_cal_detail);
    this->ret_value = item->ret_value;
}

bool operation_item::mode_oper_cal_value_from(operation_item * item1, operation_item * item2){
    if(this->mode == operation_mode::OPERATOR){

        switch (this->mode_oper_like_operator){
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
            catch (zero_divider_exception zde){
                return this->is_cal_successful = false;
            }
            break;
        default:
            return this->is_cal_successful = false;
        }
        ostrs cal_command(ostrs::ate);
        cal_command << item1->str_cal_command << ' ';
        cal_command << this->mode_oper_like_operator << ' ';
        cal_command << item2->str_cal_command;
        this->str_cal_command.assign(cal_command.str());

        ostrs cal_detail(ostrs::ate);
        cal_detail << item1->str_cal_detail << ' ';
        cal_detail << this->mode_oper_like_operator << ' ';
        cal_detail << item2->str_cal_detail;
        this->str_cal_detail.assign(cal_detail.str());

        return this->is_cal_successful = true;
    }
    else return this->is_cal_successful = false;
}

bool operation_item::mode_oper_cal_value_from(operation_item * item){
    if(this->mode == operation_mode::OPERATOR){
        if(this->mode_oper_like_operator == '-'){
            ostrs cal_oper(ostrs::ate);
            cal_oper << this->mode_oper_like_operator;

            this->ret_value = number(0) - this->second->ret_value;
            this->str_cal_command.assign(cal_oper.str() + item->str_cal_command);
            this->str_cal_detail.assign(cal_oper.str() + item->str_cal_detail);
            return this->is_cal_successful = true;
        }
    }
    return this->is_cal_successful = false;
}

bool operation_item::successful(){
    if(this->is_bracket_surrounded){
        this->left_bracket->is_cal_successful = true;
        this->right_bracket->is_cal_successful = true;

        this->str_cal_command.assign("(" + this->str_cal_command + ")");
        this->str_cal_detail.assign("(" + this->str_cal_detail + ")");
    }
    return this->is_cal_successful = true;
}

bool operation_item::abandon_right_part(){
    if(this->is_bracket_surrounded) return false;
    return true;
}

bool operation_item::fail(){
    return false;
}

operation::operation(){
    this->list_operations = new std::list<p_item>();
    this->list_output = new std::list<p_item>();
    this->stack_temp = new std::stack<p_item>();
    this->left_bracket_location = new std::list<size_t>();
}

operation::~operation(){
    if(list_operations != nullptr) delete list_operations;
    if(list_output != nullptr) delete list_output;
    if(stack_temp != nullptr) delete stack_temp;
    if(left_bracket_location != nullptr) delete left_bracket_location;
}

bool operation::add_operation(operation_mode mode, std::string str_oper, std::string source){
    switch (mode){
    case operation_mode::DICE :{
        if(list_operations->size() > 0
            && (list_operations->back()->is_num_like()
                || list_operations->back()->is_right_bracket())) return false;
        operation_item * oper_item = new operation_item(
            operation_mode::DICE,
            str_oper,
            source);
        p_item p_oper_item(oper_item);
        this->list_output->push_back(p_oper_item);
        this->list_operations->push_back(p_oper_item);
        this->no_dice = false;
        this->i_num_like_count++;
        return true;
    } break;
    case operation_mode::NUMBER:{
        if(list_operations->size() > 0 
            && (list_operations->back()->is_num_like()
                || list_operations->back()->is_right_bracket())) return false;
        operation_item * oper_item = new operation_item(
            operation_mode::NUMBER,
            str_oper,
            source);
        p_item p_oper_item(oper_item);
        this->list_output->push_back(p_oper_item);
        this->list_operations->push_back(p_oper_item);

        this->i_num_like_count++;
        return true;
    } break;
    case operation_mode::OPERATOR:{
        operation_item * oper_item = new operation_item(
            operation_mode::OPERATOR,
            str_oper,
            source);
        if(list_operations->size() > 0 && list_operations->back()->is_operation() && oper_item->mode_oper_like_operator != '-') return false;
        
        if(list_operations->size() == 0 || list_operations->back()->is_operation()){
            oper_item->is_unary_operator = true; 
            oper_item->mode_oper_priority = 2;
        }
        
        while (stack_temp->size() > 0
            && stack_temp->top()->is_operation()
            && stack_temp->top()->mode_oper_priority >= oper_item->mode_oper_priority){
            this->list_output->push_back(stack_temp->top());
            this->stack_temp->pop();
        }
        p_item p_oper_item(oper_item);
        this->stack_temp->push(p_oper_item);
        this->list_operations->push_back(p_oper_item);

        if(!oper_item->is_unary_operator) this->i_binary_operator_count++;
        return true;
    }break;
    case operation_mode::BRACKET:{
        operation_item * oper_item = new operation_item(
            operation_mode::BRACKET,
            str_oper,
            source);
        if(oper_item->is_left_bracket()){
            if(list_operations->size() > 0
                && (list_operations->back()->is_num_like()
                    || list_operations->back()->is_right_bracket())) return false;

            this->left_bracket_location->push_back(this->list_output->size());
            p_item p_oper_item(oper_item);
            this->stack_temp->push(p_oper_item);
            this->list_operations->push_back(p_oper_item);
            return true;
        }
        else{ 
            if(this->left_bracket_location->size() == 0) return false;
            this->left_bracket_location->pop_back();
            bool is_empty_bracket = true;
            while (!this->stack_temp->top()->is_left_bracket()){
                this->list_output->push_back(this->stack_temp->top());
                this->stack_temp->pop();
                is_empty_bracket = false;
            }
            p_item p_oper_item(oper_item);
            if(!is_empty_bracket){
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

void operation::gen_tail(){
    using ostrs = ostrs;
    ostrs ot(ostrs::ate);
    for(auto iter = list_operations->cbegin();iter != list_operations->cend();iter ++){
        if((*iter)->is_cal_successful) continue;
        else ot << (*iter)->source;
    }
    this->tail.assign(ot.str());
}

bool operation::calculate(){
    if(this->left_bracket_location->size() > 0){
        size_t pos_of_ire_blacket = this->left_bracket_location->front();
        while (this->list_output->size() > pos_of_ire_blacket){
            if(this->list_output->back()->is_num_like()) this->i_num_like_count--;
            else if(this->list_output->back()->is_operation() 
                && !this->list_output->back()->is_unary_operator) this->i_binary_operator_count--;
            this->list_output->pop_back();
        }

        while (this->left_bracket_location->size() > 0){
            if(this->stack_temp->top()->is_left_bracket())
                this->left_bracket_location->pop_back();

            if(this->stack_temp->size() > 0 && this->stack_temp->top()->is_operation()
                && !this->stack_temp->top()->is_unary_operator) this->i_binary_operator_count--;
            this->stack_temp->pop();
        }
    }

    if(this->stack_temp->size() > 0 && this->i_binary_operator_count > (this->i_num_like_count - 1)){
        this->stack_temp->pop();
    }

    while (this->stack_temp->size() > 0){
        this->list_output->push_back(stack_temp->top());
        this->stack_temp->pop();
    }

    if(this->list_output->size() == 0) return false;

    std::list<p_item> list_vals;

    while (this->list_output->size() > 0){
        if(this->list_output->front()->is_num_like()){
            list_vals.push_back(list_output->front());
            list_output->pop_front();
        }
        else if(this->list_output->front()->is_operation()){

            if(list_vals.size() >= 1 && list_output->front()->is_unary_operator){
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

    if(list_vals.size() > 0){
        if(list_vals.front()->calculate()){
            this->str_roll_command.assign(list_vals.front()->str_cal_command);
            this->str_roll_detail.assign(list_vals.front()->str_cal_detail);
            this->ret_value = list_vals.front()->ret_value;
            return true;
        }
    }

    return false;
}

size_t operation::size(){
    return this->list_operations->size();
}