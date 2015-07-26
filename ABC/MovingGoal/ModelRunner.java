
import global.*;
import java.util.ArrayList;

import standardABC.s_abc;
import modifiedABC.m1_abc;
import modifiedABC.m2_abc;
import modifiedABC.m3_abc;
import modifiedABC.m4_abc;
import modifiedABC.m5_abc;
import modifiedABC.m6_abc;

import standardABC.sABCParameters;
//import abc.ABCModel;

import com.panayotis.gnuplot.*;
import com.panayotis.gnuplot.plot.DataSetPlot;
import com.panayotis.gnuplot.style.*;

public class ModelRunner {

	private static ParameterHolder gC_params;
	
	private static ArrayList<TestFunction> fct;
	
	private static JavaPlot gC_plot;

	private static java.text.DecimalFormat sc_fn = new java.text.DecimalFormat("0.##E00");
	
	
	/* -------- Start --------------------------------------- */
		
	public void start(	
			ArrayList aC_fct,
			ParameterHolder aC_params,
			JavaPlot aC_plot
			) {
		
		gC_params = aC_params;
		gC_plot = aC_plot;
		
		java.text.NumberFormat fn = (java.text.NumberFormat) java.text.NumberFormat
				.getInstance(java.util.Locale.GERMANY);
		fn.setMinimumFractionDigits(2);
		fn.setMaximumFractionDigits(2);

		/* run each experiment */
		int counter = 0;
		
		/* loop through all functions. */
		do {

			TestFunction cur_fct = (TestFunction) aC_fct.get(counter);
			System.out.println();
			
			String lC_meanlast;
			/* -- ABC 1 -- */
			/* 
			  
			System.out.println("sABC1");
			gC_params.put(GlobalParameters.Name, "standardABC1");
			standardABC1 lC_abc1 = new standardABC1(cur_fct, gC_params);
			lC_meanlast = runABC(lC_abc1);
			System.out.println(lC_meanlast); 
			*/
			/* -- ABC 2 -- */
			//System.out.println("sABC2");
			//gC_params.put(GlobalParameters.Name, "standardABC2");
			//standardABC2 lC_abc2 = new standardABC2(cur_fct, gC_params);
			//lC_meanlast = runABC(lC_abc2); 
			//System.out.println(lC_meanlast);

			/* -- standard ABC -- */
			gC_params.put(GlobalParameters.Name, "sabc");
			runABC(cur_fct); 
			
						
			/* m1_abc */
			//gC_params.put(GlobalParameters.Name, "m1_abc");
			//runABC(cur_fct); 
			
			/* m2_abc */
			//gC_params.put(GlobalParameters.Name, "m2_abc");
			//runABC(cur_fct); 
						
			/* m3_abc */
			//gC_params.put(GlobalParameters.Name, "m3_abc");
			//runABC(cur_fct); 
						
			//gC_params.put(GlobalParameters.Name, "m4_abc");
			//runABC(cur_fct); 
			
			/*gC_params.put(GlobalParameters.Name, "m5_abc");
			runABC(cur_fct); 
			*/
			gC_params.put(GlobalParameters.Name, "m6_abc");
			runABC(cur_fct); 
			
			
			
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

	private static double[] run(ModelInterface aC_model, int ai_mcn, double[] ada_bestvalues) {

		int li_breakcond = 0;

		java.text.NumberFormat fn = (java.text.NumberFormat) java.text.NumberFormat.getInstance(java.util.Locale.GERMANY);
		fn.setMaximumFractionDigits(100);
		
		double best = 10E10;
		
		TestFunction fct = aC_model.getFct();
		fct.resetSeverity();
		
		while (li_breakcond != ai_mcn) {

			/* Iterate one step, method returns true if a perfect solution was found inside,
			 * when having fit(i) = 0, prevents devision by Zero (calc Select Probs) and 
			 * breaks further calculations (onlookers).
			 */
			
			aC_model.Iterate();
			fct.addToSeverity(0.1);
			
			// save the last best
			Solution lC_globalbest = aC_model.getBestSolution();
			
			/* on every step the best, later be used to make a mean with more experiments */
			ada_bestvalues[li_breakcond] = lC_globalbest.value;
						
			li_breakcond++;
		}

		double [] ld_best_it = new double[2];
		ld_best_it[0] = best;
		ld_best_it[1] = li_breakcond; 
		
		return ld_best_it;
	}
	
	
	private static void runABC(TestFunction cur_fct) {	
				
		
		/* get nr of runs */
		Integer nr_runs = (Integer) gC_params.get(GlobalParameters.nr_runs);
		
		/* array holding best values for each run */
		double[] values = new double[nr_runs];
		
		/* array holding the iteration number when goal was reached */
		int [] lia_iterations = new int[nr_runs];
		
		int li_popSize = (Integer) gC_params.get(GlobalParameters.popSize);
	 	
		int li_iterations;
		
		/* decide how many evaluations, depends on function */
		double li_fctid = cur_fct.getID();
		if(li_fctid == 1 || li_fctid == 2) {
			li_iterations = (Integer) gC_params.get(GlobalParameters.numEval1) / li_popSize;
		} else {
			li_iterations = (Integer) gC_params.get(GlobalParameters.numEval2) / li_popSize;
		}
	
		/* for plotting, mean value of each cycle and cycle number */
		double [][]lda_bestvalue = new double[nr_runs][li_iterations]; 
		
		System.out.println(gC_params.get(GlobalParameters.Name)); 
		
		for(int i = 0; i < nr_runs; i++) {
			
			ModelInterface lC_model = null;
			
			if(gC_params.get(GlobalParameters.Name) == "sabc") { 
				lC_model = new s_abc(cur_fct, gC_params);
			} else if(gC_params.get(GlobalParameters.Name) == "m1_abc") {
				lC_model = new m1_abc(cur_fct, gC_params);
			} else if(gC_params.get(GlobalParameters.Name) == "m2_abc") {
				lC_model = new m2_abc(cur_fct, gC_params);
			} else if(gC_params.get(GlobalParameters.Name) == "m3_abc") {
				lC_model = new m3_abc(cur_fct, gC_params);
			} else if(gC_params.get(GlobalParameters.Name) == "m4_abc") {
				lC_model = new m4_abc(cur_fct, gC_params);
			}  else if(gC_params.get(GlobalParameters.Name) == "m5_abc") {
				lC_model = new m5_abc(cur_fct, gC_params);
			}  else if(gC_params.get(GlobalParameters.Name) == "m6_abc") {
				lC_model = new m6_abc(cur_fct, gC_params);
			} 
			
			if(lC_model != null) {
								
				double [] ld_best_it = run(lC_model, li_iterations, lda_bestvalue[i]); 
				values[i] = ld_best_it[0];
				lia_iterations[i] = (int) ld_best_it[1];
				
				
				System.out.print(".");
				
			} else { break; }
			
		}
		
		/* calculate and print the mean, median, best, worsed, expected number of iterations
		 * for reaching the goal/optima from all the experiments
		 */
		System.out.print("\n[" + cur_fct.getName() + "]");
		//printIterationStatistics(lia_iterations);
		
		/* calculate mean for each cycle */
		double [][] lda_offperformance = new double[li_iterations][2];
		double [][] lda_globalbest = new double[li_iterations][2];
	
		double ld_bestvaluesum = 0;
		
		for(int i = 0; i < li_iterations; i++) {
			
			/* calculate mean of best value in each iteration */
			ld_bestvaluesum += lda_bestvalue[0][i];
			System.out.println(lda_bestvalue[0][i]);
		
			lda_offperformance[i][1] = (ld_bestvaluesum / (i + 1 ));
			lda_offperformance[i][0] = i;
			
			lda_globalbest[i][1] = lda_bestvalue[0][i];
			lda_globalbest[i][0] = i; 
		}
		
		double ld_mean = mean(values);
	  		
		DataSetPlot lC_ptmp = new DataSetPlot(lda_offperformance);
		PlotStyle lC_style = new PlotStyle(Style.LINES);
		lC_ptmp.setPlotStyle(lC_style);
		lC_ptmp.setTitle((String) gC_params.get(GlobalParameters.Name) + ":op");
		gC_plot.addPlot( lC_ptmp);
		
		DataSetPlot lC_btmp = new DataSetPlot(lda_globalbest);
		//PlotStyle lC_bstyle = new PlotStyle(Style.LINES);
		lC_btmp.setPlotStyle(lC_style);
		lC_btmp.setTitle((String) gC_params.get(GlobalParameters.Name) + ":gbest");
		gC_plot.addPlot( lC_btmp);
		
		
		System.out.println( sc_fn.format(ld_mean) + " +/- " + sc_fn.format( sd(values, ld_mean)) + "\t");
			
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
