#!/bin/bash

set -xe

PROJECT="drone"
CURR_DIR="$(pwd)"
MAIN_CHECK="$(echo ${CURR_DIR} | rev | cut -d / -f 1 | rev)"

if [ ${MAIN_CHECK} = "main" ]; then
    pushd ..
    idf.py build	
    popd
elif [ ${MAIN_CHECK} = ${PROJECT} ]; then
    idf.py build
else
	echo idk where you are
fi
