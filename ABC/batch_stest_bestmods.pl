#!/usr/bin/perl

use strict;

#my @methodname = ("SABC", "ABCgbest", "ABCgbestdist_1", "ABCgbestrank_1", "ABCgbestrank_3" ); 
my @methodname = ("ABCgbestrank_3" ); 


my @fcts = ("sc_stest", "sm_stest", "gr_stest", "rg_stest", "rn_stest", "ac_stest");

my $testtype = "stest";

my $basedir =  "/home/andrej/Desktop/latex-arbeit/final_figs/ABC_TABLES/ABC_BESTMOD_COMPARE_STEST/";

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
