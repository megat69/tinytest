#!/bin/bash

USUAL_COLOR='\033[0;32m'
FAILURE_COLOR='\033[0;31m'
NEUTRAL_COLOR='\033[0;33m'
NO_COLOR='\033[0m'

BIN_DIRECTORY="./build"

if [ $# -eq 0 ]; then
    echo -e "${FAILURE_COLOR}No test executable name to test provided.${NO_COLOR}"
    exit 1
fi

if [ $1 == "-h" ]; then
    echo -e "Runs through each provided test."
    echo -e "Tests are provided through command line arguments, being the name of any test executable, minus the 'test_' prefix.\n"
    echo -e "Example : ./tests.sh tinytest1"
    echo -e "\tWill run the executable '$BIN_DIRECTORY/test_tinytest1'"
    exit 0
fi

# Goes through each test provided through the command line
for EXECUTABLE in "$@"
do
    # Runs the test
    echo -e "${USUAL_COLOR}Running tests of project '${NEUTRAL_COLOR}${EXECUTABLE}${USUAL_COLOR}'${NO_COLOR}"
    "$BIN_DIRECTORY/test_$EXECUTABLE" $@
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
done
