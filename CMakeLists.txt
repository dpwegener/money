cmake_minimum_required(VERSION 3.0.0)
project(Money VERSION 0.1.0)

find_package(Catch2 CONFIG REQUIRED)

add_subdirectory(lib)

add_executable(money-test 
    money-test-main.cpp
    money-types-test.cpp
)

target_link_libraries(money-test
     money-lib 
     Catch2::Catch2
)

include(CTest)
enable_testing()
include(Catch)
catch_discover_tests(money-test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
