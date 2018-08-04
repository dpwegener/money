cmake_minimum_required(VERSION 3.0.0)
project(Money VERSION 0.1.0)

add_subdirectory(lib)

include(CTest)
enable_testing()

add_executable(money-test 
    money-test-main.cpp
    money-types-test.cpp
)

target_link_libraries(money-test PRIVATE money-lib)

add_test(NAME MoneyTest COMMAND money-test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
