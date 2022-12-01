enable_testing()
include(GoogleTest)

find_package(GTest REQUIRED)

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
    src/dicebot/utils/utils.cpp)
add_executable(${TEST_BUILD_NAME} ${TEST_SOURCE_ROLL_TEST})
gtest_discover_tests(${TEST_BUILD_NAME})

# entry test
set(X entry_test)
set(TEST_BUILD_NAME dicebot.gtest.${X})
add_executable(${TEST_BUILD_NAME}
    src/tests/${X}.cpp 
    ${DICEBOT_SOURCE})
gtest_discover_tests(${TEST_BUILD_NAME})
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/entry_test_db)
target_compile_definitions(${TEST_BUILD_NAME} PRIVATE DB_FOLDER="${CMAKE_BINARY_DIR}/entry_test_db")

# number test
set(X number_test)
set(TEST_BUILD_NAME dicebot.gtest.${X})
add_executable(${TEST_BUILD_NAME} 
    src/tests/${X}.cpp
    src/dicebot/utils/number.cpp)
gtest_discover_tests(${TEST_BUILD_NAME})

# data test
set(X data_test)
set(TEST_BUILD_NAME dicebot.gtest.${X})
add_executable(${TEST_BUILD_NAME} 
    src/tests/${X}.cpp
    ${DICEBOT_SOURCE})
gtest_discover_tests(${TEST_BUILD_NAME})
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data_test_db_1)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data_test_db_2)
target_compile_definitions(${TEST_BUILD_NAME} PRIVATE DB_FOLDER_1="${CMAKE_BINARY_DIR}/data_test_db_1")
target_compile_definitions(${TEST_BUILD_NAME} PRIVATE DB_FOLDER_2="${CMAKE_BINARY_DIR}/data_test_db_2")
target_compile_definitions(${TEST_BUILD_NAME} PRIVATE DB_FOLDER_3="${CMAKE_BINARY_DIR}/data_test_db_3")
target_compile_definitions(${TEST_BUILD_NAME} PRIVATE DB_FOLDER_4="${CMAKE_BINARY_DIR}/data_test_db_4")
    
foreach(X IN LISTS TEST_NAME_GROUP)
    set(TEST_BUILD_NAME dicebot.gtest.${X})
    target_link_libraries(${TEST_BUILD_NAME} sqlite3)
    target_link_libraries(${TEST_BUILD_NAME} GTest::GTest)
    target_compile_options(${TEST_BUILD_NAME} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/utf-8>)
endforeach(X IN TEST_NAME_GROUP)