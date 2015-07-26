#!/usr/bin/perl

use strict;

my $method = "ABCgbestrank_3";


my @fct = ("sc", "sm", "gr", "rg", "rn", "ac");
my $testtype = "stest";

my $basedir =  "/home/andrej/Desktop/latex-arbeit/final_figs/ABC_DATA/BESTMODS_POPSIZE/";

my @popsizes = (4,6,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200);
my $runs = 100;

my $printmode = 2; # default

for (my $k = 0; $k <= $#fct; $k++) {

	my $curr_fct = $fct[$k];

	my $fname = $basedir . $curr_fct . "_" . $method;

	open (MYFILE, '>'.$fname) || die("failed to create file: $fname");


	foreach my $psize (@popsizes) {

	    my $realfct = $curr_fct . "_" . $testtype;

	    print "java ModelTester -fct $realfct -method $method -n $psize -pmode $printmode -runs $runs \n";
	    open( APPCALL, "java ModelTester -fct $realfct -method $method -n $psize -pmode $printmode -runs $runs |") || die "exec failed";
	    
	    while( <APPCALL> ) {
		
		if(/#/) {
		    print $_;
		} else {
		    print $psize . "\t" . $_;
		    print MYFILE $psize . "\t" . $_;
		}
	    }
	}

	    close MYFILE;

}


exit;
