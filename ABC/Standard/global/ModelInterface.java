
package global;

import java.util.ArrayList;

import testfunction.TestFunction;

public interface ModelInterface {

    //public void Initiate();
    public void Init(TestFunction aC_fcttest, ParameterHolder aC_params);
    public void Iterate();
    public Solution getBestSolution();
    public int getPopSize();
    //public void getData(double[] ada_x, double[] ada_y); // TODO : delete this or impl. 
    //public double getBest();
    public boolean optimaReached(double ad_value);
    public void resetFctCounter();
    public String getFctName();
    public int getFctID();
    public int getFctCalls();
    public ArrayList getSolutions(int li_type);
    public double getUpdateRate();
    
    /* added for the use in ElasticNet and other regression functions */
	public double getMSE(double[] indices);
   
}