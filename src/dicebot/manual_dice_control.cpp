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

	manual_dice_control::manual_dice_control(){
		//map = new manual_map();

		database::database_manager * databaseControl = database::database_manager::get_instance();
		int i_ret_code = databaseControl->register_table(MANUALDICE_TABLE_NAME, MANUALDICE_TABLE_DEFINE);
		is_no_sql_mode = i_ret_code != SQLITE_OK;

		if (manual_dice_control::instance != nullptr) {
			delete(manual_dice_control::instance);
		}
		else manual_dice_control::instance = this;
	}


	manual_dice_control::~manual_dice_control(){
		//if(map != nullptr) delete map;

		if (manual_dice_control::instance == this) {
			manual_dice_control::instance = nullptr;
		}
	}

	p_manual manual_dice_control::create_manual_dice(const int64_t user_id, const int64_t group_id, const std::string & command){
		p_manual pmd_manual_dice = std::make_shared<manual_dice>(command);
		if(!(*pmd_manual_dice)) return nullptr;

		manual_kpair pair = manual_kpair(user_id,group_id);

		manual_map::iterator iter = find(pair);
		if(iter != this->end()){
			erase(iter);
		}
		this->insert(manual_pair(pair, pmd_manual_dice));

		if (!is_no_sql_mode) {
			if(exist_database(pair)){
				update_database(pair,pmd_manual_dice);
			}
			else insert_database(pair,pmd_manual_dice);
		}

		return pmd_manual_dice;
	}

	p_manual manual_dice_control::roll_manual_dice(const int64_t user_id, const int64_t group_id, const std::string & command){
		manual_kpair pair = manual_kpair(user_id,group_id);
		auto find_ret = this->find(pair);
		if(find_ret != this->end()){
			p_manual pmd_manualdice = find_ret->second;
			pmd_manualdice->roll(command);
			if (!is_no_sql_mode) {
				this->update_database(pair,pmd_manualdice);
			}
			return pmd_manualdice;
		}
		else{
			p_manual pmd_manualdice = std::make_shared<manual_dice>();
			if (!is_no_sql_mode && this->read_database(pair,pmd_manualdice)) {
				pmd_manualdice->roll(command);
				this->update_database(pair,pmd_manualdice);
			}
			this->insert(manual_pair(pair,pmd_manualdice));
			return pmd_manualdice;
		}
	}

	p_manual manual_dice_control::kill_manual_dice(const int64_t user_id, const int64_t group_id, const std::string & command){
		manual_kpair pair = manual_kpair(user_id,group_id);
		auto find_ret = this->find(pair);

		if(find_ret != this->end()){
			p_manual pmd_manualdice = find_ret->second;
			pmd_manualdice->kill(command);
			if (!is_no_sql_mode) {
				this->update_database(pair,pmd_manualdice);
			}
			return pmd_manualdice;
		}
		else{
			p_manual pmd_manualdice = std::make_shared<manual_dice>();
			if (!is_no_sql_mode && this->read_database(pair,pmd_manualdice)) {
				pmd_manualdice->kill(command);
				this->update_database(pair,pmd_manualdice);
			}
			this->insert(manual_pair(pair,pmd_manualdice));
			return pmd_manualdice;
		}
	}

	p_manual manual_dice_control::add_manual_dice(const int64_t user_id, const int64_t group_id, const std::string & command){
		manual_kpair pair = manual_kpair(user_id,group_id);
		auto find_ret = this->find(pair);

		if(find_ret != this->end()){
			p_manual pmd_manualdice = find_ret->second;
			pmd_manualdice->add(command);
			if (!is_no_sql_mode) {
				this->update_database(pair,pmd_manualdice);
			}
			return pmd_manualdice;
		}
		else{
			p_manual pmd_manualdice = std::make_shared<manual_dice>();
			if (!is_no_sql_mode && this->read_database(pair,pmd_manualdice)) {
				pmd_manualdice->add(command);
				this->update_database(pair,pmd_manualdice);
			}
			this->insert(manual_pair(pair,pmd_manualdice));
			return pmd_manualdice;
		}
	}

	p_manual manual_dice_control::killall_manual_dice(const int64_t user_id, const int64_t group_id){
		manual_kpair pair = manual_kpair(user_id,group_id);
		auto find_ret = this->find(pair);

		if(find_ret != this->end()){
			p_manual pmd_manualdice = find_ret->second;
			pmd_manualdice->killall();
			if (!is_no_sql_mode) {
				this->update_database(pair,pmd_manualdice);
			}
			return pmd_manualdice;
		}
		else{
			p_manual pmd_manualdice = std::make_shared<manual_dice>();
			if (!is_no_sql_mode && this->read_database(pair,pmd_manualdice)) {
				pmd_manualdice->killall();
				this->update_database(pair,pmd_manualdice);
			}
			this->insert(manual_pair(pair,pmd_manualdice));
			return pmd_manualdice;
		}
	}

	int manual_dice_control::sqlite3_callback_query_manualdice(void * data, int argc, char ** argv, char ** azColName){
		if (argc == 3) {
			std::string * pstr_ret = (std::string *) data;
			*pstr_ret = std::string(argv[2]);
		}
		return SQLITE_OK;
	}

	bool manual_dice_control::insert_database(manual_kpair manual_dice_key,p_manual manual_dice_target){
		sqlite3 * database = database::database_manager::get_database();
		char * pchar_err_message = nullptr;
		std::string str_encoded_manualdice(manual_dice_target->endcode());
		ostrs ostrs_sql_command(ostrs::ate);
		ostrs_sql_command.str("insert into " MANUALDICE_TABLE_NAME " values ( ");
		ostrs_sql_command << manual_dice_key.first << ", " << manual_dice_key.second << ", '" << str_encoded_manualdice << "'" << ");";
		int ret_code_2 = sqlite3_exec(database, ostrs_sql_command.str().c_str(), &database::database_manager::sqlite3_callback, (void*)&i_data_database_update, &pchar_err_message);
		#ifdef _DEBUG
		if (ret_code_2 != SQLITE_OK) {logger::log("dicebot insert_database",std::string(sqlite3_errmsg(database)));}
		#endif
		return ret_code_2 == SQLITE_OK;
	}

	bool manual_dice_control::update_database(manual_kpair manual_dice_key,p_manual manual_dice_target){
		sqlite3 * database = database::database_manager::get_database();
		char * pchar_err_message = nullptr;
		std::string str_encoded_manualdice(manual_dice_target->endcode());

		ostrs ostrs_sql_command(ostrs::ate);
		ostrs_sql_command.str("update " MANUALDICE_TABLE_NAME " set ");
		ostrs_sql_command << " source ='" << str_encoded_manualdice << "'";
		ostrs_sql_command << " where qqid =" << manual_dice_key.first << " and groupid =" << manual_dice_key.second;
		int ret_code_2 = sqlite3_exec(database, ostrs_sql_command.str().c_str(), &database::database_manager::sqlite3_callback, (void*)&i_data_database_update, &pchar_err_message);
		#ifdef _DEBUG
		if (ret_code_2 != SQLITE_OK) {logger::log("dicebot update_database",std::string(sqlite3_errmsg(database)));}
		#endif
		return ret_code_2 == SQLITE_OK;
	}

	bool manual_dice_control::read_database(manual_kpair manual_dice_key,p_manual manual_dice_target){
		sqlite3 * database = database::database_manager::get_database();
		std::string str_encoded_manualdice(manual_dice_target->endcode());
		ostrs ostrs_sql_command(ostrs::ate);
		ostrs_sql_command << "SELECT * FROM " MANUALDICE_TABLE_NAME " where qqid =" << manual_dice_key.first << " and groupid =" << manual_dice_key.second;
		std::string str_manualdice_read;
		char * pchar_err_message = nullptr;
		int ret_code = sqlite3_exec(database, ostrs_sql_command.str().c_str(), &sqlite3_callback_query_manualdice, (void*)&str_manualdice_read, &pchar_err_message);
		if (ret_code == SQLITE_OK) {
			if (str_manualdice_read.length() > 0) {
				manual_dice_target->decode(str_manualdice_read);
				return true;
			}else return false;
		}
		else {
			#ifdef _DEBUG
			{logger::log("dicebot read_database",std::string(sqlite3_errmsg(database)));}
			#endif
			this->is_no_sql_mode = true;
			return false;
		}
	}

	bool manual_dice_control::exist_database(manual_kpair manual_dice_key){
		sqlite3 * database = database::database_manager::get_database();
		ostrs ostrs_sql_command(ostrs::ate);
		ostrs_sql_command << "SELECT * FROM " MANUALDICE_TABLE_NAME " where qqid =" << manual_dice_key.first << " and groupid =" << manual_dice_key.second;
		std::string str_manualdice_read;
		char * pchar_err_message = nullptr;
		int ret_code = sqlite3_exec(database, ostrs_sql_command.str().c_str(), &sqlite3_callback_query_manualdice, (void*)&str_manualdice_read, &pchar_err_message);
		#ifdef _DEBUG
		if (ret_code != SQLITE_OK) {logger::log("dicebot exist_database",std::string(sqlite3_errmsg(database)));}
		#endif
		return ret_code == SQLITE_OK;
	}
}