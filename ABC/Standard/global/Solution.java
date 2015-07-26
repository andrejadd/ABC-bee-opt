
package global;


public class Solution implements Cloneable {

    public double[] indices;
    public double value;
    public double select_prob;
    
    public int rank = 0;
    public double dist;
    
    public double rank_prob;
    public double dist_prob;
    
    public int toref;
    public int limit = 0;
    public int newpostype = -1;
    
    public Solution(int pos_size) {
    	this.indices = new double[pos_size];
    }
    
    public Solution(double[] ada_position) {
    	indices = ada_position;
    }
    
    
    public Solution(double [] ada_indices, double ad_value) {
  
    	indices = ada_indices;
    	value = ad_value;
    }
    
    public Solution(Solution aC_solution) {
    	
    	indices = aC_solution.indices.clone();
    	value = aC_solution.value;
    	select_prob = aC_solution.select_prob;
    	
    }
    
    //	public void init(int possize) {
    //    double[] posx = new double[possize];
    //}

    public void set(double[] ada_position, double ad_value) {
	
	this.indices = ada_position;
	this.value = ad_value;
    }
    
    
    public Object clone() { 
        
        Solution d = new Solution(this.indices.length);
        d.indices = (double[])this.indices.clone();
        d.value = this.value;
        d.limit = this.limit;
        d.select_prob = this.select_prob;
        
        return d; 
    }
    
 public double getNode(int ai_index) {
    	
    	if(ai_index < 0) {
    		ai_index = indices.length + ai_index;
    	}
    	if(ai_index >= indices.length) {
    		ai_index = ai_index - indices.length;
    	}
    	
    	return indices[ai_index];
    	
    }
    
    public void setNode(int ai_node, int ai_index) {
    	
    	/* check if in range and wrap */
    	while(ai_index < 0) {
    		ai_index += indices.length;
    	}
    	
    	while(ai_index >= indices.length) {
    		ai_index -= indices.length;
    	}
    	
    	indices[ai_index] = ai_node;
    	
    }
    
    public int size() {
    	return indices.length;
    }
    
    public double[] getPosition() {
    	return indices;
    }
    
}

