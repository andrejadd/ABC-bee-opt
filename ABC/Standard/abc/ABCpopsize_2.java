package abc;


import global.ModelInterface;
import global.Solution;
import global.ParameterHolder;
import global.GlobalParameters;

import java.util.ArrayList;
import java.util.Random;

import testfunction.TestFunction;

public class ABCpopsize_2 implements ModelInterface {

	
	private static TestFunction fct1;
	private ArrayList<Solution> gC_solutions = new ArrayList<Solution>();;
		
	private static Random generator;
	
	//ParameterHolder gC_params;
	
	/* size of solution vector */
	private int gi_dim;
	private int gi_limit;
	
	/* nr. solutions, ebs and obs */
	private int gi_neb;
	private int gi_nob;
	
	/* the rate of successful solution update */
	private double gd_updaterate;
	
	/* for DEBUG */
	private static int gi_cycle = 0;
	
	public ABCpopsize_2() {}
	
	public void Init(TestFunction am_fcttest, 
			ParameterHolder aC_params) {
	
		fct1 = am_fcttest;
		//gC_params = aC_params;
		
		generator = new Random(System.currentTimeMillis());
		
		gi_dim = fct1.getDimension();
		gi_neb = (Integer) aC_params.get(GlobalParameters.NEB);
		gi_nob = (Integer) aC_params.get(GlobalParameters.NOB);
		//gi_n = (Integer)aC_params.get(GlobalParameters.popSize) / 2;
		//gi_limit = gi_neb * gi_dim;
		gi_limit = 500; /* moderater Wert, kommt Schaffer auch gut damit */
		
		

		for (int i = 0; i < gi_neb; i++) {	    
			double[] x = fct1.getRandomParam();
			double ld_value = fct1.solve(x);

			Solution lC_tmp = new Solution(x, ld_value);
			lC_tmp.limit = 0;
			gC_solutions.add(lC_tmp);
		}
		
	}

	public void Iterate() {
		
		gi_cycle++;
		
		ArrayList<Solution> gC_newsolutions = new ArrayList<Solution>();
		
		boolean lb_optimareached = false;
		
		int li_updatecounter = 0;
		
		/* 1: EBs modify positions and save in new solution array  */
		for(int i = 0; i < gi_neb; i++) {
			
			Solution lC_tmp = gC_solutions.get(i);
			double [] lda_newposition = modifyposition(lC_tmp, i);
			lC_tmp.limit++;
			
			double ld_value = fct1.solve(lda_newposition);
			
			if(fct1.better(ld_value, lC_tmp.value)) {
				lC_tmp.newpostype = 1;
				lC_tmp.limit = 0;
				lC_tmp.set(lda_newposition, ld_value);
				li_updatecounter++;
			}
		}
		
		/* 3: check if one has reached the optima/goal */
		for(int i = 0; i < gi_neb; i++) {
			
			Solution lC_currentsol = gC_solutions.get(i);
			if(fct1.optimaReached(lC_currentsol.value)) {
				lb_optimareached = true;
			}
		}
		
		if(!lb_optimareached) {
		
			/* 4: Calculate Prob. values P_i */
			 calcSelectProbs();
			 
			 gC_newsolutions.clear();
			 
			 /* 5: OB select and modify immediately (as in matlab ABC) */
			 for(int i = 0; i < gi_nob; i++) {
				 
				 int li_selected = select();
				 Solution lC_tmpref = gC_solutions.get(li_selected);
				 lC_tmpref.limit++;
				 
				 double [] lda_newposition = modifyposition(lC_tmpref, li_selected);
				 
				 double newvalue = fct1.solve(lda_newposition);
				
				 if(fct1.better(newvalue, lC_tmpref.value)) {
					 lC_tmpref.set(lda_newposition, newvalue);
					 lC_tmpref.newpostype = 1;
					 lC_tmpref.limit = 0;
					 li_updatecounter++;
				 }				 
			 }
			 
			 /* 6: Replace the last solution with the highest abandon counter (as in the ABC matlab) */
			 int li_highest_limit = -1;
			 int li_index = -1;
			 int nr_ab = 0;
			 /* look for the highest limit value */
			 for(int i = 0; i < gi_neb; i++) {
				Solution lC_sol = gC_solutions.get(i);
	
				if(lC_sol.limit >= gi_limit) {
					
					if(lC_sol.limit > li_highest_limit) {  // check if highest
						/* update highest limit */
						li_highest_limit = lC_sol.limit;
						/* remember the solution */
						li_index = i;
						nr_ab++;
					}
				}
			 }
			 
			 /* 7: Scout the highest with random solution 
			  * check if li_index was set to an abandon candidate 
			  * */
			if(li_index != -1) {
			
				Solution lC_sol = gC_solutions.get(li_index); 
			 	double [] lda_tmp = fct1.getRandomParam();
				lC_sol.set(lda_tmp, fct1.solve(lda_tmp));
				lC_sol.limit = 0;
				lC_sol.newpostype = 0;
			} 
			 
		}
		
		gd_updaterate =  (double) li_updatecounter /  gi_neb; 
	}

	private double[] modifyposition(Solution aC_solution, int ai_index) {
		
		double [] position = aC_solution.getPosition();
		double [] newposition = position.clone();
		
		int k, j;
    	
		do {
			k = generator.nextInt(gi_neb);
			j = generator.nextInt(gi_dim);
	    } while (ai_index == k); 
		
		Solution lC_refsol = gC_solutions.get(k);
		double [] lda_refpos = lC_refsol.getPosition();
		
		double phi_ij = (generator.nextDouble() * 2) - 1;
		
		newposition[j] = position[j] + phi_ij * (position[j] - lda_refpos[j]);
    	
		// check if in range of function space
		newposition[j] = fct1.checkrange(newposition[j]);
		
		return newposition;
		
	}
	
	/*public double getBest() {
		// TODO Auto-generated method stub
		return 0;
	}*/

	public Solution getBestSolution() {
		
		/* transform to Array, this is just a ref copy ! 
		 * All changes on Array elements will have effect in ArrayList! */
		Object[] lCa_sortedSolutions =  gC_solutions.toArray();
		
		/* sort the fitness of solutions */
		java.util.Arrays.sort(lCa_sortedSolutions, new Value_Comparator()); // TODO : fix warning

		/* first one is best */	
		return (Solution)lCa_sortedSolutions[0];
	}

	private class Value_Comparator implements java.util.Comparator {
		
		public int compare(Object arg0, Object arg1) {
			
			// compare the value of the agents solution, needed for sorting
			Solution tmp0 = (Solution) arg0;
			Solution tmp1 = (Solution) arg1;
			
			if(tmp0.value < tmp1.value) {
				return 0;
			}
			
			return 1;
		}
		
		
	}
	
	public boolean optimaReached(double ad_value) {
		   return fct1.optimaReached(ad_value);
	   }
	
	
	public void calcSelectProbs(){

		double sum_solution = 0;
		
		/* sum up all solutions */
		for(int i = 0; i < gi_neb; i++) {
			
		    Solution lC_tmpsol = (Solution) gC_solutions.get(i);
		    
		    if(lC_tmpsol.value >= 0) {
			    sum_solution += ( 1 / (1 + lC_tmpsol.value));  
		    } else if (lC_tmpsol.value < 0) {   
		    	sum_solution += 1 + Math.abs(lC_tmpsol.value);  // this makes no sense, but in ABC its done
		    	
		    	// since we have no values smaller zero, notify me 
		    	throw new NullPointerException();
		    }
		    
		}
		
		/* set each probability of selection */ 
		for(int i = 0; i < gi_neb; i++) {
			Solution lC_tmpsol = (Solution) gC_solutions.get(i);
		    
		    /* sets value of reference to the solution */ 
			if(lC_tmpsol.value >= 0) {
				lC_tmpsol.select_prob = (1 / (1 + lC_tmpsol.value)) / sum_solution;
			} else {
				lC_tmpsol.select_prob = 1 + Math.abs(lC_tmpsol.value) / sum_solution;
			}
		    	    
		}
   }
	
	
	  public int select() {
	   		
			double rand = generator.nextDouble();
			double sum = 0;
		
			// select a solution from the blackboard via roulette wheel selection
			for(int i = 0; i < gi_neb; i++) {
			    Solution tmpsol = (Solution) gC_solutions.get(i);
			    	
			    	if( (rand > sum) && (rand <= (sum + tmpsol.select_prob))) {
			    		
			    		return i;  // the entry selected from the BB, needed later to make update in bb
			    		
					}
			    
			    sum += tmpsol.select_prob;
			}
			
			return -1;
	    }
	  
		public void resetFctCounter() {
			fct1.resetcallcounter();
		}
		
		public String getFctName() {
			return fct1.getName();
		}
		
		public int getFctID() {
			return fct1.getID();
		}
		
		/* debug */
		public int getFctCalls() {
			return fct1.callcounter;
		}
		
		public ArrayList getSolutions(int li_type) {
			return gC_solutions;
		}
		
		public double getUpdateRate() {
			return gd_updaterate;
		}
		
		public int getPopSize() {
			return gi_neb + gi_nob;
		}
}
