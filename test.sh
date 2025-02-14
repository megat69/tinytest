#!/bin/bash

USUAL_COLOR='\033[0;32m'
FAILURE_COLOR='\033[0;31m'
NEUTRAL_COLOR='\033[0;33m'
NO_COLOR='\033[0m'

BIN_DIRECTORY="./build"
TEST_ARGUMENTS=""  # Arguments to pass on to each test binary, such as -q, -s, -v, or -e

if [ $# -ne 0 ] && [[ $1 == -* ]]; then
    TEST_ARGUMENTS="$1"
    shift
fi

if [ $# -eq 0 ]; then
    ALL_EXECUTABLES_TEMP="$(find "${BIN_DIRECTORY}" -maxdepth 1 -type f -name "test_*")"
    ALL_EXECUTABLES=""
    for EXECUTABLE in $ALL_EXECUTABLES_TEMP
    do
        ALL_EXECUTABLES="${ALL_EXECUTABLES} ${EXECUTABLE#"${BIN_DIRECTORY}/test_"}"
    done

    if [ -z "$ALL_EXECUTABLES" ]; then
        echo -e "${FAILURE_COLOR}No test executable name to test provided, and none in test folder.${NO_COLOR}"
        exit 1
    fi
else
    ALL_EXECUTABLES="$@"
fi

if [ "$1" = "-h" ]; then
    echo -e "Runs through each provided test."
    echo -e "Tests are provided through command line arguments, being the name of any test executable, minus the 'test_' prefix.\n"
    echo -e "Example : ./tests.sh [dash-args (-q|-v|-s|-e)] tinytest1"
    echo -e "\tWill run the executable '$BIN_DIRECTORY/test_tinytest1'"
    echo -e "If no name is provided, will run every test in the test directory."
    echo -e "The dash-args (-q, -v, -s, -e) will be passed to each test, and correspond to TinyTest command line arguments."
    exit 0
fi

# Goes through each test provided through the command line
for EXECUTABLE in $ALL_EXECUTABLES
do
    # Runs the test
    echo -e "${USUAL_COLOR}Running tests of project '${NEUTRAL_COLOR}${EXECUTABLE}${USUAL_COLOR}'${NO_COLOR}"
    "$BIN_DIRECTORY/test_$EXECUTABLE" $TEST_ARGUMENTS
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
