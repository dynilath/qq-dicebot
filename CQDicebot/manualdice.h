#pragma once

#define TYPE_LIST_DICE std::vector<std::pair<int,int>>
#define TYPE_PAIR_DICE std::pair<int,int>

class manualDice
{
private :
	TYPE_LIST_DICE * pintlist_dice;
public:
	manualDice();
	~manualDice();
	manualDice(const std::string & source);
	void roll(const std::string & source);
	void kill(const std::string & source);
	void add(const std::string & source);
	std::string endcode();
	std::string ToString();
	int status;
	int i_sum_result;
	void decode(std::string & source);
};

