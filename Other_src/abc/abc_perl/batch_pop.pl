#!/usr/bin/perl

use strict;


#my @fct = ("sc", "gr", "rg", "rn", "ac");

my @fct = ("sm");

my @neb = (5,10, 20, 30, 50, 70);
my @nob = (5,10, 20, 30, 40);
my @noblimits = (1,150);



foreach my $curr_fct (@fct) {

foreach my $curr_neb (@neb) {

    my $dir = "POPSIZETEST/";
    my $fname = $dir . $curr_fct . "_" . $curr_neb;
  
    open (MYFILE, '>'.$fname) || die("failed to create file");

#    foreach my $curr_nob (@nob) {
    for(my $z = $noblimits[0]; $z <= $noblimits[1]; $z += 5) {	
	

	open( APPCALL, "./run.pl $curr_neb $z |") || die "exec failed";

	while( <APPCALL> ) {
	    print ".";
	    print MYFILE $z . "\t" . $_;
	}

    }

	close MYFILE
}
}

exit;
