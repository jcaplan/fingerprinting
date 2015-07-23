#!/usr/bin/python

import sys
import re

version = []
comments = []
timescale = []
signals = []
times = []

def main(argv):

	i = 0
	coreNames = ["cpu0","cpu1","cpum"]
	for name in coreNames:
		f = open("FprintPlatform_" + name + ".ivcd", 'r')
		lines = f.readlines()

		version.append(getVersion(lines))
		comments.append(getComments(lines))
		timescale.append(getTimeScale(lines))
		signals.append(getSignals(lines,name))
		times.append(getTimes(lines))
		f.close()
	
	#update the symbols
	newSignalDic,newSignals = formatSignals()
	updatedTimes = updateTimes(newSignalDic)
	#sort by timestamp
	updatedTimes.sort(key=lambda x: x[0])	
	mergedTimes = mergeTimeList(updatedTimes)


	#print out the merged file

	f = open("platform.vcd",'w')
	f.write("$version\n")
	f.write(version[0])
	f.write("\n$end\n")
	f.write("$comment merged vcds for all cores $end\n")
	f.write("$timescale " + timescale[0] + " $end\n")
	f.write("$scope module platform $end\n")
	for sig in newSignals:
		f.write(" $var wire 1 " + sig[1] + " " + sig[0] + " $end\n")
	f.write("$upscope $end\n$enddefinitions $end\n")


	for time in mergedTimes:
		f.write("#" + str(time[0]) + "\n")
		for event in time[1]:
			f.write(event + "\n")
	f.close()




def getVersion(lines):
	for i in xrange(len(lines)):
		line = lines[i]
		line = line.strip()
		if line	== "$version":
			line =lines[i+1]
			line = line.strip()
			return line
	return "",0		


def getComments(lines):
	comments = []
	for i in xrange(len(lines)):
		line = lines[i]
		line = line.strip()
		if line.startswith("$comment"):
			comments.append(line)
		elif(line.startswith("$timescale")):
			return (comments,i)

def getTimeScale(lines):
	for i in xrange(len(lines)):
		line = lines[i]
		line = line.strip()
		if(line.startswith("$timescale")):
			line = line.split(" ")
			return line[1]


def getSignals(lines,name):
	signals = {}

	for i in xrange(len(lines)):
		line = lines[i]
		line = line.strip()
		if(line.startswith("$var")):
			line = line.split(" ")
			signals[name + "_" + re.sub(r'[0-9]+_',"",line[4],0)] = line[3]
	return signals

def getTimes(lines):
	times = []

	time = 0
	values = []
	foundTime = False
	firstLine = 0
	for i in xrange(len(lines)):
		line = lines[i].strip()
		if(line.startswith("$enddefinitions")):
			firstLine = i + 1
			break

	for i in range(firstLine,len((lines))):	
		line = lines[i].strip()
		if(line.startswith("#")):
			if (len(values) > 0):
				times.append((time,values))
			time = int(line[1:])
			values = []
		else:
			values.append(line)

	return times

def formatSignals():
	id = 33
	newSignalDic = []
	newSignals = []
	for i in xrange(len(signals)):
 		newSignalDic.append({})
 		signal = []
 		signal = signals[i]
		signalList = signal.items()
		for sig in signalList:
			#first replace the oldID with new ID in signal
			oldID = sig[1]
			newID = chr(id)
			
			newSignalDic[i][oldID] = newID
			newSignals.append((sig[0],newID))
			id += 1
	return (newSignalDic,newSignals)

def updateTimes(newSignalDic):
	for i in xrange(len(signals)):
	#then go through the times and replace the signals
		for j in xrange(len(times[i])):
			time = times[i][j]
			
			oldEvents = time[1]
			
			newEvents = []
			for event in oldEvents:
				newEvents.append(event.replace(event[1],newSignalDic[i][event[1]]))
			
			times[i][j] = (time[0],newEvents)	
	return times[0] + times[1] + times[2]


def mergeTimeList(allTimes):
	mergedTimes = [allTimes[0]]
	for i in range(1,len(allTimes)):
		time = allTimes[i]	
		if(mergedTimes[-1][0] == time[0]):
			mergedTimes[-1] = (mergedTimes[-1][0],mergedTimes[-1][1] + time[1])
		else:
			mergedTimes.append(time)
	return mergedTimes


if __name__ == "__main__":
   main(sys.argv[1:])




