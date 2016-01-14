Note: first collect the files located on amonhen:

/data/gkosta/supercomp_data/adpcm_decode/dat/adpcm_decode_memonly_log.csv
/data/gkosta/supercomp_data/adpcm_encode/dat/adpcm_encode_memonly_log.csv
/data/gkosta/supercomp_data/basicmath/dat/basicmath_memonly_log.csv
/data/gkosta/supercomp_data/bf_decode/dat/bf_decode_memonly_log.csv
/data/gkosta/supercomp_data/bf_encode/dat/bf_encode_memonly_log.csv
/data/gkosta/supercomp_data/bitcount/dat/bitcount_memonly_log.csv
/data/gkosta/supercomp_data/cjpeg/dat/cjpeg_memonly_log.csv
/data/gkosta/supercomp_data/crc/dat/crc_memonly_log.csv
/data/gkosta/supercomp_data/dijkstra/dat/dijkstra_memonly_log.csv
/data/gkosta/supercomp_data/djpeg/dat/djpeg_memonly_log.csv
/data/gkosta/supercomp_data/fft/dat/fft_memonly_log.csv
/data/gkosta/supercomp_data/fft_inverse/dat/fft_inverse_memonly_log.csv
/data/gkosta/supercomp_data/qsort/dat/qsort_memonly_log.csv
/data/gkosta/supercomp_data/rijndael/dat/rijndael_memonly_log.csv
/data/gkosta/supercomp_data/sha/dat/sha_memonly_log.csv
/data/gkosta/supercomp_data/small_bench/dat/small_bench_memonly_log.csv
/data/gkosta/supercomp_data/stringsearch/dat/stringsearch_memonly_log.csv
/data/gkosta/supercomp_data/susan_edge/dat/susan_edge_memonly_log.csv


put in folder called dat/ in same directory as python script


then run abridge.py. This parses the raw data and collects only the relevant information.




Then you can run any of the hamdistance*.py files.
plotly script requires plotly installed on computer with account key set up.
https://plot.ly/python/getting-started/

Otherwise matplotlib is used.

choose which benchmarks to run by changing the benches list at the top of the file.

the hamdistance_pdf.py script puts all of the plots into a single pdf but the plots
are not produced correctly by matplotlib for some reason.

