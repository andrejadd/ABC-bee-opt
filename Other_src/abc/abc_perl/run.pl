#!/usr/bin/perl

use strict;

use s_abc;
use ABCpopsize;

#use m3_abc;

#use s_abc_ratiov1;
use statistics;
use functions;

my $runs = 3;
my $repbest = 0;
my $optvalue = 0.01;
my $feval = \&functions::sphere;
my $lb = -100;
my $ub = 100;
my $dim = 30;
my $MCN = 10000;


my @x;
my @y;

my $neb = 50;
my $nob = 50;

if($#ARGV == 1) { # got two arguments
  $neb = $ARGV[0];
  $nob = $ARGV[1];
}

while($runs--) {

#    my ($valx, $cyclex, $evalsx) = s_abc::abc($repbest, 100, $optvalue, $feval, $dim, $MCN, $lb, $ub); 
    my ($valy, $cycley, $evalsy) = ABCpopsize::abc($repbest, $neb, $nob, $optvalue, $feval, $dim, $MCN, $lb, $ub); 

#    push @x, $evalsx;
    push @y, $evalsy;
 
#    print $evalsx . " - " . $valx . "\t" . $evalsy . " - " . $valy . "\n";
}

#print "\nmean x: " . statistics::mean(@x) . "\t" . ", y: " . statistics::mean(@y) . "\n";

#print "p-value: " . statistics::WilcoxonRankSum(\@x, \@y) . "\n";

print statistics::mean(@y) . "\n";

exit;


