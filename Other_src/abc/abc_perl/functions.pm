package      functions;


### Include your variables and functions here

sub schaffer {
   my @param = @_;

   my $val1 = ($param[0]**2 + $param[1]**2);
   my $val2 = sqrt($val1);
   my $val3 = sin($val2)**2 - 0.5;
   my $val4 = (1 + 0.001 * $val1)**2;
   
   return (0.5 + ($val3/$val4));

}

sub sphere { 
    my @param = @_;
    
    my $fvalue = 0;

    foreach(@param) {
	$fvalue += $_ * $_;
    }
    
    return $fvalue;

}




1;
