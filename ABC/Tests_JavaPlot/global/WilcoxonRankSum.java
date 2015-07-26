package global;

import java.io.*;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class WilcoxonRankSum {

	public static int Test(double[] x, double[] y ) {
		
		int lb_significant = -1;
		
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
	        String[] cmd = new String[1];
	        cmd[0]= "./runWilcoxon.sh";
	        Process p = Runtime.getRuntime().exec(cmd);
	        BufferedReader input = new BufferedReader (new InputStreamReader(p.getInputStream()));
	            
	        Pattern lC_pa = Pattern.compile("alternative hypothesis");
	        Pattern lC_pgreater = Pattern.compile("greater");
	        Pattern lC_pless = Pattern.compile("less");
	            
	        while ((line = input.readLine()) != null) {
	            	
	        	Matcher m = lC_pa.matcher(line);
	            if(m.find()) {
	                	
	            	Matcher lC_mless = lC_pless.matcher(line);
	                Matcher lC_mgreater = lC_pgreater.matcher(line);
	                	
	                if(lC_mgreater.find()) {
	                	System.out.println("greater");
	                	lb_significant = 1;
	                } else if(lC_mless.find())  {
	                	System.out.println("less");
	                	lb_significant = 0;
	                }
	            }
	         }
	         input.close();
	     } catch (Exception err) {
	    	 err.printStackTrace();
	     }
		return lb_significant;
	}
}
