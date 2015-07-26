import com.panayotis.gnuplot.JavaPlot;
import com.panayotis.gnuplot.plot.DataSetPlot;
import com.panayotis.gnuplot.dataset.PointDataSet;
import com.panayotis.gnuplot.dataset.Point;

import org.rosuda.JRI.*;
import global.*;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.regex.*;

public class plot {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
			
		int lb_significant = -1;
        
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
          }
          catch (Exception err) {
            err.printStackTrace();
          }
	}

}
