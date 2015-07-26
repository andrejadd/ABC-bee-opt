



import java.util.Random;
import testfunction.*;

public class DEAgent  {

    static Random generator;
    Blackboard gC_bb;

    // Cross-Over Rate, 
    private static double gd_crossover = 0.8; 
   
    private int gi_index;
    static int gi_totalnr = 0;

    // this holds the trial position and value
    public double[] gda_s_position;
    public double  gd_s_value;
    
    public double[] position;
    public double value;
    TestFunction gC_fct;
    // the DE variant, 0 is default and corresponds to DE/rand/1/bin
    private int gi_type = 0;
   

 
    public DEAgent(Blackboard aC_bb, TestFunction aC_fct, double[] ada_position, double ad_value, int ai_id, double ad_crossover,  int ai_type ) {

    	//super(aC_fct, ada_position, ai_id);
    	gC_fct = aC_fct;
    	
    	// the variant/type of DE
    	gi_type = ai_type;

    	// need generator here, because velocity update needs rand inside loop
    	generator = new Random(System.currentTimeMillis());
	
		// copy blackboard reference
		gC_bb = aC_bb; 
		
		// get value at position and post to global best if better
		value = ad_value;//gC_fct.solve(ada_position);
	
		gi_index = ai_id;
		if(gi_index > gi_totalnr) {
			gi_totalnr = gi_index; // 
		}
		
		gda_s_position = new double[ada_position.length];
		position = ada_position.clone();
		
		gd_crossover = ad_crossover;
		
    }
    
    public void post() {
	// examine quality of position
	double ld_val = gC_fct.solve(position);
	value = ld_val;

	// post to blackboard
	//gC_bb.post(gi_index, position, ld_val);
    }



    public void modifyposition(double ad_f_factor) {

    	// randomly pick three solutions, all different from each other
    	Solution lC_a, lC_b, lC_c;

    	int li_a, li_b, li_c;  
    	do { li_a = generator.nextInt(gi_totalnr + 1); } while (li_a==gi_index);
    	do { li_b = generator.nextInt(gi_totalnr + 1); } while (li_a==gi_index || li_b==li_a);
    	do { li_c = generator.nextInt(gi_totalnr + 1); } while (li_a==gi_index || li_c==li_a || li_c==li_b);
	
		lC_a = gC_bb.getSolution(li_a); 
		lC_b = gC_bb.getSolution(li_b); 
		lC_c = gC_bb.getSolution(li_c); 

		// first copy whole current position
		gda_s_position = (double []) position.clone();


		int n = generator.nextInt(position.length); // start at position index
		int L = generator.nextInt(position.length); // init with random number
		
		if(gi_type == 0) {
			de_rand_1_bin(lC_a, lC_b, lC_c, ad_f_factor);
		}
		if(gi_type == 1) {
			de_rand_1_exp(lC_a, lC_b, lC_c, n, L, ad_f_factor);
		}

		gd_s_value = gC_fct.solve(gda_s_position);
    }


    /* DE/rand/1/exp
       
       cross-over scheme: do crossover with block of length L, starting at position n
     
       From Storn: DE 1996, Running Article 
       And here original source code.
       
       public void apply (double F, double Cr, int dim, double[]x, double[]gen_best, double[][]g0) {
          prepare (dim);
	  do  { 
	      x[i] = g0[0][i] + F * (g0[1][i] - g0[2][i]);
              i = ++i % dim;
	  } while ((deRandom.nextDouble() < Cr) && (++counter < dim));
       }

    */

    public void de_rand_1_exp(Solution aC_a, Solution aC_b, Solution aC_c, int ai_n, int ai_L, double af_f_factor) {

		double[] lda_posa = aC_a.getPosition();
		double[] lda_posb = aC_b.getPosition();
		double[] lda_posc = aC_c.getPosition();
		int ld_dim = aC_a.size();

	

    	
    	do  {

    		gda_s_position[ai_n] = lda_posc[ai_n] + af_f_factor * (lda_posa[ai_n] - lda_posb[ai_n]);
    		ai_n = ++ai_n % ld_dim;
    	} while( (generator.nextDouble() < gd_crossover) && (++ai_L < ld_dim));
    }


    /* DE/rand/1/bin
       
       cross-over scheme: foreach parameter decide to make crossover (rand() < CR)
     
       From Storn: DE in Journal of Global Optimization: 1997
       And here original source code.
       
       public void apply (double F, double Cr, int dim, double[]x, double[]gen_best, double[][]g0) {
       prepare (dim);
       while (counter++ < dim) { 
           if ((deRandom.nextDouble() < Cr) || (counter == dim))  // <- here the last parameter(noise paramter) is always taken
	   x[i] = g0[0][i] + F * (g0[1][i] - g0[2][i]);
	   i = ++i % dim;
       }
    */

    public void de_rand_1_bin(Solution aC_a, Solution aC_b, Solution aC_c, double af_f_factor) {

    	double[] lda_posa = aC_a.getPosition();
    	double[] lda_posb = aC_b.getPosition();
    	double[] lda_posc = aC_c.getPosition();
    	int ld_dim = aC_a.size();

    	// randomly pick parameter inside dimension, ensures in loop below that trial gets at least one paramter
    	int rnb_i = generator.nextInt(ld_dim);

	
	
    	for(int j = 0; j < position.length; j++) {
		
    		if(generator.nextDouble() <= gd_crossover || j == rnb_i) {  // <- don't have a noise , choose some
    			gda_s_position[j] = lda_posc[j] + af_f_factor * (lda_posa[j] - lda_posb[j]);
    		} 
    	}
    }


    // this method must be run after the modifyposition() of all agents in population has been executed
    public void updateposition() {

	if(gC_fct.better(gd_s_value, value)) {
	    position = (double [])gda_s_position.clone();
	    value = gd_s_value;

	    // now the bb can be updated
	    gC_bb.post(gi_index, position, value);
	}
    }
}