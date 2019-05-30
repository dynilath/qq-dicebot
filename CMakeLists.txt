cmake_minimum_required(VERSION 3.9)
project(coolq-dicebot)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_TEST_SIGNATURE MATCHES "^linux64_test$")
  set(CMAKE_CXX_FLAGS "/utf-8 ${CMAKE_CXX_FLAGS}")  # use UTF-8 source files
  set(CMAKE_CXX_FLAGS "/MP ${CMAKE_CXX_FLAGS}")  # build with object level parallelism
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# include(cotire)
include(FindVcpkgIncludeDir)

if(CMAKE_BUILD_TYPE MATCHES "^Debug$")
  # fix Vcpkg debug library lookup bug, see https://github.com/Microsoft/vcpkg/issues/1626
  list(APPEND CMAKE_IGNORE_PATH ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib)
endif()

# with cqdicebot triplet, gtest should be linked like this
# appveyor is properly installed with gtest, there is no problem with it
if(NOT CMAKE_CI_CONFIG MATCHES "^Appveyor$" AND NOT CMAKE_TEST_SIGNATURE MATCHES "^linux64_test$")
  message(STATUS "Using link_directories() for Local envir.")
  if(CMAKE_BUILD_TYPE MATCHES "^Debug$")
    link_directories(${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/Debug/lib)
  else()
    link_directories(${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib)
  endif()
endif()

include_directories(${VCPKG_INCLUDE_DIR})
include_directories(src)

add_definitions(-DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
                -D_WIN32_WINNT=0x0501
                -DWIN32_LEAN_AND_MEAN
                -DNOMINMAX)

find_package(unofficial-iconv REQUIRED)
find_package(SQlite3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS serialization)

enable_testing()
include(GoogleTest)

if(CMAKE_CI_CONFIG MATCHES "^Appveyor$" OR CMAKE_TEST_SIGNATURE MATCHES "^linux64_test$") 
  find_package(GTest REQUIRED)
  message(STATUS "Using find_package() for Appveyor.")
endif()

include(FixLinkConflict)

file(GLOB_RECURSE DICEBOT_SOURCE src/dicebot/*.cpp src/dicebot/*/*.cpp)

# build target
if(NOT CMAKE_TEST_SIGNATURE MATCHES "^linux64_test$")
  file(GLOB_RECURSE SOURCE_FILES src/main.cpp src/cqsdk/*.cpp)
  set(LIB_NAME "com.dynilath.coolqdicebot")
  add_library(${LIB_NAME} SHARED ${SOURCE_FILES} ${DICEBOT_SOURCE})
  target_link_libraries(${LIB_NAME} unofficial::iconv::libiconv unofficial::iconv::libcharset)
  target_link_libraries(${LIB_NAME} sqlite3)
  target_link_libraries(${LIB_NAME} ${Boost_LIBRARIES})
  # cotire(${LIB_NAME})
endif()

set(BASE64SRC src/cqsdk/utils/vendor/cpp-base64/base64.cpp)

# build test target
file(GLOB_RECURSE HTEST_SOURCE src/tests/htest.cpp ${BASE64SRC})
set(HTEST_NAME "dicebot")
add_executable(${HTEST_NAME} ${HTEST_SOURCE} ${DICEBOT_SOURCE})
target_link_libraries(${HTEST_NAME} sqlite3)
target_link_libraries(${HTEST_NAME} ${Boost_LIBRARIES})

# build tests
set(TEST_NAME_GROUP 
  roll_test 
  entry_test
  number_test
  data_test)

# roll test
set(X roll_test)
set(TEST_BUILD_NAME dicebot.gtest.${X})
file(GLOB_RECURSE TEST_SOURCE_ROLL_TEST
  src/tests/${X}.cpp
  src/dicebot/utils/number.cpp
  src/dicebot/random/*.cpp
  src/dicebot/entity/manual_dice.cpp
  src/dicebot/dice_roller.cpp
  src/dicebot/utils/utils.cpp
  ${BASE64SRC})
add_executable(${TEST_BUILD_NAME} ${TEST_SOURCE_ROLL_TEST})

# protocol test
set(X entry_test)
set(TEST_BUILD_NAME dicebot.gtest.${X})
add_executable(${TEST_BUILD_NAME}
  src/tests/${X}.cpp 
  ${BASE64SRC} 
  ${DICEBOT_SOURCE})

# number test
set(X number_test)
set(TEST_BUILD_NAME dicebot.gtest.${X})
add_executable(${TEST_BUILD_NAME} 
  src/tests/${X}.cpp
  src/dicebot/utils/number.cpp)

# data test
set(X data_test)
set(TEST_BUILD_NAME dicebot.gtest.${X})
add_executable(${TEST_BUILD_NAME} 
  ${BASE64SRC}
  src/tests/${X}.cpp
  ${DICEBOT_SOURCE})

foreach(X IN LISTS TEST_NAME_GROUP)
  set(TEST_BUILD_NAME dicebot.gtest.${X})
  target_link_libraries(${TEST_BUILD_NAME} sqlite3)
  target_link_libraries(${TEST_BUILD_NAME} ${Boost_LIBRARIES}) 

# link gtest
  if(CMAKE_CI_CONFIG MATCHES "^Appveyor$" OR CMAKE_TEST_SIGNATURE MATCHES "^linux64_test$")
    target_link_libraries(${TEST_BUILD_NAME} GTest::GTest)
  else()
    if(CMAKE_BUILD_TYPE MATCHES "^Debug$")
      target_link_libraries(${TEST_BUILD_NAME} gtestd)
    else()
      target_link_libraries(${TEST_BUILD_NAME} gtest)
    endif()
  endif()
endforeach(X IN TEST_NAME_GROUP)

gtest_add_tests(TARGET ${TEST_BUILD_NAME}
  TEST_LIST testList
)

# post build script
if(NOT CMAKE_TEST_SIGNATURE MATCHES "^linux64_test$")
  add_custom_command(TARGET ${LIB_NAME}
                    POST_BUILD
                    COMMAND
                    powershell -ExecutionPolicy Bypass -NoProfile -File "${PROJECT_SOURCE_DIR}/scripts/post_build.ps1" ${LIB_NAME} "$<TARGET_FILE_DIR:${LIB_NAME}>")
endif()
