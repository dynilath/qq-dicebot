#include <cstdint>
#include <string>

struct PrivateMessageEventData {
    int64_t user_id;
    std::string user_nickname;
    std::string raw_msg;
};

struct GroupMessageEventData {
    int64_t group_id;
    int64_t user_id;
    std::string user_nickname;
    std::string raw_msg;
};

struct DiscussMessageEventData {
    int64_t discuss_id;
    int64_t user_id;
    std::string user_nickname;
    std::string raw_msg;
};

extern void on_enable(const char *);
extern void on_disable();
extern void on_private_message(PrivateMessageEventData);
extern void on_group_message(GroupMessageEventData);
extern void on_discuss_message(DiscussMessageEventData);

extern void send_private_message(int64_t, const std::string&);
extern void send_group_message(int64_t, const std::string&);
extern void send_discuss_message(int64_t, const std::string&);
extern void log_info(const std::string&);
extern void log_warn(const std::string&);