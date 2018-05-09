#pragma once

#include <string>

struct CQ_TYPE_QQ
{
	int64_t							QQID;	//QQ号
	std::string						nick;	//昵称
	int								sex;	//性别
	int								age;	//年龄
};

struct CQ_Type_GroupMember
{
	int64_t				GroupID;					// 群号
	int64_t			   	QQID;						// QQ号
	std::string			username;					// QQ昵称
	std::string			nick;						// 群名片
	int					sex;						// 性别 0/男 1/女
	int					age;						// 年龄
	std::string         area;						// 地区
	std::string			jointime;					// 入群时间
	int				lastsent;					// 上次发言时间
	std::string			level_name;					// 头衔名字
	int				permission;					// 权限等级 1/成员 2/管理员 3/群主
	bool				unfriendly;					// 不良成员记录
	std::string			title;						// 自定义头衔
	int				titleExpiretime;			// 头衔过期时间
	bool				nickcanchange;				// 管理员是否能协助改名
};
class CQTool
{
public:
	CQTool();
	~CQTool();

	bool GetStrangerInfo(int auth, int64_t qqid, CQ_TYPE_QQ& StrangerInfo);
	bool CQ_Tools_TextToStranger(std::string decodeStr, CQ_TYPE_QQ& StrangerInfo);

	bool GetGroupMemberInfo(int auth, int64_t groupID, int64_t qqid, CQ_Type_GroupMember& groupMember);
	void AddLog(int32_t AuthCode, int32_t priority, const char *category, const char *content);
	bool getDefaultName(const int32_t i_AuthCode, const int64_t uint64_fromGroupOrDiscuss, const int64_t uint64_fromQQ, std::string & str_origin_name, bool isfromGroup);
private:
	bool CQ_Tools_TextToGroupMember(std::string decodeStr, CQ_Type_GroupMember& groupMember);
	int64_t CoolQ_Tools_GetNum(int index, long length, const std::string &decodeStr);
	std::string CoolQ_Tools_GetStr(int index, const std::string &decodeStr);
	int64_t From256to10(std::string str256);

	int authCode;
};