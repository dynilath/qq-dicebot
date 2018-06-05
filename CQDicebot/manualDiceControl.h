#pragma once

class manualDice;

#define TYPE_KEY_MANUALDICE std::pair<int64_t, int64_t>
#define TYPE_MAP_MANUALDICE std::map<TYPE_KEY_MANUALDICE, manualDice> 
#define TYPE_PAIR_MANUALDICE std::pair<TYPE_KEY_MANUALDICE, manualDice> 

class manualDiceManager
{
	bool is_no_sql_mode;
public:
	static manualDiceManager * instance;
	TYPE_MAP_MANUALDICE * map_manualdicemap;
	manualDiceManager();
	~manualDiceManager();
	manualDice * createManualDice(const int64_t i_QQID, const int64_t i_QQGroupID, const std::string & command);
	manualDice * rollManualDice(const int64_t i_QQID, const int64_t i_QQGroupID, const std::string & command);
	manualDice * killManualDice(const int64_t i_QQID, const int64_t i_QQGroupID, const std::string & command);
	manualDice * addManualDice(const int64_t i_QQID, const int64_t i_QQGroupID, const std::string & command);
	manualDice * killallManualDice(const int64_t i_QQID, const int64_t i_QQGroupID);
	static int sqlite3_callback_query_manualdice(void * data, int argc, char ** argv, char ** azColName);
};

