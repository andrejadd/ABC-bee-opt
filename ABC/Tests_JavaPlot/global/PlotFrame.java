
package global;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

import java.awt.image.BufferedImage;

/** Example demonstrating drawPolyline(). **/
public class PlotFrame extends Frame {
	
	private int gi_vmin = -600;
	private int gi_vmax = 600;
	private int gi_height = 600;
	private int gi_width = 600;
	
	private double[] gda_center = {0,0};
	 private final BufferedImage bufferedImage = new BufferedImage(600,600, BufferedImage.TYPE_INT_RGB);
	private Graphics2D g;    
	
	private double[][] gda_data;
	
	public PlotFrame() {
		 addWindowListener(new WindowAdapter(){  public void windowClosing(WindowEvent e) {  dispose();  }});
		 g = bufferedImage.createGraphics();
		
		setSize(gi_width + 10, gi_height + 27);
		setVisible(true);
	}

	//private static JFrame frame;
	  public void paint(Graphics g) {
	        g.drawImage(bufferedImage, 5, 22,gi_width,gi_height, this);
	    }

	  public void drawDate(double[][] ada_data) {
		  
		  gda_data = ada_data;
		  draw();
			
	  }
	 
	   public void draw() {
	        //Graphics g = bufferedImage.getGraphics();
	        	        
		   g.setColor(Color.WHITE);
		   g.fill(new Rectangle(0, 0, gi_width, gi_height));
		   
	        /* paint center */
		    g.setColor(Color.RED);
		    int gi_xoff = ((int)gi_width / 2) - (int)gda_center[0];  
		    int gi_yoff = ((int)gi_height / 2) - (int) gda_center[1];  
			g.fillOval(gi_xoff, gi_yoff , 5, 5);
	      
			/* paint all points */
			g.setColor(Color.BLUE);
						
			//g.drawLine(0, 0, 600, 600);
			    				
			if(gda_data != null) {
				for(int i = 0; i < gda_data[0].length; i++) {
							
					/* TODO float case */
					int xoff = ((gi_width / 2) * (int)gda_data[0][i]) / gi_vmax;
					int yoff = ((gi_height / 2) * (int)gda_data[1][i]) / gi_vmax;
			    
					gi_xoff = ((int)gi_width / 2) + xoff;  
					gi_yoff = ((int)gi_height / 2) - yoff;  
				   
					g.fillOval(gi_xoff, gi_yoff , 5, 5);
				}
			}
			
	        repaint();
	    }
	   
	   public void setCenter(double[] ad_center) {
		   gda_center = ad_center;
		   repaint();
	   }
  
  
} 