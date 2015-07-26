
package global;

//import java.util.ArrayList;
import java.util.Random;

public abstract class TestFunction {

    Random generator;
    public int dimension;
    private double gd_lowerbound;
    private double gd_upperbound;
    
    /* this is the optima or some goal above/below it */
    private double  gd_optima;  
    
    /* need this to see where we are relativ to the optima */
    private int gi_minmax = 0;  // 0 is minimize, 1 is max.
    
    /* for identifying */
    private int gi_fctid = 0;
    
    /* string name, used for output */
    private String gC_name;
    
    public int callcounter = 0;
    
    public TestFunction(int dimension, double ad_low, double ad_up, double ad_min, int ai_minmax, int ai_id, String aC_name) {
    	generator = new Random(System.currentTimeMillis());
    	this.dimension = dimension;
    	gd_lowerbound = ad_low;
    	gd_upperbound = ad_up;
    	gd_optima = ad_min;
    	gi_minmax = ai_minmax;
    	gi_fctid = ai_id;
    	gC_name = aC_name;
    }

    // DO GENERATE RANDOM PARAMETER HERE BECAUSE IS FUNCTION DEPENDENT
    public double[] getRandomParam() {
		
		double[] ret = new double[dimension];
			
		double offset_factor = Math.abs(gd_lowerbound) + Math.abs(gd_upperbound);
			
			for(int i = 0; i < dimension; i++) {
				
			    ret[i] = gd_lowerbound + (generator.nextDouble() * offset_factor); // nextDouble() produces in [0,1]
			  
			    if(ret[i] < gd_lowerbound || ret[i] > gd_upperbound) {
			    	System.out.println("error in getRange , param out of bounds");
			    	System.exit(1);
			    }
			}
			
			return ret;
    }

    // DO GENERATE RANDOM PARAMETER HERE BECAUSE IS FUNCTION DEPENDENT
    public double[] getRandomParam(double []ad_center) {
		
		double[] ret = new double[dimension];
			
		double offset_factor = Math.abs(gd_lowerbound) + Math.abs(gd_upperbound);
			
		for(int i = 0; i < dimension; i++) {
			
		    ret[i] = gd_lowerbound + (generator.nextDouble() * offset_factor) + ad_center[i] ; 
					    
		}
			
		return ret;
    }

    
    public abstract double solve (double[] param);

    /* if first arg matches criterio over second arg, return true */
    public boolean better(double ad_x1, double ad_x2) {

    	// minimize .. 
    	if(gi_minmax == 0) {
    		if(ad_x1 < ad_x2) { return true; }
    	} else { // maximize
    		if(ad_x1 > ad_x2) { return true; }
    	}
    	return false;
    }

    public double checkrange(double ad_param) {
    	
    	/* no bounds on dynamic range */
    	return ad_param;
    }
    
    public int getDimension () {
    	return dimension;
    }
    
    public void resetcallcounter() {
    	callcounter = 0;
    }
    
    public int getID () {
    	return gi_fctid;
    }
    
    public String getName() {
    	return gC_name;
    }

	public double getGd_lowerbound() {
		return gd_lowerbound;
	}

	public double getGd_upperbound() {
		return gd_upperbound;
	}
	
	/* 
	 * Checks if the value passed is bettr than the optima/goal.
	 * Returns true if the goal/optima was reached or even better (in case of a goal above the optima), otherwise false
	 * 
	 */
	public boolean optimaReached(double ad_value) {
		
		if(ad_value == gd_optima) {
			return true;
		}
		
		/* In case we have a goal that is not exactly the optimum we need to make sure 
		 * to track it if it passes the value 
		 */
		if(gi_minmax == 0) { /* Minimize */
			/* and value smaller */
			if(ad_value < gd_optima) {
				return true;
			}
		} else { /* Maximize */
			if(ad_value > gd_optima) { /* bigger */
				return true;
			}
		}
		
		return false;
		
	}
	
	public double getOptima() {
		return gd_optima;
	}
	
	public abstract void addToSeverity(double ad_value);
	public abstract void resetSeverity();
}
