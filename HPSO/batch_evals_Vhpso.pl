#!/usr/bin/perl

use strict;


my $runs = 100;
my $maxevals = 300000;
my $psotype = 1; # H-PSO
my $printmode = 1; # mean best value per cycle

# paramtype: 0 ; w=0.6,  c=1.7   (a)
#            1 ; w=0.729,c=1.494 (b)
#            2 ; wmin=0.729, wmax=0.4, c=1.7 (VH-PSO)
#            3 ; wmin=0.4  , wmax=0.729, c=1.7 (~VH-PSO)

my $paramtype = 2; 
my $iterations = 10000;

# method name for file
my $methodname = "vhpso";

# fct names for output
my @fctname = ("sc", "sm", "gr", "rg", "rn", "ac");
#my @fctname = ("sc");

# fct args for runpso
my @fct = (3, 0, 4, 1, 2, 5);
#my @fct = (3);

 


for(my $index = 0; $index <= $#fct; $index++) {
    
    my $currfctname = $fctname[$index];
    my $currfctid   = $fct[$index];

    my $dir = "/home/andrej/Desktop/latex-arbeit/final_figs/FINAL_TEST_RUNTIME/";
    
    my $fname = $dir . $currfctname . "_" . $methodname; # easier to handle
    
    print "+";
    
    open (MYFILE, '>'.$fname) || die("failed to create file");
    
    open( APPCALL, "./runpso $runs $iterations $currfctid $psotype $paramtype $printmode |") || die "exec failed";
    
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
