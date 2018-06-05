#pragma once
class nickManager
{
public:
	nickManager() noexcept;
	~nickManager();
	static nickManager * instance;
	void getNickName(const int i_AuthCode, const int64_t fromGroupOrDiscuss, const int64_t fromQQ, std::string &nickname, bool isfromGroup);
	void setNickName(const int i_AuthCode, const int64_t fromGroupOrDiscuss, const int64_t fromQQ, const std::string &nickname, bool isfromGroup);
private:
	std::map<std::pair<int64_t, int64_t>, std::string> * map_nickname_cache;
	bool is_no_sql_mode = false;
	static int sqlite3_callback_query_name(void * data, int argc, char ** argv, char ** azColName);
};

