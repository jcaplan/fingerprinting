#!/usr/bin/python

import sys
from os import walk
import os

def main(argv):
	files = []
	inputDir = argv[0]
	outputDir = argv[1]


	for (dirpath, dirnames, filenames) in walk(inputDir):
	    files.extend(filenames)
	    break


	for filename in files:

		fullName = inputDir
		if not fullName.endswith("/"):
			fullName += "/"
		fullName += filename

		inputFile = open(fullName)
		print inputFile.name

		randomSeed = 157

		fileList = []
		for loopLimit in range(10000,100001,10000):
			for randomRange in range(0,100001,10000):
				newName = "for_loop_" + str(loopLimit) + "_" + str(randomRange)
				fullOutputDir = outputDir + "/" + newName + "/"
				if not os.path.exists(fullOutputDir):
			   		os.makedirs(fullOutputDir)
			   	newFileName = filename.replace('for_loop',newName)
				fileList.append( \
						{'file' : open(fullOutputDir + newFileName, 'w'), \
						'name' : newName, \
						'loopLimit' : loopLimit, \
						'randomRange' : randomRange \
						} \
					)


		for line in inputFile.readlines():

			for outputFile in fileList:
				newline = line
				if '%LOOP_LIMIT' in line:
					newline = line.replace(r'%%LOOP_LIMIT',str(outputFile['loopLimit']))
				if '%RANDOM_RANGE' in line:
					newline = line.replace(r'%%RANDOM_RANGE',str(outputFile['randomRange']))
				if '%RANDOM_SEED' in line:
					newline = line.replace(r'%%RANDOM_SEED',str(randomSeed))
				if 'for_loop' in line:
					newline = line.replace('for_loop',outputFile['name'])
				if outputFile['randomRange'] == 0 and ' + rand()%RANDOM_RANGE' in line:
					newline = line.replace(' + rand()%RANDOM_RANGE','')
				if 'ANNOT_MAXITER(LOOP_LIMIT + RANDOM_RANGE);' in line:
					newline = '\t\tANNOT_MAXITER(' + str(outputFile['randomRange'] + outputFile['loopLimit']) + ');\n'
				outputFile['file'].write(newline)

		for f in fileList:
			f['file'].close()





if __name__ == "__main__":
   main(sys.argv[1:])




