#!/bin/bash

# add chmod +x clean.sh to allow it to wipe your directory clean from files created by logger.

rm -f dump*

rm -f logs.txt

rm tests_out

rm -f cmake-build-debug/dump*

rm -f cmake-build-debug/logs.txt