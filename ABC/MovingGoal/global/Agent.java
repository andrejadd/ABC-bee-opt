
package global;


public class Agent {

    //    public Blackboard bb;
    public TestFunction fct;  
    public Solution gC_solution;
    public int gi_id;

    public Agent(TestFunction aC_fct, double[] ada_position, int ai_id) {

    	//this.bb = blackboard;
    	fct = aC_fct;
    	gC_solution = new Solution(ada_position);
    	gi_id = ai_id;
    }
    
    public Agent(TestFunction aC_fct, Solution aC_solution, int ai_id) {

    	//this.bb = blackboard;
    	fct = aC_fct;
    	gC_solution = aC_solution;
    	gi_id = ai_id;
    }

    public double[] getposition() {
    	return gC_solution.getPosition();
    }

    public double solve() {
    	return fct.solve(gC_solution.getPosition());
	
    }
    
    public double solve(double[] pos) {
    	return fct.solve(pos);
    }

    // ? does onlooker needs this ? put into EmployedBee if not
    // determine which solution x1 or x2 is better, but this is defined in the fitness function fct
    // defined here because employed and onlooker needs it
    public boolean better(double ad_x1, double ad_x2){
	return fct.better(ad_x1, ad_x2);
    }
    
	public double getValue(){
		return gC_solution.value;
	}

}
