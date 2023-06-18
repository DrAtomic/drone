#!/bin/bash

set -xe

PROJECT="drone"
BUILD_DIR="build"

if [ -z "${IDF_PATH}" ]
then
    echo "idf not in environment"
    exit 1
fi

head_tail() {
    HEAD=$(echo "$1" | rev | cut -d '/' -f1 | rev)
    TAIL=$(echo "$1" | rev | cut -d '/' -f2- | rev)
}

CUR_DIR=$(pwd)
head_tail $CUR_DIR

while [[ $HEAD != $PROJECT ]]
do
    echo "not in here"
    cd $TAIL
    head_tail $TAIL
done

echo "in the base project dir"
pushd $BUILD_DIR
cmake ..
cmake --build .
popd

if [ -f "compile_commands.json" ]
then
    rm compile_commands.json
fi

ln -s $BUILD_DIR/compile_commands.json compile_commands.json
