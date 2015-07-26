
import global.*;

import java.util.ArrayList;

import plot.PlotFrame;
import stat.WilcoxonRankSum;

import testfunction.TestFunction;



public class ModelRunner {

	private static ParameterHolder gC_params;
	
	//private static Object gC_plot; /* place holder plot object, init in constructor if plot class exists */
	
	private static PlotFrame gC_AgentPositions; 

	private static java.text.DecimalFormat sc_fn = new java.text.DecimalFormat("0.##E00");
	
	/* data for iteration stat test */
	private static int gia_iterations[][];
	private static int gi_iterationscounter = 0;
	
	/* data for evaluation stat test */
	private static int gia_evaluations[][];
	private static int gi_evaluationscounter = 0;
	
	
	/* -------- Start --------------------------------------- */
		
	public void start(	
			ArrayList<TestFunction> aC_fct,
			ParameterHolder aC_params,
			ArrayList<String> aC_methods
			) {
		
		gC_params = aC_params;
	
		// initialize plot frame if plotting enabled
		if((Boolean)gC_params.get(GlobalParameters.PLOTENABLED) == true) {
			gC_AgentPositions = new PlotFrame();
		} 
		
		
		java.text.NumberFormat fn = (java.text.NumberFormat) java.text.NumberFormat
				.getInstance(java.util.Locale.GERMANY);
		fn.setMinimumFractionDigits(2);
		fn.setMaximumFractionDigits(2);
	
		/* run each experiment */
		int counter = 0;
		
		/* loop through all functions. */
		do {

			TestFunction cur_fct = aC_fct.get(counter);
			gia_iterations = new int[2][];
			gia_evaluations = new int[2][];
			
			gi_iterationscounter = 0;
			gi_evaluationscounter = 0;
			
			/* loop through the methods */
			for(int i = 0; i < aC_methods.size(); i++) {
				runABC(cur_fct, aC_methods.get(i)); 
			}
			
			/* do statistics summary in case PRINTMODE is 0 */			
			if((Integer)gC_params.get(GlobalParameters.PRINTMODE) == 0) {
				
				if(gC_params.containsKey(GlobalParameters.WILCOXONTEST)) {
				
					//System.out.print("Iterations : x lesser y: " + WilcoxonRankSum.Test(gia_iterations[0], gia_iterations[1]));
					//System.out.println(", y lesser x: " + WilcoxonRankSum.Test(gia_iterations[1], gia_iterations[0]));
			
					System.out.print("Evaluations: x lesser y: " + WilcoxonRankSum.Test(gia_evaluations[0], gia_evaluations[1]));
					System.out.println(", y lesser x: " + WilcoxonRankSum.Test(gia_evaluations[1], gia_evaluations[0]));
				}
			}
			
			
			
		} while (++counter != aC_fct.size());
	
	}

	private static double mean(double[] ada_x) {

		double sum = 0;
		for (int i = 0; i < ada_x.length; i++) {
			sum += ada_x[i];
		}

		return (sum / ada_x.length);
	}

	private static double sd(double[] ada_x, double ad_mean) {

		double sum = 0;

		for (int i = 0; i < ada_x.length; i++) {
			sum += Math.pow((ada_x[i] - ad_mean), 2);
		}

		return Math.sqrt(sum / ada_x.length);
	}

	private static double[] run(ModelInterface aC_model, int ai_mcn, double[] ada_bestvalues, int[] aia_evalstep) {

		int li_breakcond = 0;
		Solution[] best_sols = new Solution[ai_mcn];

		java.text.NumberFormat fn = (java.text.NumberFormat) java.text.NumberFormat.getInstance(java.util.Locale.GERMANY);
		fn.setMaximumFractionDigits(100);
		
		double best = 10E10;

		/* this counter is increased every iteration the best did not improve */
		int best_not_improved_counter = 0;
		
		int max_none_improvements = 100;

		//int li_maxevals = (Integer) gC_params.get(GlobalParameters.MAXEVALS);
				
		while(li_breakcond < ai_mcn) {
			/* Iterate one step, method returns true if a perfect solution was found inside,
			 * when having fit(i) = 0, prevents devision by Zero (calc Select Probs) and 
			 * breaks further calculations (onlookers).
			 */
			
			aC_model.Iterate();
			
			//li_totalevals += aC_model.getFctCalls(); /* since pop size can change, keep track of actual agents (evals) used in each iteration */
			aia_evalstep[li_breakcond] = aC_model.getFctCalls(); //(double)li_totalevals;
			
			/* save the last best */
			best_sols[li_breakcond] = aC_model.getBestSolution();
			
			/* print out the score and variables for all solutions (used for surface plotting) */
			if(  (Integer)gC_params.get(GlobalParameters.PRINTMODE) == 5 ) {
				
				ArrayList<Solution> solutions_tmp = aC_model.getSolutions(0);
			
				/* sum up all solutions */
				for(int i = 0; i < solutions_tmp.size(); i++) {
				
					Solution lC_tmpsol = (Solution) solutions_tmp.get(i);
					System.out.print(lC_tmpsol.value + "\t" + lC_tmpsol.indices[0] + "\t" + lC_tmpsol.indices[1] + "\n");
				}
			}
			/* print MSE of each solution */
			if(  (Integer)gC_params.get(GlobalParameters.PRINTMODE) == 6 ) {
				
				ArrayList<Solution> solutions_tmp = aC_model.getSolutions(0);
			
				/* sum up all solutions */
				for(int i = 0; i < solutions_tmp.size(); i++) {
				
					Solution lC_tmpsol = (Solution) solutions_tmp.get(i);
					double mse_value = aC_model.getMSE(lC_tmpsol.indices);
					
					System.out.print(mse_value + "\t" + lC_tmpsol.indices[0] + "\t" + lC_tmpsol.indices[1] + "\n");
				}
			}
			
		
			/* check if current best better than all time best */
			if (best_sols[li_breakcond].value < best) {
			
				/* reset the improved counter */
				best_not_improved_counter = 0;
				
				/* overwrite best score */
				best = best_sols[li_breakcond].value;
				
				if (aC_model.optimaReached(best)  ) {
					if(ada_bestvalues!= null) {
						ada_bestvalues[li_breakcond] = best;
					}
					break;
				}
			} else {
				/* setup the not improved counter */
				best_not_improved_counter++;
			}
				

			/* on every step the best, later be used to make a mean with more experiments */
			if((gC_params.get(GlobalParameters.TIMEPLOT) == "MEANBESTVALUES") && (ada_bestvalues != null)) {
				ada_bestvalues[li_breakcond] = best;
			} else if(gC_params.get(GlobalParameters.TIMEPLOT) == "UPDATERATE" && (ada_bestvalues != null)) {
				ada_bestvalues[li_breakcond] = aC_model.getUpdateRate();
			}
							
			/* do plotting of agents position */
			if((Boolean)gC_params.get(GlobalParameters.PLOTENABLED) == true) {

				if( (gC_params.get(GlobalParameters.PLOTTYPE) == "EBPOSITIONS") || (gC_params.get(GlobalParameters.PLOTTYPE) == "OBPOSITIONS") ) {
				
				ArrayList<Solution> lC_solutions = null;
				
				if(gC_params.get(GlobalParameters.PLOTTYPE) == "EBPOSITIONS") {
					lC_solutions = aC_model.getSolutions(0);
				} else {
					lC_solutions = aC_model.getSolutions(1);
				}
				
				double[][] lda_positions = new double[4][lC_solutions.size()];
				
				for (int i = 0; i < lC_solutions.size(); i++) {	  
					
					Solution lC_sol = lC_solutions.get(i);
					double[] x = lC_sol.getPosition();
					// only get first 2 coordinates
					lda_positions[0][i] = x[0]; 
					lda_positions[1][i] = x[1];
					lda_positions[2][i] = lC_sol.limit;
					lda_positions[3][i] = lC_sol.newpostype;
											
				}
				
				if(gC_AgentPositions != null) {
					gC_AgentPositions.drawData(lda_positions);
				}
				
				 try {
					 int milliseconds = (Integer)gC_params.get(GlobalParameters.PLOTSLEEP);
					  Thread.sleep(milliseconds);
				  } catch(Exception ie) {};
				}
			}
			
			/* exit loop if the best solution was not improved for max. count number */
			if(best_not_improved_counter > max_none_improvements) {
				break;
			}
			
			li_breakcond++;
		}

		double [] ld_best_it = new double[3];
		ld_best_it[0] = best;                    /* best value seen */            
		ld_best_it[1] = li_breakcond;            /* number of iterations until stop  */
		ld_best_it[2] = aC_model.getFctCalls();  /* number of evaluations/calls to problem function (more than iterations dependent on nr. of bees)*/
		return ld_best_it;
	}
		
	private static void runABC(TestFunction cur_fct, String aC_method) {	
				
		
		/* get nr of runs */
		Integer nr_runs = (Integer) gC_params.get(GlobalParameters.nr_runs);
		
		/* array holding best values for each run */
		double[] values = new double[nr_runs];
		
		/* array holding the iteration number when goal was reached */
		int [] lia_iterations = new int[nr_runs];

		/* array holding the evaluations needed to reach goal */
		int [] lia_evaluations = new int[nr_runs];
		
		int li_popSize = (Integer) gC_params.get(GlobalParameters.NEB) + (Integer) gC_params.get(GlobalParameters.NEB);
	 	
		int li_iterations = (Integer) gC_params.get(GlobalParameters.MAXEVALS) / li_popSize;
		 		  
		/* for plotting, mean value of each cycle and cycle number */
		double [][]lda_bestvalue = null;
		if((Integer)gC_params.get(GlobalParameters.PRINTMODE) == 3) {
			lda_bestvalue = new double[nr_runs][li_iterations]; 
		}
		
		int []lia_evalsteps = new int[li_iterations];
		
		/* for plotting, set the domain range */
		if(gC_AgentPositions != null) {
			if(gC_params.containsKey(GlobalParameters.PLOTMAX) && gC_params.containsKey(GlobalParameters.PLOTMAX)) {
				gC_AgentPositions.setRange((Double)gC_params.get(GlobalParameters.PLOTMAX), (Double)gC_params.get(GlobalParameters.PLOTMAX));
			} else {
				gC_AgentPositions.setRange(cur_fct.getGd_lowerbound(0), cur_fct.getGd_upperbound(0));
			}
		}
		
		//if((Integer)gC_params.get(GlobalParameters.PRINTMODE) == 0) {
		//	System.out.println(gC_params.get(aC_method));
		//}
				
		for(int i = 0; i < nr_runs; i++) {
			
			/* reset before Model Init (fct. calls already in constructor) */
			cur_fct.resetcallcounter();		
			
			ModelInterface lC_model = null;
			
			try {
				lC_model = (ModelInterface)( Class.forName( "abc." + aC_method ).newInstance() );
				lC_model.Init(cur_fct, gC_params);
			
				if(lC_model == null) { throw new NullPointerException("Method class created is null."); }
				
			} catch(Exception noclass) {
				System.out.println("error creating method class : " + noclass.getMessage());
				System.exit(-1);
			}
			
			double [] ld_best_it = null;
							
			if(lda_bestvalue != null) {
				ld_best_it = run(lC_model, li_iterations, lda_bestvalue[i], lia_evalsteps);
			} else {
				ld_best_it = run(lC_model, li_iterations, null, lia_evalsteps);
			}
			
			values[i] = ld_best_it[0];
			lia_iterations[i] = (int) ld_best_it[1];
			lia_evaluations[i] = (int) ld_best_it[2];
				
			if((Integer)gC_params.get(GlobalParameters.PRINTMODE) == 0) {
				System.out.print(".");
			} else if ((Integer)gC_params.get(GlobalParameters.PRINTMODE) == 1){
				System.out.println((int) ld_best_it[2]);
			} else if ((Integer)gC_params.get(GlobalParameters.PRINTMODE) == 4){
				
				/* print the best solution for this run, including the score and parameters */
				Solution bestsol = lC_model.getBestSolution();
				
				System.out.print(bestsol.value + " ");  // score of best solution
				for(int l = 0; l < bestsol.indices.length; l++) {
					System.out.print(bestsol.indices[l] + " "); // each parameter
				}
				System.out.print("\n");
				
			}
		}
		
		/* calculate and print the mean, median, best, worsed, expected number of iterations
		 * for reaching the goal/optima from all the experiments
		 */
		if((Integer)gC_params.get(GlobalParameters.PRINTMODE) == 0) {
				System.out.print("\n[" + cur_fct.getName() + "]");
				System.out.print(":evals=" + cur_fct.callcounter);
		}
		cur_fct.resetcallcounter();
		
		
		/* Do the best mean value plot for each Iteration */
		double [][] lda_bestmeanv = new double[li_iterations][2];
		
	
		if(lda_bestvalue != null) {
			/* Do the best mean value plot for each Iteration or Evaluation */
			for(int i = 0; i < li_iterations; i++) {
			
				/* calculate mean of best value in each iteration */
				double ld_sum_value = 0;
			
				for(int j = 0; j < nr_runs; j++) { /* from all runs */
					ld_sum_value += lda_bestvalue[j][i];
				}
				lda_bestmeanv[i][1] = (ld_sum_value / nr_runs);
				if(lda_bestmeanv[i][1] == 0) {
					if((Integer)gC_params.get(GlobalParameters.PRINTMODE) == 3) {
						System.out.println(lia_evalsteps[i] + "\t" + lda_bestmeanv[i][1]);
					}
					break;
				}
				
				if((Integer)gC_params.get(GlobalParameters.PRINTMODE) == 3) {
					System.out.println(lia_evalsteps[i] + "\t" + lda_bestmeanv[i][1]);
				}
				
				if(gC_params.get(GlobalParameters.PLOTXTYPE) == "EVALUATIONS") {
					lda_bestmeanv[i][0] = lia_evalsteps[i];
				} else { /* save iteration number */
					lda_bestmeanv[i][0] = i;
				}
			}
				
		}
			
				
		double ld_mean = mean(values);
		
		if((Integer)gC_params.get(GlobalParameters.PRINTMODE) == 0) {
			System.out.println( "\tbest result: " + sc_fn.format(ld_mean) + " +/- " + sc_fn.format( sd(values, ld_mean)) + "\t");
		} 
		
		if ((Integer)gC_params.get(GlobalParameters.PRINTMODE) == 2) {
			
			double ld_meaneval = 0;
			for(int z = 0; z < lia_evaluations.length; z++) {
				ld_meaneval += lia_evaluations[z];
			}
			System.out.println(ld_meaneval/lia_evaluations.length);
			//System.out.println(mean((double[])lia_evaluations));
		}
		
		if((Integer)gC_params.get(GlobalParameters.PRINTMODE) == 0) {
			printIterationStatistics(lia_iterations);
			printIterationStatistics(lia_evaluations);
		} 
		
		/* copy the ref of iterations to the dataset that will be stat. tested later */	
		if(gi_iterationscounter < 2) {
			gia_iterations[gi_iterationscounter++] = lia_iterations;
		}
		if(gi_evaluationscounter < 2) {
			gia_evaluations[gi_evaluationscounter++] = lia_evaluations;
		}
		
		
		
	}
		
	public static void printIterationStatistics(int [] aia_iterations) {
		
		double ld_mean = 0;
		int li_size = aia_iterations.length;
		int li_best = aia_iterations[0];
		int li_worsed = aia_iterations[0];
		
		for(int i = 0; i < li_size; i++) {
			/* sum up the iterations */
			ld_mean += aia_iterations[i];
			
			if(aia_iterations[i] < li_best) {
				li_best = aia_iterations[i];
			}
			if(aia_iterations[i] > li_worsed) {
				li_worsed = aia_iterations[i];
			}

		}
		
		/* mean of iterations needed to get to goal/optima */
		ld_mean /= li_size;
		
		/* get median, sort first and pick value in the middle */
		java.util.Arrays.sort(aia_iterations);
		double ld_median;
		
		/* if size even take the mean of the middle 2 */
		if((li_size % 2) == 0) {
			int li_tmp = aia_iterations[li_size/2] + aia_iterations[li_size/2 - 1];
			ld_median = li_tmp / 2;
			
		} else {
			ld_median = aia_iterations[li_size/2];
		}
		
		
		System.out.println("\tavg: " + ld_mean + "\tmedian: " + ld_median + "\tmax: " + li_worsed + "\tmin: " + li_best);
		
		
	}

	
}
