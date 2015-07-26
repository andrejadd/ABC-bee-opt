#!/usr/bin/perl

use strict;

my @methodname = ("SABC", "ABCrank", "ABCgbest", "ABCcomb");

my @fct = ("sc", "sm", "gr", "rg", "rn", "ac");

my $basedir =  "../../FINAL_EXP_RESULT/METHOD_DATA_STEST/";


my $curr_popsize = 100;
my $runs = 100;

my $printmode = 1; # default

foreach my $method (@methodname) {

    for (my $k = 0; $k <= $#fct; $k++) {

	my $curr_fct = $fct[$k];

	my $fname = $basedir . $method . "/NORANGELIMIT/N100/" . $method . "-nrl_" . $curr_fct;



	open (MYFILE, '>'.$fname) || die("failed to create file");

	open( APPCALL, "java ModelTester -fct $curr_fct -method $method -n $curr_popsize -pmode 1 -runs $runs -crange false |") || die "exec failed";
	    
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

exit;
