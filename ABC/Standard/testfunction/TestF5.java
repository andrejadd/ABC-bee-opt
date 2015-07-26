package testfunction;

public class TestF5 extends TestFunction {

	public TestF5(int dimension, double ad_low, double ad_up, double ad_min,
			int ai_minmax, int ai_id, String aC_name, boolean ab_checkrange) {
		super(dimension, ad_low, ad_up, ad_min, ai_minmax, ai_id, aC_name, ab_checkrange);
		// TODO Auto-generated constructor stub
	}

	@Override
	public double solve(double[] param) {
		
		callcounter++;
	
		double sum = 0;
				
		for(int i = 0; i < (dimension - 1); i++) {
			double gd_x1 = param[i+1] + gd_move;
			double gd_x2 = param[i] + gd_move;
			
			sum += 100 * Math.pow(gd_x1 - Math.pow( gd_x2, 2), 2) + Math.pow(gd_x2 - 1, 2);
		}
		
		return sum;
	}

}
