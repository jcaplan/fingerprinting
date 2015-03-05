#!/bin/bash

for file in *.dot; do

   dot -Tpdf ${file} -o "${file%.*}.pdf"

done


pdflatex codegen.tex 

pdflatex codegen.tex 



exit 0
