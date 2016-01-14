from __future__ import division
import numpy as np
from matplotlib.backends.backend_pdf import PdfPages
import matplotlib.pyplot as plt

benches = ["adpcm_decode","adpcm_encode","basicmath","bf_decode","bf_encode","bitcount","cjpeg","crc","dijkstra","djpeg","fft_inverse",
"fft", "qsort","rijndael","sha","stringsearch","susan_edge"]

blocksizes = [748,1496,2044,2992,4088,4092,5724,8184,8176,11448,16368,16384,22896,32768]

with PdfPages('results.pdf') as pdf:
	# # do the densities for 1-10
	for bench in benches:
	#	for each blocksize need an array and to count 
		path = "abridged/" + bench + "_abridged.csv"
		print "starting work on " + bench
		f = open(path,'r',1)

		hammingDistancesDic = [{},{},{},{},{},{},{},{},{},{},{},{},{},{}]
		hammingDistancesList =  [[],[],[],[],[],[],[],[],[],[],[],[],[],[]]

		lineCount = 0
		for line in f:
			tokens = line.split(",")
			count = 0;
			for blocksize in blocksizes:
				hd = tokens[count].rstrip()
				if(hd.isdigit()):
					hd = int(hd)
					if(hammingDistancesDic[count].has_key(hd)):
						
						#for density
						oldFreq = hammingDistancesDic[count].get(hd)
						newFreq = oldFreq + 1
						hammingDistancesDic[count][hd] = newFreq 


						#for distribution
						hammingDistancesList[count].append(int(hd))
					else:
						hammingDistancesDic[count][hd] = 1


				count += 1
			lineCount += 1

		#first subplot

		plt.figure(figsize=(8.5, 11))
		plt.suptitle(bench)
		plt.subplot(2,1,1)
		for index in xrange(6,14):
			frequencies = hammingDistancesDic[index].items()
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



		plt.title(" Density")
		plt.xlabel('Hamming Distance')
		plt.legend( loc='upper right', numpoints = 1 )

		print "done " + bench + " density"

		plt.subplot(2,1,2)
		for index in xrange(6,14):
			values, base = np.histogram(hammingDistancesList[index],bins=100000)
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
		plt.legend( loc='bottom right', numpoints = 1 )

		pdf.savefig();
		plt.close()
		print "done " + bench + " distribution\n\n"


