#!/usr/bin/perl
# this batch generates runtime data (iterations - mean fct. value) for SABC over several popsizes

use strict;

my @methodname = ("SABC");

my $basedir = "/home/andrej/Desktop/latex-arbeit/final_figs/ABC_DATA/POPSIZES_WITHOUTOB_RUNTIME/";
my $runs = 100;


my @fcts = ("sc", "sm", "gr", "rg", "rn", "ac");


my @nebs = (15,15,50,50);
my @nobs = (15,0 ,50,0);



my $method = "SABC";

for (my $id = 0; $id < ($#nebs + 1); $id ++) {

    my $neb = $nebs[$id];
    my $nob = $nobs[$id];


    foreach my $fct (@fcts) {
	    
	my $fname = $basedir . $method . "_neb" . $neb . "nob" . $nob . "_" . $fct;
	
	print "target: " . $fname . "\n";
	
	open (MYFILE, '>'.$fname) || die("failed to create file");
	
	print "fct: " . $fct . ", method: " . $method . ", neb: " . $neb . ", nob: " . $nob . ", runs: " . $runs . "\n";
	
	my $cfct = $fct . "_stest";
	
	open( APPCALL, "java ModelTester -fct $cfct -method $method -neb $neb -nob $nob -runs $runs -pmode 3  |") || die "exec failed";
	
	while( <APPCALL> ) {
		print MYFILE $_;
		
	    }
	close MYFILE;
	    
    }
}


exit;
