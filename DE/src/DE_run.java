

//import java.util.ArrayList;

import testfunction.TestF1;
import testfunction.TestF2;
import testfunction.TestF3;
import testfunction.TestF4;
import testfunction.TestF5;
import testfunction.TestF6;
import testfunction.TestFunction;


public class DE_run {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
	
		int PRINTMODE = 0;
		
		// population size
		int li_n = 60;
		
		// max. evaluation number
		int li_men = 500000;
		
		int li_runs = 5;
			
		int gi_defaultDim = 30;
		//boolean lb_fctset = false;
		int li_type = 0; 	/* 0 -> /bin, 1 -> /exp */
		boolean lb_f_fixed = false;
		
		double ld_crossover = 0.8;
		double ld_fscale = 0.5;
		
		TestFunction fct = null;

		for(int argopt = 0; argopt < args.length; argopt++) {
			
			if(args[argopt].equals("-fct")) {
				
				if(argopt++ < args.length) {
					
					if(args[argopt].equals("sc")) {  fct = new TestF1(2, -100, 100, 0.00001, 0, 1, "Schaffer");  }
					if(args[argopt].equals("sm")) {  fct = new TestF2(gi_defaultDim, -100, 100, 0.01, 0, 2, "Sphere");      }
					if(args[argopt].equals("gr")) {  fct = new TestF3(gi_defaultDim, -600, 600, 0.1, 0, 3, "Griewank");     }
					if(args[argopt].equals("rg")) {  fct = new TestF4(gi_defaultDim, -5.12, 5.12, 100, 0, 4, "Rastrigin");  }
					if(args[argopt].equals("rn")) {  fct = new TestF5(gi_defaultDim, -30, 30, 100, 0, 5, "Rosenbrock");     }
					if(args[argopt].equals("ac")) {  fct = new TestF6(gi_defaultDim, -32, 32, 0.1, 0, 6, "Ackley"); }
					
					
					/* No Optima - NaN */
					/* 
					if(args[argopt].equals("sc")) {  fct.add(new TestF1(2, -100, 100, Double.NaN, 0, 1, "Schaffer"));  }
					if(args[argopt].equals("sm")) { fct.add(new TestF2(gi_defaultDim, -100, 100, Double.NaN, 0, 2, "Sphere"));      }
					if(args[argopt].equals("gr")) { fct.add(new TestF3(gi_defaultDim, -600, 600, Double.NaN, 0, 3, "Griewank"));     }
					if(args[argopt].equals("rg")) { fct.add(new TestF4(gi_defaultDim, -5.12, 5.12, Double.NaN, 0, 4, "Rastrigin"));  }
					if(args[argopt].equals("rn")) { fct.add(new TestF5(gi_defaultDim, -30, 30, Double.NaN, 0, 5, "Rosenbrock"));     }
					if(args[argopt].equals("ac")) { fct.add(new TestF6(gi_defaultDim, -32, 32, Double.NaN, 0, 6, "Ackley"));         } 
					*/
					
				}
				continue;
			}
			
			if(args[argopt].equals("-type")) {
				
				if(argopt++ < args.length) {
					
					li_type = (int) Integer.parseInt(args[argopt]);
				}
				continue;
			}
			
			if(args[argopt].equals("-runs")) {
				
				if(argopt++ < args.length) {
					
					li_runs = (int) Integer.parseInt(args[argopt]);
				}
				continue;
			}
			
			if(args[argopt].equals("-men")) {
				
				if(argopt++ < args.length) {
					
					li_men = (int) Integer.parseInt(args[argopt]);
				}
				continue;
			}
			
			if(args[argopt].equals("-cr")) {
				
				if(argopt++ < args.length) {
					
					ld_crossover = (int) Double.parseDouble(args[argopt]);
				}
				continue;
			}
			
			if(args[argopt].equals("-ffixed")) {
				
				if(argopt++ < args.length) {
					
					if((int) Integer.parseInt(args[argopt]) == 0) {
						lb_f_fixed = false;
					} else {
						lb_f_fixed = true;
					}
				}
				continue;
			}
		}
		
		if(fct == null ) {
			
			System.out.println("args at least: -fct [sc|sm|gr|rg|rn|ac]");
			System.out.println("args full: -fct [..] -type [0|1] -runs n -mcn m -ffixed [0|1]");

			System.exit(0);
			
		}
		
		System.out.println("# DE type: " + li_type + ", fct: " + fct.getName() + ", runs: " + li_runs + ", Max-Evals: " + li_men + ", popSize: " + li_n);
		System.out.println("#    scale factor F: " + ld_fscale + ", F fixed: " + lb_f_fixed + ", crossover rate: " + ld_crossover);  
		
		int li_mcn = (li_men / (li_n)) + 1;
		
		double lda_bestvalues[][] = new double[li_runs][li_mcn];
		
		for(int run = 0; run < li_runs; run++) {
			
			fct.resetcallcounter();
			
		    ModelInterface lC_de = new DEModel(fct, li_n, li_type, lb_f_fixed, ld_fscale, ld_crossover);
			
			Solution lC_sol;
			
			boolean lb_goalreached = false; 
			
			for(int j = 0; j < li_mcn; j++) {			
			
				lC_de.Iterate();
		
				lC_sol = lC_de.getBestSolution();
			
				lda_bestvalues[run][j] = lC_sol.value;
				
				if(PRINTMODE == 0) {
					if(lC_de.optimaReached(lC_sol.value)) {
						lb_goalreached = true;	
						break;
					}
				}
			}
			
			if(lb_goalreached && (PRINTMODE == 0)) {
//				System.out.print(fct.callcounter + "\t" + j + "\t" +  lC_sol.value + "\n");
//				System.out.print(fct.callcounter + "\n");
					System.out.print(fct.callcounter + "\n");
			} else if ( (!lb_goalreached) && (PRINTMODE == 0) ) {
					System.out.print("-1\n");
			}
			
		} 
		
		if(PRINTMODE == 1) {
			
			for(int j = 0; j < li_mcn; j++) {	
				
				double ld_vsum = 0;
				for(int run = 0; run < li_runs; run++) {
					ld_vsum += lda_bestvalues[run][j];
				}
				
				int li_evals = ( (j+1)*li_n);
				System.out.println( li_evals + "\t" + (ld_vsum/li_runs) );
				if(ld_vsum == 0.0) {
					break;
				}
			}
			
		}
	}
}
