#!/usr/bin/perl

use strict;

my $popsize = 40;
my $runs = 100;
my $maxevals = 300000;
my $psotype = 0; # gbest PSO
my $paramtype = 0; # a) params -> w= 0.6, c=1.7

# Achtung, dieser wert kann w beeinflussen bei H-PSO
my $iterations = 10000;

# method name for file
my $methodname = "psogbest";

# fct names for output
my @fctname = ("sc", "sm", "gr", "rg", "rn", "ac");

# fct args for runpso
my @fct = (3, 0, 4, 1, 2, 5);
 


for(my $index = 0; $index <= $#fct; $index++) {
    
    my $currfctname = $fctname[$index];
    my $currfctid   = $fct[$index];

    my $dir = "../../FINAL_EXP_RESULT/LASTEXP/RUNTIME/";
    
    my $fname = $dir . $currfctname . "_" . $methodname; # easier to handle
    
    print "+";
    
    open (MYFILE, '>'.$fname) || die("failed to create file");
    
    open( APPCALL, "./runpso $runs $iterations $currfctid $psotype $paramtype |") || die "exec failed";
    
    while( <APPCALL> ) {
	if(!/#/) {
	    print MYFILE $_;
	} else {
	    print $_;
	}
    }
    
    close MYFILE
}


exit;
