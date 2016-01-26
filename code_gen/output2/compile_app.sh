#!/bin/bash


NIOS2COMMANDSHELL=/home/jonah/altera/13.1/nios2eds/nios2_command_shell.sh

for i in {0..1} M
do
    pushd cpu${i}
    ${NIOS2COMMANDSHELL} make all
    popd
done

