#!/bin/bash

if [ $# -eq 1 ] ;then 
	file=$1
	inkscape -D -z --file=${file}.svg --export-pdf=${file}.pdf

fi
