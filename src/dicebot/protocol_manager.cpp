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
    this->identifier_regex = "h(?:help)?";
    this->identifier_list = {"help","h"};
    this->help_message = base64::decode(
        "RGljZUJvdCBieSBkeW5pbGF0aCB2MS42LjAK6K6/6Zeu6aG555"
        "uu5Li76aG1aHR0cDovL2dpdGh1Yi5jb20vZHluaWxhdGgvY29v"
        "bHEtZGljZWJvdArojrflvpfkvb/nlKjor7TmmI7ku6Xlj4rkuo"
        "bop6Pmm7TlpJrlhoXlrrnjgIIK5L2/55So5pa55byP566A5LuL"
        "77yaCi5yIDFkOCsxZDYrMyDmma7pgJrpqrDlrZAKLmMgY29j6a"
        "qw5a2QCi53bzQgb3dvZOmqsOWtkAoud240IG53b2TpqrDlrZAK"
        "LmYgRkFURemqsOWtkAror6bnu4bmjIfku6Tlj4LogIPor7fliY"
        "3lvoDpobnnm67kuLvpobXjgIIK5aaC5p6c5Zyo5L2/55So5Lit"
        "6YGH5Yiw5LqGYnVn77yM5oiW6ICF5pyJ5LuA5LmI5Yqf6IO95b"
        "u66K6u77yM5qyi6L+O5Zyo6aG555uu5Li76aG16YCa6L+HaXNz"
        "dWXlj43ppojvvIzmhJ/osKLmgqjnmoTluK7liqnjgII=");
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
