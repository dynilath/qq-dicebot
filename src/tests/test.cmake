enable_testing()
include(GoogleTest)

find_package(GTest REQUIRED)

set(BASE64SRC 
    extern/cqcppsdk/src/utils/vendor/cpp-base64/base64.cpp
    extern/cqcppsdk/src/utils/base64.cpp
)

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

# entry test
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
    src/tests/${X}.cpp
    ${BASE64SRC}
    ${DICEBOT_SOURCE})
  
foreach(X IN LISTS TEST_NAME_GROUP)
    set(TEST_BUILD_NAME dicebot.gtest.${X})
    target_link_libraries(${TEST_BUILD_NAME} sqlite3)
    target_link_libraries(${TEST_BUILD_NAME} GTest::GTest)
endforeach(X IN TEST_NAME_GROUP)

gtest_add_tests(TARGET ${TEST_BUILD_NAME}
    TEST_LIST testList
)