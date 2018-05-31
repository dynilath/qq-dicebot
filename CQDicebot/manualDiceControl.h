#pragma once
class manualDiceControl
{
	bool is_no_sql_mode;
public:
	static manualDiceControl * instance;
	manualDiceControl();
	~manualDiceControl();
	void createManualDice(const int64_t QQID, const int64_t QQGroupID, const std::string & command);
	void rollManualDice(const int64_t QQID, const int64_t QQGroupID, const std::string & command);
	void killManualDice(const int64_t QQID, const int64_t QQGroupID, const std::string & command);
};

