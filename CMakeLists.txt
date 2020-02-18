cmake_minimum_required(VERSION 3.9)
project(cqcppsdk-demo)

set(CMAKE_CXX_STANDARD 17)
set(VCPKG_TARGET_ARCHITECTURE x86)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)

include_directories(src)
include_directories(extern/cqcppsdk/include)
include_directories(${VCPKG_INCLUDE_DIR})

include(extern/cqcppsdk/cqcppsdk.cmake)

find_package(SQlite3 REQUIRED)

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

include(src/tests/test.cmake)
