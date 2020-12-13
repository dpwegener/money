cmake_minimum_required(VERSION 3.10.0)
project(MoneyLib VERSION 0.0.1 LANGUAGES CXX)

add_library(money-lib INTERFACE)

target_include_directories(money-lib
    INTERFACE inc/
)

target_compile_features( money-lib 
    INTERFACE cxx_std_14
)
