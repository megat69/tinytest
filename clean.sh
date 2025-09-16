#!/bin/bash

CleanBinaries() {
    rm -f ./build/test_*
    echo "Cleaned binaries"
}

CleanBuildFiles() {
    rm -rf ./build/*
    echo "Cleaned build files"
}

CleanDocs() {
    rm -rf ./doc/html/
    rm -rf ./doc/latex/
    echo "Cleaned documentation files"
}

if [ -z "$1" ] || [ "$1" = "all" ]
then
    CleanBinaries
    CleanBuildFiles
    #CleanDocs
    exit 0
else
    for ARGUMENT in "$@"
    do
        case "$ARGUMENT" in
            bin)
                CleanBinaries ;;
            build)
                CleanBuildFiles ;;
            doc|docs|documentation)
               CleanDocs ;;
            *)
                echo "Argument inconnu : \"$ARGUMENT\"" >&2 ; exit 1
        esac
    done
fi
