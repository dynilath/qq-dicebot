#include "stdafx.h"
#include "QTool.h"
#include "cqp.h"
#include "Base64.h"
#include <math.h>

CQTool::CQTool()
{
}


CQTool::~CQTool()
{
}

bool CQTool::GetStrangerInfo(int auth, int64_t qqid, CQ_TYPE_QQ& StrangerInfo)
{
	std::string encodeStr = CQ_getStrangerInfo(auth, qqid, true);
	if (0 == encodeStr.size())
	{
		return false;
	}
	std::string decodeStr;
	Base64::Decode(encodeStr.c_str(), &decodeStr);
	if (decodeStr.size() < 20)
	{
		CQ_addLog(auth, CQLOG_ERROR, "CQTool::GetStrangerInfo", "decodeStr.size() < 20");
		return false;
	}
	return CQ_Tools_TextToStranger(decodeStr, StrangerInfo);
}

bool CQTool::CQ_Tools_TextToStranger(std::string decodeStr, CQ_TYPE_QQ& StrangerInfo)
{
	int i = 0;
	int count = 8;

	StrangerInfo.QQID = CoolQ_Tools_GetNum(i, count, decodeStr);
	i += count;

	StrangerInfo.nick = CoolQ_Tools_GetStr(i, decodeStr);
	i += 2 + StrangerInfo.nick.size();

	count = 4;
	StrangerInfo.sex = (int)CoolQ_Tools_GetNum(i, 4, decodeStr);
	i += count;

	count = 4;
	StrangerInfo.age = (int)CoolQ_Tools_GetNum(i, 4, decodeStr);
	i += count;

	return true;
}

bool CQTool::GetGroupMemberInfo(int auth, int64_t groupID, int64_t qqid, CQ_Type_GroupMember& groupMember)
{
	std::string encodeStr = CQ_getGroupMemberInfoV2(auth, groupID, qqid, false);
	if (0 == encodeStr.size())
	{
		return false;
	}
	std::string decodeStr;
	Base64::Decode(encodeStr.c_str(), &decodeStr);
	if (decodeStr.size() < 40)
	{
		CQ_addLog(auth, CQLOG_ERROR, "CQTool::GetGroupMemberInfo", "decodeStr.size() < 40");
		return false;
	}
	return CQ_Tools_TextToGroupMember(decodeStr, groupMember);
}

bool CQTool::CQ_Tools_TextToGroupMember(std::string decodeStr, CQ_Type_GroupMember& groupMember)
{
	int i = 0;
	int count = 8;
	groupMember.GroupID = CoolQ_Tools_GetNum(i, count, decodeStr);
	i += count;

	count = 8;
	groupMember.QQID = CoolQ_Tools_GetNum(i, count, decodeStr);
	i += count;

	groupMember.username = CoolQ_Tools_GetStr(i, decodeStr);
	i += 2 + groupMember.username.size(); //2 means string length bit

	groupMember.nick = CoolQ_Tools_GetStr(i, decodeStr);
	i += 2 + groupMember.nick.size();

	count = 4;
	groupMember.sex = (int)CoolQ_Tools_GetNum(i, 4, decodeStr);
	i += count;

	count = 4;
	groupMember.age = (int)CoolQ_Tools_GetNum(i, 4, decodeStr);
	i += count;

	groupMember.area = CoolQ_Tools_GetStr(i, decodeStr);
	i += 2 + groupMember.area.size();

	count = 4;
	groupMember.jointime = (int)CoolQ_Tools_GetNum(i, 4, decodeStr);
	i += count;

	count = 4;
	groupMember.lastsent = (int)CoolQ_Tools_GetNum(i, 4, decodeStr);
	i += count;

	groupMember.level_name = CoolQ_Tools_GetStr(i, decodeStr);
	i += 2 + groupMember.level_name.size();

	count = 4;
	groupMember.permission = (int)CoolQ_Tools_GetNum(i, 4, decodeStr);
	i += count;

	count = 4;
	groupMember.unfriendly = (CoolQ_Tools_GetNum(i, 4, decodeStr) == 1);
	i += count;

	groupMember.title = CoolQ_Tools_GetStr(i, decodeStr);
	i += 2 + groupMember.title.size();

	count = 4;
	groupMember.titleExpiretime = (int)CoolQ_Tools_GetNum(i, 4, decodeStr);
	i += count;

	count = 4;
	groupMember.nickcanchange = (CoolQ_Tools_GetNum(i, 4, decodeStr) == 1);
	i += count;

	return true;
}

int64_t CQTool::CoolQ_Tools_GetNum(int index, long length, const std::string &decodeStr)
{
	std::string subString = decodeStr.substr(index, length);
	return From256to10(subString);
}

std::string CQTool::CoolQ_Tools_GetStr(int index, const std::string &decodeStr)
{
	int length = (int)CoolQ_Tools_GetNum(index, 2, decodeStr);
	if (length == 0)
	{
		return "";
	}

	return decodeStr.substr(index + 2, length);
}


int64_t CQTool::From256to10(std::string str256)
{
	int64_t sum = 0;
	for (int i = 0; i < (int)str256.size(); i++)
	{
		sum = sum * 256 + (unsigned char)str256[i];
	}
	return sum;
}

void CQTool::AddLog(int32_t AuthCode, int32_t priority, const char *category, const char *content)
{
	CQ_addLog(AuthCode, priority, category, content);
}


bool CQTool::getDefaultName(const int32_t i_AuthCode, const int64_t uint64_fromGroupOrDiscuss, const int64_t uint64_fromQQ, std::string &str_origin_name, bool isfromGroup)
{
	if (isfromGroup) {
		CQ_Type_GroupMember groupMember;
		if (GetGroupMemberInfo(i_AuthCode, uint64_fromGroupOrDiscuss, uint64_fromQQ, groupMember)) {
			if (groupMember.nick.length() == 0) str_origin_name = groupMember.username;
			else str_origin_name = groupMember.nick;
			if (str_origin_name.length() == 0) str_origin_name = std::to_string(uint64_fromQQ);
			return true;
		}
	}
	else {
		CQ_TYPE_QQ discussMember;
		if (GetStrangerInfo(i_AuthCode, uint64_fromQQ, discussMember)) {
			str_origin_name = discussMember.nick;
			if (str_origin_name.length() == 0) str_origin_name = std::to_string(uint64_fromQQ);
			return true;
		}
	}
	return false;
}