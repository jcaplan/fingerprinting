#!/bin/bash

mkdir -p figures

for file in *.dot; do

   dot -Tpdf ${file} -o "figures/${file%.*}.pdf"

done


pdflatex codegen.tex 
bibtex codegen.aux
pdflatex codegen.tex 



exit 0
