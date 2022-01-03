cmake_minimum_required(VERSION 3.11)
project(dicebot)

set(CMAKE_CXX_STANDARD 17)
set(VCPKG_TARGET_ARCHITECTURE x86)

include_directories(src)
include_directories(extern/cqcppsdk/include)

add_definitions(-D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# fix sqlite3 include and link for appveyor
find_path(SQLITE_INCLUDE sqlite3.h)
message(SQLITE_INCLUDE is ${SQLITE_INCLUDE})
include_directories(${SQLITE_INCLUDE})
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set(SQLITE_LIB_DIR ${SQLITE_INCLUDE}/../debug/lib/)
else()
  set(SQLITE_LIB_DIR ${SQLITE_INCLUDE}/../lib/)
endif()
link_directories(${SQLITE_LIB_DIR})

find_package(SQLite3 REQUIRED)

file(GLOB_RECURSE CORN_SOURCE extern/corncppsdk/**.cc)

file(GLOB_RECURSE DICEBOT_SOURCE src/dicebot/**.cpp)

file(GLOB_RECURSE SOURCE_FILES src/main.cpp src/oicq_impl.cpp)

set(LIB_NAME "dicebot")
add_library(${LIB_NAME} SHARED ${DICEBOT_SOURCE} ${SOURCE_FILES} ${CORN_SOURCE})
target_link_libraries(${LIB_NAME} PRIVATE SQLite3)

include(src/tests/test.cmake)

