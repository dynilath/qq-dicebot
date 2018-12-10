#pragma once

bool base_split_dice(const std::string &str_input, std::string & str_output, bool giving_detailed_message);

class operation_mode {
public:
	static const int DICE_OPERATION_MODE_NUMBER = 1;
	static const int DICE_OPERATION_MODE_DICE = 3;
	static const int DICE_OPERATION_MODE_OPERATOR = 4;
	static const int DICE_OPERATION_MODE_BRACKET = 8;
};

class operation_item {
private:
	void assign_value_from(operation_item * item);
	bool mode_oper_cal_value_from(operation_item * item1, operation_item * item2);
	bool mode_oper_cal_value_from(operation_item * item);
	
	bool successful();
	bool abandon_right_part();
	bool fail();


public:
	bool is_num_like();
	bool is_operation();
	bool is_right_bracket();
	bool is_left_bracket();

	std::string source;
	std::shared_ptr<operation_item> first = nullptr;
	std::shared_ptr<operation_item> second = nullptr;

	bool is_bracket_surrounded = false;
	std::shared_ptr<operation_item> left_bracket = nullptr;
	std::shared_ptr<operation_item> right_bracket = nullptr;


	bool is_unary_operator = false;
	int i_operation_mode;
	std::string mode_num_like_source;
	char mode_oper_like_operator;
	int mode_oper_priority;
	bool calculate();

	bool is_cal_successful = false;
	
	number ret_value;

	std::string str_cal_command;
	std::string str_cal_detail;

	operation_item(int mode, std::string oper, std::string source);
	operation_item(const operation_item & copy);
	~operation_item();
};

class operation {
private:
	std::list<std::shared_ptr<operation_item>> * list_operations;

	int i_num_like_count = 0;
	int i_binary_operator_count = 0;


	std::list<std::shared_ptr<operation_item>> * list_output = nullptr;
	std::stack<std::shared_ptr<operation_item>> * stack_temp = nullptr;
	std::list<size_t> * left_bracket_location = nullptr;
public:
	bool no_dice = true;

	std::string str_roll_command;
	std::string str_roll_detail;
	std::string tail;

	number ret_value;

	operation();
	~operation();
	bool add_operation(int i_oper_mode, std::string str_oper, std::string source);
	void gen_tail();
	bool calculate();
	size_t size();
};

bool binary_tree_split_dice(
	const std::string & str_input, 
	std::string & str_roll_command, 
	std::string & str_roll_detail,
	std::string & str_roll_result, 
	std::string & message);