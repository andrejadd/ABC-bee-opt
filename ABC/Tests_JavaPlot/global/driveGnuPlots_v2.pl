#!/usr/bin/perl -w
use strict;




sub main {


    my $numberOfStreams = 1;
    print "Will display $numberOfStreams Streams (in $numberOfStreams windows)...\n";

    my $samples = <STDIN>;
    my @minmax = split(/:/, <STDIN>);
    chomp $samples;
    chomp($minmax[1]);
    
    my @sampleSizes;
    for(my $i=0; $i<$numberOfStreams; $i++) {
	push @sampleSizes, $samples;
	print "Stream ".($i+1)." will use a window of $samples samples.\n";
    }
    my @ranges;
    for(my $i=0; $i<$numberOfStreams; $i++) {
	my $miny = $minmax[0];
	my $maxy = $minmax[1];
	push @ranges, [ $miny, $maxy ];
	print "Stream ".($i+1)." will use a range of [$miny, $maxy]\n";
    }

    my @gnuplots;
    my @buffers;
    shift @ARGV; # number of streams

    for(my $i=0; $i<$numberOfStreams; $i++) {
	shift @ARGV; # sample size
	shift @ARGV; # miny
	shift @ARGV; # maxy
	local *PIPE;
	open PIPE, "|gnuplot" || die "Can't initialize gnuplot number ".($i+1)."\n";
	select((select(PIPE), $| = 1)[0]);
	push @gnuplots, *PIPE;
	#print PIPE "set xtics\n";
	#print PIPE "set ytics\n";

	print PIPE "set xrange [".($ranges[$i]->[0]).":".($ranges[$i]->[1])."]\n";
	print PIPE "set yrange [".($ranges[$i]->[0]).":".($ranges[$i]->[1])."]\n";

	print PIPE "set style data points\n";
	print PIPE "set grid\n";
	my @data = [];
	push @buffers, @data;
    }

    my $streamIdx = 0;
    select((select(STDOUT), $| = 1)[0]);
   

    my $pip = $gnuplots[$streamIdx];
    my $loop = 0;
    my $run = 1;

    while($run) {

	print STDOUT $loop++ . "\n";

	my @buf;
	my $samplecounter = $samples;

	while(<>) {
	    chomp;

	    if(/exit/) {
		$run = 0;
		last;
	    }
	    push @buf, $_;
	    $samplecounter--;
	    if($samplecounter == 0) {
		last;
	    }
	}

	
	print $pip "plot \"-\" notitle\n";
    
	for my $elem (reverse @buf) {
	    print $pip ($elem)."\n";
	}
	
	print $pip "e\n";
	
    }

    print $pip "exit;\n";
    close $pip;
    
}

main;
