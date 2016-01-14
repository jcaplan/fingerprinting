from __future__ import division
import numpy as np
import plotly.plotly as py
import plotly.graph_objs as go

benches = ["adpcm_encode","adpcm_decode","basicmath","bf_decode","bf_encode","bitcount","cjpeg","crc","dijkstra","djpeg","fft_inverse",
"fft", "qsort","rijndael","sha","stringsearch","susan_edge"]

blocksizes = [748,1496,2044,2992,4088,4092,5724,8184,8176,11448,16368,16384,22896,32768]


# # do the densities for 1-10
# for bench in benches:
# #	for each blocksize need an array and to count 
# 	path = "abridged/" + bench + "_abridged.csv"
# 	print path
# 	f = open(path,'r',1)

# 	hammingDistances = [{},{},{},{},{},{},{},{},{},{},{},{},{},{}]
# 	trace = []
# 	lineCount = 0
# 	for line in f:
# 		tokens = line.split(",")
# 		count = 0;
# 		for blocksize in blocksizes:
# 			hd = tokens[count].rstrip()
# 			if(hd.isdigit()):
# 				hd = int(hd)
# 				if(hammingDistances[count].has_key(hd)):
# 					oldFreq = hammingDistances[count].get(hd)
# 					newFreq = oldFreq + 1
# 					hammingDistances[count][hd] = newFreq 
# 				else:
# 					hammingDistances[count][hd] = 1


# 			count += 1
# 		lineCount += 1

# 	#start with 748 as an example
# 	# print hammingDistances

# 	for index in xrange(6,14):
# 		frequencies = hammingDistances[index].items()
# 		frequencies.sort()
# 		data_x = []
# 		data_y = []
		
# 		for i in xrange(0,11):
			
# 			f = (frequencies[i][0],frequencies[i][1]/lineCount)
# 			frequencies[i] = f
# 			data_x.append(f[0])
# 			data_y.append(f[1])
		
# 		trace.append(go.Scatter(
# 			x = data_x,
# 			y = data_y,
# 			mode = 'lines',
# 			name = str(blocksizes[index]) + "B"
# 			)
# 		)

# 	layout = dict(xaxis = dict(title = 'Hamming Distance'),
# 				  yaxis = dict(title = 'Frequency'))
# 	fig = dict(data=trace, layout=layout)
# 	# py.iplot(fig, filename='hd_d/' + bench + '_d')
# 	py.iplot(fig, filename=bench + '_d')
# 	print "done " + bench

for bench in benches:
	print bench
	path = "abridged/" + bench + "_abridged.csv"
	print path
	f = open(path,'r',1)
	trace = []

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

		trace.append(go.Scatter(
			x = base[:-1],
			y = new,
			mode = 'lines',
			name = str(blocksizes[index]) + "B"
			)
		)

	# plt.title('Distribution')
	layout = dict(xaxis = dict(title = 'Hamming Distance'),
				  yaxis = dict(
				  				type='log',
				  				title = 'CDF'
				  				)
				  )
	fig = dict(data=trace, layout=layout)
	# py.iplot(fig, filename='hd_c/' + bench + '_c')
	py.iplot(fig, filename=bench + '_c')

	print "done " + bench
# 	plt.show()


