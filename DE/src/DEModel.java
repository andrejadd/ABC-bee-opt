


import testfunction.*;

import java.util.ArrayList;
import java.util.Random;

public class DEModel implements ModelInterface {
    
    static Random generator;
    private Blackboard bb;
  
    private ArrayList<DEAgent> agents;
    private static TestFunction fct1;
    private int nr_agents;
    private int dimension;
    private double gd_crossover;
    private double gd_scaling;
    private int gi_type; 
        
    /* the rate of successful solution update */
	private double gd_updaterate;
	
	private boolean gb_f_fixed = true;
    private double gd_f_min = 0.3;
    private double gd_f_max = 0.9;

    public DEModel (TestFunction aC_fct, int ai_n, int ai_type, boolean ab_f_fixed, double ad_fscale, double ad_cr) {
		
		generator = new Random(System.currentTimeMillis());
		
		nr_agents = ai_n;
		dimension   = aC_fct.getDimension();
		fct1 = aC_fct;
		gd_crossover = ad_cr;
		gd_scaling = ad_fscale; // default, may change dependend on gb_f_fixed true or false
		gi_type = ai_type;
		gb_f_fixed = ab_f_fixed;
		Initiate();
    }

    public void Initiate() {

    	// holds all solutions
    	bb = new Blackboard(nr_agents, dimension, fct1);  
	
    	// this is the swarm
    	agents = new ArrayList<DEAgent>();
		
    	// Create agents with randomly uniform distribution in domain space
    	for (int i = 0; i < nr_agents; i++) {	    
	    
    		// create random position
    		double[] x = fct1.getRandomParam();

    		double ld_value = fct1.solve(x);
    		
    		// add to agent list
    		agents.add(new DEAgent(bb, fct1, x, ld_value, i, gd_crossover, gi_type));

    		// add to blackboard
    		bb.add(x, ld_value );
    	}
     }

    public void Iterate() {

    	/* modifies all positions according to DE algorithm
    	   this does not change yet the agents actual position, cause we need it
    	*/
    	for (int i = 0; i < nr_agents; i++) {
    		DEAgent a = (DEAgent) agents.get(i);
    		
    		
    		if(!gb_f_fixed) {
    		    gd_scaling = gd_f_min + generator.nextDouble()*(gd_f_max - gd_f_min);
    		
    		}
    		a.modifyposition(gd_scaling);
    	}

    	/* actually updates the position,value inside the agent
    	   must be called after all agents modifyposition() was called because an agents takes 3 other agents
    	   as reference and we want the old data, can be changed of course (and is probably better)
    	*/
    	for (int i = 0; i < nr_agents; i++) {
    		DEAgent a = (DEAgent) agents.get(i);
    		a.updateposition();
    	}
	
    }


    
    public Solution getBestSolution() {

    	return (Solution) bb.getBestSolution();

    }
    
    public String getAgentData() {
	
	String tmp = new String();
	
	java.text.NumberFormat fn = (java.text.NumberFormat) java.text.NumberFormat.getInstance();
	fn.setMaximumFractionDigits(3);

	for (int i = 0; i < agents.size(); i++) {	    
	    
	    // create random position
	    DEAgent tmpagent = (DEAgent) agents.get(i);
	    
	    for(int j = 0; j < dimension; j++) {
		tmp += "\tx[" + j + "]=" + fn.format(tmpagent.position[j]) + " ";
	    }

	    tmp += ", value=" + fn.format(tmpagent.value) + "\n";
	}
	return tmp;
	
	}
    
    public void getData(double[] ada_x, double[] ada_y) {
 	   
    }
    
    public double getBest() {
    	return -1.0;
    }

    public ArrayList getSolutions(int li_type) {
		return agents;
	}
    
    public int getFctCalls() {
		return fct1.callcounter;
	}
    
    public int getFctID() {
		return fct1.getID();
	}
    
    public double getUpdateRate() {
		return gd_updaterate;
	}
    
    public int getPopSize() {
		return agents.size();
	}
    
    public void resetFctCounter() {
		fct1.resetcallcounter();
	}
    
    public String getFctName() {
		return fct1.getName();
	}
    
    public boolean optimaReached(double ad_value) {
		   return fct1.optimaReached(ad_value);
	   }
}


