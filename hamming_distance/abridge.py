benches = ["adpcm_encode","adpcm_decode","basicmath","bf_decode","bf_encode","bitcount","cjpeg","crc","dijkstra","djpeg","fft_inverse",
"fft", "qsort","rijndael","sha","stringsearch","susan_edge"]

blocksizes = [748,1496,2044,2992,4088,4092,5724,8184,8176,11448,16368,16384,22896,32768]

for bench in benches:
	print bench
	path = "dat/" + bench + "_memonly_log.csv"
	print path
	f = open(path,'r',1)
	output = open("abridged/" + bench + "_abridged.csv",'w')
	s = ""
	lineCount = 0
	for line in f:
		tokens = line.split(",")
		poly = tokens[0]
		if(poly == '82F63B78'):
			hammingDistance = tokens[5]
			s += hammingDistance + ","
			lineCount += 1
			if(lineCount == 14):
				s = s[:-1] + "\n"
				output.write(s)
				s = ""
				lineCount = 0
