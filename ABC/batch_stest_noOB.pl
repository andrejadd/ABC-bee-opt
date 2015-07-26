#!/usr/bin/perl

use strict;

my $local |= 1;

#my @methodname = ("ABCgbestrank_1", "ABCgbestrank_2", "ABCgbestrank_3");
#my @methodname = ("ABCsel_dist_1", "ABCsel_dist_2", "ABCsel_dist_3");
my  $method = "SABC";


my @fct = ("sc_stest", "sm_stest", "gr_stest", "rg_stest", "rn_stest", "ac_stest");
my $testtype = "stest";

my $basedir =  "Output/POPSIZE_STEST/";

#my @popsizes = (10,30,60,100,120,140);
my @nebs = (15, 15,50,50);
my @nobs = (15, 0, 50, 0);

my $runs = 100;


for (my $id = 0; $id < ($#nebs + 1); $id ++) {

    my $neb = $nebs[$id];
    my $nob = $nobs[$id];


    foreach my $curr_fct (@fct) { 

	my $fname = $basedir . $method . "_neb" . $neb . "nob" . $nob . "_" . $curr_fct;

	print "target: " . $fname . "\n";

	open (MYFILE, '>'.$fname) || die("failed to create file");
	
	open( APPCALL, "java ModelTester -fct $curr_fct -method $method -neb $neb -nob $nob -pmode 1 -runs $runs -pmode 1 |") || die "exec failed";
	    
	while( <APPCALL> ) {
	    
	    if(/#/) {
		print $_;
	    } else {
		print MYFILE $_;
		print $_;
	    }
	}
    
    
	close MYFILE;
    }
}

exit;
