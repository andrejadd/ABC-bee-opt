#!/usr/bin/perl

use strict;


#my @methodname = ("ABCgbestrank_1", "ABCgbestrank_2", "ABCgbestrank_3");
#my @methodname = ("ABCsel_dist_1", "ABCsel_dist_2", "ABCsel_dist_3");
my  @methodname = ("ABCgbestrank_1");

my @fcts = ("sc", "sm", "gr", "rg", "rn", "ac");
my $testtype ="stest";

my $basedir =  "/home/andrej/Desktop/latex-arbeit/final_figs/ABC_AND_ABCGBESTRANK_WITH_OTHERMETHODS_STEST/";

my @nebs = (15);
my @nobs = (15);

my $runs = 100;

my $printmode = 1; # default

for (my $nid= 0; $nid < ($#nebs+1); $nid++) { 

    my $neb = $nebs[$nid]; 
    my $nob = $nobs[$nid]; 

    foreach my $method (@methodname) {

	foreach my $fct (@fcts) {


#	    my $fname = $basedir . $method . "_neb" . $neb . "-nob" . $nob . "_" .  $fct . "_stest";
	    my $fname = $basedir . $method . "_" . $fct;

	    print "target: " . $fname . "\n";

	    open (MYFILE, '>'.$fname) || die("failed to create file");
	
	    my $actualfct = $fct . "_" . $testtype;
	    open( APPCALL, "java ModelTester -fct $actualfct -method $method -neb $neb -nob $nob -pmode 1 -runs $runs |") || die "exec failed";
	    
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
