
package plot;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

import java.awt.image.BufferedImage;

/** Example demonstrating drawPolyline(). **/
public class PlotFrame extends JFrame {
	
	private int gi_upperborder = 27;
	private int gi_sideborder = 10;
	private double gd_vmin = -600;
	private double gd_vmax = 600;
	private int gi_height = 600;
	private int gi_width = 600;
	
	private double[] gda_center = {0,0};
	 private BufferedImage bufferedImage = new BufferedImage(600,600, BufferedImage.TYPE_INT_RGB);
	private Graphics2D g_image;    
	
	private double[][] gda_data;
	private double[][] gda_olddata;
	
	public PlotFrame() {
		 addWindowListener(new WindowAdapter(){  public void windowClosing(WindowEvent e) {  dispose();  }});
		 /* addWindowListener(new WindowAdapter(){  
			 public void windowResized(WindowEvent e) { 
				 System.out.println("resize"); }
			 }
		 ); */ 
		 
		 g_image = bufferedImage.createGraphics();
		
		setSize(gi_width + 10, gi_height + 27);
		setVisible(true);
	}

	//private static JFrame frame;
	  public void paint(Graphics g) {
		  
		  if( ( (getHeight() - gi_upperborder) != gi_height) || ( (getWidth()- gi_sideborder) != gi_width)) {
			  gi_height = getHeight() - gi_upperborder;
			  gi_width = getWidth() - gi_sideborder;
			  drawBuffer(true);
			  repaint();
		  } 
			  
		  g.drawImage(bufferedImage, 5, 22,gi_width,gi_height, this);
		  
	    }

	  public void drawData(double[][] ada_data) {
		  gda_olddata = gda_data;
		  gda_data = ada_data;
		  drawBuffer(false);
		  repaint();
			
	  }
	 
	   public void drawBuffer(boolean ab_resize) {
		   
		   if(ab_resize) {
			   
			   bufferedImage = new BufferedImage(gi_width,gi_height, BufferedImage.TYPE_INT_RGB);
			   g_image = bufferedImage.createGraphics();
		   }
		   
	        //Graphics g = bufferedImage.getGraphics();
	        g_image.clearRect(0, 0, gi_width, gi_height);	        
		   g_image.setColor(Color.WHITE);
		     g_image.fill(new Rectangle(0, 0, gi_width, gi_height));
		   
	        /* paint center */
		    g_image.setColor(Color.RED);
		    int gi_xoff = ((int)gi_width / 2) - (int)gda_center[0];  
		    int gi_yoff = ((int)gi_height / 2) - (int) gda_center[1];  
			g_image.fillOval(gi_xoff, gi_yoff , 5, 5);
	      
			/* paint all points */
			
			//g_image.drawLine(10, 100, 450, 30);
			    				
			if(gda_data != null) {
				for(int i = 0; i < gda_data[0].length; i++) {
							
					double xoff =  Math.round( ( (gi_width  / 2) * gda_data[0][i] ) / Math.abs(gd_vmax) ) ;
					double yoff =  Math.round( ( (gi_height / 2) * gda_data[1][i] ) / Math.abs(gd_vmin) ) ;
								    
					gi_xoff = ((int)gi_width / 2) +  (int) xoff;  
					gi_yoff = ((int)gi_height / 2) - (int) yoff;  
				   
					if(gda_data[2][i] == 0) {
						
						if(gda_data[3][i] == 0) { /* a random placed particle */
						  g_image.setColor(Color.ORANGE);
						} else { /* an improved particle */
							if(gda_olddata != null) {
								if(gda_olddata[0][i] != gda_data[0][i]|| gda_olddata[1][i] != gda_data[1][i]) {
									g_image.setColor(Color.gray);
									
									double xoldoff =  Math.round( ( (gi_width  / 2) * gda_olddata[0][i] ) / Math.abs(gd_vmax) ) ;
									double yoldoff =  Math.round( ( (gi_height / 2) * gda_olddata[1][i] ) / Math.abs(gd_vmin) ) ;
												    
									int gi_xoldoff = ((int)gi_width / 2) +  (int) xoldoff;  
									int gi_yoldoff = ((int)gi_height / 2) - (int) yoldoff;  
									g_image.fillOval(gi_xoldoff, gi_yoldoff , 4, 4);
									g_image.drawLine(gi_xoldoff+2, gi_yoldoff+2, gi_xoff+2, gi_yoff+2);
								}
							}
							g_image.setColor(Color.GREEN);
						}
					} else {	
						  g_image.setColor(Color.BLACK);
					}
					g_image.fillOval(gi_xoff, gi_yoff , 4, 4);
				}
			}
			
			bufferedImage.flush();
	        
	    }
	   
	   
	   public void update( Graphics g )
	   {
	    paint(g);//super.update(g);
	   }
	   
	   public void setCenter(double[] ad_center) {
		   gda_center = ad_center;
		   repaint();
	   }
	   
	   public void setRange(double ad_min, double ad_max) {
		   gd_vmin = ad_min;
		   gd_vmax = ad_max;
	   }
	   
	   public void resized(int k ) {
	   
	   }
  
  
} 