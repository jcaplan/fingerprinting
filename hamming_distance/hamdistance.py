from __future__ import division
import numpy as np
from scipy.interpolate import UnivariateSpline
import matplotlib.pyplot as plt

benches = ["adpcm_encode","adpcm_decode","basicmath","bf_decode","bf_encode","bitcount","cjpeg","crc","dijkstra","djpeg","fft_inverse",
"fft", "qsort","rijndael","sha","stringsearch","susan_edge"]

blocksizes = [748,1496,2044,2992,4088,4092,5724,8184,8176,11448,16368,16384,22896,32768]


# # do the densities for 1-10
for bench in benches:
#	for each blocksize need an array and to count 
	path = "abridged/" + bench + "_abridged.csv"
	print path
	f = open(path,'r',1)

	hammingDistances = [{},{},{},{},{},{},{},{},{},{},{},{},{},{}]
	lineCount = 0
	for line in f:
		tokens = line.split(",")
		count = 0;
		for blocksize in blocksizes:
			hd = tokens[count].rstrip()
			if(hd.isdigit()):
				hd = int(hd)
				if(hammingDistances[count].has_key(hd)):
					oldFreq = hammingDistances[count].get(hd)
					newFreq = oldFreq + 1
					hammingDistances[count][hd] = newFreq 
				else:
					hammingDistances[count][hd] = 1


			count += 1
		lineCount += 1

	#start with 748 as an example
	# print hammingDistances

	for index in xrange(6,14):
		frequencies = hammingDistances[index].items()
		frequencies.sort()
		x = []
		y = []
		
		for i in xrange(0,11):
			
			f = (frequencies[i][0],frequencies[i][1]/lineCount)
			frequencies[i] = f
			x.append(f[0])
			y.append(f[1])
		

		plt.plot(x[0:10], y[0:10],marker='o',label=str(blocksizes[index]) + "B")
		# print frequencies



	plt.title(bench)
	plt.xlabel('Hamming Distance')
	plt.legend( loc='upper right', numpoints = 1 )
	plt.savefig("density/" + bench + ".png")
	plt.savefig("density/" + bench + ".pdf")
	plt.close()
	print "done " + bench

for bench in benches:
	print bench
	path = "abridged/" + bench + "_abridged.csv"
	print path
	f = open(path,'r',1)

	lineCount = 0
	hammingDistances =  [[],[],[],[],[],[],[],[],[],[],[],[],[],[]]
	for line in f:
		tokens = line.split(",")
		count = 0
		for blocksize in blocksizes:
			hd = tokens[count].rstrip() #strip whitespace
			if(hd.isdigit):
				hammingDistances[count].append(int(hd))
			count += 1
		lineCount += 1




	for index in xrange(6,14):
		values, base = np.histogram(hammingDistances[index],bins=100000)
		cumulative = np.cumsum(values)
		# plot the cumulative function
		new = []
		for i in cumulative:
			new.append(i / float(lineCount))

		plt.plot(base[:-1], new,label=str(blocksizes[index]) + "B")
		
	plt.title('Distribution')
	plt.xlabel('Hamming Distance')
	plt.xscale('log', nonposy='clip')
	plt.grid(b=True,which='major',linestyle='--')
	plt.legend( loc='upper left', numpoints = 1 )

	plt.savefig("cumulative/" + bench + ".png")
	plt.savefig("cumulative/" + bench + ".pdf")
	plt.close()
	print "done " + bench
# 	plt.show()


