#!/usr/bin/perl

use strict;

my $method = "ABCgbestrank_3";


my @fct = ("sc", "sm", "gr", "rg", "rn", "ac");
my $testtype = "stest";

my $basedir =  "/home/andrej/Desktop/latex-arbeit/final_figs/ABC_DATA/BESTMODS_POPSIZE/";

my @popsizes = (10,30,50);
my $runs = 10;

my $printmode = 1; # default

for (my $k = 0; $k <= $#fct; $k++) {

	my $curr_fct = $fct[$k];

	my $fname = $basedir . $curr_fct . "_" . $method;

	open (MYFILE, '>'.$fname) || die("failed to create file: $fname");


	foreach my $psize (@popsizes) {

	    my $realfct = $curr_fct . "_" . $testtype;

	    my @fevals = ();

	    print "java ModelTester -fct $realfct -method $method -n $psize -pmode $printmode -runs $runs \n";
	    open( APPCALL, "java ModelTester -fct $realfct -method $method -n $psize -pmode $printmode -runs $runs |") || die "exec failed";
	    
	    while( <APPCALL> ) {
		
		if(/#/) {
		    print $_;
		} else {
		    chop;
		    push @fevals, $_;
		    print $_;
		}
	    }
	    

	    print MYFILE $psize . "\t" . mean(\@fevals) . "\n";
	    close MYFILE;
	}
}


exit;

sub mean() {
    
    my ($ref) = shift;

    my $size =  ($#{$ref} + 1);

    my $sum = 0;

    foreach my $value (@{$ref}) {

	$sum += $value;
    }
   
    return $sum/$size;
};



