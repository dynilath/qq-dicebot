#pragma once

#define TYPE_KEY_MANUALDICE std::pair<int64_t, int64_t>
#define TYPE_MAP_MANUALDICE std::map<TYPE_KEY_MANUALDICE, manualDice> 
#define TYPE_PAIR_MANUALDICE std::pair<TYPE_KEY_MANUALDICE, manualDice> 

class manualDiceControl
{
	bool is_no_sql_mode;
public:
	static manualDiceControl * instance;
	TYPE_MAP_MANUALDICE * map_manualdicemap;
	std::string * str_detail_string;
	manualDiceControl();
	~manualDiceControl();
	manualDice * createManualDice(const int64_t i_QQID, const int64_t i_QQGroupID, const std::string & command);
	manualDice * rollManualDice(const int64_t i_QQID, const int64_t i_QQGroupID, const std::string & command);
	manualDice * killManualDice(const int64_t i_QQID, const int64_t i_QQGroupID, const std::string & command);
};

