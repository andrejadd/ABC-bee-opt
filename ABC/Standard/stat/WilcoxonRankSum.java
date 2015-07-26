package stat;

import java.io.*;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class WilcoxonRankSum {

	public static Double Test(int[] x, int[] y ) {
		
		double ld_pvalue = -1;
				
		/* write data */
		String filex = "x.dat";
		String filey = "y.dat";
		  
		try {
			PrintWriter outFilex = new PrintWriter(new BufferedWriter(new FileWriter(filex)));
			PrintWriter outFiley = new PrintWriter(new BufferedWriter(new FileWriter(filey)));
	
			for (int i = 0; i < x.length; i++) { 
				outFilex.println(x[i]);        // and print it to the file
			 }
			for (int i = 0; i < y.length; i++) { 
				outFiley.println(y[i]);        // and print it to the file
			 }
			
			outFilex.close();  // standard file housekeeping
			outFiley.close();  // standard file housekeeping
		} catch (Exception ie) { System.out.print(ie.getMessage()); };

	
		
		try {
			String line;
	        String[] cmd = new String[3];
	       
	        cmd[0]="R";
	        cmd[1]="-f";
	        cmd[2]="./rwilcoxon.r";
	        Process p = Runtime.getRuntime().exec(cmd);
	        BufferedReader input = new BufferedReader (new InputStreamReader(p.getInputStream()));
	            
	        Pattern lC_pa = Pattern.compile("p-value");
	            
	        while ((line = input.readLine()) != null) {
	            	
	        	Matcher m = lC_pa.matcher(line);
	            if(m.find()) {
	            	String s = line.split( " " )[5];
	            	//String s = line.substring( 1, line.length() - 1 ).split( " " )[4] ;
	                ld_pvalue = Double.valueOf(s);
	                //System.out.println("p-value: " + ld_pvalue);
	                break;
	            }
	         }
	         input.close();
	     } catch (Exception err) {
	    	 err.printStackTrace();
	     }
	     	     
		 return ld_pvalue;
	}
}
