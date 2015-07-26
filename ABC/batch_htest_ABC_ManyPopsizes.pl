#!/usr/bin/perl

use strict;

my $local |= 1;

#my @methodname = ("ABCgbestrank_1", "ABCgbestrank_2", "ABCgbestrank_3");
#my @methodname = ("ABCsel_dist_1", "ABCsel_dist_2", "ABCsel_dist_3");
my  @methodname = ("SABC");


#my @fct = ("sc_htest", "sm_htest", "gr_htest", "rg_htest", "rn_htest", "ac_htest");
my @fct = ("rn_htest");

my $basedir =  "/home/andrej/Desktop/latex-arbeit/final_figs/ABC_DATA/ABCMANYPOPSIZES_HTEST_EXP1/";

my @popsizes = (10);

my $runs = 100;
my $standardlimit = "false";

my $printmode = 1; # default

foreach my $curr_popsize (@popsizes) {

    my $neb = int($curr_popsize / 2);
    my $nob = $curr_popsize - $neb;

    foreach my $method (@methodname) {

	foreach my $curr_fct (@fct) { 


	    my $fctname = $curr_fct;
	    $fctname =~ s/_htest//;
	    my $fname = $basedir . $method . "_n" . $curr_popsize . "_" . $fctname;

	    print "target: " . $fname . "\n";

	    open (MYFILE, '>'.$fname) || die("failed to create file");
	
	    open( APPCALL, "java ModelTester -fct $curr_fct -method $method -neb $neb -nob $nob -pmode 1 -runs $runs -standardlimit $standardlimit |") || die "exec failed";
	    
	    while( <APPCALL> ) {

		if(/#/) {
		    print $_;
		} else {
		    chomp();
		    if($_ > 1000000) {
			print MYFILE "-1\n";
			print -1 . "\t";
		    } else {
			print MYFILE $_ . "\n";
			print $_ . "\t";
		    }

		}
	    }
	    print "\n";
	    close MYFILE;
	}
    }
}

exit;
