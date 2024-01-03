#!/bin/bash

# run chmod +x run_tests.sh in console to allow it to compile and run tests.

gcc app_state_logger.c app_state_logger.h logger_tests.c -o tests_out

./tests_out