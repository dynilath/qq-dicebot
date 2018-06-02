#include "stdafx.h"
#include "manualdice.h"
#include "databaseManager.h"
#include "manualDiceControl.h"


#define MANUALDICE_TABLE_NAME "manualdice"
#define MANUALDICE_TABLE_DEFINE "create table " MANUALDICE_TABLE_NAME \
				"(qqid      int     NOT NULL," \
				"groupid   int     NOT NULL," \
				"source     text    NOT NULL," \
				"current    text    NOT NULL," \
				"primary    key    (QQID,GROUPID));"

manualDiceControl * manualDiceControl::instance = nullptr;

manualDiceControl::manualDiceControl()
{
	databaseManager * databaseControl = databaseManager::getInstance();
	int i_ret_code = databaseControl->registerTable(MANUALDICE_TABLE_NAME, MANUALDICE_TABLE_DEFINE);
	is_no_sql_mode = i_ret_code != SQLITE_OK;
	map_manualdicemap = new TYPE_MAP_MANUALDICE();

	if (manualDiceControl::instance != nullptr) {
		delete(manualDiceControl::instance);
	}
	else manualDiceControl::instance = this;
}


manualDiceControl::~manualDiceControl()
{
	delete(map_manualdicemap);
	delete(str_detail_string);
	if (manualDiceControl::instance == this) {
		manualDiceControl::instance = nullptr;
	}
}

manualDice * manualDiceControl::createManualDice(const int64_t i_QQID, const int64_t i_QQGroupID, const std::string & command)
{
	manualDice md_manualdice(command);

	TYPE_MAP_MANUALDICE::iterator iter_manualdicemap = this->map_manualdicemap->find(TYPE_KEY_MANUALDICE(i_QQID, i_QQGroupID));
	if (iter_manualdicemap != this->map_manualdicemap->end()) {
		this->map_manualdicemap->erase(iter_manualdicemap);
		this->map_manualdicemap->insert(TYPE_PAIR_MANUALDICE(TYPE_KEY_MANUALDICE(i_QQID, i_QQGroupID), md_manualdice));
	}
	else {
		this->map_manualdicemap->insert(TYPE_PAIR_MANUALDICE(TYPE_KEY_MANUALDICE(i_QQID, i_QQGroupID), md_manualdice));
	}
	return &md_manualdice;
	std::ostringstream ostrs_result(std::ostringstream::ate);
	ostrs_result << "在桌子上放置了一些骰子 ";
	ostrs_result << "当前状态: " << md_manualdice.ToString();
	ostrs_result << " = " << md_manualdice.i_sum_result;
	this->str_detail_string = new std::string(ostrs_result.str());
}

manualDice * manualDiceControl::rollManualDice(const int64_t i_QQID, const int64_t i_QQGroupID, const std::string & command)
{
	TYPE_MAP_MANUALDICE::iterator iter_manualdicemap = this->map_manualdicemap->find(TYPE_KEY_MANUALDICE(i_QQID, i_QQGroupID));
	if (iter_manualdicemap != this->map_manualdicemap->end()) {
		manualDice md_manualdice = (*iter_manualdicemap).second;
		md_manualdice.roll(command);
		std::ostringstream ostrs_result(std::ostringstream::ate);
		ostrs_result << "重骰 " << command << " 骰子 ";
		ostrs_result << "当前状态: " << md_manualdice.ToString();
		ostrs_result << " = " << md_manualdice.i_sum_result;
		this->str_detail_string = new std::string(ostrs_result.str());
		return &md_manualdice;
	}
}

manualDice * manualDiceControl::killManualDice(const int64_t i_QQID, const int64_t i_QQGroupID, const std::string & command)
{
	TYPE_MAP_MANUALDICE::iterator iter_manualdicemap = this->map_manualdicemap->find(TYPE_KEY_MANUALDICE(i_QQID, i_QQGroupID));
	if (iter_manualdicemap != this->map_manualdicemap->end()) {
		manualDice md_manualdice = (*iter_manualdicemap).second;
		md_manualdice.kill(command);
		return & md_manualdice;
	}
	return nullptr;
}

