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

manualDiceManager * manualDiceManager::instance = nullptr;

manualDiceManager::manualDiceManager()
{
	databaseManager * databaseControl = databaseManager::getInstance();
	int i_ret_code = databaseControl->registerTable(MANUALDICE_TABLE_NAME, MANUALDICE_TABLE_DEFINE);
	is_no_sql_mode = i_ret_code != SQLITE_OK;
	map_manualdicemap = new TYPE_MAP_MANUALDICE();

	if (manualDiceManager::instance != nullptr) {
		delete(manualDiceManager::instance);
	}
	else manualDiceManager::instance = this;
}


manualDiceManager::~manualDiceManager()
{
	delete(map_manualdicemap);
	delete(str_detail_string);
	if (manualDiceManager::instance == this) {
		manualDiceManager::instance = nullptr;
	}
}

manualDice * manualDiceManager::createManualDice(const int64_t i_QQID, const int64_t i_QQGroupID, const std::string & command)
{
	manualDice md_manualdice(command);
	manualDice * md_manualdice_ret = nullptr;
	TYPE_MAP_MANUALDICE::iterator iter_manualdicemap = this->map_manualdicemap->find(TYPE_KEY_MANUALDICE(i_QQID, i_QQGroupID));
	if (iter_manualdicemap != this->map_manualdicemap->end()) {
		this->map_manualdicemap->erase(iter_manualdicemap);
		auto pair_insert_ret = this->map_manualdicemap->insert(TYPE_PAIR_MANUALDICE(TYPE_KEY_MANUALDICE(i_QQID, i_QQGroupID), md_manualdice));
		if(pair_insert_ret.second){
			md_manualdice_ret = & ((*(pair_insert_ret.first)).second);
		}
	}
	else {
		auto pair_insert_ret = this->map_manualdicemap->insert(TYPE_PAIR_MANUALDICE(TYPE_KEY_MANUALDICE(i_QQID, i_QQGroupID), md_manualdice));
		if (pair_insert_ret.second) {
			md_manualdice_ret = &((*(pair_insert_ret.first)).second);
		}
	}
	return md_manualdice_ret;

	/*
	std::ostringstream ostrs_result(std::ostringstream::ate);
	ostrs_result << "在桌子上放置了一些骰子 ";
	ostrs_result << "当前状态: " << md_manualdice->ToString();
	ostrs_result << " = " << md_manualdice->i_sum_result;
	this->str_detail_string = new std::string(ostrs_result.str());
	*/
}

manualDice * manualDiceManager::rollManualDice(const int64_t i_QQID, const int64_t i_QQGroupID, const std::string & command)
{
	TYPE_MAP_MANUALDICE::iterator iter_manualdicemap = this->map_manualdicemap->find(TYPE_KEY_MANUALDICE(i_QQID, i_QQGroupID));
	if (iter_manualdicemap != this->map_manualdicemap->end()) {
		manualDice * md_manualdice = &((*iter_manualdicemap).second);
		md_manualdice->roll(command);
		return md_manualdice;
	}
}

manualDice * manualDiceManager::killManualDice(const int64_t i_QQID, const int64_t i_QQGroupID, const std::string & command)
{
	TYPE_MAP_MANUALDICE::iterator iter_manualdicemap = this->map_manualdicemap->find(TYPE_KEY_MANUALDICE(i_QQID, i_QQGroupID));
	if (iter_manualdicemap != this->map_manualdicemap->end()) {
		manualDice * md_manualdice = &((*iter_manualdicemap).second);
		md_manualdice->kill(command);
		return md_manualdice;
	}
	return nullptr;
}

manualDice * manualDiceManager::addManualDice(const int64_t i_QQID, const int64_t i_QQGroupID, const std::string & command)
{
	TYPE_MAP_MANUALDICE::iterator iter_manualdicemap = this->map_manualdicemap->find(TYPE_KEY_MANUALDICE(i_QQID, i_QQGroupID));
	if (iter_manualdicemap != this->map_manualdicemap->end()) {
		manualDice * md_manualdice = &((*iter_manualdicemap).second);
		md_manualdice->add(command);
		return md_manualdice;
	}
	return nullptr;
}

