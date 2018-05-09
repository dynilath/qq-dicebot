// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>


// TODO: reference additional headers your program requires here
#include "stdint.h"
#include "sqlite3.h"
#include <string>
#include <map>
#include <sstream>
#include <vector>

#define NICK_TABLE_NAME "nickname"
#define APP_DIR "./app/com.dynilath.coolqdicebot/"
#define MAX_DICE_UNIT_COUNT 10
#define MAX_DICE_NUM 50
#define MAX_DICE_FACE 500