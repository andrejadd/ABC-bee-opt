#!/usr/bin/perl

use strict;

#my @methodname = ("ABCsel_rank_3", "ABC", "ABCsel_gbest_2c");

my @methodname = ("ABCgbestrank3");

my @fct = ("sc", "sm", "gr", "rg", "rn", "ac");

my @popsize = (4, 6, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200);
#my @popsize = (4, 6);

foreach my $method (@methodname) {

    for (my $k = 0; $k <= $#fct; $k++) {

	my $curr_fct = $fct[$k];
	my $dir = "../../FINAL_EXP_RESULT/BESTMODS_POPSIZE/";

	my $fname = $dir . $curr_fct . "_" . $method;

	open (MYFILE, '>'.$fname) || die("failed to create file");
    
	foreach my $curr_popsize (@popsize) {
	      
	    open( APPCALL, "java ModelTester -fct $curr_fct -method $method -n $curr_popsize -runs 100 -pmode 2 |") || die "exec failed";
	    
	    while( <APPCALL> ) {
		if(/#/) {
		    print $_;
		}else {
		    print ".";
		    print MYFILE $curr_popsize . "\t" . $_;
		}
		
	    }
	    
	}
	close MYFILE;
	print "+";
    }
}

exit;
