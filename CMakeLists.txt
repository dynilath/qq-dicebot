cmake_minimum_required(VERSION 3.11)
project(dicebot)

set(CMAKE_CXX_STANDARD 17)
set(VCPKG_TARGET_ARCHITECTURE x86)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)

include_directories(src)
include_directories(extern/cqcppsdk/include)

add_definitions(-D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)

# fix sqlite3 include and link for appveyor
find_path(SQLITE_INCLUDE sqlite3.h)
include_directories(${SQLITE_INCLUDE})
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set(SQLITE_LIB_DIR ${SQLITE_INCLUDE}/../debug/lib/)
else()
  set(SQLITE_LIB_DIR ${SQLITE_INCLUDE}/../lib/)
endif()
link_directories(${SQLITE_LIB_DIR})

include(extern/cqcppsdk/cqcppsdk.cmake)

find_package(sqlite3 REQUIRED)

file(GLOB_RECURSE DICEBOT_SOURCE src/dicebot/**.cpp)

file(GLOB_RECURSE SOURCE_FILES src/main.cpp)

file(READ "app_id.txt" APP_ID)
string(STRIP "${APP_ID}" APP_ID)

cq_set_app_id(${APP_ID})

set(LIB_NAME "app")

if(MSVC AND (CMAKE_SIZEOF_VOID_P EQUAL 4))
    cq_add_app(${LIB_NAME} ${SOURCE_FILES} ${DICEBOT_SOURCE})
    target_link_libraries(${LIB_NAME} sqlite3)
    cq_add_install_script(${LIB_NAME} "${PROJECT_SOURCE_DIR}/scripts/install.ps1")
endif()

set(CQCPPSDK_DEV_MODE ON)
cq_add_app(${LIB_NAME}_dev ${SOURCE_FILES} ${DICEBOT_SOURCE})
target_link_libraries(${LIB_NAME}_dev sqlite3)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_link_libraries(${LIB_NAME}_dev stdc++fs)
endif()

include(src/tests/test.cmake)
