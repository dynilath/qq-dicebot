#pragma once

#include "./common.h"
#include "./protocol_base.h"
#include "./profile.h"
namespace dicebot::protocol{
	class protocol_set_roll : public protocol_base
	{
		std::regex filter_name;
	public:
		protocol_set_roll();
		bool resolve_request(
			std::string const & message,
			event_info & event,
			std::string & response) override;
	};

	class protocol_set_var : public protocol_base
	{
	private:
		std::regex filter_var;
		std::regex filter_command;
		std::regex filter_name;
	public:
		protocol_set_var();
		bool resolve_request(
			std::string const & message,
			event_info & event,
			std::string & response) override;
	};

    class protocol_list : public protocol_base{
        using list_call_t = std::function<bool(
            protocol_list const & self,
            std::string const &,
            event_info const &,
            std::string &)>;
        
        using call_map_t = std::map<std::string,list_call_t>;
        using call_map_value_t = call_map_t::value_type;

        using gen_var_t = std::function<void(
            profile::user_profile::user_var_map_type const &,
            std::string const &,
            std::string const &,
            output_constructor &)>;

        using gen_macro_t = std::function<void(
            profile::user_profile::mac_roll_map_type const &,
            std::string const &,
            std::string const &,
            output_constructor &)>;

        using gen_defr_t = std::function<void(
            profile::user_profile::def_roll_map_type const &,
            std::string const &,
            output_constructor &)>;

    private:
        std::regex filter_command;
        call_map_t call_map;
    public:
        static gen_var_t var_msg;
        static gen_macro_t macro_msg;
        static gen_defr_t defr_msg;
        protocol_list();
        bool resolve_request(
            std::string const & message,
            event_info & event,
            std::string & response) override;
    };

    class protocol_delete : public protocol_base{
        using delete_call_t = std::function<bool(
            protocol_delete const & self,
            std::string const &,
            event_info const &,
            std::string &)>;
        using call_map_t = std::map<std::string,delete_call_t>;
        using call_map_value_t = call_map_t::value_type;
    private:
        std::regex filter_command;
        call_map_t call_map;
    public:
        protocol_delete();
        bool resolve_request(
            std::string const & message,
            event_info & event,
            std::string & response) override;
    };
}