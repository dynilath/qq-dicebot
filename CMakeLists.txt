cmake_minimum_required(VERSION 3.9)
project(coolq-dicebot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "/utf-8 ${CMAKE_CXX_FLAGS}")  # use UTF-8 source files
set(CMAKE_CXX_FLAGS "/MP ${CMAKE_CXX_FLAGS}")  # build with object level parallelism

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

include(cotire)
include(FindVcpkgIncludeDir)
include(GoogleTest)

if(CMAKE_BUILD_TYPE MATCHES "^Debug$")
    # fix Vcpkg debug library lookup bug, see https://github.com/Microsoft/vcpkg/issues/1626
    list(APPEND CMAKE_IGNORE_PATH ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib)
endif()

include_directories(${VCPKG_INCLUDE_DIR})
include_directories(src)

add_definitions(-DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
                -D_WIN32_WINNT=0x0501
                -DWIN32_LEAN_AND_MEAN
				-DNOMINMAX)

find_package(Iconv REQUIRED)
find_package(Sqlite3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS serialization)
find_package(GTest REQUIRED)

include(FixLinkConflict)

set(LIB_NAME "com.dynilath.coolqdicebot")

add_library(${LIB_NAME} src/main.cpp)

target_link_libraries(${LIB_NAME} ${ICONV_AND_DEPS_LIBRARIES})
target_link_libraries(${LIB_NAME} sqlite3)
target_link_libraries(${LIB_NAME} ${Boost_LIBRARIES})

cotire(${LIB_NAME})

add_custom_command(TARGET ${LIB_NAME}
                   POST_BUILD
                   COMMAND
                   powershell -ExecutionPolicy Bypass -NoProfile -File "${PROJECT_SOURCE_DIR}/scripts/post_build.ps1" ${LIB_NAME} "$<TARGET_FILE_DIR:${LIB_NAME}>")

set(TEST_BUILD_NAME "dicebot.gtest")
add_executable(${TEST_BUILD_NAME}  src/test.cpp)
target_link_libraries(${TEST_BUILD_NAME} ${ICONV_AND_DEPS_LIBRARIES})
target_link_libraries(${TEST_BUILD_NAME} sqlite3)
target_link_libraries(${TEST_BUILD_NAME} ${Boost_LIBRARIES})
target_link_libraries(${TEST_BUILD_NAME} GTest::Main)

gtest_add_tests(TARGET ${TEST_BUILD_NAME}
  TEST_LIST testList
)