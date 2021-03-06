#!/usr/bin/perl

package s_abc;

use strict;
use warnings;
use functions;
use statistics;

sub abc {
    
    my ($repbest, $popSize, $optvalue, $feval, $dim, $MCN, $lb, $ub) = @_;

    my $n = int($popSize/2);
    my $cycle = 0;
    my $limit = $n * $dim;
    
    my @solutions;
    my @values;
    my @fitness;
    my @abcounters;
    
    my $gbfit;
    my $gbvalue;
    

    for(my $i = 0; $i < $n; $i++) {

	my @newsolution;
	
	for(my $j = 0; $j < $dim; $j++) {
	    push (@newsolution, $lb + (($ub - $lb) * rand()));
	}
	
	push(@solutions, [ @newsolution ]);
	
	my $lvalue = $feval->(@newsolution);
	push( @values, $lvalue );
	push( @fitness, fit($lvalue));
	
	push( @abcounters, 0);
    }
    
    $gbfit = $fitness[0];
    $gbvalue = $values[0];
    
    my @improvedhistory;
    my $historylength = 10;
    my $historycounter = 0;

    do {

	my $improved = 0;

	
# EB Phase
	my @modsolutions;
	
	for(my $i = 0; $i < $n; $i++) {
	    
	    my $sol = $solutions[$i];
	    
	    my $j = int(rand($dim));
	    my $k;
	    do {
		$k = int(rand($n));
	    } while ($k == $i);
	    
	    my $solrefk = $solutions[$k];
	    my @newsolution = @$sol; 
	    
	    $newsolution[$j] = $newsolution[$j] + ( ($newsolution[$j] - @$solrefk[$j]) * (rand(2)-1) ) ;
	    
	    push @modsolutions, [@newsolution];
	    
	}
	
	
# eval, fitness, greedy
	my @values_tmp;
	my @fitness_tmp;
	
	foreach(@modsolutions) {
	    push @values_tmp, $feval->(@$_);
	}
	foreach(@values_tmp) {
	    push @fitness_tmp, fit($_);
	}
	
	
# do greedy seletion
	for(my $i = 0; $i < $n; $i++) {
	    
	    if($fitness_tmp[$i] > $fitness[$i]) {
		
		$solutions[$i] = $modsolutions[$i];
		$values[$i] = $values_tmp[$i];
		$fitness[$i] = $fitness_tmp[$i];
		$abcounters[$i] = 0;

		$improved++;
		
	    } else {
		$abcounters[$i]++;
	    }
	}
	
# calc Select Probs
	my $fitness_sum = 0;
	my @sprobs;
	
	foreach(@fitness) {
	    $fitness_sum += $_;
	}
	foreach(@fitness) {
	    push @sprobs,  ($_ / $fitness_sum);
	}
	
# OB Phase

	for(my $i = 0; $i < $n; $i++) {
	    my $randv = rand();
	    my $accrandv = 0;
	    
	    for(my $s = 0; $s < $n; $s++) {
		if( ($randv > $accrandv) && ($randv < ($accrandv + $sprobs[$s]))) {
		    # get selected
		    
		    my $solref = $solutions[$s];
		    
		    my $j = int(rand($dim));
		    my $k;
		    do {
			$k = int(rand($n));
		    } while ($k == $s);
		    
		    my $solrefk = $solutions[$k];
		    
		    my @newsolution = @$solref;
		    
		    $newsolution[$j] = $newsolution[$j] + ( ($newsolution[$j] - @$solrefk[$j]) * (rand(2)-1) ) ;
		    
		    my $newvalue = $feval->(@newsolution);
		    my $newfit = fit($newvalue);
		    
		    # if better replace right here
		    if($newfit > $fitness[$s]) {
			$solutions[$s] = [ @newsolution ];
			$values[$s] = $newvalue;
			$fitness[$s] = $newfit;
			$abcounters[$s] = 0;

			$improved++;
		    }
		    
		    last;
		} 
		$accrandv += $sprobs[$s];
	    }
	}
	
	
# remember best
	my ($cfit, $mindex) = max(@fitness);
	if($cfit > $gbfit) {
	    $gbfit = $cfit;
	    $gbvalue = $values[$mindex];
	    if($repbest) {
		print $cycle . "\t" . $gbvalue . "\n";
	    }
	}
	# Minimization
	if($gbvalue < $optvalue  ) {
	    return ($gbvalue, $cycle);

	}
	
# Scout Phase
	my ($abcount, $abindex) = max(@abcounters);
	if($abcount > $limit) {
	    $abcounters[$abindex] = 0;
	    my @randsolution;
	    for(my $i = 0; $i < $n; $i++) {
		push @randsolution, $lb + (($ub - $lb) * rand());
	    }
	    
	    $solutions[$abindex] = [ @randsolution ];
	    
	    my $lvalue = $feval->(@randsolution);
	    $values[$abindex] = $lvalue;
	    
	    $fitness[$abindex] = fit($lvalue);
	}

	if($historycounter++ >= $historylength) {
	    # take mean and print
	    my $m = statistics::mean(@improvedhistory);
	    print int($m) .   "\n";
	    @improvedhistory = 0;
	    shift @improvedhistory;
	    
	    $historycounter = 0;
	} else {
	    push @improvedhistory, $improved;
	}
	
	
	
    } while($cycle++ < $MCN);

    return ($gbvalue, $cycle);
}





sub fit {
    my $param = shift;

    return (1 / (1 + $param));


}

sub max {
    my $max = $_[0];
    my $mindex = 0;
    my $i = 0;
    foreach(@_){

	if($_ > $max) {
	    $max = $_;
	    $mindex = $i;
	}
	$i++;
	
    }

    return ($max, $mindex);
}



1;
