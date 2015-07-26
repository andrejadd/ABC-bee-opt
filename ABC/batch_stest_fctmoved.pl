#!/usr/bin/perl

use strict;

#my @methodname = ("SABC", "ABCrank", "ABCgbest", "ABCcomb");
my @methodname = ("SABC");

#my @fct = ("scmoved", "smmoved", "grmoved", "rgmoved", "rnmoved", "acmoved");
my @fct = ("scmoved");

my $basedir =  "/home/andrej/AndrejAderhold_d-arbeit/d-arbeit/EigeneDoks/latex-arbeit/final_figs/METHOD_DATA_STEST/ABC/SABC/FCT_MOVED_CENTER/N30/";


#/home/andrej/Desktop/AndrejAderhold_d-arbeit/latex-arbeit/final_figs/METHOD_DATA_STEST/ABC/SABC/FCT_MOVED_CENTER/N30/";

my $curr_popsize = 30;
my $runs = 100;

my $printmode = 1; # default

foreach my $method (@methodname) {

    for (my $k = 0; $k <= $#fct; $k++) {

	my $curr_fct = $fct[$k];

	my $fname = $basedir . $method . "_" . $curr_fct;


	open (MYFILE, '>'.$fname) || die("failed to create file");
	 
	open( APPCALL, "java ModelTester -fct $curr_fct -method $method -n $curr_popsize -pmode 1 -runs $runs |") || die "exec failed";
	    
	while( <APPCALL> ) {

	    if(/#/) {
		print $_;
	    } else {

		if($_ > 500000) {
		    print MYFILE "-1\n";
		    print "-1\n";
		} else {
		    print MYFILE $_;
		    print $_;
		}
	    }
	}

	close MYFILE;
}
}

exit;
