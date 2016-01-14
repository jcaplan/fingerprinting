#!/usr/bin/perl -w
use strict;
use warnings;

my $benches = [
'adpcm_decode',
'adpcm_encode',
'basicmath',
'bf_decode',
'bf_encode',
'bitcount',
'cjpeg',
'crc',
'dijkstra',
'djpeg',
'fft',
'fft_inverse',
'qsort',
'rijndael',
'sha',
'stringsearch',
'susan_edge',
];

my $path;
my $fh;
my $line;
my @vars;
my $hds = {};

my $bench;
my $hd;
my $block_size;
my $lines_count;
my $block_sizes_count;
my $poly;

foreach (@$benches) {
    $bench = $_;
    $path = sprintf ('%s/dat/%s_memonly_log.csv',$bench ,$bench); 
    #printf ("%s\n", $path);
    
    if (-e $path) {
        open($fh, '<:encoding(UTF-8)', $path) or die 'failed to open file';
    }
    $lines_count = 0;
    while (<$fh>) {        
        chomp;
        $line = $_;
        @vars = split(',',$line);          
        
        $poly = $vars[0];   
   
        # only need one poly
        if ($poly ne '82F63B78') {
            next;
        }
        
        $block_size = $vars[1];
        $hd = $vars[5];

        if (not exists $hds->{$block_size} ) {
            $hds->{$block_size} = 0;
        }
        $hds->{$block_size} += $hd;
        
        $lines_count++;        
    }
    my $block_sizes_count = keys %$hds;
    my @keys_l = sort { $a <=> $b} keys %$hds;
    print ("---------------------------\n");
    printf ("%s\n",$bench);
    printf ("%d results total\n",$lines_count / $block_sizes_count);
    print ("---------------------------\n");
    printf ("%10s %10s %10s\n",'block size','hd', 'rel hd');
    foreach ( @keys_l ) {
        $hds->{$_} = $hds->{$_}  * $block_sizes_count / $lines_count;
        printf ("%10d %10.2f %10.8f\n",$_,$hds->{$_},$hds->{$_}/(8*$_));
    }

   $hds = {};
}
