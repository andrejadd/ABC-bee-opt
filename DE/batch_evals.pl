#!/usr/bin/perl

use strict;

my $METHOD = "DE";
my $popsize = 60;
my $runs = 100;
my $men = 500000;

# fct names for output
my @fctnames = ("sc", "sm", "gr", "rg", "rn", "ac");

foreach my $fct (@fctnames) {
    
    my $dir = "../../FINAL_EXP_RESULT/METHOD_DATA_STEST/";
    
    my $fname = $dir . $METHOD . "/" . $METHOD . "_n" . $popsize . "_" . $fct; 
    
    print "+";
    
    open (MYFILE, '>'.$fname) || die("failed to create file");
    
    open( APPCALL, "java DE_run -fct $fct -men $men -runs $runs|") || die "exec failed";
    
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
