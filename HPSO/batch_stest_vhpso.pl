#!/usr/bin/perl

use strict;

local $| = 1; # set autoflash on

my $popsize = 31;
my $runs = 100;
my $psotype = 1; # H-PSO
my $paramtype = 2; # a) varying inertia weight, 
my $printmode = 0; # prints evals to reach the goal, 1 prints mean best solution per cycle

my $iterations = 10000;

# method name for file
my $methodname = "vhpso";

# fct names for output
#my @fctname = ("sc", "sm", "gr", "rg", "rn");
my @fctname = ("rg");

my $ac_paramtype = 4;

# fct args for runpso
#my @fct = (3, 0, 4, 1, 2, 5);
my @fct = (1);


for(my $index = 0; $index <= $#fct; $index++) {
    
    my $currfctname = $fctname[$index];
    my $currfctid   = $fct[$index];

    my $dir = "/home/andrej/Desktop/latex-arbeit/final_figs/METHOD_DATA_STEST/VHPSO/";
    
    my $fname = $dir . $methodname . "_" . $currfctname; # easier to handle
    
    open (MYFILE, '>'.$fname) || die("failed to create file");

    my $ptype = $paramtype; 
    if($currfctid == 5) { # this is ac, uses special parameter type (4), this changes branch degree to d=2
	$ptype = $ac_paramtype;
    }
    open( APPCALL, "./runpso $runs $iterations $currfctid $psotype $ptype $printmode |") || die "exec failed";
    
    print "\n";

    while( <APPCALL> ) {
	if(!/#/) {
	    print $_;
	    print MYFILE $_;
	} else {
	    
	    print $_;
	}
    }
    
    close MYFILE
}


exit;
