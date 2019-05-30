#pragma once

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <initializer_list>
#include <sstream>

#include "../../cqsdk/utils/vendor/cpp-base64/base64.h"
#include "../constants.h"
#ifdef _DEBUG
#include "../utils/logger.h"
#endif

namespace dicebot::profile {

    class var_pair : public std::pair<int, int> {
    public:
        friend class boost::serialization::access;

        template <class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & this->first;
            ar & this->second;
        }

        var_pair(int first, int second) : std::pair<int, int>(first, second) {}
        var_pair(){};
    };

    const var_pair var_rs_on(1, 1);
    const var_pair var_rs_off(1, 0);

#pragma region profile template

    template <class _profile_Key, class _profile_Val, class _container_t = std::unordered_map<_profile_Key, _profile_Val>>
    class profile_template : public _container_t {
    public:
        profile_template(){};
        profile_template(std::initializer_list<typename _container_t::value_type> l) : _container_t(l){};

        profile_status get(_profile_Key const &var_type, _profile_Val &var) const noexcept {
            auto target = this->find(var_type);
            if (target != this->cend()) {
                var = target->second;
                return profile_status::finished;
            } else
                return profile_status::not_exist;
        }

        profile_status set(_profile_Key const &var_type, _profile_Val const &new_var) noexcept {
            auto target = this->find(var_type);
            if (target != this->end()) {
                target->second = new_var;
                return profile_status::finished;
            } else {
                auto t = this->insert(typename _container_t::value_type(var_type, new_var));
                if (t.second)
                    return profile_status::finished;
                else
                    return profile_status::not_exist;
            }
        }

        profile_status add(_profile_Key const &var_type, _profile_Val const &new_var) noexcept {
            auto target = this->find(var_type);
            if (target == this->end()) {
                this->insert(typename _container_t::value_type(var_type, new_var));
                return profile_status::finished;
            } else
                return profile_status::exceed;
        }

        std::string encode() const noexcept {
            std::ostringstream strs;
            boost::archive::binary_oarchive oa(strs);
            oa << this->size();
            auto iter_list = this->cbegin();
            for (; iter_list != this->cend(); iter_list++) {
                oa << ((*iter_list).first);
                oa << ((*iter_list).second);
            }
            return base64_encode((const unsigned char *)(strs.str().c_str()), strs.str().size());
        }

        bool decode(std::string const &source) noexcept {
            this->clear();
            // this->swap(std::map<_profile_Key,_profile_Val>());
            std::string source_copy(source);
            source_copy = base64_decode(source_copy);
            std::istringstream iss(source_copy);
            boost::archive::binary_iarchive ia(iss);
            size_t len = 0;
            ia >> len;
            try {
                for (size_t i_iter = 0; i_iter < len; i_iter++) {
                    _profile_Key first;
                    ia >> first;
                    _profile_Val second;
                    ia >> second;
                    this->insert(typename _container_t::value_type(first, second));
                }
            } catch (std::exception e) {
#ifdef _DEBUG
                logger::log("dicebot profile", std::string(e.what()));
#endif
                return false;
            }
            return true;
        }
    };
#pragma endregion

#pragma region user profile
    class user_profile {
    public:
        using sys_var_map_t = profile_template<sys_var_type, var_pair>;
        using def_roll_map_t = profile_template<def_roll_type, std::string>;
        using mac_roll_map_t = profile_template<std::string, std::string>;
        sys_var_map_t sys_vars = {{sys_var_type::rs_on, var_rs_on}};
        def_roll_map_t def_roll;
        mac_roll_map_t mac_rolls;
        bool encode(std::vector<std::string> &target) const {
            target.clear();
            target.push_back(this->sys_vars.encode());
            target.push_back(this->def_roll.encode());
            target.push_back(this->mac_rolls.encode());
            return true;
        }

        bool decode(char **argv) {
            if (!this->sys_vars.decode(argv[0])) return false;
            if (!this->def_roll.decode(argv[1])) return false;
            if (!this->mac_rolls.decode(argv[2])) return false;
            return true;
        }
    };

#pragma endregion
} // namespace dicebot::profile