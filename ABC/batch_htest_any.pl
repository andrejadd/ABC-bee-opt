#!/usr/bin/perl

use strict;


#my @methodname = ("ABCgbestrank_1", "ABCgbestrank_2", "ABCgbestrank_3");
#my @methodname = ("ABCsel_dist_1", "ABCsel_dist_2", "ABCsel_dist_3");
my  @methodname = ("SABC");

my @fcts = ("sc_htest", "sm_htest", "gr_htest", "rg_htest", "rn_htest", "ac_htest");

my $basedir =  "/home/andrej/Desktop/latex-arbeit/final_figs/ABC_SIGTEST/";

my @nebs = (15,15,50,50);
my @nobs = (15,0,50,0);

my $runs = 100;

my $printmode = 1; # default

for (my $nid= 0; $nid <= $#nebs; $nid++) { 

    my $neb = $nebs[$nid]; 
    my $nob = $nobs[$nid]; 

    foreach my $method (@methodname) {

	foreach my $fct (@fcts) {


	    my $fname = $basedir . $method . "_neb" . $neb . "-nob" . $nob . "_" .  $fct;

	    print "target: " . $fname . "\n";

	    open (MYFILE, '>'.$fname) || die("failed to create file");
	
	    open( APPCALL, "java ModelTester -fct $fct -method $method -neb $neb -nob $nob -pmode 1 -runs $runs |") || die "exec failed";
	    
	    while( <APPCALL> ) {

		if(/#/) {
		    print $_;
		} else {
		
		    print MYFILE $_;
		    chomp();
		    print $_ . "\t";
		}
	    }
	    print "\n";
	    close MYFILE;
	}
    }
}

exit;
