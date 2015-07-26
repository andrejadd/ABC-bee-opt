package abc;


import global.ModelInterface;
import global.Solution;
import global.ParameterHolder;
import global.GlobalParameters;

import java.util.ArrayList;
import java.util.Random;

import testfunction.TestFunction;

public class ABCgbestrank_3 implements ModelInterface {

	
	private static TestFunction fct1;
	private ArrayList<Solution> gC_solutions = new ArrayList<Solution>();;
		
	private static Random generator;
	
	//ParameterHolder gC_params;
	
	/* size of solution vector */
	private int gi_dim;
	private int gi_limit;
	
	/* nr. solutions, ebs and obs */
	private int gi_n;
	
	/* the rate of successful solution update */
	private double gd_updaterate;
	
	/* for DEBUG */
	private static int gi_cycle = 0;
	
	public ABCgbestrank_3() {
		
	}
	
	public void Init(TestFunction aC_fcttest, ParameterHolder aC_params) {
		

		fct1 = aC_fcttest;
		
		//gC_params = aC_params;
		
		generator = new Random(System.currentTimeMillis());
		
		gi_dim = fct1.getDimension();
		gi_n = (Integer) aC_params.get(GlobalParameters.NEB);
		
		gi_limit = gi_n * gi_dim;
		
		if(gi_limit < 100) {
			gi_limit = 100;
		}

		for (int i = 0; i < gi_n; i++) {	    
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
		
		rankSolutions();
		
		/* 1: EBs modify positions and save in new solution array  */
		for(int i = 0; i < gi_n; i++) {
			
			Solution lC_tmp = gC_solutions.get(i);
			double [] lda_newposition = modifyposition(lC_tmp, i);
			lC_tmp.limit++;
			
			/* set new v_i , replace later */
			gC_newsolutions.add(new Solution(lda_newposition,fct1.solve(lda_newposition)));
		}
		
		/* 2: Replace with greedy selection */
		for(int i = 0; i < gi_n; i++) {
			
			Solution lC_currentsol = gC_solutions.get(i);
			Solution lC_newsol = gC_newsolutions.get(i);
			
			if(fct1.better(lC_newsol.value, lC_currentsol.value)) {
				lC_newsol.newpostype = 1;
				lC_newsol.limit = 0;
				gC_solutions.set(i, lC_newsol);
				li_updatecounter++;
			} 
		}
		
		/* 3: check if one has reached the optima/goal */
		for(int i = 0; i < gi_n; i++) {
			
			Solution lC_currentsol = gC_solutions.get(i);
			if(fct1.optimaReached(lC_currentsol.value)) {
				lb_optimareached = true;
			}
		}
		
		if(!lb_optimareached) {
		
			/* 4: Calculate Prob. values P_i */
			 calcSelectProbs();
			 
			 rankSolutions();
			 
			 gC_newsolutions.clear();
			 
			 /* 5: OB select and modify immediately (as in matlab ABC) */
			 for(int i = 0; i < gi_n; i++) {
				 
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
			 for(int i = 0; i < gi_n; i++) {
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
		
		gd_updaterate =  (double) li_updatecounter /  gi_n; 
	}
	
	private double[] modifyposition(Solution aC_solution, int ai_index) {
		
		double [] position = aC_solution.getPosition();
		double [] newposition = position.clone();
		int li_maxw = gi_n;
		
		int j;
    	int li_ownrank = aC_solution.rank;
    	int li_maxrank = gi_n;
    	
    	
    	int li_wnd = 1;
    	if(li_maxw > 0) {
    		li_wnd = generator.nextInt(li_maxw) + 1;
    	}

		
		int li_lwnd = li_wnd;
		int li_rwnd = li_wnd;
		
		if(li_ownrank <= li_lwnd) {
			li_lwnd = li_ownrank - 1;
		}
		if( (li_ownrank + li_rwnd) > li_maxrank) {
			li_rwnd = li_maxrank - li_ownrank;
		}
		int k = 0;
		
		
		try {
			/* TODO: make sure k is not i ?? */
			k = (li_ownrank - li_lwnd ) + generator.nextInt(li_lwnd + li_rwnd);
		} catch(Exception ie) {
			System.out.println(ie.getMessage());
			System.exit(-1);
		}
		
		/* TODO: IMPORTANT: make sure k is not i, as smaller the popsize as more often this can happen 
		 * It should be treated, otherwise is the parameter update with k no help */
		if(k == ai_index) {
				// do something
				//System.out.println("k = i = " + k + "\n");
		}
		
		if( k >= li_ownrank) {
			k+=1; /* shift one to the right */
		}
		
		Solution lC_refsol = getByRank(k);
		if(lC_refsol == null) {
			throw new NullPointerException();
		}
		
		j = generator.nextInt(gi_dim);
	    
		double [] lda_refpos = lC_refsol.getPosition();
		
		/* get global best */
		Solution lC_refgbest = getBestSolution();
		double [] lda_gbestpos = lC_refgbest.getPosition();
		
		double phi_ij = (generator.nextDouble() * 2) - 1;
		double ld_gamma = generator.nextDouble() -1;
		
		double ld_rand = phi_ij * (position[j] - lda_refpos[j]); 
		double ld_social = ld_gamma *( position[j] - lda_gbestpos[j]);
		
		newposition[j] = position[j] + ld_rand + ld_social;
		    	
		// check if in range of function space
		newposition[j] = fct1.checkrange(newposition[j]);
		
		return newposition;
		
	}
	
	private Solution getByRank(int ai_rank) {
		
		for(int i = 0; i < gi_n; i++) {
			Solution lC_tmp = gC_solutions.get(i);
			if(lC_tmp.rank == ai_rank) {
				return lC_tmp;
			}
			
		}
		
		return null;
	}

  private void rankSolutions() {
		  
		  Object[] lCa_sortedSolutions =  gC_solutions.toArray();
			
		 /* sort the fitness of solutions */
		java.util.Arrays.sort(lCa_sortedSolutions, new Value_Comparator()); // TODO : fix warning

		/* start with this rank for the best solution at index 0 */
		int li_rank = gC_solutions.size();
		  
		for(int i =0; i < lCa_sortedSolutions.length; i++) {
			Solution lC_sol = (Solution) lCa_sortedSolutions[i];
			lC_sol.rank = li_rank--;
		}
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
		for(int i = 0; i < gi_n; i++) {
			
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
		for(int i = 0; i < gi_n; i++) {
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
		
		public ArrayList getSolutions(int li_type) {
			return gC_solutions;
		}
		
		public double getUpdateRate() {
			return gd_updaterate;
		}
		
		public int getPopSize() {
			return gi_n * 2;
		}
		
		/* create for the ElasticNet function for testing, might be deleted or called in a different way */
		public double getMSE(double[] params) {
			return fct1.getMSE(params);
				
		}
}
