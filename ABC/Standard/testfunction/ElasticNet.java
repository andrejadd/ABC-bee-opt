package testfunction;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class ElasticNet extends TestFunction {

	private double[][] predictor; 
	private int response_idx;
	private double[] response;
	private int sample_size;
	private double lambda_1 = 0;   /* the Lasso penalty */
	private boolean ENABLE_INTERCEPT = true;
	private boolean ENABLE_LAMBDA1_AS_OPTPARAMETER = false;   /* if true, also try to optimize for lambda1 */
	
	
	public ElasticNet(int ai_response, double ad_low, double ad_up, double ad_min, int ai_minmax, int ai_id, String aC_name, boolean ab_checkrange) {
		super(0, ad_low, ad_up, ad_min, ai_minmax, ai_id, "ElasticNet", ab_checkrange);
    	
		

		if(ai_response == 0) {
			/* FIXME: we should probably throw an error here */
			System.out.println("Error: the response variable has to be larger than 0");
			System.exit(-1);
		}
		
		/* OK, this appears to make little sense but I like to use 1,2,.. as response indices when calling the code to 
		 * be conform with R (and don't mix it up at some point). However, internally it's still goes 0,1,.. with the array indexing 
		 * so I have to decrease it here.
		 */
		response_idx = ai_response;
		response_idx--;
    	
    }

	@Override
	public double solve(double[] param) {
		
		int dim = getDimension();
		
		callcounter++;
    	
    	if(dim != param.length) {
    		return -1; // through exception here
    	}

    	/* holds the moved position, moved by the function center */
    	double[] moved_param = new double[dim];
    	
    	/* should not change unless gd_move moves the center of the function */
    	for(int i = 0; i < dim; i++) { 
    		moved_param[i] = param[i] + gd_move;
    	}
    	
    	/* Ordinary Least Square */
    	double ols = 0;
    	
    	for(int i = 0; i < sample_size; i++) {

        	double y_estimate = 0;

    		for (int j = 0; j < dim; j++) {
    			y_estimate += (predictor[i][j] * moved_param[j]);   
    		}
    		
    		ols += Math.pow(response[i] - y_estimate, 2);
    		
    	}
    	
    	/* L1 penalty */
    	double l1_penalty = 0;

    	for(int i = 0; i < dim;i++) {
    		l1_penalty += Math.abs(moved_param[i]); 
    	}
    	    	
    	return (ols + lambda_1 * l1_penalty);
	}
	
	public double getMSE(double[] param) {
		
		int dim = getDimension();
		
		double lse = 0;
    	
    	for(int i = 0; i < sample_size; i++) {

        	double y_estimate = 0;

    		for (int j = 0; j < dim; j++) {
    			y_estimate += (predictor[i][j] * param[j]);   
    		}
    		
    		lse += Math.pow(response[i] - y_estimate, 2);
    		
    		
    	}
    	
    	return (lse / sample_size);
	
	}
	
	/* set the LASSO penalty factor, if zero there is no penalty */
	public void setLambda1(double ai_lambda1) {
		
		lambda_1 = ai_lambda1;
	
	}
	
	private double[][] readCSV(String filename) throws IOException {
		
		/* initialized while reading */
	    double[][] numbers = null;
	    
	    BufferedReader bufLRdr = new BufferedReader(new FileReader(filename));
		
	    /* get number of lines equals number of samples */
	    int lines = 0;
	    while (bufLRdr.readLine() != null) lines++;
	    bufLRdr.close();
	    
	    /* reopen, BufferedReader is bad with going back to start of file */
	    BufferedReader bufRdr = new BufferedReader(new FileReader(filename));
	    
	    String line = null;
	    int row = 0;
	    
	    /* read each line of text file */
	    while((line = bufRdr.readLine()) != null) {
	    	String field[] = line.split(",");
	    
	    	/* init here, have to know number of variables per line */
	    	if(numbers == null) {
	    		numbers = new double[lines][field.length];
	    	}
	    	
	    	for(int i = 0; i < field.length; i++) {
	    		numbers[row][i] = Double.parseDouble(field[i]);
	    	}
	    	row++;
		}
			    
	    /* close the file */
	    bufRdr.close();	
	    
	    return numbers;
	
	}
	
	public void readData(String aS_datafile) {

		/* here in goes the data we read */
    	double[][] data = null;
    	
    	/* check if file ok */ 
    	if(aS_datafile == null) {
    		System.out.println("Error: you have to specify a data file (using -f [filename]) for this function");
    		System.exit(1);
        }
    	
    	/* read a data file */
    	try {
			data = readCSV(aS_datafile);
		} catch (IOException e) {
			e.printStackTrace();
			System.exit(1);
		} 
    	
    	/* overwrite, number of rows */ 
    	sample_size = data.length;

    	/* get number columns */
    	int dim = data[0].length;

    	/* check if we have an intercept */
    	if(ENABLE_INTERCEPT) {
    		dim++;
    	}
    	
    	/* check if to include lambda1 as a parameter to optimize over */
    	if(ENABLE_LAMBDA1_AS_OPTPARAMETER) {
    		dim++;
    	}
    	
    	/* init predictor matrix */ 
    	predictor =  new double[sample_size][dim-1]; 
    	    	    	
    	/* init response vector */
    	response = new double[sample_size];
    	
    	/* copy elements in data to predictor matrix and response vector */
    	for(int i = 0; i < sample_size; i++) {
    		
    		int col_offset = 0;
    		for(int j = 0; j < data.length; j++) {
    			
    			/* if it's the response idx, copy to response vector */
    			if(j == response_idx) {
    				response[i] = data[i][j];
    				continue;
    			}
    			    			
    			predictor[i][col_offset] = data[i][j];
    			col_offset++;
    		}
    	}
    	/* FIXTHIS */
    	/* add the intercept and/or penalty terms to the end of the design matrix */
    	for(int i = 0; i < sample_size; i++) {
    		
    		int col_offset = 0;
    		for(int j = data.length; j < dim; j++) {
    			
    	    	/* if intercept enabled and the first  and this is the last loop then we have read
    			 * all data and add the intercept covariate (1) here
    			 */
    			if(ENABLE_INTERCEPT && (j == data.length)) {
    				predictor[i][col_offset] = 1;
    				continue;
    			}
    			
    			col_offset++;
    		}
    	}
		
    	/* doing this to subtract the response variable (FIXME: when we add an intercept we dont need to subtract 1) */
    	setDimension(dim--);	

    	/* set a new boundary for the last argument, i.e. lambda1 penalty factor */
    	if(ENABLE_LAMBDA1_AS_OPTPARAMETER) {
    		setBoundary(1, dim, 0); // lower
    		setBoundary(2, dim, 50); // upper
    	}

    	
	}
	
	/* set false if to disable intercept, on default it is enabled */
	public void setIntercept(boolean ab_value) {
		ENABLE_INTERCEPT = ab_value;
	}

}

