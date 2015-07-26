
package global;

import java.util.ArrayList;

public class Blackboard {

    public ArrayList<Solution> solutions;
   
    
    // blackboard knows function, easier to evaluate stuff here
    // cause many classes do the same, therefor do it here
    public TestFunction fct1;
    
    public Blackboard(int nrsolutions, int possize, TestFunction aC_fct) {

    	solutions = new ArrayList<Solution>(nrsolutions);

    	for(int i = 0; i < nrsolutions; i++) {
    		Solution tmpsol = new Solution(possize);
    		solutions.add(tmpsol);
    	}
    	
       	fct1 = aC_fct;
    }


    public void post(int index, double[] position, double value) {
    				
    	 Solution sol = (Solution) solutions.get(index);
    	 sol.set(position, value);
    }

    public Solution getSolution(int index) {
	
    	return (Solution) solutions.get(index);
	
    }

    public ArrayList getAllSolutions() {
	
	return solutions;
    }

    public Solution getBestSolution() {

		// get first element
		Solution tmpsol = (Solution) solutions.get(0);
	
		// store the best solution here, will be returned
		Solution bestsol = tmpsol;
	
		// loop through all solutions
		for(int i = 1; i < solutions.size(); i++) {
		    tmpsol = (Solution) solutions.get(i);
	
		    if( fct1.better(tmpsol.value, bestsol.value) ) { // if current solution better than best one
			   bestsol = tmpsol;
		    }
		}
		return bestsol;
    }
    
    
       
}

