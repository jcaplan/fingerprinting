#!/bin/bash

igen.exe --batch platform.tcl --writec platform --writexml platform.xml
make NOVLNV=1