package testfunction;


public class MovingGoal_Sphere extends TestFunction {
	
	private double gd_severity = 0;
	
	public MovingGoal_Sphere(int dimension, double ad_low, double ad_up, double ad_min,
			int ai_minmax, int ai_id, String aC_name) {
		super(dimension, ad_low, ad_up, ad_min, ai_minmax, ai_id, aC_name);
		// TODO Auto-generated constructor stub
	} 
	
	public double solve(double[] param) {
		callcounter++;
		double sum = 0;
		
		for (int i = 0; i < dimension; i++) {
			sum += Math.pow( (param[i] - gd_severity), 2);
		}
		
		return sum; 
		
	}
	
	public void changeSeverity(double ad_value) {
		gd_severity += ad_value;
	}
	
}

