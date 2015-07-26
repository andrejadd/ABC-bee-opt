function ObjVal = schaffer(Chrom,switc);
   
      Dim=size(Chrom,2);
      if(Dim > 2) 
         printf('wrong dimension on Schaffer function.');
         ret
        end
   
        val1 = sum(Chrom.^2, 2);
        val2 = sqrt(val1);
        val3 = sin(val2) .^2;
        val4 =  val3 - 0.5;
       
        val5 = 1 + 0.001 * val1;
        val6 = val5 .^ 2;
        
        ObjVal = 0.5 + (val4 ./ val6);
        
  
   end   
  

% End of function
