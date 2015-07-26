#!/usr/bin/perl

use strict;

my @methodname = (
    "ABCsel_gbest_1",
    "ABCsel_gbest_2a",
    "ABCsel_gbest_2b",
    "ABCsel_gbest_2c",
    "ABCsel_gbest_3a3",
    "ABCsel_gbest_3a9",
    "ABCsel_gbest_3b3",
    "ABCsel_gbest_3b9",
    "ABCphi_1",
    "ABCphi_2",
    "ABCphi_3a",
    "ABCphi_3b",
    "ABCphi_4a",
    "ABCphi_4b"

);

my %popsize = (
    ABCsize20   =>  20
);

my %realclassname = (
    ABCsize20   =>  "SABC"
);


my @fct = ("sc", "sm", "gr", "rg", "rn", "ac");
my$testtype = "stest";


my $basedir =  "Output/ABC/";


my $defaultpopsize = 100;
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

	my $fctshort = $fct[$k];
	my $curr_fct = $fctshort . "_" . $testtype;

	my $tdir1 = $basedir . $method;
	my $tdir2 = $tdir1   . "/N100";

	if (! (-d $tdir1)) {
	    `mkdir $tdir1`;
	}

	if (! (-d $tdir2)) {
	    `mkdir $tdir2`;
	}

	my $fname = $tdir2 . "/" . $method . "_" . $fctshort;

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
