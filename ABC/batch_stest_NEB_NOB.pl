#!/usr/bin/perl

use strict;

my $local |= 1;

#my @methodname = ("ABCgbestrank_1", "ABCgbestrank_2", "ABCgbestrank_3");
#my @methodname = ("ABCsel_dist_1", "ABCsel_dist_2", "ABCsel_dist_3");
my  $method = "SABC";


my @fct = ("sc", "sm", "gr", "rg", "rn", "ac");
#my @fct = ("gr");

my $basedir =  "/home/andrej/Desktop/latex-arbeit/final_figs/POPSIZE_STEST_D50/";

#my @popsizes = (10,30,60,100,120,140);
my @nebs = (5, 10, 20,40,60);
my @nobs = (0,1,2,3,4,5,6,7,8,9,10,11,12,15,20,30,40,50,60,70,80, 100);

my $runs = 100;
my $dimension = 50;
my $standardlimit = "false";

my $printmode = 2; # default


foreach my $curr_fct (@fct) { 

    foreach my $neb (@nebs) {

	my $fname = $basedir . $curr_fct . "_" . $neb;

	print "target: " . $fname . "\n";

	open (MYFILE, '>'.$fname) || die("failed to create file");
	
	foreach my $nob (@nobs) {

	    open( APPCALL, "java ModelTester -fct $curr_fct -method $method -neb $neb -nob $nob -pmode 1 -runs $runs -standardlimit $standardlimit -pmode $printmode -dim $dimension|") || die "exec failed";
	    
	    while( <APPCALL> ) {

		if(/#/) {
		    print $_;
		} else {
		    print MYFILE $nob . "\t" . $_;
		    print $nob . "\t" . $_;
		}
	    }
	}
			
	close MYFILE;
    }
}

exit;
