#pragma once

#define TYPE_LIST_DICE std::vector<std::pair<int,int>>
#define TYPE_PAIR_DICE std::pair<int,int>

class manual_dice
{
private :
	TYPE_LIST_DICE * pintlist_dice;
public:
	manual_dice();
	~manual_dice();
	manual_dice(const std::string & source);
	void roll(const std::string & source);
	void kill(const std::string & source);
	void add(const std::string & source);
	void killall();
	std::string endcode();
	std::string ToString();
	int status;
	int i_sum_result;
	void decode(std::string & source);
};

