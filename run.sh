#!/bin/bash

if [ $# -eq 0 ]; then
  EXECUTABLE=test_letters
else
  EXECUTABLE=$1
  shift
fi

USUAL_COLOR='\033[0;32m'
FAILURE_COLOR='\033[0;31m'
NEUTRAL_COLOR='\033[0;33m'
NO_COLOR='\033[0m'
echo -e "${USUAL_COLOR}Running project...${NO_COLOR}"
"./build/$EXECUTABLE" $@
EXIT_CODE=$?
if [ $EXIT_CODE == 0 ]
then
echo -e "${USUAL_COLOR}Program exited with code $EXIT_CODE${NO_COLOR}"
else
echo -e "${NEUTRAL_COLOR}Program exited with code ${FAILURE_COLOR}$EXIT_CODE${NO_COLOR}"
fi
if [ $EXIT_CODE == 139 ]
then
    echo -e "${FAILURE_COLOR}Program crashed. Cleaning terminal...${NO_COLOR}"
    read -n 1 -s
    stty sane
    clear
fi