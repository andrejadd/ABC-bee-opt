#!/usr/bin/perl

use strict;

local $| = 1; # set autoflash on

my $runs = 100;

my $psotype = 1; # H-PSO

my $paramtype = 0; # a) params -> w= 0.6, c=1.7

my $printmode = 0; # prints evals to reach the goal, 1 prints mean best solution per cycle

my $iterations = 10000;

# method name for file
my $methodname = "hpsoa";

# fct names for output
my @fctname = ("sc", "sm", "gr", "rg", "rn", "ac");

# fct args for runpso
my @fct = (3, 0, 4, 1, 2, 5);


for(my $index = 0; $index <= $#fct; $index++) {
    
    my $currfctname = $fctname[$index];
    my $currfctid   = $fct[$index];

    my $dir = "../../FINAL_EXP_RESULT/METHOD_DATA_STEST/HPSO/";
    
    my $fname = $dir . $methodname . "_" . $currfctname; # easier to handle
    
    open (MYFILE, '>'.$fname) || die("failed to create file");
    
    open( APPCALL, "./runpso $runs $iterations $currfctid $psotype $paramtype $printmode |") || die "exec failed";
    
    print "\n";

    while( <APPCALL> ) {
	if(!/#/) {
	    print MYFILE $_;
	    if(/-1/) {
		print "-1"; 
	    } else {
		print ".";
	    }
	} else {
	    print $_;
	}
    }
    
    close MYFILE
}


exit;
