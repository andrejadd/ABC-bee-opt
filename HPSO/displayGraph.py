from Tkinter import *
import Pmw

import cPickle
from string import capitalize

#COLORMAP = ['black',  'grey50','grey70', 'grey75', 'grey80',  'gray50', 'cyan', 'deeppink', 'forestgreen', 'navy','orangered', 'olivedrab', 'peru', 'purple', 'red4', 'steelblue', 'springgreen', 'turquoise4', 'lightslategrey']
#COLORMAP = ['grey60', 'grey70', 'grey80']
COLORMAP = ['blue', 'red', 'green3', 'orange',  'magenta3', 'cyan', 'deeppink', 'forestgreen', 'navy','orangered', 'olivedrab', 'peru', 'purple', 'red4', 'steelblue', 'springgreen', 'turquoise4', 'lightslategrey']

SMOOTHDESC = {'linear' : "A single line segment is drawn, connecting both data points",
              'step' : "Two line segments are drawn.\nThe first is a horizontal line segment that steps the next X-coordinate.\nThe second is a vertical line, moving to the next Y-coordinate.",
              'natural' : "Multiple segments between data points are generated using a cubic spline.",
              'quadratic' : "Multiple segments between data points are generated using a quadratic spline."}

#['red', '#ff9900', 'blue', '#00cc00', 'black', 'grey','green','yellow','#0000cc','#cc0000','#0099ff', '#99ff00']


class GraphDisplay:
    dragging = 0
    x0 = 0
    y0 = 0
    x1 = 0
    y1 = 0


    g = None
    master = None
    graphwin = None
    vector_x = []
    vector_y = []

    smoothing='linear'
    dashes=0
    symbols=0
    linewidth = 1

    activeLegends = {}
    minX = 0
	
    def shiftX(self):
		if (self.minX + 100 < len(self.vector_x)):
			self.minX += 100
			self.g.xaxis_configure(min=self.minX)
		else:
			self.minX = 0		
			self.g.xaxis_configure(min="")



    def reset(self):
		#clear graph area and reset axis
        self.g.xaxis_configure(min="", max="")
        self.g.yaxis_configure(min="", max="")
    	pass


    def dashesOnOff(self):
        self.dashes = not self.dashes
        i=len(self.g.element_show())-1
        for curvename in self.g.element_show():
            if self.dashes:
                self.g.element_configure(curvename, dashes=i)
                i -= 1
            else:
                self.g.element_configure(curvename, dashes=0)

    def changeLinewidth(self):
        #choose linewidth from (1,2,3)
        self.linewidth = ((self.linewidth)%3)+1
        for curvename in self.g.element_show():
            self.g.element_configure(curvename, linewidth=self.linewidth)



    def smooth(self):
        if self.smoothing == 'linear': self.smoothing='step'
        elif self.smoothing == 'step': self.smoothing='natural'
        elif self.smoothing == 'natural': self.smoothing='quadratic'
        else: self.smoothing = 'linear'
        for curvename in self.g.element_show():
            self.g.element_configure(curvename, smooth=self.smoothing)
        self.smoothbutton.configure(text = capitalize(self.smoothing))
        self.balloon.bind(self.smoothbutton, SMOOTHDESC[self.smoothing])

    def symbolsOnOff(self):
        self.symbols = not self.symbols
#        for curvename in self.g.element_show():
        curvename = "Setting 0"
        if self.symbols:
            self.g.element_configure(curvename, symbol='circle')
            self.g.element_configure(curvename, linewidth=0)
            self.g.element_configure(curvename, pixels=1.5)
        else:
            self.g.element_configure(curvename, symbol='')


    def showSnap(self):
        img = PhotoImage(name="image", master=self.graphwin)
        self.g.snap(img)           # take snapshot
        img.write("graph.gif")  # and save it to file.
        print "Snapshot taken an written to file: graph.gif"

    def postscript(self):
        #pheight = g.postscript_cget("paperheight")
        #print "Default paperheight: ", pheight
        ######################## postscript_configure #########################
        #all = g.postscript_configure()

        #print "All available options: ",
        #for option in all.keys():
        #   print option,

        # alter the default postscript settings:
        
        self.g.axis_configure("x", limitsfont="{Arial Narrow} 12")
        self.g.axis_configure("y", limitsfont="{Arial Narrow} 12")
        self.g.legend_configure(font="Arial 12")
        #namelist = ["Random", "Subtree 1", "Subtree 2", "Subtree 3"]
#        for curve in range(4):
#            self.g.element_configure("Setting "+str(curve), label=namelist[curve])
        
        self.g.postscript_configure(
          center = 0,                    # place plot in upper left corner
          colormode="gray",              # grayscale rather than colors
          decorations=0)                 # and no decorations.

        #ps = g.postscript_output()        # get the ps-code
        #print ps                          # print it to screen

        self.g.postscript_output("graph.ps")     # ..or more naturally, save it to file.
        print "wrote file graph.ps"
        

    def zoom(self, x0, y0, x1, y1):
        #store current setting before zooming
        self.oldZoom.append((self.g.xaxis_limits()[0], self.g.yaxis_limits()[0], self.g.xaxis_limits()[1], self.g.yaxis_limits()[1]))
        
        self.g.xaxis_configure(min=x0, max=x1)
        self.g.yaxis_configure(min=y0, max=y1)

    def zoomOut(self, event):
        if (len(self.oldZoom) > 0):
            #zoomed once at least
            if (len(self.oldZoom) > 1):
                #restore last zoom
                rect = self.oldZoom.pop()
            elif (len(self.oldZoom) == 1):
                #take first zoom
                #rect = self.oldZoom[0]
				rect = ['']*4

            x0_old = rect[0]
            y0_old = rect[1]
            x1_old = rect[2]
            y1_old = rect[3]

            self.g.xaxis_configure(min=x0_old, max=x1_old)
            self.g.yaxis_configure(min=y0_old, max=y1_old)
        else:
            #not zoomed yet, nothing to zoom out
            pass

    def mouseDrag(self, event):
        (self.x1, self.y1) = self.g.invtransform(event.x, event.y)

        self.g.marker_configure("marking rectangle", 
            coords = (self.x0, self.y0, self.x1, self.y0, self.x1, self.y1, self.x0, self.y1, self.x0, self.y0))

        #pass the event on to printPos
        self.printPos(event)

    def mouseUp(self, event):
        if self.dragging:
            self.g.unbind(sequence="<Motion>")
            self.g.marker_delete("marking rectangle")

            if self.x0 <> self.x1 and self.y0 <> self.y1:

                # make sure the coordinates are sorted
                if self.x0 > self.x1: self.x0, self.x1 = self.x1, self.x0
                if self.y0 > self.y1: self.y0, self.y1 = self.y1, self.y0

                if event.num == 1:
                    self.zoom(self.x0, self.y0, self.x1, self.y1) # zoom in
                else:
                    (X0, X1) = self.g.xaxis_limits()
                    k  = (X1-X0)/(self.x1-self.x0)
                    self.x0 = X0 -(self.x0-X0)*k
                    self.x1 = X1 +(X1-self.x1)*k

                    (Y0, Y1) = self.g.yaxis_limits()
                    k  = (Y1-Y0)/(self.y1-self.y0)
                    self.y0 = Y0 -(self.y0-Y0)*k
                    self.y1 = Y1 +(Y1-self.y1)*k

                    print "zoom out"
                    self.zoom(self.x0, self.y0, self.x1, self.y1) # zoom out
                    
        #rebind motion to statusbar
        self.g.bind(sequence="<Motion>",   func=self.printPos)


    def mouseDown(self, event):
        self.dragging = 0

        if self.g.inside(event.x, event.y):
            self.dragging = 1
            (self.x0, self.y0) = self.g.invtransform(event.x, event.y)

            self.g.marker_create("line", name="marking rectangle", dashes=(2, 2))
            self.g.bind(sequence="<Motion>",  func=self.mouseDrag)

    def printPos(self, event):
        pos = self.g.invtransform(event.x, event.y)
        x = str(round(pos[0],4))
        y = str(round(pos[1],4))        
        
        self.messageBar.message('state',"("+x+", "+y+")")



    def legendMouseDown(self, event):
       
        pos = "@" +str(event.x) +"," +str(event.y)
        legend = self.g.legend_get(pos)    # get the selected legend.
        
        if self.activeLegends.has_key(legend):
            self.g.legend_deactivate(legend)      # deactivate the legend
            self.g.element_deactivate(legend)     # deactivate the data as well
            del self.activeLegends[legend]        # remove key from dictionary 
        else:
            self.g.pen_configure("activeLine", linewidth=self.linewidth*3, symbol="")
            #assuming a legend entry of form "Setting x" !!!
            #self.g.pen_configure("activeLine", color=COLORMAP[int(legend[8:])])
            self.g.legend_activate(legend)        # mark the new legend.
            self.g.element_activate(legend)       # mark the data as well
            self.activeLegends[legend] = 1        # set a mark in the dictionary

    def initLegend(self):
        self.g.pen_configure("activeLine", linewidth=self.linewidth*3, symbol="", color='red')
        self.g.legend_bind("all", "<ButtonPress>", self.legendMouseDown)
        self.g.legend_configure(anchor="ne",
                                activebackground="blue")


    def initStatusBar(self):

        # Create and pack the MessageBar.
        self.messageBar = Pmw.MessageBar(self.graphwin,
                entry_width = 60,
                entry_relief='groove',
                labelpos = 'w',
                label_text = 'Pos:')
        self.messageBar.pack(fill = 'x', expand = 0, padx = 10, pady = 5)
        self.messageBar.configure(entry_foreground = "black")



    def set(self, vector_x, vector_y):
        self.vector_x = vector_x
        self.vector_y = vector_y

    def configureAxisDescending(self, axis, descend):
        self.g.axis_configure(axis, descending=descend)

            
    def __init__(self, master, vector_x, vector_y, curves, title, curvenames=[]):
        self.master = master
        self.vector_x = vector_x
        self.vector_y = vector_y
        
        self.graphwin = Toplevel()        

        symbols  = 0


        self.g = Pmw.Blt.Graph(self.graphwin)                     # make a new graph area

        self.balloon = Pmw.Balloon(self.graphwin)


        self.g.pack(expand=1, fill='both')
        color = COLORMAP
        #	color = ['red', '#ff9900', 'blue', '#00cc00', 'black', 'grey']

        for c in range(len(curves)):
			# for each curve...
			if (curvenames == []):
				curvename = "Setting " + str(curves[c])          # make a curvename
			else:
				curvename = curvenames[c]
			self.g.line_create(curvename,                   # and create the graph
                          xdata=self.vector_x,              # with x data,
                          ydata=self.vector_y[c],           # and  y data
                          color=color[curves[c]%len(COLORMAP)],              # and a color
                          dashes=0,                    # and no dashed line
                          linewidth=self.linewidth,                 # and 2 pixels wide
                          symbol='',
                          pixels=2,
                          outlinewidth=1)                   # ...and no disks

        self.g.configure(title=title)          # enter a title

        self.g.bind(sequence="<ButtonPress-1>",   func=self.mouseDown)
        self.g.bind(sequence="<ButtonRelease-1>", func=self.mouseUp  )
        self.g.bind(sequence="<ButtonPress-3>",   func=self.zoomOut)
        self.g.bind(sequence="<Motion>",   func=self.printPos)

        self.initLegend()

        # make s row of buttons
        buttons = Pmw.ButtonBox(self.graphwin)
        buttons.pack(fill='both', expand=0, padx=10, pady=10)
        buttons.add('ShiftX', command=self.shiftX)
        buttons.add('Grid',       command=self.g.grid_toggle)
        buttons.add('Dashes',       command=self.dashesOnOff)
        buttons.add('Symbols',       command=self.symbolsOnOff)
        buttons.add('Linewidth',       command=self.changeLinewidth)
        self.smoothbutton = buttons.add(capitalize(self.smoothing),     command=self.smooth)
        self.balloon.bind(self.smoothbutton, SMOOTHDESC[self.smoothing])
        buttons.add('Snapshot',       command=self.showSnap)
        buttons.add('Postscript',       command=self.postscript)
        buttons.add('Quit',       command=self.graphwin.destroy)

        self.initStatusBar()


        self.oldZoom = []


        self.master.after(20)            # wait 0.02 second
        self.master.update_idletasks()

class showDescription:
    param = None
    descwin = None

    def __init__(self, master, filename):
        descwin = Toplevel()
        f = open(filename, 'r')
        self.param = cPickle.load(f)
        f.close()

	# Create the dialog.
	dialog = Pmw.TextDialog(master, scrolledtext_labelpos = 'n',
		title = 'batchParameter',
		defaultbutton = 0,
		label_text = filename)
	dialog.withdraw()
	dialog.insert('end', self.param.functionString())
	dialog.insert('end', self.param)
	dialog.configure(text_state = 'disabled')
        dialog.activate()

class editParameters:
    param = None
    paramwin = None
    desc = None
    observ = None
    filename = ""

    def save(self):
        f = open(self.filename, 'r')
        oldparam = cPickle.load(f)
        f.close()
        oldparam.description = self.desc.get(1.0, END)
        oldparam.observation = self.observ.get(1.0, END)
        f = open(self.filename, 'w')
        cPickle.dump(oldparam, f)
        f.close()
        print "wrote ", self.filename

    def __init__(self, master, filename):
        self.filename = filename
        self.paramwin = Toplevel()
        f = open(filename, 'r')
        self.param = cPickle.load(f)
        f.close()

        label = Label(self.paramwin, text="Description")
        label.pack()
        self.desc = Text(self.paramwin)
        self.desc.tag_config("WRAP", wrap=WORD)
        self.desc.insert(END,self.param.description, "WRAP")
        self.desc.pack()

        label = Label(self.paramwin, text="Observation")
        label.pack()
        self.observ = Text(self.paramwin)
        self.observ.tag_config("WRAP", wrap=WORD)
        self.observ.insert(END,self.param.observation, "WRAP")
        self.observ.pack()
        
        # make s row of buttons
        buttons = Pmw.ButtonBox(self.paramwin)
        buttons.pack(fill='both', expand=0, padx=10, pady=10)
        buttons.add('Save',       command=self.save)
        buttons.add('Close',       command=self.paramwin.destroy)

