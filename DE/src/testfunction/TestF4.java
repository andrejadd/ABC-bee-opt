package testfunction;

public class TestF4 extends TestFunction {

	public TestF4(int dimension, double ad_low, double ad_up, double ad_min,
			int ai_minmax, int ai_id, String aC_name) {
		super(dimension, ad_low, ad_up, ad_min, ai_minmax, ai_id, aC_name);
		// TODO Auto-generated constructor stub
	}

	@Override
	public double solve(double[] param) {
		
		callcounter++;
		
		double sum = 0;
				
		
		for(int i = 0; i < dimension; i++) {
			double gd_x = param[i] + gd_move;
			
			sum += (Math.pow( gd_x, 2) - 10 * Math.cos(2 * Math.PI * gd_x) + 10); 
		}
		
		return sum;
	}

}
