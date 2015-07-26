
package testfunction;

//import java.util.ArrayList;
//import java.util.Random;

public class TestF1 extends TestFunction {

    
    public TestF1(int dimension, double ad_low, double ad_up, double ad_min, int ai_minmax, int ai_id, String aC_name) {
    	super(dimension, ad_low, ad_up, ad_min, ai_minmax, ai_id, aC_name);
    }

    public double solve(double[] param) {
    	
    	callcounter++;
    	
    	if(dimension != param.length) {
    		return -1; // through exception here
    	}

    	double gd_x1 = param[0] + gd_move;
    	double gd_x2 = param[1] + gd_move;

    	
    	double a = Math.pow(gd_x1, 2) + Math.pow(gd_x2, 2);
    	double b = Math.sqrt( a);
    	double c = Math.pow( Math.sin(b), 2) - 0.5;
    	//double c = Math.pow( Math.sin(b/180 * Math.PI), 2) - 0.5;
    	
    	double d = Math.pow(1 + 0.001 * a, 2 );
    	double e = 0.5 + c / d;
    	
    	return e;
    
    }

}
