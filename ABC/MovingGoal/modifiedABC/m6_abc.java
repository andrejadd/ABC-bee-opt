package modifiedABC;


import global.ModelInterface;
import global.Solution;
import global.TestFunction;
import global.ParameterHolder;
import global.GlobalParameters;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.Random;

public class m6_abc implements ModelInterface {

	
	private static TestFunction fct1;
	private ArrayList<Solution> gC_solutions = new ArrayList<Solution>();
	private ArrayList<Solution> gC_obbest = new ArrayList<Solution>();
	public int [] ga_abandoncounter;
	
	private static Random generator;
	
	ParameterHolder gC_params;
	
	/* size of solution vector */
	private int gi_dim;
	private int gi_limit;
	
	/* nr. solutions, ebs and obs */
	private int gi_n;
	
	/* for DEBUG */
	private static int gi_cycle = 0;
	
	public m6_abc( TestFunction am_fcttest, 
			ParameterHolder aC_params) {
	
		fct1 = am_fcttest;
		gC_params = aC_params;
		
		generator = new Random(System.currentTimeMillis());
		
		gi_dim = fct1.getDimension();
		gi_n = (Integer)aC_params.get(GlobalParameters.popSize) / 2;
		gi_limit = (gi_n * gi_dim);
		
		ga_abandoncounter = new int[gi_n];
    	for(int i = 0; i < ga_abandoncounter.length; i++ ) {
    		ga_abandoncounter[i] = 0;
    	}
		
		/* Create random positions */
		Initiate();
	}

	public void Initiate() {

		for (int i = 0; i < gi_n; i++) {	    
			double[] x = fct1.getRandomParam();
			double ld_value = fct1.solve(x);

			Solution lC_tmp = new Solution(x, ld_value);
			lC_tmp.limit = 0;
			gC_solutions.add(lC_tmp);
			gC_obbest.add((Solution)lC_tmp.clone());
		}
		
	}

	public void Iterate() {
		
		/* Two types of selections possible:
		 * 1. select best m from n
		 * 2. random wheel selection, where fitness is probability of selection
		 * Use 1. here..
		 */
		gi_cycle++;
		
		modifyEB_fly_abandonChange();
		
		boolean lb_optimareached = false;
		for(int i = 0; i < gi_n; i++) {
			
			Solution lC_currentsol = gC_solutions.get(i);
			if(fct1.optimaReached(lC_currentsol.value)) {
				lb_optimareached = true;
			}
		}
		
		if(!lb_optimareached) {
		
			calcSelectProbs_v2();

			modifyOB_fly();
			 
		}
	}

	private double[] modifyposition(Solution aC_solution, int ai_index, String aC_type) {
		
		double [] position = aC_solution.getPosition();
		double [] newposition = position.clone();
		
		int k, j;
    	
		do {
			k = generator.nextInt(gi_n);
			j = generator.nextInt(gi_dim);
	    } while (ai_index == k); 
		
		Solution lC_refsol = null;
		
		if(aC_type == "eb") {
			lC_refsol = gC_solutions.get(k);
		} else if(aC_type == "ob") {
			lC_refsol = gC_obbest.get(k);
		}
		
		double [] lda_refpos = lC_refsol.getPosition();
		
		double phi_ij = (generator.nextDouble() * 2) - 1;
		
		newposition[j] = position[j] + phi_ij * (position[j] - lda_refpos[j]);
    	
		// check if in range of function space
		newposition[j] = fct1.checkrange(newposition[j]);
		
		return newposition;
		
	}
	
	private double[] modifyposition_v2(Solution aC_solution, int ai_index, String aC_type) {
		
		double [] position = aC_solution.getPosition();
		double [] newposition = position.clone();
		
		int k;
		int li_randdims = 2;
		
		java.util.HashSet<Integer> lC_set = new java.util.HashSet<Integer>();
		for(int i = 0; i < gi_dim; i++) {
			lC_set.add(i);
		}
		
		java.util.HashSet<Integer> lC_dims = new java.util.HashSet<Integer>();
		do {
			int j = generator.nextInt(gi_dim);
			if(!lC_dims.contains(j)) {
				lC_dims.add(j);
				lC_set.remove(j);
			}
		} while(lC_dims.size() != li_randdims);

		
		do {
			k = generator.nextInt(gi_n);
		} while (ai_index == k); 
		
		Solution lC_refsol = null;
		
		if(aC_type == "eb") {
			lC_refsol = gC_solutions.get(k);
		} else if(aC_type == "ob") {
			lC_refsol = gC_obbest.get(k);
		}
		
		double [] lda_refpos = lC_refsol.getPosition();
		
		double phi_ij = (generator.nextDouble() * 2) - 1;
		
		for(Iterator li_i = lC_dims.iterator(); li_i.hasNext();) {	
			Integer j = (Integer) li_i.next();
			
			newposition[j] = position[j] + phi_ij * (position[j] - lda_refpos[j]);
		}
    	
		
		return newposition;
		
	}
	
	public double getBest() {
		// TODO Auto-generated method stub
		return 0;
	}

	public Solution getBestSolution() {
		
		
		/* transform to Array, this is just a ref copy ! 
		 * All changes on Array elements will have effect in ArrayList! */
		Object[] lCa_sorted_Solutions =  gC_solutions.toArray();
		Object[] lCa_sorted_obbest =  gC_obbest.toArray();
		
		
		/* sort the fitness of solutions */
		java.util.Arrays.sort(lCa_sorted_Solutions, new Value_Comparator()); // TODO : fix warning
		java.util.Arrays.sort(lCa_sorted_obbest, new Value_Comparator()); // TODO : fix warning

		Solution lC_solution = (Solution)lCa_sorted_Solutions[0];
		Solution lC_obbest = (Solution)lCa_sorted_obbest[0];
		
		return lC_solution;
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
		
		public void modifyEB_greedy() {
			
			ArrayList<Solution> gC_newsolutions = new ArrayList<Solution>();
		
			/* 5: , eb modify position */
			for(int i = 0; i < gi_n; i++) {
				
				Solution lC_tmp = gC_solutions.get(i);
				double [] lda_newposition = modifyposition_v2(lC_tmp, i, "eb");
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
		}

		
		/* 5: eb modify position and writeback on the fly*/
		public void modifyEB_fly() {
			
			for(int i = 0; i < gi_n; i++) {
				
				Solution lC_tmp = gC_solutions.get(i);
				double [] lda_newposition = modifyposition_v2(lC_tmp, i, "eb");
				double ld_newvalue = fct1.solve(lda_newposition);
				incrAbandonCounter(i);
				
				if(fct1.better(ld_newvalue, lC_tmp.value)) {
					ga_abandoncounter[i] = 0;
					lC_tmp.set(lda_newposition, ld_newvalue);
					gC_solutions.set(i, lC_tmp);
				} 
			}
		}
		
		
		public void modifyEB_fly_abandonChange() {
			
			 
			 Solution lC_center = getBestSolution();
			
			for(int i = 0; i < gi_n; i++) {
				
				Solution lC_tmp = gC_solutions.get(i);
				
				/* if solution is abandoned */
				if(ga_abandoncounter[i] >= gi_limit) {
					
				 	double [] lda_tmp = fct1.getRandomParam(lC_center.getPosition());
					lC_tmp.set(lda_tmp, fct1.solve(lda_tmp));
					ga_abandoncounter[i] = 0; 
				
				} else {
					
					double [] lda_newposition = modifyposition_v2(lC_tmp, i, "eb");
					double ld_newvalue = fct1.solve(lda_newposition);
					incrAbandonCounter(i);
				
					if(fct1.better(ld_newvalue, lC_tmp.value)) {
						lC_tmp.limit = 0;
						lC_tmp.set(lda_newposition, ld_newvalue);
						gC_solutions.set(i, lC_tmp);
					} else if(ga_abandoncounter[i] >= gi_limit){
						double [] lda_tmp = fct1.getRandomParam(lC_center.getPosition());
						lC_tmp.set(lda_tmp, fct1.solve(lda_tmp));
						ga_abandoncounter[i] = 0;
					}
				}
			}
		}
		
		public void modifyOB_greedy() {
			
			ArrayList<Solution> gC_newsolutions = new ArrayList<Solution>();
			
			/* 7: Calculate Prob. values P_i */
			 calcSelectProbs_v2();
			 
			 gC_newsolutions.clear();
			 
			 /* OB select and modify */
			 for(int i = 0; i < gi_n; i++) {
				 
				 int li_selected = select();
				 Solution lC_solution = gC_solutions.get(li_selected);
				 lC_solution.limit++;
				 
				 double [] lda_newposition = modifyposition(lC_solution, -1, "eb");
				 
				 /* set new v_i , replace later */
				 Solution lC_solref = new Solution(lda_newposition, fct1.solve(lda_newposition));
				 lC_solref.toref = li_selected;
				 gC_newsolutions.add(lC_solref);
				 
			 }
			 
			 /* 9: OB select the better one and writeback */
			 for(int i = 0; i < gi_n; i++) {
				 
				 Solution lC_newsol = gC_newsolutions.get(i);
				 int li_refid = lC_newsol.toref;
				 
				 Solution lC_currentsol = gC_solutions.get(li_refid);
					
				 /* if better overwrite und reset limit */
				 if(fct1.better(lC_newsol.value, lC_currentsol.value)) {
					 ga_abandoncounter[li_refid] = 0;
					 gC_solutions.set(li_refid, lC_newsol);
				 } 
				 
				 /* check if also better than own ob best local */
				 Solution lC_obbest = gC_obbest.get(i);
				 /* check new pos */
				 if(fct1.better(lC_newsol.value, lC_obbest.value)) {
					 gC_obbest.set(i, (Solution) lC_newsol.clone());
				 } 
				 /* check old pos  */
				 if(fct1.better(lC_currentsol.value, lC_obbest.value)) {
				 	 gC_obbest.set(i, (Solution) lC_currentsol.clone());
				 }
			 }
		}

		public void modifyOB_fly() {
						 
			/* OB select and modify */
			for(int i = 0; i < gi_n; i++) {
				 
				int li_selected = select();
				Solution lC_solution = gC_solutions.get(li_selected);
				incrAbandonCounter(li_selected);
				 
				double [] lda_newposition = modifyposition(lC_solution, -1, "ob");
				Solution lC_newsolution = new Solution(lda_newposition, fct1.solve(lda_newposition));
				
				/* check if new solution better than the selected solution  */
				 if(fct1.better(lC_newsolution.value, lC_solution.value)) {
					 ga_abandoncounter[li_selected] = 0;
					 lC_newsolution.select_prob = lC_solution.select_prob;
					 gC_solutions.set(li_selected, (Solution) lC_newsolution.clone());
					 
					 if(lC_newsolution.value == 0.0) {
						 System.out.print("*");
						 break;
					 }
					 /* make sure the selection probs are updated */
					 calcSelectProbs_v2();
				 } 
				 
				 /* check if new solution better than own ob best local */
				 Solution lC_obbest = gC_obbest.get(i);
				 if(fct1.better(lC_newsolution.value, lC_obbest.value)) {
					 gC_obbest.set(i, (Solution) lC_newsolution.clone());
				 } 
				 
				 /* check if selected solution better than ob best (to always have the best in memory) */
				 //if(fct1.better(lC_solution.value, lC_obbest.value)) {
				//	 gC_obbest.set(i, (Solution) lC_solution.clone());
				 //}
			}
		}

	    public boolean incrAbandonCounter(int ai_index) {

	    	ga_abandoncounter[ai_index] += 1;
	    	if(ga_abandoncounter[ai_index] >= gi_limit) {
	    		
	    		return true;
	    	}
	    	
	    	return false; 
	    }
	    
		public TestFunction getFct() {
			return fct1;
		}


}
