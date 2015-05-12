#!/bin/bash

LIST=`find CompiledC/*/ -regextype posix-extended -regex .*[\.][ch]`

echo "$LIST"

cp -f $LIST rtos/critical_library/

rm -f rtos/critical_library/ert_main.c