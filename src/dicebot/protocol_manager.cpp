#include "./protocol_manager.h"

#include <memory>

using namespace dicebot;

void protocol_manager::register_dice(protocol::p_protocol protocol){
    for(std::string const & s : protocol->identifier_list){
        auto iter = this->protocol_map.find(s);
        if(iter != protocol_map.end()) continue;
        protocol_map.insert(
            protocol_pair_t(s,protocol)
        );
    }

    protocol_regex_list.push_back(protocol->identifier_regex);
}

void protocol_manager::create_command_regex(){
    ostrs ostrs_stream(ostrs::ate);
    ostrs_stream << "^(";
    bool is_first = true;
    auto iter = this->protocol_regex_list.cbegin();
    for(; iter != this->protocol_regex_list.cend(); iter++){
        if(is_first){
            is_first = false;
        }
        else ostrs_stream << "|";
        ostrs_stream << *iter;
    }
    ostrs_stream << ") *";
    this->regex_command = 
        std::regex(ostrs_stream.str(), std::regex_constants::icase);
}

protocol::p_protocol protocol_manager::get_protocol(std::string command) const{
    std::transform(command.begin(),command.end(),command.begin(),tolower);
    auto iter = this->protocol_map.find(command);
    if(iter == protocol_map.cend()) return nullptr;
    else return (*iter).second;
}

std::regex const * protocol_manager::get_regex_command() const{
    return &(this->regex_command);
}