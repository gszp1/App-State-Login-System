cmake_minimum_required(VERSION 3.27)
project(App_State_Login_System C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(App_State_Login_System
        app_state_logger.c
        app_state_logger.h
        logger_tests.c)
