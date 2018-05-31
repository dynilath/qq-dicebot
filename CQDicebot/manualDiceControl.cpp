#include "manualDiceControl.h"
#include "stdafx.h"
#include "databaseManager.h"

#define MANUALDICE_TABLE_NAME "manualdice"
#define MANUALDICE_TABLE_DEFINE "create table " MANUALDICE_TABLE_NAME \
				"(qqid      int     NOT NULL," \
				"groupid   int     NOT NULL," \
				"source     text    NOT NULL," \
				"current    text    NOT NULL," \
				"primary    key    (QQID,GROUPID));"

manualDiceControl::manualDiceControl()
{
	databaseManager * databaseControl = databaseManager::getInstance();
	int i_ret_code = databaseControl->registerTable(MANUALDICE_TABLE_NAME, MANUALDICE_TABLE_DEFINE);
	is_no_sql_mode = i_ret_code != SQLITE_OK;
	if (manualDiceControl::instance != nullptr) {
		delete(manualDiceControl::instance);
	}
	else manualDiceControl::instance = this;
}


manualDiceControl::~manualDiceControl()
{
	if (manualDiceControl::instance == this) {
		manualDiceControl::instance = nullptr;
	}
}

void manualDiceControl::createManualDice(const int64_t QQID, const int64_t QQGroupID, const std::string & command)
{
}

void manualDiceControl::rollManualDice(const int64_t QQID, const int64_t QQGroupID, const std::string & command)
{
}

void manualDiceControl::killManualDice(const int64_t QQID, const int64_t QQGroupID, const std::string & command)
{
}

