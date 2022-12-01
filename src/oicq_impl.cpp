#include "oicq_func.hpp"

#define GENERAL_DLL_FUNC(_FUNC) \
    extern "C" __declspec(dllexport) void func_##_FUNC

using msg_callback_t = void(*)(int64_t, const char *);

#define OICQ_CALLBACK(_CALLBACK_MAME, _CALL) \
using _CALLBACK_MAME##_call_t = _CALL; \
_CALLBACK_MAME##_call_t _CALLBACK_MAME;


namespace impl{

OICQ_CALLBACK(send_private_message, msg_callback_t)
OICQ_CALLBACK(send_group_message, msg_callback_t)
OICQ_CALLBACK(send_discuss_message, msg_callback_t)
OICQ_CALLBACK(log_info, void(*)(const char *))
OICQ_CALLBACK(log_warn, void(*)(const char *))

}

#define FUNC_SET_CALL_BACK(_FUNC_PTR) GENERAL_DLL_FUNC(set_##_FUNC_PTR)(decltype(impl::_FUNC_PTR) para){impl::_FUNC_PTR = para;}

FUNC_SET_CALL_BACK(send_private_message)
FUNC_SET_CALL_BACK(send_group_message)
FUNC_SET_CALL_BACK(send_discuss_message)
FUNC_SET_CALL_BACK(log_info)
FUNC_SET_CALL_BACK(log_warn)


GENERAL_DLL_FUNC(enable)(const char * path)
{
    on_enable(path);
}

GENERAL_DLL_FUNC(disable)()
{
    on_disable();
}

GENERAL_DLL_FUNC(private_message)(int64_t user_id, const char * user_nickname, const char * raw_message)
{
    on_private_message({user_id,user_nickname,raw_message});
}

GENERAL_DLL_FUNC(group_message)(int64_t group_id, int64_t user_id, const char * user_nickname, const char * raw_message)
{
    on_group_message({group_id, user_id, user_nickname, raw_message});
}

GENERAL_DLL_FUNC(discuss_message)(int64_t discuss_id, int64_t user_id, const char * user_nickname, const char * raw_message)
{
    on_discuss_message({discuss_id, user_id, user_nickname, raw_message});
}

void send_private_message(int64_t p1, const std::string& p2){
    impl::send_private_message(p1,p2.c_str());
}
void send_group_message(int64_t p1, const std::string& p2){
    impl::send_group_message(p1,p2.c_str());
}
void send_discuss_message(int64_t p1, const std::string& p2){
    impl::send_discuss_message(p1,p2.c_str());
}
void log_info(const std::string& p1){
    impl::log_info(p1.c_str());
}
void log_warn(const std::string& p1){
    impl::log_warn(p1.c_str());
}