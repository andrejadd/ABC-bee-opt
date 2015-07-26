#!/usr/bin/perl

use strict;

my @methodname = ("SABC", "ABCrank", "ABCgbest", "ABCgbestrank");
my @fct = ("sc", "sm", "gr", "rg", "rn", "ac");

my $basedir =  "$HOME/workspace/FINAL_EXP_RESULT/METHOD_DATA_STEST/";

my $target = "$HOME/workspace/FINAL_EXP_RESULT/BESTMODS_MOVED_CENTER/N100/";


foreach my $method (@methodname) {

    for (my $k = 0; $k <= $#fct; $k++) {

	my $curr_fct = $fct[$k];

	my $fname = $basedir . $method . "/FCT_MOVED_CENTER/N100/" . $method . "_" . $curr_fct;

	`cp $fname $target`;
    }
}

exit;
