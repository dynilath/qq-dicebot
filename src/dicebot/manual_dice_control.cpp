#include "./manual_dice_control.h"

#include "sqlite3.h"

#include "./manual_dice.h"
#include "./database_manager.h"


namespace dicebot{
#define MANUALDICE_TABLE_NAME "manualdice"
#define MANUALDICE_TABLE_DEFINE "create table " MANUALDICE_TABLE_NAME \
				"(qqid      int     NOT NULL," \
				"groupid   int     NOT NULL," \
				"source     text    NOT NULL," \
				"primary    key    (QQID,GROUPID));"

	manual_dice_control * manual_dice_control::instance = nullptr;

	manual_dice_control::manual_dice_control()
	{
		database::database_manager * databaseControl = database::database_manager::get_instance();
		int i_ret_code = databaseControl->register_table(MANUALDICE_TABLE_NAME, MANUALDICE_TABLE_DEFINE);
		is_no_sql_mode = i_ret_code != SQLITE_OK;
		map_manualdicemap = new TYPE_MAP_MANUALDICE();

		if (manual_dice_control::instance != nullptr) {
			delete(manual_dice_control::instance);
		}
		else manual_dice_control::instance = this;
	}


	manual_dice_control::~manual_dice_control()
	{
		delete(map_manualdicemap);
		if (manual_dice_control::instance == this) {
			manual_dice_control::instance = nullptr;
		}
	}

	manual_dice * manual_dice_control::create_manual_dice(const int64_t i_QQID, const int64_t i_QQGroupID, const std::string & command)
	{
		manual_dice md_manualdice(command);
		manual_dice * md_manualdice_ret = nullptr;
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

		if (!is_no_sql_mode) {
			sqlite3 * database = database::database_manager::get_database();
			std::string str_encoded_manualdice(md_manualdice.endcode());
			std::ostringstream ostrs_sql_command(std::ostringstream::ate);
			ostrs_sql_command << "SELECT * FROM " MANUALDICE_TABLE_NAME " where qqid =" << i_QQID << " and groupid =" << i_QQGroupID;
			std::string str_manualdice_read;
			char * pchar_err_message = nullptr;
			int ret_code = sqlite3_exec(database, ostrs_sql_command.str().c_str(), &sqlite3_callback_query_manualdice, (void*)&str_manualdice_read, &pchar_err_message);
			if (ret_code == SQLITE_OK) {
				if (str_manualdice_read.length() > 0) {
					std::ostringstream ostrs_sql_command_2(std::ostringstream::ate);
					ostrs_sql_command_2.str("update " MANUALDICE_TABLE_NAME " set ");
					ostrs_sql_command_2 << " source ='" << str_encoded_manualdice << "'";
					ostrs_sql_command_2 << " where qqid =" << i_QQID << " and groupid =" << i_QQGroupID;
					int ret_code_2 = sqlite3_exec(database, ostrs_sql_command_2.str().c_str(), &database::database_manager::sqlite3_callback, (void*)&i_data_database_update, &pchar_err_message);
	#ifdef _DEBUG
					if (ret_code_2 != SQLITE_OK) {
						logger::log("dicebot manual_dice",std::string(sqlite3_errmsg(database)));
					}
	#endif
				}
				else {
					std::ostringstream ostrs_sql_command_2(std::ostringstream::ate);
					ostrs_sql_command_2.str("insert into " MANUALDICE_TABLE_NAME " values ( ");
					ostrs_sql_command_2 << i_QQID << ", " << i_QQGroupID << ", '" << str_encoded_manualdice << "'" << ");";
					int ret_code_2 = sqlite3_exec(database, ostrs_sql_command_2.str().c_str(), &database::database_manager::sqlite3_callback, (void*)&i_data_database_update, &pchar_err_message);
	#ifdef _DEBUG
					if (ret_code_2 != SQLITE_OK) {
						logger::log("dicebot manual_dice",std::string(sqlite3_errmsg(database)));
					}
	#endif
				}
			}
			else is_no_sql_mode = true;
		}

		return md_manualdice_ret;
	}

	manual_dice * manual_dice_control::roll_manual_dice(const int64_t i_QQID, const int64_t i_QQGroupID, const std::string & command)
	{
		manual_dice * pmd_manualdice = nullptr;
		char * pchar_err_message = nullptr;
		TYPE_MAP_MANUALDICE::iterator iter_manualdicemap = this->map_manualdicemap->find(TYPE_KEY_MANUALDICE(i_QQID, i_QQGroupID));
		if (iter_manualdicemap != this->map_manualdicemap->end()) {
			pmd_manualdice = &((*iter_manualdicemap).second);
			pmd_manualdice->roll(command);
			if (!is_no_sql_mode) {
				std::string str_encoded_manualdice(pmd_manualdice->endcode());
				sqlite3 * database = database::database_manager::get_database();
				std::ostringstream ostrs_sql_command_2(std::ostringstream::ate);
				ostrs_sql_command_2.str("update " MANUALDICE_TABLE_NAME " set ");
				ostrs_sql_command_2 << " source ='" << str_encoded_manualdice << "'";
				ostrs_sql_command_2 << " where qqid =" << i_QQID << " and groupid =" << i_QQGroupID;
				int ret_code_2 = sqlite3_exec(database, ostrs_sql_command_2.str().c_str(), &database::database_manager::sqlite3_callback, (void*)&i_data_database_update, &pchar_err_message);
	#ifdef _DEBUG
				if (ret_code_2 != SQLITE_OK) {
						logger::log("dicebot manual_dice",std::string(sqlite3_errmsg(database)));
				}
	#endif
			}
			return pmd_manualdice;
		}
		else {
			if (!is_no_sql_mode) {
				sqlite3 * database = database::database_manager::get_database();
				std::ostringstream ostrs_sql_command(std::ostringstream::ate);
				ostrs_sql_command << "SELECT * FROM " MANUALDICE_TABLE_NAME " where qqid =" << i_QQID << " and groupid =" << i_QQGroupID;
				std::string str_manualdice_read;
				char * pchar_err_message = nullptr;
				int ret_code = sqlite3_exec(database, ostrs_sql_command.str().c_str(), &sqlite3_callback_query_manualdice, (void*)&str_manualdice_read, &pchar_err_message);
				if (ret_code == SQLITE_OK) {
					if (str_manualdice_read.length() > 0) {
						pmd_manualdice = new manual_dice();
						pmd_manualdice->decode(str_manualdice_read);
						pmd_manualdice->roll(command);
						auto pair_insert_ret = this->map_manualdicemap->insert(TYPE_PAIR_MANUALDICE(TYPE_KEY_MANUALDICE(i_QQID, i_QQGroupID), *pmd_manualdice));
						delete(pmd_manualdice);
						return &((*(pair_insert_ret.first)).second);
					}
				}
				else is_no_sql_mode = true;
			}
		}
		return nullptr;
	}

	manual_dice * manual_dice_control::kill_manual_dice(const int64_t i_QQID, const int64_t i_QQGroupID, const std::string & command)
	{
		manual_dice * pmd_manualdice = nullptr;
		char * pchar_err_message = nullptr;
		TYPE_MAP_MANUALDICE::iterator iter_manualdicemap = this->map_manualdicemap->find(TYPE_KEY_MANUALDICE(i_QQID, i_QQGroupID));
		if (iter_manualdicemap != this->map_manualdicemap->end()) {
			pmd_manualdice = &((*iter_manualdicemap).second);
			pmd_manualdice->kill(command);
			if (!is_no_sql_mode) {
				std::string str_encoded_manualdice(pmd_manualdice->endcode());
				sqlite3 * database = database::database_manager::get_database();
				std::ostringstream ostrs_sql_command_2(std::ostringstream::ate);
				ostrs_sql_command_2.str("update " MANUALDICE_TABLE_NAME " set ");
				ostrs_sql_command_2 << " source ='" << str_encoded_manualdice << "'";
				ostrs_sql_command_2 << " where qqid =" << i_QQID << " and groupid =" << i_QQGroupID;
				int ret_code_2 = sqlite3_exec(database, ostrs_sql_command_2.str().c_str(), &database::database_manager::sqlite3_callback, (void*)&i_data_database_update, &pchar_err_message);
	#ifdef _DEBUG
				if (ret_code_2 != SQLITE_OK) {
					logger::log("dicebot manual_dice",std::string(sqlite3_errmsg(database)));
				}
	#endif
			}
			return pmd_manualdice;
		}
		else {
			if (!is_no_sql_mode) {
				sqlite3 * database = database::database_manager::get_database();
				std::ostringstream ostrs_sql_command(std::ostringstream::ate);
				ostrs_sql_command << "SELECT * FROM " MANUALDICE_TABLE_NAME " where qqid =" << i_QQID << " and groupid =" << i_QQGroupID;
				std::string str_manualdice_read;
				char * pchar_err_message = nullptr;
				int ret_code = sqlite3_exec(database, ostrs_sql_command.str().c_str(), &sqlite3_callback_query_manualdice, (void*)&str_manualdice_read, &pchar_err_message);
				if (ret_code == SQLITE_OK) {
					if (str_manualdice_read.length() > 0) {
						pmd_manualdice = new manual_dice();
						pmd_manualdice->decode(str_manualdice_read);
						pmd_manualdice->kill(command);
						auto pair_insert_ret = this->map_manualdicemap->insert(TYPE_PAIR_MANUALDICE(TYPE_KEY_MANUALDICE(i_QQID, i_QQGroupID), *pmd_manualdice));
						delete(pmd_manualdice);
						return &((*(pair_insert_ret.first)).second);
					}
				}
				else is_no_sql_mode = true;
			}
		}
		return nullptr;
	}

	manual_dice * manual_dice_control::add_manual_dice(const int64_t i_QQID, const int64_t i_QQGroupID, const std::string & command)
	{
		manual_dice * pmd_manualdice = nullptr;
		char * pchar_err_message = nullptr;
		TYPE_MAP_MANUALDICE::iterator iter_manualdicemap = this->map_manualdicemap->find(TYPE_KEY_MANUALDICE(i_QQID, i_QQGroupID));
		if (iter_manualdicemap != this->map_manualdicemap->end()) {
			pmd_manualdice = &((*iter_manualdicemap).second);
			pmd_manualdice->add(command);
			if (!is_no_sql_mode) {
				std::string str_encoded_manualdice(pmd_manualdice->endcode());
				sqlite3 * database = database::database_manager::get_database();
				std::ostringstream ostrs_sql_command_2(std::ostringstream::ate);
				ostrs_sql_command_2.str("update " MANUALDICE_TABLE_NAME " set ");
				ostrs_sql_command_2 << " source ='" << str_encoded_manualdice << "'";
				ostrs_sql_command_2 << " where qqid =" << i_QQID << " and groupid =" << i_QQGroupID;
				int ret_code_2 = sqlite3_exec(database, ostrs_sql_command_2.str().c_str(), &database::database_manager::sqlite3_callback, (void*)&i_data_database_update, &pchar_err_message);
	#ifdef _DEBUG
				if (ret_code_2 != SQLITE_OK) {
					logger::log("dicebot manual_dice",std::string(sqlite3_errmsg(database)));
				}
	#endif
			}
			return pmd_manualdice;
		}
		else {
			if (!is_no_sql_mode) {
				sqlite3 * database = database::database_manager::get_database();
				std::ostringstream ostrs_sql_command(std::ostringstream::ate);
				ostrs_sql_command << "SELECT * FROM " MANUALDICE_TABLE_NAME " where qqid =" << i_QQID << " and groupid =" << i_QQGroupID;
				std::string str_manualdice_read;
				char * pchar_err_message = nullptr;
				int ret_code = sqlite3_exec(database, ostrs_sql_command.str().c_str(), &sqlite3_callback_query_manualdice, (void*)&str_manualdice_read, &pchar_err_message);
				if (ret_code == SQLITE_OK) {
					if (str_manualdice_read.length() > 0) {
						pmd_manualdice = new manual_dice();
						pmd_manualdice->decode(str_manualdice_read);
						pmd_manualdice->kill(command);
						auto pair_insert_ret = this->map_manualdicemap->insert(TYPE_PAIR_MANUALDICE(TYPE_KEY_MANUALDICE(i_QQID, i_QQGroupID), *pmd_manualdice));
						delete(pmd_manualdice);
						return &((*(pair_insert_ret.first)).second);
					}
				}
				else is_no_sql_mode = true;
			}
		}
		return nullptr;
	}

	manual_dice * manual_dice_control::killall_manual_dice(const int64_t i_QQID, const int64_t i_QQGroupID)
	{
		manual_dice * pmd_manualdice = nullptr;
		char * pchar_err_message = nullptr;
		TYPE_MAP_MANUALDICE::iterator iter_manualdicemap = this->map_manualdicemap->find(TYPE_KEY_MANUALDICE(i_QQID, i_QQGroupID));
		if (iter_manualdicemap != this->map_manualdicemap->end()) {
			pmd_manualdice = &((*iter_manualdicemap).second);
			pmd_manualdice->killall();
			if (!is_no_sql_mode) {
				std::string str_encoded_manualdice(pmd_manualdice->endcode());
				sqlite3 * database = database::database_manager::get_database();
				std::ostringstream ostrs_sql_command_2(std::ostringstream::ate);
				ostrs_sql_command_2.str("update " MANUALDICE_TABLE_NAME " set ");
				ostrs_sql_command_2 << " source ='" << str_encoded_manualdice << "'";
				ostrs_sql_command_2 << " where qqid =" << i_QQID << " and groupid =" << i_QQGroupID;
				int ret_code_2 = sqlite3_exec(database, ostrs_sql_command_2.str().c_str(), &database::database_manager::sqlite3_callback, (void*)&i_data_database_update, &pchar_err_message);
	#ifdef _DEBUG
				if (ret_code_2 != SQLITE_OK) {
					logger::log("dicebot manual_dice",std::string(sqlite3_errmsg(database)));
				}
	#endif
			}
			return pmd_manualdice;
		}
		else {
			if (!is_no_sql_mode) {
				sqlite3 * database = database::database_manager::get_database();
				std::ostringstream ostrs_sql_command(std::ostringstream::ate);
				ostrs_sql_command << "SELECT * FROM " MANUALDICE_TABLE_NAME " where qqid =" << i_QQID << " and groupid =" << i_QQGroupID;
				std::string str_manualdice_read;
				char * pchar_err_message = nullptr;
				int ret_code = sqlite3_exec(database, ostrs_sql_command.str().c_str(), &sqlite3_callback_query_manualdice, (void*)&str_manualdice_read, &pchar_err_message);
				if (ret_code == SQLITE_OK) {
					if (str_manualdice_read.length() > 0) {
						pmd_manualdice = new manual_dice();
						pmd_manualdice->decode(str_manualdice_read);
						pmd_manualdice->killall();
						auto pair_insert_ret = this->map_manualdicemap->insert(TYPE_PAIR_MANUALDICE(TYPE_KEY_MANUALDICE(i_QQID, i_QQGroupID), *pmd_manualdice));
						delete(pmd_manualdice);
						return &((*(pair_insert_ret.first)).second);
					}
				}
				else is_no_sql_mode = true;
			}
		}
		return nullptr;
	}

	int manual_dice_control::sqlite3_callback_query_manualdice(void * data, int argc, char ** argv, char ** azColName)
	{
		if (argc == 3) {
			std::string * pstr_ret = (std::string *) data;
			*pstr_ret = std::string(argv[2]);
		}
		return SQLITE_OK;
	}
}