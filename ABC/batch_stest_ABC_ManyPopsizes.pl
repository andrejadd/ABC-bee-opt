#!/usr/bin/perl

use strict;

my $local |= 1;

#my @methodname = ("ABCgbestrank_1", "ABCgbestrank_2", "ABCgbestrank_3");
#my @methodname = ("ABCsel_dist_1", "ABCsel_dist_2", "ABCsel_dist_3");
my  @methodname = ("SABC");


my @fct = ("sc", "sm", "gr", "rg", "rn", "ac");

my $basedir =  "/home/andrej/Desktop/latex-arbeit/final_figs/ABCMANYPOPSIZES_STEST/";

my @popsizes = (6,10,30,60,100,120,140);


my $runs = 100;
my $standardlimit = "false";

my $printmode = 1; # default

foreach my $curr_popsize (@popsizes) {

    my $neb = int ($curr_popsize / 2);

    foreach my $method (@methodname) {

	foreach my $curr_fct (@fct) { 

	    my $fname = $basedir . $method . "_neb" . $neb . "_" . $curr_fct;

	    print "target: " . $fname . "\n";

	    open (MYFILE, '>'.$fname) || die("failed to create file");
	
	    open( APPCALL, "java ModelTester -fct $curr_fct -method $method -neb $neb -nob 0 -pmode 1 -runs $runs -standardlimit $standardlimit |") || die "exec failed";
	    
	    while( <APPCALL> ) {

		if(/#/) {
		    print $_;
		} else {
		
		    print MYFILE $_;
		    chomp();
		    print $_ . "\t";
		}
	    }
	    print "\n";
	    close MYFILE;
	}
    }
}

exit;
