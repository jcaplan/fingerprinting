#!/bin/bash

for file in "fprint" "no_fprint"
do
	dot -Tpdf ${file}T.dot -o ${file}T.pdf
	gnome-open ${file}T.pdf 
	latex ${file}.tex
	dvips ${file}.dvi 
	ps2pdf ${file}.ps 
	gnome-open ${file}.pdf
done
