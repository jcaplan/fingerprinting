#!/usr/bin/python

import sys
import re

version = []
comments = []
timescale = []
signals = []
times = []
coreNames = ["cpu0","cpu1","cpu2","cpu3","cpum"]
	
def main(argv):

	i = 0
	for name in coreNames:
		f = open("output/" + name + ".ivcd", 'r')
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

	printGtk()

def printGtk():
	f = open("platform.sav",'w')

	front = """[*]
[*] GTKWave Analyzer v3.3.58 (w)1999-2014 BSI
[*] Sun Jan 17 20:02:13 2016
[*]
[dumpfile] "/home/jonah/fingerprinting/ovp_platform/platform_4_tlb/platform.vcd"
[dumpfile_mtime] "Sun Jan 17 19:59:43 2016"
[dumpfile_size] 6006
[savefile] "/home/jonah/fingerprinting/ovp_platform/platform_4_tlb/platform.sav"
[timestart] 0
[size] 1319 722
[pos] -1 -1
*-27.328293 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
[sst_width] 223
[signals_width] 222
[sst_expanded] 1
[sst_vpaned_height] 211
"""
	f.write(front)

	for i in xrange(len(coreNames)):
		if(i == len(coreNames) - 1):
			f.write('@800201\n')
		else:
			f.write('@800200\n')
		f.write('-' + coreNames[i] + '\n')
		f.write('@28\n')
		
		exclude = ['None','Idle','Stat']	
		filter_func = lambda s: not any(x in s for x in exclude)
		
		for signal in filter(filter_func,signals[i]):
			f.write('platform.' + signal + '\n')
		if(i == len(coreNames) - 1):
			f.write('@1000201\n')
		else:
			f.write('@1000200\n')
		f.write('-' + coreNames[i] + '\n')

	f.write('[pattern_trace] 1\n')
	f.write('[pattern_trace] 0\n')
	f.write('@800200')
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
			# signals[name + "_" + re.sub(r'[0-9]+_',"",line[4],0)] = line[3]
			signals[name + "_" + line[4]] = line[3]
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

	result = [];
	for time in times:
		result += time
	return result


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




