import global.ParameterHolder;
import global.GlobalParameters;


import java.util.ArrayList;

import testfunction.*;


//import com.panayotis.gnuplot.JavaPlot;

import plot.PlotFrame;




public class ModelTester {
	
	private static ParameterHolder gC_params = new ParameterHolder();
	
	public static void main(String[] args) {
			
		/* default parameter of population size, can be adjusted with program parameter */
		gC_params.put(GlobalParameters.NEB, 50);
		gC_params.put(GlobalParameters.NOB, 50);
		
		/* repeats the method run this number of times */
		gC_params.put(GlobalParameters.nr_runs, 100);
		
		/* if goal was not reached, returns a evaluation number above 500000 (should be -1) */
		gC_params.put(GlobalParameters.MAXEVALS, 200000);
		
		/* the default is true and limits the arena */
		gC_params.put(GlobalParameters.CHECKRANGE, true);
		
		/* Print Modes : 
		 * 	0 - standard with average/median/max/minimum and statistics
		 *  1 - evaluations per run
		 *  2 - average evaluations over all runs
		 *  3 - average solution value in each cycle (but prints out evaluation)
		 */
		gC_params.put(GlobalParameters.PRINTMODE,1);
		
		/* Standard Limit : false
		 * 	false - off, the limit has to be limit > 100 (avoid too small limits)
		 *  true - on , the limit is purely limit = neb * dim (allows very small limits, can produce bad results for small limits)
		 */
		gC_params.put(GlobalParameters.STANDARDLIMIT,false);
		
		
		/* Plotting related parameters, currently disabled and deprecated */
		gC_params.put(GlobalParameters.TIMEPLOT, "MEANBESTVALUES");
		gC_params.put(GlobalParameters.PLOTXTYPE, "EVALUATIONS");
		gC_params.put(GlobalParameters.PLOTSLEEP, 100);
		
		gC_params.put(GlobalParameters.PLOTTYPE, "EBPOSITIONS");
		
		int gi_defaultDim = 30;
		
		/* response variable for regression functions */
		int li_response = 1;
		
		/* lambda 1, LASSO penalty scaling factor, 0 means penalty is off */
		double li_lambda1 = 0;
		
		/* filename of the data file for a function that needs data */
		String lS_datafile = null;
		
		/*  init the functions */
		ArrayList<TestFunction>fct = new ArrayList<TestFunction>();
		
		/* holds the names of the methods to execute */
		ArrayList<String> lC_methods = new ArrayList<String>();
				
		String ls_fct = null;
		
		for(int argopt = 0; argopt < args.length; argopt++) {
						
			if(args[argopt].equals("-fct")) {
				
				if(argopt++ < args.length) {
					ls_fct = args[argopt];
				}
				
				continue;
			}
			
			if(args[argopt].equals("-limit")) {
				
				if(argopt++ < args.length) {
					
					gC_params.put(GlobalParameters.LIMIT, (int) Integer.parseInt(args[argopt]));
				}
				continue;
			}
			
			/* total population size; if this is set, it is assumed to have a 1:1 Ratio among EBs and OBs */ 
			if(args[argopt].equals("-n")) {
				
				if(argopt++ < args.length) {
					int li_n = (int) (Integer.parseInt(args[argopt]) / 2);
					gC_params.put(GlobalParameters.NEB, li_n);
					gC_params.put(GlobalParameters.NOB, li_n);
				}
				continue;
			}
			
			/* number of employed bees */
			if(args[argopt].equals("-neb")) {
				
				if(argopt++ < args.length) {
					gC_params.put(GlobalParameters.NEB, (int) Integer.parseInt(args[argopt]));
				}
				continue;
			}

			if(args[argopt].equals("-nob")) {
				
				if(argopt++ < args.length) {
					gC_params.put(GlobalParameters.NOB, (int) Integer.parseInt(args[argopt]));
				}
				continue;
			}
			
			if(args[argopt].equals("-dim")) {
				
				if(argopt++ < args.length) {
					gi_defaultDim = (int) Integer.parseInt(args[argopt]);
				}
				continue;
			}
			
			if(args[argopt].equals("-method")) {
				
				if(argopt++ < args.length) {
					lC_methods.add((String) args[argopt]);
				}
				continue;
		    }
			
			if(args[argopt].equals("-runs")) {
				
				if(argopt++ < args.length) {
					gC_params.put(GlobalParameters.nr_runs, (int) Integer.parseInt(args[argopt]) );
				}
				continue;
			}
			
			if(args[argopt].equals("-standardlimit")) {
				
				if(argopt++ < args.length) {
					gC_params.put(GlobalParameters.STANDARDLIMIT, (boolean) Boolean.parseBoolean(args[argopt]));
				}
				continue;
			}
			
			if(args[argopt].equals("-pmode")) {
				
				if(argopt++ < args.length) {
					gC_params.put(GlobalParameters.PRINTMODE, (int) Integer.parseInt(args[argopt]) );
				}
				continue;
			}
			
			if(args[argopt].equals("-trounds")) {
				
				if(argopt++ < args.length) {
					gC_params.put(GlobalParameters.TOURNAMENT_ROUNDS, (int) Integer.parseInt(args[argopt]) );
				}
				continue;
			}
			
			if(args[argopt].equals("-crange")) {
				
				if(argopt++ < args.length) {
					gC_params.put(GlobalParameters.CHECKRANGE, (boolean) Boolean.parseBoolean(args[argopt]));
					
				}
				continue;
			}
			
			/* response variable for regression function */
			if(args[argopt].equals("-response")) {
				
				if(argopt++ < args.length) {
					li_response = (int) Integer.parseInt(args[argopt]);
				}
				continue;
				
			}
			/* response variable for regression function */
			if(args[argopt].equals("-l1")) {
				if(argopt++ < args.length) {
					li_lambda1 = (double) Double.parseDouble(args[argopt]);
				}
				continue;
			}
			/* file name of data file */
			if(args[argopt].equals("-f")) {
				if(argopt++ < args.length) {
					lS_datafile = args[argopt];
				}
				continue;
			}

		} 

		
		if(ls_fct != null) {
				Double elasticNet_goal = new Double(0);
				Double sc_goal = new Double(0.00001);
				Double sm_goal = new Double(0.01);
				Double gr_goal = new Double(0.1);
				Double rg_goal = new Double(100);
				Double rn_goal = new Double(100);
				Double ac_goal = new Double(0.1);
				
				Double sc_htest_goal = new Double(1E-25);
				Double sm_htest_goal = new Double(1E-10);
				Double gr_htest_goal = new Double(1E-9);
				Double rg_htest_goal = new Double(1E-7);
				Double rn_htest_goal = new Double(1);
				Double ac_htest_goal = new Double(1E-7);
				
				/* Runtime mode, no goals used (runs until end of max evaluations)  */
				if(  (Integer)gC_params.get(GlobalParameters.PRINTMODE) == 3 ) {
					sc_goal = Double.NaN;
					sm_goal = Double.NaN;
					gr_goal = Double.NaN;
					rg_goal = Double.NaN;
					rn_goal = Double.NaN;
					ac_goal = Double.NaN;
					
					sc_htest_goal = Double.NaN;
					sm_htest_goal = Double.NaN;
					gr_htest_goal = Double.NaN;
					rg_htest_goal = Double.NaN;
					rn_htest_goal = Double.NaN;
					ac_htest_goal = Double.NaN;
				}
						
				if(ls_fct.equals("ElasticNet")) { 
					/* done a little trick here, used response variable instead of dimension, dimension is set in constructor - depends on input data */
					ElasticNet newFct = new ElasticNet(li_response, -5, 5, elasticNet_goal, 0, 1, "ElasticNet", (Boolean) gC_params.get(GlobalParameters.CHECKRANGE));
					newFct.setLambda1(li_lambda1);
					newFct.readData(lS_datafile);
					fct.add(newFct);  
				}
				
				if(ls_fct.equals("sc_stest")) { fct.add(new TestF1(2, -100, 100, sc_goal, 0, 1, "Schaffer", (Boolean) gC_params.get(GlobalParameters.CHECKRANGE)));  }
				if(ls_fct.equals("sm_stest")) { 
					fct.add(new TestF2(gi_defaultDim, -100, 100, sm_goal, 0, 2, "Sphere", (Boolean) gC_params.get(GlobalParameters.CHECKRANGE)));      }
				if(ls_fct.equals("gr_stest")) { fct.add(new TestF3(gi_defaultDim, -600, 600, gr_goal, 0, 3, "Griewank", (Boolean) gC_params.get(GlobalParameters.CHECKRANGE)));     }
				if(ls_fct.equals("rg_stest")) { fct.add(new TestF4(gi_defaultDim, -5.12, 5.12, rg_goal, 0, 4, "Rastrigin", (Boolean) gC_params.get(GlobalParameters.CHECKRANGE)));  }
				if(ls_fct.equals("rn_stest")) { fct.add(new TestF5(gi_defaultDim, -30, 30, rn_goal, 0, 5, "Rosenbrock", (Boolean) gC_params.get(GlobalParameters.CHECKRANGE)));     }
				if(ls_fct.equals("ac_stest")) { fct.add(new TestF6(gi_defaultDim, -32, 32, ac_goal, 0, 6, "Ackley", (Boolean) gC_params.get(GlobalParameters.CHECKRANGE))); }
				
				if(ls_fct.equals("sc_htest")) { fct.add(new TestF1(2, -100, 100, sc_htest_goal, 0, 1, "Schaffer", (Boolean) gC_params.get(GlobalParameters.CHECKRANGE)));  }
				if(ls_fct.equals("sm_htest")) { fct.add(new TestF2(gi_defaultDim, -100, 100, sm_htest_goal, 0, 2, "Sphere", (Boolean) gC_params.get(GlobalParameters.CHECKRANGE)));      }
				if(ls_fct.equals("gr_htest")) { fct.add(new TestF3(gi_defaultDim, -600, 600, gr_htest_goal, 0, 3, "Griewank", (Boolean) gC_params.get(GlobalParameters.CHECKRANGE)));     }
				if(ls_fct.equals("rg_htest")) { fct.add(new TestF4(gi_defaultDim, -5.12, 5.12, rg_htest_goal, 0, 4, "Rastrigin", (Boolean) gC_params.get(GlobalParameters.CHECKRANGE)));  }
				if(ls_fct.equals("rn_htest")) { fct.add(new TestF5(gi_defaultDim, -30, 30, rn_htest_goal, 0, 5, "Rosenbrock", (Boolean) gC_params.get(GlobalParameters.CHECKRANGE)));     }
				if(ls_fct.equals("ac_htest")) { fct.add(new TestF6(gi_defaultDim, -32, 32, ac_htest_goal, 0, 6, "Ackley", (Boolean) gC_params.get(GlobalParameters.CHECKRANGE))); }
							
				/* Standard test setting with moved function */
				if(ls_fct.equals("scmoved")) { 
					TestFunction fcttmp = new TestF1(2, -100, 100, sc_goal, 0, 1, "Schaffer_moved", (Boolean) gC_params.get(GlobalParameters.CHECKRANGE));
					fcttmp.move(80);
					fct.add(fcttmp); 
				}  
				if(ls_fct.equals("smmoved")) { 
					TestFunction fcttmp = new TestF2(gi_defaultDim, -100, 100, sm_goal, 0, 2, "Sphere_moved", (Boolean) gC_params.get(GlobalParameters.CHECKRANGE));
					fcttmp.move(80);
					fct.add(fcttmp);  
				}
				if(ls_fct.equals("grmoved")) { 
					TestFunction fcttmp = new TestF3(gi_defaultDim, -600, 600, gr_goal, 0, 3, "Griewank_moved", (Boolean) gC_params.get(GlobalParameters.CHECKRANGE));
					fcttmp.move(480);
					fct.add(fcttmp);  
				}
				if(ls_fct.equals("rgmoved")) { 
					TestFunction fcttmp = new TestF4(gi_defaultDim, -5.12, 5.12, rg_goal, 0, 4, "Rastrigin_moved", (Boolean) gC_params.get(GlobalParameters.CHECKRANGE)); 
					fcttmp.move(4.09);
					fct.add(fcttmp);
				}
				if(ls_fct.equals("rnmoved")) { 
					TestFunction fcttmp = new TestF5(gi_defaultDim, -30, 30, rn_goal, 0, 5, "Rosenbrock_moved", (Boolean) gC_params.get(GlobalParameters.CHECKRANGE)); 
					fcttmp.move(24);
					fct.add(fcttmp);
				}
				if(ls_fct.equals("acmoved")) { 
					TestFunction fcttmp = new TestF6(gi_defaultDim, -32, 32, ac_goal, 0, 6, "Ackley_moved", (Boolean) gC_params.get(GlobalParameters.CHECKRANGE)); 
					fcttmp.move(25.6);
					fct.add(fcttmp);
				} 	
				
		} 
		
		/* check if minimum nr. arguments specified */
		if(fct.size() == 0 || (lC_methods.size() == 0) ) {
			System.out.println("usage: java ModelTester -fct [fct] -method [method] -pmode [print mode] -runs [runs] [-n [total popSize] | -neb [nr. EBs] -nob [nr. OBs]];");
			System.out.println(" e.g.: java ModelTester -fct sm_stest -method SABC -pmode 2 -runs 30 -n 100");
			System.out.println("       executes standard ABC on the sphere function with standard test goals for 30 runs in print mode 2.");
			System.out.println(" e.g.: java ModelTester -fct rn_htest -method ABCgbest -pmode 2 -runs 30 -neb 15 -nob 0");
			System.out.println("       executes ABCgbest on the rosenbrock function with hard test goals with 15 employed bees and no onlooker bees.");
			System.out.println(" e.g.: java ModelTester -fct ac_stest -method ABCgbest -pmode 3 -n 30");
			System.out.println("       executes ABCgbest on the ackley function for 200000 function evaluations. Note, pmode 3 does not exit (ignores) when test goal reached.\n");
			System.out.println("  -f");
			System.out.println("             the data file in the case it is required (e.g. for the ElasticNet function)");
			System.out.println("  -lambda1");
			System.out.println("             the scaling factor of the LASSO penalty (default is 0, i.e. no penalty)");
			System.out.println("  -response");
			System.out.println("             the index of the variable in the data file designated as response variable (range: 1,2,..)");
			
			System.out.println("Print Modes:  	0 - summary statistics for evaluations and results");
			System.out.println("                1 - evaluations per run");
			System.out.println("                2 - average evaluations over all runs");
			System.out.println("                3 - average solution value in each cycle (but prints out evaluation");
			System.out.println("                4 - best solution found per run with score (1st column) and function parameters");
			System.out.println("                5 - scores (1st column) and function parameters for all solutions visited");
			System.out.println("                6 - same as (5) but for MSE (only for ElasticNet)");
			System.out.println("See ModelTester.java for more optional settings");

			System.exit(-1);
		}
		
		/* print out all settings */
		String lC_setup = "#"; 
		
		for(int i = 0; i < lC_methods.size(); i++) {
			lC_setup += " method: " + lC_methods.get(i) + ", "; 
		}
		for(int i = 0; i < fct.size(); i++) {
			lC_setup += " fct: " +  fct.get(i).getName() + ", ";
		}
		lC_setup += " neb: " +  (Integer) gC_params.get(GlobalParameters.NEB) + ", ";
		lC_setup += " nob: " +  (Integer) gC_params.get(GlobalParameters.NOB) + ", ";
		lC_setup += " runs: " + (Integer) gC_params.get(GlobalParameters.nr_runs) + ", ";
		lC_setup += " pmode: " + (Integer) gC_params.get(GlobalParameters.PRINTMODE) + ", ";
		lC_setup += " checkrange: " + (Boolean) gC_params.get(GlobalParameters.CHECKRANGE) + ", ";
		System.out.println(lC_setup);
		
		ModelRunner lC_run = new ModelRunner();
		
		gC_params.put(GlobalParameters.PLOTENABLED, false);
		
		lC_run.start(fct, gC_params, lC_methods);
		
	}
		
}
