#pragma once

#include "./common.h"
#include "./profile.h"

namespace dicebot::profile{
    using sptr_user_profile = std::shared_ptr<user_profile>;
    using profile_map = std::map<int64_t, sptr_user_profile>;
    using profile_pair = std::pair<int64_t, sptr_user_profile>;
    namespace profile_db{
        bool read_database(user_profile & profile, int64_t const user_id);
        bool write_database(user_profile const & profile, int64_t const user_id);
        bool insert_database(user_profile const & profile, int64_t const user_id);
        bool update_database(user_profile const & profile, int64_t const user_id);
        bool exist_database(int64_t const user_id);
        int sqlite3_callback_query_profile(void * data, int argc, char ** argv, char ** azColName);
    }

    class profile_manager : profile_map{
        static profile_manager * instance;
    public:
        profile_manager() noexcept;
        ~profile_manager();

        static profile_manager * get_instance(){
            return profile_manager::instance;
        }

        template<class _Key_t, class _Val_t>
        bool get_value(_Key_t const & key, _Val_t & var, int64_t const user_id){
            auto iter = this->find(user_id);
            if(iter != this->end()){
                return iter->second->get_var<_Key_t,_Val_t>(key,var);
            }
            else{
                sptr_user_profile upf = std::make_shared<user_profile>();
                if(is_no_sql_mode){
                    auto t = this->insert(profile_pair(user_id,upf));
                    if(!t.second) return false;
                }
                else if(profile_db::read_database(*upf,user_id)){
                    auto t = this->insert(profile_pair(user_id,upf));
                    if(!t.second) return false;
                    if( t.first->second->get_var<_Key_t,_Val_t>(key,var)){
                        profile_db::write_database(*(t.first->second),user_id);
                        return true;
                    }
                }
            }
            return false;
        }

        template<class _Key_t, class _Val_t>
        bool set_value(_Key_t const & key, _Val_t const & var, int64_t const user_id){
            auto iter = this->find(user_id);
            if(iter != this->end()){
                if(iter->second->set_var<_Key_t,_Val_t>(key,var)){
                    if(!is_no_sql_mode) profile_db::write_database(*(iter->second),user_id);
                    return true;
                }
            }
            else{
                sptr_user_profile upf = std::make_shared<user_profile>();
                if(is_no_sql_mode){
                    auto t = this->insert(profile_pair(user_id,upf));
                    if(!t.second) return false;
                }
                else if(profile_db::read_database(*upf,user_id)){
                    auto t = this->insert(profile_pair(user_id,upf));
                    if(!t.second) return false;
                    if( t.first->second->set_var<_Key_t,_Val_t>(key,var)){
                        profile_db::write_database(*(t.first->second),user_id);
                        return true;
                    }
                }
            }
            return false;    
        }

        bool reset_var(int64_t const user_id){
            auto iter = this->find(user_id);
            if(iter != this->end()){
                if(iter->second->reset_var()){
                    if(!is_no_sql_mode) profile_db::write_database(*(iter->second),user_id);
                    return true;
                }
            }
            else{
                sptr_user_profile upf = std::make_shared<user_profile>();
                if(is_no_sql_mode){
                    auto t = this->insert(profile_pair(user_id,upf));
                    if(!t.second) return false;
                }
                else if(profile_db::read_database(*upf,user_id)){
                    auto t = this->insert(profile_pair(user_id,upf));
                    if(!t.second) return false;
                    if(t.first->second->reset_var()){
                        profile_db::write_database(*(t.first->second),user_id);
                        return true;
                    }
                }
            }
            return false;
        }

        bool force_update(int64_t const user_id){
            if(is_no_sql_mode) return false;

            auto iter = this->find(user_id);
            if(iter == this->end()) return false;
            else{
                return profile_db::write_database(*(iter->second),user_id);
            }
            return false;
        }

        sptr_user_profile get_profile(int64_t const user_id){
            auto iter = this->find(user_id);
            if(iter != this->end()){
                return iter->second;
            }
            else{
                if(is_no_sql_mode) return nullptr;

                sptr_user_profile upf = std::make_shared<user_profile>();
                if(profile_db::read_database(*upf,user_id)){
                    auto t = this->insert(profile_pair(user_id,upf));
                    if(!t.second) return nullptr;
                    if(t.first->second->reset_var()){
                        profile_db::write_database(*(t.first->second),user_id);
                        return upf;
                    }
                }
                return nullptr;
            }
        }
    };
}