
package testfunction;

//import java.util.ArrayList;
import java.util.Random;

public abstract class TestFunction {

    Random generator;
    
    /* I set this private, so when to change dimension it has to be done via setDimension() */
    private int dimension;
    
    /* lower and upper boundary of parameter space for each parameter */
    private double[] gd_lowerbound;   
    private double[] gd_upperbound;
    
    /* use this to initialize the boundary array to some value in the case they were reset */
    private double[] default_boundary = {-10,10};
    
    /* this is the optima or some goal above/below it */
    private double  gd_optima;  
    
    /* need this to see where we are relative to the optima */
    private int gi_minmax = 0;  // 0 is minimize, 1 is max.
    
    /* for identifying */
    private int gi_fctid = 0;
    
    /* string name, used for output */
    private String gC_name;
    
    public int callcounter = 0;
    
    /* add this value to the input parameter, will move the whole function */
    public double gd_move = 0;
    
    /* this is just for testing, should be true on default */
    public boolean gb_checkrange = false;
    
    public TestFunction(int dimension, double ad_low, double ad_up, double ad_min, int ai_minmax, int ai_id, String aC_name, boolean ab_checkrange) {
    	generator = new Random(System.currentTimeMillis());
    	this.dimension = dimension;
    	
    	/* set lower and upper bounds, for parameter specific boundaries use setBoundary() */ 
    	gd_lowerbound = new double[dimension];
    	gd_upperbound = new double[dimension];
    	
    	for(int i = 0; i < dimension; i++) {
    		gd_lowerbound[i] = ad_low;
    		gd_upperbound[i] = ad_up;
    	}
    	
    	/* save as defaults in the case of dimension resize */
    	default_boundary[0] = ad_low;
    	default_boundary[1] = ad_up;
    	
    	
    	gd_optima = ad_min;
    	gi_minmax = ai_minmax;
    	gi_fctid = ai_id;
    	gC_name = aC_name;
    	gb_checkrange = ab_checkrange;
    }

    /* Generate random parameters */
    public double[] getRandomParam() {
		
		double[] ret = new double[dimension];
			
		for(int i = 0; i < dimension; i++) {
		
			double offset_factor = Math.abs(gd_lowerbound[i]) + Math.abs(gd_upperbound[i]);
				
			ret[i] = gd_lowerbound[i] + (generator.nextDouble() * offset_factor); // nextDouble() produces in [0,1]
			  
			if(ret[i] < gd_lowerbound[i] || ret[i] > gd_upperbound[i]) {
				System.out.println("error in getRange , param out of bounds");
				System.exit(1);
			}
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

    public double checkrange(double ad_param, int param_idx) {
    	
    	if(gb_checkrange) {
    		if(ad_param < gd_lowerbound[param_idx]) {
    			return gd_lowerbound[param_idx];
    		}
    		if(ad_param > gd_upperbound[param_idx]) {
    			return gd_upperbound[param_idx];
    		}
    	}

    	return ad_param;
    }
    
    
    public int getDimension () {
    	return dimension;
    }
    
    public void setDimension(int ai_dim) {
    	dimension = ai_dim;

    	/* free */
    	gd_lowerbound = null;
    	gd_lowerbound = null;
    	
    	/* resize lower and upper boundary */ 
    	gd_lowerbound = new double[dimension];
    	gd_upperbound = new double[dimension];
    	
    	/* set to default */
    	for(int i = 0; i < dimension; i++) {
    		gd_lowerbound[i] = default_boundary[0];
    		gd_upperbound[i] = default_boundary[1];
    	}
    }
    
    /* set boundaries for parameters individually, ai_boundary = 1 (lower) and 2 (upper) */
    public void setBoundary(int ai_boundary, int ai_idx, double ad_value) {
    	if(ai_boundary == 1) {
    		gd_lowerbound[ai_idx] = ad_value;
    	} else {
    		gd_upperbound[ai_idx] = ad_value;
    	}
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

	public double getGd_lowerbound(int ai_index) {
		return gd_lowerbound[ai_index];
	}

	public double getGd_upperbound(int ai_index) {
		return gd_upperbound[ai_index];
	}
	
	/* 
	 * Checks if the value passed is bettr than the optima/goal.
	 * Returns true if the goal/optima was reached or even better (in case of a goal above the optima), otherwise false
	 * 
	 */
	public boolean optimaReached(double ad_value) {
		
		if(Double.isNaN(gd_optima)) {
			return false;
		}
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
	
	public void move(double value) {
		gd_move = value;
	}
	
	public void setcheckrange(boolean ab_checkrange) {
		gb_checkrange = ab_checkrange;
	}

	/* create for ElasticNet.java for regression stuff (to not only have solve() */
	public double getMSE(double[] params) {
		// TODO Auto-generated method stub
		
		return 0;
	}
}
