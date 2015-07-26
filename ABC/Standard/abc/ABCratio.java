package abc;


import global.ModelInterface;
import global.Solution;
import global.ParameterHolder;
import global.GlobalParameters;

import java.util.ArrayList;
import java.util.Random;

import testfunction.TestFunction;

public class ABCratio implements ModelInterface {

	
	private static TestFunction fct1;
	private ArrayList<Solution> gC_solutions = new ArrayList<Solution>();;
		
	private static Random generator;
	
	ParameterHolder gC_params;
	
	/* size of solution vector */
	private int gi_dim;
	private int gi_limit;
	private double gd_updaterate = -1;
	
	/* nr. solutions, ebs and obs */
	private int gi_n;
	private int gi_obs;
		
	/* scaler */
	//private double gd_minscale =0.1;
	//private double gd_maxscale = 1.3;
		
	
	/* for DEBUG */
	private static int gi_cycle = 0;
	
	public ABCratio() {}
	
	public void Init(TestFunction am_fcttest, 
			ParameterHolder aC_params) {
	
		fct1 = am_fcttest;
		gC_params = aC_params;
		
		generator = new Random(System.currentTimeMillis());
		
		gi_dim = fct1.getDimension();
		int li_totaln =  (Integer)aC_params.get(GlobalParameters.NEB) +  (Integer)aC_params.get(GlobalParameters.NOB);;
		
		double ld_tmp = (double) 1/10 * li_totaln;
		gi_n = (int) ld_tmp;
		
		//gi_limit = gi_n * gi_dim;
		gi_limit = 50 * gi_dim;
		
		gi_obs = li_totaln - gi_n;
		
		for (int i = 0; i < gi_n; i++) {	    
			double[] x = fct1.getRandomParam();
			double ld_value = fct1.solve(x);

			Solution lC_tmp = new Solution(x, ld_value);
			lC_tmp.limit = 0;
			gC_solutions.add(lC_tmp);
		}
		
		
	/*	if(gC_params.get(GlobalParameters.OUTPUT) == "solution_details") {
			for (int i = 0; i < gi_n; i++) {	    
				Solution lC_sol = gC_solutions.get(i);
				double[] x = lC_sol.getPosition();
				System.out.println(x[0] + " " + x[1]);
			}
		}*/
		
		//gd_phistep = Math.abs((gd_maxscale - gd_minscale) / gi_limit);
	}

	public void Iterate() {
		
		/* Two types of selections possible:
		 * 1. select best m from n
		 * 2. random wheel selection, where fitness is probability of selection
		 * Use 1. here..
		 */
		gi_cycle++;
		
		ArrayList<Solution> gC_newsolutions = new ArrayList<Solution>();
		
		boolean lb_optimareached = false;
		
		/* 5: , eb modify position */
		for(int i = 0; i < gi_n; i++) {
			
			Solution lC_tmp = gC_solutions.get(i);
			double [] lda_newposition = modifyposition(lC_tmp, i);
			lC_tmp.limit++;
			
			/* set new v_i , replace later */
			gC_newsolutions.add(new Solution(lda_newposition,fct1.solve(lda_newposition)));
		}
		
		/* 6: Replace with greedy selection */
		for(int i = 0; i < gi_n; i++) {
			
			Solution lC_currentsol = gC_solutions.get(i);
			Solution lC_newsol = gC_newsolutions.get(i);
			
			if(fct1.better(lC_newsol.value, lC_currentsol.value)) {
				lC_newsol.limit = 0;
				gC_solutions.set(i, lC_newsol);
			} 
		}
		
		for(int i = 0; i < gi_n; i++) {
			
			Solution lC_currentsol = gC_solutions.get(i);
			if(fct1.optimaReached(lC_currentsol.value)) {
				lb_optimareached = true;
			}
		}
		
		if(!lb_optimareached) {
		
			/* 7: Calculate Prob. values P_i */
			 calcSelectProbs_v2();
			 
			 gC_newsolutions.clear();
			 
			 
			 /* OB select and modify immediately (as in matlab ABC) */
			 for(int i = 0; i < gi_obs; i++) {
				 
				 int li_selected = select();
				 Solution lC_tmpref = gC_solutions.get(li_selected);
				 lC_tmpref.limit++;
				 
				 double [] lda_newposition = modifyposition(lC_tmpref, li_selected);
				 
				 double newvalue = fct1.solve(lda_newposition);
				 
				 // check if improved
				 if(fct1.better(newvalue, lC_tmpref.value)) {
					 lC_tmpref.set(lda_newposition, newvalue);
					 lC_tmpref.newpostype = 1;
					 lC_tmpref.limit = 0;
//					 li_updatecounter++;
				 }				 
			 }
			 
			 /* 10: Replace randomly the solution with the highest abandon counter */
			 int li_highest_limit = -1;
			 int li_index = -1;
			 int nr_ab = 0;
			 /* look for the highest limit value */
			 for(int i = 0; i < gi_n; i++) {
				 
				Solution lC_sol = gC_solutions.get(i);
				
				/* if abandonded get new postion, solve and reset limit */
				if(lC_sol.limit >= gi_limit) {
					
					if(lC_sol.limit > li_highest_limit) {
						/* update highest limit */
						li_highest_limit = lC_sol.limit;
						/* remember the solution */
						li_index = i;
						nr_ab++;
					}
				}
			 }
			 
			 /* get random solution for the candidate, 
			  * check if li_index was set to an abandon candidate 
			  * */
			if(li_index != -1) {
			
				Solution lC_sol = gC_solutions.get(li_index); 
			 	double [] lda_tmp = fct1.getRandomParam();
				lC_sol.set(lda_tmp, fct1.solve(lda_tmp));
				lC_sol.limit = 0;
			} 
			 
		}
		
		
	}

	private double[] modifyposition(Solution aC_solution, int ai_index) {
		
		double [] position = aC_solution.getPosition();
		double [] newposition = position.clone();
		
		int k, j;
    	
		do {
			k = generator.nextInt(gi_n);
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
	
	public double getBest() {
		// TODO Auto-generated method stub
		return 0;
	}

	public Solution getBestSolution() {
		
		/* transform to Array, this is just a ref copy ! 
		 * All changes on Array elements will have effect in ArrayList! */
		Object[] lCa_sortedSolutions =  gC_solutions.toArray();
		
		/* sort the fitness of solutions */
		java.util.Arrays.sort(lCa_sortedSolutions, new Value_Comparator()); // TODO : fix warning

		/* first one is best */	
		return (Solution)lCa_sortedSolutions[0];
	}

	public void getData(double[] ada_x, double[] ada_y) {
		// TODO Auto-generated method stub

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
	
	public void calcSelectProbs_v1(){

		double sum_solution = 0;
		
		/* sum up all solutions */
		for(int i = 0; i < gi_n; i++) {
			
		    Solution lC_tmpsol = (Solution) gC_solutions.get(i);
		    
		    if(lC_tmpsol.value == 0) {
		    	throw new NullPointerException();
		    	//System.out.println("devision by zero: ABCBlackboard.java:calcSelectProbs() - a solution value is zero!");
		    	//System.exit(1);
		    }
		    sum_solution += ( 1 / lC_tmpsol.value);
		    
		}
		
		/* set each probability of selection */ 
		for(int i = 0; i < gi_n; i++) {
			Solution lC_tmpsol = (Solution) gC_solutions.get(i);
		    
		    /* sets value of reference to the solution */ 
		    lC_tmpsol.select_prob = (1 / lC_tmpsol.value) / sum_solution;
		    	    
		}
   }
	
	
	public void calcSelectProbs_v2(){

		double sum_solution = 0;
		
		/* sum up all solutions */
		for(int i = 0; i < gi_n; i++) {
			
		    Solution lC_tmpsol = (Solution) gC_solutions.get(i);
		    
		    if(lC_tmpsol.value == 0) {
		    	throw new NullPointerException();
		    	
		    }
		    sum_solution += ( 1 / (1 + lC_tmpsol.value));
		    
		}
		
		/* set each probability of selection */ 
		for(int i = 0; i < gi_n; i++) {
			Solution lC_tmpsol = (Solution) gC_solutions.get(i);
		    
		    /* sets value of reference to the solution */ 
		    lC_tmpsol.select_prob = (1 / (1 + lC_tmpsol.value)) / sum_solution;
		    	    
		}
   }
	
	
	  public int select() {
	   		
			double rand = generator.nextDouble();
			double sum = 0;
		
			// select a solution from the blackboard via roulette wheel selection
			for(int i = 0; i < gi_n; i++) {
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
		
		public ArrayList getSolutions() {
			return gC_solutions;
		}
		
		public int getPopSize() {
			return gi_n * 2;
		}
		
		public ArrayList getSolutions(int li_type) {
			
			
			return gC_solutions;
		}
		
		public double getUpdateRate() {
			return gd_updaterate;
		}
}
