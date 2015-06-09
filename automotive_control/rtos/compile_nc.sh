#!/bin/bash


for i in {0..1} Mon
do
    pushd ucos${i}
    make all
    popd
done

