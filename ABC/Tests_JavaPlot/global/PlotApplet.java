
package global;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

/** Example demonstrating drawPolyline(). **/
public class PlotApplet extends JPanel {
	
	private int gi_vmin = -600;
	private int gi_vmax = 600;
	private double[] gda_center = {0,0};
	
	private double[][] gda_data;
	

	private static JFrame frame;
	
	public PlotApplet () {
		gda_data = new double[0][0];
	}
	
	 private static void createAndShowGUI() {
	        //Create and set up the window.
	        frame = new JFrame("Frame");
	        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

	        PlotApplet panel = new PlotApplet();
	        frame.add(panel);
	        
	        //Display the window.
	        frame.pack();
	        frame.setSize(600, 600);
	       
	        frame.setVisible(true);
	        
	         }
	 
	 
	 
  public void showWindow () { 
	  //Schedule a job for the event-dispatching thread:
      //creating and showing this application's GUI.
      javax.swing.SwingUtilities.invokeLater(new Runnable() {
          public void run() {
              createAndShowGUI();
          }
      });
  } 
  
  public void setCenter(double[] ad_center) {
	  gda_center = ad_center;
	  frame.repaint();
  }
  
  protected void paintComponent(Graphics g) {
	  	  
	    super.paintComponent(g);
	    g.setColor(new Color(1));
		  
	    int gi_height = frame.getHeight();
	    int gi_width = frame.getWidth();
	    
	    
	    /* paint center */
	    g.setColor(Color.RED);
	    int gi_xoff = ((int)gi_width / 2) - (int)gda_center[0];  
	    int gi_yoff = ((int)gi_height / 2) - (int) gda_center[1];  
		g.fillOval(gi_xoff, gi_yoff , 5, 5);
	    
	    /* paint all points */
		g.setColor(Color.BLACK);
	    
		double [][] gg = new double[2][3];
		
		gg[0][0] = 550;
		gg[1][0] = 590;
		gg[0][1] = -300;
		gg[1][1] = 150;
		gg[0][2] = 250;
		gg[1][2] = -150;
				
		gda_data = gg;
		
		for(int i = 0; i < gda_data[0].length; i++) {
					
			/* TODO float case */
			int xoff = ((gi_width / 2) * (int)gda_data[0][i]) / gi_vmax;
			int yoff = ((gi_height / 2) * (int)gda_data[1][i]) / gi_vmax;
	    
			gi_xoff = ((int)gi_width / 2) + xoff;  
			gi_yoff = ((int)gi_height / 2) - yoff;  
		   
			g.fillOval(gi_xoff, gi_yoff , 5, 5);
		}
	   }
  
  private void paintData() {
	  Graphics g = getGraphics();
	  
	   int gi_height = frame.getHeight();
	    int gi_width = frame.getWidth();
	    
	  /* paint center */
	    g.setColor(Color.RED);
	    int gi_xoff = ((int)gi_width / 2) - (int)gda_center[0];  
	    int gi_yoff = ((int)gi_height / 2) - (int) gda_center[1];  
		g.fillOval(gi_xoff, gi_yoff , 5, 5);
  }
} 