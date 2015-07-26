
package testfunction;

//import java.util.ArrayList;
//import java.util.Random;

public class TestF3 extends TestFunction {

    
    public TestF3(int dimension, double ad_low, double ad_up, double ad_min, int ai_minmax, int ai_id, String aC_name) {
    	super(dimension, ad_low, ad_up, ad_min, ai_minmax, ai_id, aC_name);
    }

    public double solve(double[] param) {
    	
    	callcounter++;
    	
    	if(dimension != param.length) {
    		return -1; // through exception here
    	}

    	double sum = 0;
    	double prod = 1;
    	

    	for(int i = 0; i < dimension; i++) { 

    		double gd_x = param[i] + gd_move;
    		
    		sum += ( gd_x * gd_x );
    		
    		prod *= ( Math.cos( gd_x / Math.sqrt(i+1)));

    	}
	
    	return (sum/4000 - prod + 1);

    }

}
