#include "./protocol_manager.h"
#include "../cqsdk/utils/base64.h"

namespace base64 = cq::utils::base64;
using namespace dicebot;
using namespace protocol;

void protocol_manager::register_dice(protocol::p_protocol protocol){
    for(std::string const & s : protocol->identifier_list){
        auto iter = this->protocol_cmd_map.find(s);
        if(iter != this->protocol_cmd_map.end()) continue;

        this->protocol_cmd_map.insert(
            protocol_pair_t(s,protocol)
        );
    }
    protocol_list.push_back(protocol);
}

void protocol_manager::finish_initialization(){
    auto protocol_help = std::make_shared<protocol::protocol_help>();
    for(const protocol::p_protocol & prot : this->protocol_list){
        protocol_help->register_help(prot);
    }
    protocol_help->generate_filter_command();
    this->register_dice(protocol_help);

    ostrs ostrs_stream(ostrs::ate);
    ostrs_stream << "^(";
    bool is_first = true;
    auto iter = this->protocol_list.cbegin();
    for(; iter != this->protocol_list.cend(); iter++){
        if(is_first){
            is_first = false;
        }
        else ostrs_stream << "|";
        ostrs_stream << (*iter)->identifier_regex;
    }
    ostrs_stream << ") *";
    this->regex_command = 
        std::regex(ostrs_stream.str(), std::regex_constants::icase);
}

protocol::p_protocol const protocol_manager::get_protocol(std::string command) const{
    std::transform(command.begin(),command.end(),command.begin(),tolower);
    auto iter = this->protocol_cmd_map.find(command);
    if(iter == protocol_cmd_map.cend()) return nullptr;
    else return (*iter).second;
}

std::regex const * protocol_manager::get_regex_command() const{
    return &(this->regex_command);
}

protocol_help::protocol_help(){
    this->is_stand_alone = true;
    this->identifier_regex = "h(?:elp)?";
    this->identifier_list = {"help","h"};
    this->help_message = base64::decode(
        "RGljZUJvdCBieSBkeW5pbGF0aCB2MS42LjIgcHJl"
        "dmlldwrkvb/nlKjmlrnlvI/nroDku4vvvJoKLnIg"
        "MWQ4KzFkNiszIOaZrumAmumqsOWtkAouYyBjb2Pp"
        "qrDlrZAKLndvNCBvd29k6aqw5a2QCi53bjQgbndv"
        "ZOmqsOWtkAouZiBGQVRF6aqw5a2QCuabtOWkmuaM"
        "h+S7pO+8mnNldCB2YXIgbGlzdCBkZWxldGUgbWFu"
        "dWFsIG5hbWUK5L2/55SoIC5oZWxwIFvmjIfku6Tl"
        "kI3np7BdIOadpeiOt+W+l+WvueW6lOaMh+S7pOea"
        "hOW4ruWKqQoK6K6/6Zeu6aG555uu5Li76aG1aHR0"
        "cDovL2dpdGh1Yi5jb20vZHluaWxhdGgvY29vbHEt"
        "ZGljZWJvdArojrflvpfkvb/nlKjor7TmmI7ku6Xl"
        "j4rkuobop6Pmm7TlpJrlhoXlrrnjgIIK5aaC5p6c"
        "5Zyo5L2/55So5Lit6YGH5Yiw5LqGYnVn77yM5oiW"
        "6ICF5pyJ5LuA5LmI5Yqf6IO95bu66K6u77yM5qyi"
        "6L+O5Zyo6aG555uu5Li76aG16YCa6L+HaXNzdWXl"
        "j43ppojvvIzmhJ/osKLmgqjnmoTluK7liqnjgII="
        );
}

bool protocol_help::register_help(p_protocol v_protocol){
    this->protocol_regex_list.push_back(v_protocol->identifier_regex);
    for(const std::string & var: v_protocol->identifier_list){
        auto i = this->help_map.insert(help_pair_t(var,v_protocol->help_message));
        if(!i.second) return false;
    }
    return true;
}

void protocol_help::generate_filter_command(){
    ostrs ostrs_stream(ostrs::ate);
    ostrs_stream << "^(";
    bool is_first = true;
    auto iter = this->protocol_regex_list.cbegin();
    for(; iter != this->protocol_regex_list.cend(); iter++){
        if(is_first){
            is_first = false;
        }
        else ostrs_stream << "|";
        ostrs_stream << (*iter);
    }
    ostrs_stream << ")";
    this->filter_command = 
        std::regex(ostrs_stream.str(), std::regex_constants::icase);
}

bool protocol_help::resolve_request(
    std::string const & message,
    event_info & event,
    std::string & response){

    std::smatch m;
    std::regex_search(message,m,this->filter_command);

    if(m.size()==0){
        response.assign(this->help_message);
        return true;
    }
    else{
        auto iter = this->help_map.find(m[1]);
        if(iter != this->help_map.end()){
            response.assign(iter->second);
            return true;
        }
        else return false;
    }
}
