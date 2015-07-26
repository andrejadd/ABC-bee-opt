#!/usr/bin/perl

use strict;

#my @methodname = ("ABCsel_dist_1", "ABCgbestdist_1", "SABC", "ABCgbest");
my @methodname = ("ABCgbestrank_1", "ABCgbestrank_3", "ABCsel_rank_1");

 

#my @methodtitle = ("ABCrank", "ABCgbest", "ABCgbestrank", "ABC", "ABCgbestdist", "ABCdist");

my $basedir = "/home/andrej/workspace/FINAL_EXP_RESULT/METHOD_DATA_RUNTIME/ABC/";
my $runs = 100;



my @fct = ("sc", "sm", "gr", "rg", "rn", "ac");

my @popsize = (100);

for(my $methid = 0; $methid <= $#methodname; $methid++) {

    my $method = $methodname[$methid];
    #my $methtitle = $methodtitle[$methid];

    foreach my $curr_fct (@fct) {
    
	foreach my $curr_popsize (@popsize) {

	    my $targetdir = $basedir . $method;
	    
	    if (! (-d $targetdir)) {
		`mkdir $targetdir`;
	    }

	    my $popdir = $targetdir . "/N" . $curr_popsize;
	    
	    if (! (-d $popdir)) {
		`mkdir $popdir`;
	    }

	    my $fname = $popdir . "/" . $method . "_" . $curr_fct;

	    print "target: " . $fname . "\n";

	    open (MYFILE, '>'.$fname) || die("failed to create file");

	    print "fct: " . $curr_fct . ", method: " . $method . ", n: " . $curr_popsize . ", runs: " . $runs . "\n";
	      
	    open( APPCALL, "java ModelTester -fct $curr_fct -method $method -n $curr_popsize -runs $runs -pmode 3 |") || die "exec failed";
	    
	    while( <APPCALL> ) {
		print MYFILE $_;
		
	    }
	    close MYFILE;
	    
	}
    }
}

exit;
