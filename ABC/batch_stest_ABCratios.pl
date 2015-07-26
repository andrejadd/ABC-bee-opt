#!/usr/bin/perl

use strict;

my  $method = "SABC";

my @functions = ("sc", "sm", "gr", "rg", "rn", "ac");

my @ratios =     (1/30  ,1/20   ,1/10  ,1/8  ,1/6   , 1/4  , 1/3 , 1/2 , 3/4 );
my @rationames = ("1/30", "1/20","1/10","1/8", "1/6", "1/4","1/3", "1/2", "3/4");


my %fctnames = (
    sc => "Schaffer",
    sm => "Sphere",
    gr => "Griewank",
    rg => "Rastrigin",
    rn => "Rosenbrock",
    ac => "Ackley"

);

my $basedir =  "/home/andrej/Desktop/latex-arbeit/final_figs/ABCratios/";

my @popsizes = (10,30,60,100,120, 140);
#my @popsizes = (30);
my $runs = 100;

my $printmode = 2; # returns mean function evaluations over all runs

foreach my $curr_popsize (@popsizes) {

    # foreach population a new file
    my $fname = $basedir . $method . "_n" . $curr_popsize;

    open (MYFILE, '>'.$fname) || die("failed to create file");

    # write column names
    print MYFILE " \t";
    foreach my $fct (@functions) {
	print MYFILE $fctnames{$fct} . "\t";
    }
    print MYFILE "\n";
	
    
    # loop through ratios
    for (my $ratioid = 0; $ratioid <= $#ratios; $ratioid++) {

	my $ratio = $ratios[$ratioid];
	my $rationame = $rationames[$ratioid];

	print MYFILE $rationame . "\t";

	# at least neb = 2, if < 2 , write 0 (no valid run possible) 
	my $neb = int($curr_popsize * $ratio); 

	if($neb < 2) {
	    	foreach my $fct (@functions) {
		    print MYFILE "0\t";
		}
		print MYFILE "\n";
		next;
	}

	
	my $nob = $curr_popsize - $neb;

	foreach my $fct (@functions) {

		
	    open( APPCALL, "java ModelTester -fct $fct -method $method -neb $neb -nob $nob -pmode $printmode -runs $runs |") || die "exec failed";
	    
	    while( <APPCALL> ) {

		if(/#/) {
		    print $_;
		} else {
		    chomp();
		    print MYFILE int($_) . "\t";
		    print $_ . "\n";
		}
	    }
	    	
	}
	print MYFILE "\n";

    }
    close MYFILE;
}

exit;
