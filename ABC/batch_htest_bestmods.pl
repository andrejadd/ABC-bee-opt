#!/usr/bin/perl

use strict;

my @methodname = ("SABC", "ABCgbest", "ABCgbestdist_1", "ABCgbestrank_1", "ABCgbestrank_3"); 


my @fcts = ("sc_htest", "sm_htest", "gr_htest", "rg_htest", "rn_htest", "ac_htest");



my $basedir =  "Output/BESTMODS_COMPARE_HTEST/";

my @popsizes = (30,100);
my $runs = 100;

my $printmode = 1; # default

foreach my $curr_popsize (@popsizes) {

    foreach my $method (@methodname) {

	foreach my $fct (@fcts) { 

	    my $fname = $basedir . $method . "_n" . $curr_popsize . "_" . $fct;
	    print "target: " . $fname . "\n";

	    open (MYFILE, '>'.$fname) || die("failed to create file");
	    
	    open( APPCALL, "java ModelTester -fct $fct -method $method -n $curr_popsize -pmode 1 -runs $runs |") || die "exec failed";
	    
	    while( <APPCALL> ) {
		
		if(/#/) {
		    print $_;
		} else {
		    print $_;
		    print MYFILE $_;
		}
	    }
	    
	    close MYFILE;
	}
    }
}
exit;
