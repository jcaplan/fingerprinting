#!/bin/bash


for i in {0..1} M
do
    pushd ucos${i}
    make all
    popd
done

