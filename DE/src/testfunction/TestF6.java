package testfunction;

public class TestF6 extends TestFunction {

	public TestF6(int dimension, double ad_low, double ad_up, double ad_min,
			int ai_minmax, int ai_id, String aC_name) {
		super(dimension, ad_low, ad_up, ad_min, ai_minmax, ai_id, aC_name);
		// TODO Auto-generated constructor stub
	}

	@Override
	public double solve(double[] param) {
		
		callcounter++;
	
		int n = param.length;
						
		double a = 0;
		double e = 0;
		
		for(int i = 0; i < n; i++) {
			
			double gd_x = param[i] + gd_move;
			
			a += Math.pow(gd_x, 2);
		
			e += Math.cos( (2 * Math.PI) * gd_x);
		}
		
		a /= n;
		e /= n;
		
		double b = -0.2 * Math.sqrt(a);
		double c = Math.exp(b);
		double d = -20 * c;
				
		double f = Math.exp(e);
			
		double result = d - f + 20 + Math.E;
		
		return result;
	}

}
