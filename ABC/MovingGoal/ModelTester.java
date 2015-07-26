
import global.MovingGoal_Sphere;

import global.ParameterHolder;
import global.GlobalParameters;

import global.TestFunction;

import java.util.ArrayList;

import standardABC.sABCParameters;

import com.panayotis.gnuplot.JavaPlot;




public class ModelTester {
	
	private static ParameterHolder gC_params = new ParameterHolder();
	
	public static void main(String[] args) {
			
		/* set parameters BA for Rosenbrock d=4 , Exp. PHAM2005  */
		gC_params.put(GlobalParameters.popSize, 30 );
		gC_params.put(GlobalParameters.nr_runs, 1 );
		gC_params.put(GlobalParameters.numEval1, 15000 );
		gC_params.put(GlobalParameters.numEval2, 30000 );
		
		
		boolean [] lba_fcts = {
				true    /* Moving Goal */
				};  
		
		
		/*  init the functions */
		ArrayList<TestFunction>fct = new ArrayList<TestFunction>();

		if(lba_fcts[0]) {
			fct.add(new MovingGoal_Sphere(10, -50, 50, 0.0, 0, 2, "Moving_goal"));  

		}
		
		ModelRunner lC_run = new ModelRunner();
			
		JavaPlot lC_plot = new JavaPlot();
		lC_plot.set("pointsize", "0.3");
		lC_plot.set("logscale", "y");
		//lC_plot.set("logscale", "y");
		
			lC_run.start(fct, gC_params, lC_plot);
			lC_plot.plot();
		}
	
}
