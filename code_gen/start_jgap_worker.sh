#!/bin/bash

CLASSPATH=.:${JGAP_HOME}/lib/log4j.jar:${JGAP_HOME}/jgap.jar:${JGAP_HOME}/lib/commons-cli-1.2.jar:${JGAP_HOME}/lib/jcgrid.jar
CLASSPATH=${CLASSPATH}:/home/jonah/fingerprinting/code_gen/CodeGen/bin

# java -cp ${CLASSPATH} org.jgap.distr.grid.JGAPServer

java -cp ${CLASSPATH} org.jgap.distr.grid.JGAPWorker -s localhost -n myworker1