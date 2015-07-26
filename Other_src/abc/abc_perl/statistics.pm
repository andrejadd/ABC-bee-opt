package statistics;

sub WilcoxonRankSum {

    my ($xref, $yref) = @_;

		
    my $ld_pvalue = -1;
				
    open (XFILE, '>x.dat')|| die("Cannot Open File");;
    open (YFILE, '>y.dat')|| die("Cannot Open File");;

    print XFILE join("\n", @$xref) . "\n";
    print YFILE join("\n", @$yref) . "\n";

    close(XFILE);
    close(YFILE);

    #my @test = exec "R --vanilla < r_wrs.r" or die "Unable to exec : $!\n";;
    
    #foreach(@test) {
    #	if($_ =~ /p-value/) {
    #	    print $3;
    #	}
    #}


    my $return_val = -1;

    open( DIR, "R --vanilla < r_wrs.r|" ) || die "No ls"; 
    while( <DIR> ){      # read command's output from the pipe
        if($_ =~ /p-value/) {
	    my @tmp = split(" ", $_);
	    
	    $return_val = $tmp[$#tmp];
	}
    } 

    close DIR;      

    return $return_val;



    
}


sub mean {
    
    my $sum = 0;
    foreach(@_){
	$sum += $_;
    }

    return $sum/$#_;
    

}


1;    
	
	
