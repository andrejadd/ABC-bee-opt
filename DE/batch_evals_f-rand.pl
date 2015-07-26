#!/usr/bin/perl

use strict;

my $popsize = 60;
my $runs = 5;
my $maxevals = 300000;
my $psotype = 0; # gbest PSO
my $paramtype = 0; # a) params -> w= 0.6, c=1.7
my $ffixed = 0;
my $iterations = $maxevals / $popsize;



# method name for file
my $methodname = "debin_frand";

# fct names for output
my @fctnames = ("sc", "sm", "gr", "rg", "rn", "ac");

foreach my $fct (@fctnames) {
    
    my $dir = "../../FINAL_EXP_RESULT/LASTEXP/RUNTIME/";
    
    my $fname = $dir . $fct . "_" . $methodname; # easier to handle
    
    print "+";
    
    open (MYFILE, '>'.$fname) || die("failed to create file");
    
    open( APPCALL, "java DE_run -fct $fct -mcn $iterations -runs $runs -ffixed $ffixed |") || die "exec failed";
    
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
