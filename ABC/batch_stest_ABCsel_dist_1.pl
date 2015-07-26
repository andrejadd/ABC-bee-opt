#!/usr/bin/perl

use strict;

my @methodname = ("ABCsel_dist_1" 
);

my %popsize = (
    ABCsize20   =>  20
);

my %realclassname = (
    ABCsize20   =>  "SABC"
);


my @fct = ("sc", "sm", "gr", "rg", "rn", "ac");

my $basedir =  "/home/andrej/workspace/FINAL_EXP_RESULT/METHOD_DATA_STEST/ABC/";

my $defaultpopsize = 30;
my $runs = 100;

my $printmode = 1; # default

foreach my $method (@methodname) {

    my $realpopsize = $defaultpopsize;

    if(defined $popsize{$method}) {
	    $realpopsize = $popsize{$method};
    }
    
    my $realclass = $method;

    if(defined $realclassname{$method}) {
	$realclass = $realclassname{$method};
    }


    for (my $k = 0; $k <= $#fct; $k++) {

	my $curr_fct = $fct[$k];

	my $targetdir = $basedir . "/" . $method;

	if (! (-d $targetdir)) {
	    `mkdir $targetdir`;
	}

	my $fname = $targetdir . "/N" . $realpopsize . "/" . $method . "_" . $curr_fct;

	open (MYFILE, '>'.$fname) || die("failed to create file");
	
	open( APPCALL, "java ModelTester -fct $curr_fct -method $realclass -n $realpopsize -pmode 1 -runs $runs |") || die "exec failed";
	    
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
