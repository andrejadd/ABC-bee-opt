import pso

from Tkinter import *        # The Tk package
import Pmw                   # The Python MegaWidget package
import math                  # import the sin-function
import time

from displayGraph import *
from paramWindow import *
from psoParameter import *

step = 0

DIVERSITY = False
SHOWSWAPS = False

UPDATESTEP = 1

COLORMAP = ['coral', 'blue', 'green3', 'orange', 'navy', 'magenta3', 'cyan', 'deeppink', 'forestgreen', 'orangered', 'olivedrab', 'peru', 'purple', 'red4', 'steelblue', 'springgreen', 'turquoise4', 'lightslategrey']
#SWARM_DISPLAY = ["SEQ-SWARM", "PYRAMID-SWARM", "RING-SWARM", "LOCAL-SWARM", "SCOUT-SWARM", "DYN-PYRAMIDSWARM", "ADAPTIVE-HPSO"]
#FUNCTION_DISPLAY = ["Sphere", "Rastrigin", "Rosenbrock", "Schaffer","Griewank", "Ackley", "Schwefel", "Levy No.5", \
#	"Freudenstein", "Quadric", "MovingGoal", "Dynamic Rosenbrock", "DynamicRastrigin", "DynamicGriewank", "MovingPeaks"]
SWARM_DISPLAY = SWARM
FUNCTION_DISPLAY = FUNCTION

#the 2D output is colored either according to 'level' of the particles or to the 'subtree' they belong to
POSITION_DISPLAY = "subtree"

#command line output of current opimization progress
NO_OUTPUT = False

#restrict yvalues to max_y
#useful for Rosenbrock with huge function Values
MAX_Y = 4000

#SWARMTYPE = pso.pyramidSwarm  
SWARMTYPE = pso.seqSwarm #localSwarm #seqSwarm   
OPTFUNCTION = pso.rastrigin
DIM = 30
#enter proper size for pyramid swarm, otherwise the display gets confused
#the size of the pso or variant swarm (without possible scouts)
SWARMSIZE = 30
#additional random walking scouts
NR_SCOUTS = 0
#the total number of individuals for display
SIZE = SWARMSIZE + NR_SCOUTS


#height and branches ignored for seqSwarm
HEIGHT = 4  # or ringlength (ringSwarm) or nrScouts (scoutswarm)
BRANCHES = 1 # or nodesize (ringSwarm), neighbourhood size (localSwarm), nrWorkers (scoutswarm)
SWAPDELAY = 1

GOAL=100
W = 0.729 #0.4 for V-HPSO, 0.729 (std)
WMIN = 0.729 # is w0, for the top level 0.4 for A-HPSO
#W = 0.6 #0.4 for V-HPSO, 0.729 (std)
#WMIN = 0.6 # is w0, for the top level 0.4 for A-HPSO

C1 = 1.494 #1.494 (std)
#C1 = 1.7 #1.494 (std)
C2 = C1
VMAXRATIO = 0.0 # 0.0 novmax, >0 vmax is proportion of xmax
CONSTRICTION = 1.0 # 1.0 means no constriction

MAXSTEP = 0

#adaptive parameters
DECREASEBRANCHFREQ=20
MINBRANCHDEGREE=2
DECREASEBRANCHSTEP=4
DECREASEBRANCHACTION=pso.bestSubtreeNoAction

#noise parameters
NOISESTYLE = pso.noNoise #noisyFunctionValue #noisyPosition 
NOISESIGMA = 0.1

#dynamic parameters
MOVEFREQUENCY = 100
MOVEDISTANCE = 5
OPTIMUMMOVESTYLE = pso.exponentialLinearOptMove 
UPDATESTYLE = pso.updatePeriodic #updateRandom 
#DETECTIONMETHOD = pso.hierarchyChangeDetect  #hierarchyChangeDetect #noDetect 
DETECTIONMETHOD = pso.gbestDetectChange #hierarchyChangeDetect  #hierarchyChangeDetect #noDetect 
#DETECTIONMETHOD = pso.noDetect #hierarchyChangeDetect  #hierarchyChangeDetect #noDetect 
#RESPONSEMETHOD = pso.randomize16reset  #subSwarmsAdaptiveMerge #noResponse  #subSwarmsAdaptiveMerge  
#RESPONSEMETHOD = pso.subSwarmsAdaptiveMerge #noResponse  #subSwarmsAdaptiveMerge  
RESPONSEMETHOD = pso.noResponse #subSwarmsAdaptiveMerge #noResponse  #subSwarmsAdaptiveMerge  

#noise parameters
NRSWAPCHECKS = 1
REUSEOLDVALUE = pso.reuseLastBestValue
REFINEBESTVALUE = True
FIRSTCOMPAREPBEST = False
HIERARCHYCHANGE = 5

p = None
psoParam = PsoParameter3()

vel = None
vector_x = []
vector_y = []
vector2_x = []
vector2_y = []
vector3_x = []
vector3_y = []
vector_x_level = []
vector_y_level = []
vector_x_pos = []
vector_y_pos = []
vector_x_div = []
vector_y_div = []
g_pos = None
g2 = None
g3 = None

stop = False

sleepScale = None
globalBestLabel = None

paramWindow = None


def initParameter():
	psoParam.set(W, WMIN, C1, C2, VMAXRATIO, CONSTRICTION, SWARMSIZE, DIM, HEIGHT,\
		BRANCHES, SWAPDELAY, OPTFUNCTION, NOISESTYLE, NOISESIGMA, SWARMTYPE, \
		NR_SCOUTS, MAXSTEP, MOVEFREQUENCY, MOVEDISTANCE, \
		OPTIMUMMOVESTYLE, UPDATESTYLE, DETECTIONMETHOD, RESPONSEMETHOD, \
		NRSWAPCHECKS, REUSEOLDVALUE, REFINEBESTVALUE, FIRSTCOMPAREPBEST, HIERARCHYCHANGE,
		DECREASEBRANCHFREQ, MINBRANCHDEGREE, DECREASEBRANCHSTEP, DECREASEBRANCHACTION)


def openParamWindow():
	global paramWindow
	
	#if (paramWindow is None):
	paramWindow = ParameterWindow(master, p)
	
def stopExecution():
    global stop
    stop = True

def newPSO(init=False):
    global p
    global step
    global offlineSum

    offlineSum = 0

    step = 0

    if not (p is None):
        del p
    #p = pso.PSO(W, WMIN, C1, C2, VMAXRATIO, CONSTRICTION, SWARMSIZE, DIM, HEIGHT,
    #        BRANCHES, SWAPDELAY, OPTFUNCTION, SWARMTYPE, NR_SCOUTS, MAXSTEP)
    
    p = pso.PSO(psoParam.w[0], psoParam.wMin[0], psoParam.c1[0], psoParam.c2[0], psoParam.vmaxRatio[0], \
    	psoParam.constriction[0], psoParam.swarmsize[0], psoParam.dim[0], psoParam.height[0], \
    	psoParam.branches[0], psoParam.swapDelay[0], psoParam.function_type[0], \
    	psoParam.noiseStyle[0], psoParam.noiseSigma[0],\
    	psoParam.swarm_type[0], psoParam.nrScouts[0], psoParam.maxStep[0])

    	


    p.setDynamicParameters(psoParam.moveFrequency[0], psoParam.moveDistance[0], \
    	psoParam.optimumMoveStyle[0], psoParam.updateStyle[0], psoParam.detectionMethod[0], \
    	psoParam.responseMethod[0])
    p.setNoiseParameters(psoParam.nrNoisySwapChecks[0], psoParam.noisyReuseOldValue[0], \
		psoParam.noisyRefineBestValue[0], psoParam.noisyFirstComparePbest[0], \
		psoParam.hierachyChangeDetectThreshold[0])
    p.setAdaptiveParameters(psoParam.decreaseBranchFrequency[0], psoParam.minBranchDegree[0],\
    	psoParam.decreaseBranchStep[0], psoParam.decreaseBranchAction[0])
    		
	


    if (init==False):
        clearOpt()
        clearLevel()
        levelGraph.reset()
        clearVel()
        clearIndividualBest()
#        master.update_idletasks()   # update screen
        
    

def clearOpt():
	global vector_x, vector_y
	ncurves = len(vector_y)
	del vector_x[0:len(vector_x)]
	
	for i in range(ncurves):
	   del vector_y[i][0:len(vector_y[i])]
	
def clearLevel():
	global vector_x_level, vector_y_level

	del levelGraph.vector_x[0:len(vector_x_level)]

	ncurves = SWARMSIZE
#	del vector_x[0:len(vector_x_level)]
	
	for i in range(ncurves):
	   #del vector_y_level[i][0:len(vector_y_level[i])]
	   del levelGraph.vector_y[i][0:len(vector_y_level[i])]

	#print len(levelGraph.vector_x)
	#print len(levelGraph.vector_y)

def clearVel():
	global vector3_x, vector3_y
	
	npoints = SIZE
	for i in range(npoints):
	   vector3_y[0][i]=1
	   vector3_x[i]= i

def clearIndividualBest():    
	global vector2_x, vector2_y
	npoints = SIZE
	
	for i in range(npoints):
	   vector2_y[0][i]=0.5
	   vector2_y[1][i]=1
	   vector2_x[i]=i


def updateIndividualBest():
    for i in range(SIZE):
        vector2_y[0][i] = p.getBestValue(i)
        vector2_y[1][i] = p.getCurrValue(i)

def updateVel():
    for i in range(SIZE):
        vector3_y[0][i] = p.getVel(i)

def updateLevel_old(step):
    OFFSET = [0]+[0.2]*(HEIGHT-1)
    vector_x_level.append(step)
    for i in range(SWARMSIZE):
        level = p.getLevel(i)
        vector_y_level[i].append(level + OFFSET[level])
        OFFSET[level] -= 0.4 / (BRANCHES ** level)

def updateLevel(step):
	HEIGHT = p.getActualHeight()
	OFFSET = [0]+[0.2]*(HEIGHT)
    
	vector_x_level.append(step)
	for i in range(SWARMSIZE):
		level = p.getLevel(i)
		vector_y_level[i].append(level + OFFSET[level])
		OFFSET[level] -= 0.4 / (BRANCHES ** level)
	levelGraph.set(vector_x_level, vector_y_level)
	levelGraph.master.after(20)            # wait 0.02 second
    
#    levelGraph.master.update_idletasks()

def updateDiversity(step):
    vector_x_div.append(step)
#    vector_y_div[0].append(p.getAverageDiversity())
#    vector_y_div[0].append(p.getMedianDiversity())
    if (SWARMTYPE == pso.pyramidSwarm):
        for b in range(BRANCHES+1):
#            vector_y_div[BRANCHES-b].append(p.getAverageDiversitySubtree(b))
            vector_y_div[b].append(p.getAverageDiversitySubtree(b))
#            vector_y_div[b+1].append(p.getMedianDiversitySubtree(b))
    divGraph.set(vector_x_div, vector_y_div)


        
def updateOptimization(step):
    vector_x.append(step)        # for each curve...
    vector_y[0].append(p.stat.getGlobalBest(step))   # make an y-value
    vector_y[1].append(p.stat.getIterationBest(step))   # make an y-value
    vector_y[2].append(p.stat.getAverage(step))   # make an y-value
    vector_y[3].append(p.stat.getActualGlobalBest(step))   # make an y-value
    
    if (OPTFUNCTION == pso.moving_peaks):
        vector_y[4].append(p.getCurrentError())

def posPostscript():
    g_pos.postscript_configure(
          center = 0,                    # place plot in upper left corner
          colormode="grayscale",              # grayscale rather than colors
          decorations=0)
    g_pos.postscript_output("graph.ps")

def updatePos():
    global posDisplayZoomScale
    
    if (POSITION_DISPLAY == "level"):
        #display levels
        LEVELCOLOR = ["BLUE", "RED", "ORANGE", "YELLOW", "cyan", "grey"]
        for i in range(SIZE):
            vector_x_pos[i][0] = p.getPos(i,0)
            vector_x_pos[i][1] = p.getBestPos(i,0)
            vector_y_pos[i][0] = p.getPos(i,1)
            vector_y_pos[i][1] = p.getBestPos(i,1)

        vector_x_pos[SIZE][0] = p.getOptPosition(0)
        vector_x_pos[SIZE][1] = p.getOptPosition(0)
        vector_y_pos[SIZE][0] = p.getOptPosition(1)
        vector_y_pos[SIZE][1] = p.getOptPosition(1)

        i = 0
        #g_pos.element_show() does not return sorted curves, therefore we use str(range())
        for curvename in map(str, range(SIZE)): 
            g_pos.element_configure(curvename, color=LEVELCOLOR[p.getLevel(i) + 1])
            i += 1
    else:
        #display subtrees
        LEVELCOLOR = ['black', 'blue', 'red', 'green3', 'orange',  'magenta3', 'cyan', 'deeppink', 'forestgreen', 'navy','orangered', 'olivedrab', 'peru', 'purple', 'red4', 'steelblue', 'springgreen', 'turquoise4', 'lightslategrey']
		#LEVELCOLOR = ["BLACK", "RED", "BLUE", "GREEN", "YELLOW", "cyan", "grey"]
		
		
        for i in range(SIZE):
#            vector_x_pos[i][0] = p.getBestPos(i,0)
#            vector_y_pos[i][0] = p.getBestPos(i,1)
            vector_x_pos[i][0] = p.getPos(i,0)
            vector_y_pos[i][0] = p.getPos(i,1)
        

        i = 0
        #g_pos.element_show() does not return sorted curves, therefore we use str(range())
        for curvename in map(str, range(SIZE)): 
            g_pos.element_configure(curvename, color=LEVELCOLOR[p.getSubtree(i)])
            i += 1

    zoom = 1000* (1.0001 - posDisplayZoomScale.get())** 4
    g_pos.axis_configure("x", min = -zoom, max = zoom)
    g_pos.axis_configure("y", min = -zoom, max = zoom)



        
def animate(repeat=True):
    global step, stop, offlineSum
    run = True
    while(run):
        p.step()
        
        updateOptimization(step)
        updateIndividualBest()
        if ((SWARMTYPE == pso.pyramidSwarm) | (SWARMTYPE == pso.dynamicPyramidSwarm)| (SWARMTYPE == pso.adaptivePyramidSwarm)):
        	if (SHOWSWAPS):
	            updateLevel(step)
        updateVel()
        if (DIM == 2):
            updatePos()

        if (DIVERSITY):
            updateDiversity(step)

        time.sleep(sleepScale.get())
        globalBestLabel.configure(text=str(p.stat.getGlobalBest(step)))
        offlineSum += p.stat.getActualGlobalBest(step)

#        if ((step % 100 == 0) or (step % 100 == 99)):
#        	print step, p.getDistanceToGoal()

        if (step % UPDATESTEP == 0):
            master.update()   # update screen
        
        if not (NO_OUTPUT):
	        if (not (p.stat.getGoalReachedStep() == 0)) and (p.stat.getGoalReachedStep() == step):
	            print "GOAL reached at step", step
        
        step = step+1
        if (stop == True):
            print "Step", step
            print "Offline Performance", offlineSum/step
            print "Best Pos (",
            for i in range(DIM):
            	print str(p.getGlobalBestPos(i)) + ", ",
            print
            print "Nr Evaluations", p.stat.getNrEvaluations(step-1)
            stop = False
            break

        if (repeat == False):
            run = False
        
    if not (NO_OUTPUT):
        print "Step",step, "best", p.stat.getGlobalBest(step-1)
        
    globalBestLabel.configure(text=str(p.stat.getGlobalBest(step-1)))
    if not (NO_OUTPUT):
        if ((SWARMTYPE == pso.pyramidSwarm) | (SWARMTYPE == pso.dynamicPyramidSwarm)| (SWARMTYPE == pso.adaptivePyramidSwarm)):
            print "NumberSwapped", p.stat.getNrSwapped(step-1)
            for i in range(HEIGHT):
                print "on Level",i, p.stat.getNrSwappedLevel(step-1,i)
                
	



def untilGoal():
    global step, stop
    while (stop==False) and ((step==0) or (p.stat.getGlobalBest(step-1)>GOAL)):
        p.step()

        updateOptimization(step)
        updateIndividualBest()
        if ((SWARMTYPE == pso.pyramidSwarm) | (SWARMTYPE == pso.dynamicPyramidSwarm)| (SWARMTYPE == pso.dynamicPyramidSwarm)):
        	if (SHOWSWAPS):
	            updateLevel(step)
        updateVel()

        if (DIM == 2):
            updatePos()

        if (DIVERSITY):
            if (step%5 == 0):
                updateDiversity(step)


        time.sleep(sleepScale.get())

        globalBestLabel.configure(text=str(p.stat.getGlobalBest(step-1)))
        
        if (step % UPDATESTEP == 0):
            master.update()

        step = step+1
        if not (NO_OUTPUT):
	        if (step%100 == 0) and (step >0):
	            print "Step",step, "best", p.stat.getGlobalBest(step-1)
	            if ((SWARMTYPE == pso.pyramidSwarm) | (SWARMTYPE == pso.dynamicPyramidSwarm)| (SWARMTYPE == pso.dynamicPyramidSwarm)):
	                print "NumberSwapped", p.stat.getNrSwapped(step-1)
	                for i in range(HEIGHT):
	                    print "on Level",i, p.stat.getNrSwappedLevel(step-1,i)
	
	        if (not (p.stat.getGoalReachedStep() == 0)) and (p.stat.getGoalReachedStep() == step):
	            print "GOAL reached at step", step


    if (stop==True):
        stop = False
        
    print "Step",step, "best", p.stat.getGlobalBest(step-1)
    globalBestLabel.configure(text=str(p.stat.getGlobalBest(step-1)))
 
    if ((SWARMTYPE == pso.pyramidSwarm) | (SWARMTYPE == pso.dynamicPyramidSwarm)| (SWARMTYPE == pso.dynamicPyramidSwarm)):
        print "NumberSwapped", p.stat.getNrSwapped(step-1)
        for i in range(HEIGHT-1):
            print "on Level",i, p.stat.getNrSwappedLevel(step-1,i)


def maxY():
	global currY
	if not ('currY' in globals()):
		currY = 0


	if (currY == 0):
		g.axis_configure("y", min = 0, max = MAX_Y)
		currY = MAX_Y
	elif (currY == MAX_Y):
		g.axis_configure("y", min = 0, max = MAX_Y/2)
		currY = MAX_Y/2
	elif (currY == MAX_Y/2):
		g.axis_configure("y", min = 0, max = MAX_Y/4)
		currY = MAX_Y/4
	elif (currY == MAX_Y/4):
		g.axis_configure("y", min = 0, max = MAX_Y/8)
		currY = MAX_Y/8
	else:
		g.axis_configure("y", min = 0, max = "")
		currY = 0
		

			


def symbolsOnOff():
    global symbols
    symbols = not symbols
    for curvename in g.element_show():
        if symbols:
            g.element_configure(curvename, symbol='diamond')
        else:
            g.element_configure(curvename, symbol='')
            
def smooth():
    global smoothing
    
    if smoothing == 'linear': smoothing='quadratic'
    elif smoothing == 'quadratic': smoothing='natural'
    elif smoothing == 'natural': smoothing='step'
    else: smoothing = 'linear'
    for curvename in g.element_show():
        g.element_configure(curvename, smooth=smoothing)
        

	

def initIndividualBest(top):
	global vector2_x, vector2_y
	
	
	
	vector2_x = Pmw.Blt.Vector()   
	vector2_y = []
	ncurves = 2
	npoints = SIZE
	
	for y in range(ncurves):
	   vector2_y.append(Pmw.Blt.Vector())          # make vector for y-axis
	
	for n in range(npoints):
	   vector2_y[0].append(0.5);
	   vector2_y[1].append(1);
	   vector2_x.append(n);
	   
	
	g2 = Pmw.Blt.Graph(top)                     # make a new graph area
	g2.pack(expand=1, fill='both')
	color = ['red', 'blue', '#00cc00', 'black', 'grey']
	name = ['best','curr']
	for c in range(ncurves):                      # for each curve...
	   curvename = name[c]          # make a curvename
	   g2.bar_create(curvename,                   # and create the graph
	                 xdata=vector2_x,              # with x data,
	                 ydata=vector2_y[c],           # and  y data
	                 fg=color[c],              # and a color
	                 barwidth=0.2)                 # and 2 pixels wide
    
        g2.configure(barmode="aligned")

	g2.configure(title="Best found so far")          # enter a title
	
	

def initVel(top):
	global vector3_x, vector3_y
	
	vector3_x = Pmw.Blt.Vector()   
	vector3_y = []
	ncurves = 1
	npoints = SIZE
	
	for y in range(ncurves):
	   vector3_y.append(Pmw.Blt.Vector())          # make vector for y-axis
	
	for n in range(npoints):
	   vector3_y[0].append(1);
	   vector3_x.append(n);
	   
	
	g2 = Pmw.Blt.Graph(top)                     # make a new graph area
	g2.pack(expand=1, fill='both')
	color = ['red', 'blue', '#00cc00', 'black', 'grey']
	for c in range(ncurves):                      # for each curve...
	   curvename = 'sin(' + str(c) +'x)'          # make a curvename
	   g2.bar_create(curvename,                   # and create the graph
	                 xdata=vector3_x,              # with x data,
	                 ydata=vector3_y[c],           # and  y data
	                 fg='red',              # and a color
	                 barwidth=0.1)                 # and 2 pixels wide
	   
	g2.configure(title="Velocity")          # enter a title

def initPos(window):
    global vector_x_pos, vector_y_pos
    global g_pos
    global posDisplayZoomScale
    
    if (POSITION_DISPLAY == "level"):
        ncurves = SIZE + 1                 # each particle and the mean position is drawn
    else:
        ncurves = SIZE

    smoothing='linear'
    symbols  = 0
    # In this example we use Pmw.Blt.Vectors. These can mostly be used like 
    # a normal list, but changes will be updated in the graph automatically.
    # Using Pmw.Blt.Vectors is often slower, but in this case very convenient.
    vector_x_pos = []
    vector_y_pos = []
    for i in range(ncurves):
        vector_x_pos.append(Pmw.Blt.Vector())          # a vector for each x value
        vector_x_pos[i].append(i)
        vector_x_pos[i].append(i)
        vector_y_pos.append(Pmw.Blt.Vector())          # make vector for y-axis
        vector_y_pos[i].append(0)
        if (POSITION_DISPLAY == "level"):
            vector_y_pos[i].append(0)
	
    g_pos = Pmw.Blt.Graph(window)                     # make a new graph area
    g_pos.pack(expand=1, fill='both')

    names = ['globalBest','itrBest','average']
    for c in range(ncurves):
        if ((POSITION_DISPLAY == "level") and (c == ncurves-1)): #last element
            curvename = "opt"
        else:                      # for each curve...
            curvename = str(c)          # make a curvename
        g_pos.line_create(curvename,                   # and create the graph
                      xdata=vector_x_pos[c],              # with x data,
                      ydata=vector_y_pos[c],           # and  y data
                      dashes=0,                    # and no dashed line
                      linewidth=1,                 # and 2 pixels wide
                      outlinewidth=0,
                      pixels="0.06i",
                      symbol='circle')                   # ...and no disks
	   
	g_pos.configure(title=SWARM_DISPLAY[SWARMTYPE] + " " + 
	    FUNCTION_DISPLAY[OPTFUNCTION] + ", dim="+str(DIM)+" size="+str(SIZE))          # enter a title

    g_pos.axis_configure("x", min = -10, max = 10)
    g_pos.axis_configure("y", min = -10, max = 10)
    
    if (POSITION_DISPLAY == "level"):
        g_pos.element_configure("opt", color='black')

    posDisplayZoomScale = Scale(window, from_=0, to=1, resolution=0.01, orient='horizontal')
    posDisplayZoomScale.pack(expand=1, fill='x')
    b = Button(window, text='Postscript',       command=posPostscript)
    b.pack()



def initLevel():
    global vector_x_level, vector_y_level
    global levelGraph
    vector_x_level = Pmw.Blt.Vector()   
    vector_y_level = []
    ncurves = SWARMSIZE
    curves = []
    for y in range(ncurves):
        vector_y_level.append(Pmw.Blt.Vector())          # make vector for y-axis
        curves.append(y)
        
    levelGraph = GraphDisplay(master, vector_x_level, vector_y_level, curves,"title")
    levelGraph.configureAxisDescending("y",1)

def initDiversity():
    global vector_x_div, vector_y_div
    global divGraph
    vector_x_div = Pmw.Blt.Vector()   
    vector_y_div = []
    if ((SWARMTYPE == pso.pyramidSwarm) | (SWARMTYPE == pso.dynamicPyramidSwarm)):
        print "pyramid", BRANCHES
        ncurves = BRANCHES + 1
    else:
        print "nopyramid"
        ncurves = 2
    curves = range(ncurves)
    curvenames = []


    for y in range(ncurves):
		vector_y_div.append(Pmw.Blt.Vector())          # make vector for y-axis
		if (y == ncurves-1):
			curvenames.append("Random")			
		else:
			curvenames.append("Subtree " + str(y))
		


        
    divGraph = GraphDisplay(master, vector_x_div, vector_y_div, curves,"Diversity", curvenames)

def initOpt(window):
	global vector_x, vector_y
	global g
	
	ncurves = 4                  # draw 4 curves
	if (OPTFUNCTION == pso.moving_peaks):
	    ncurves += 1
	npoints = 20                  # use  8 points on each curve
	smoothing='linear'
	symbols  = 0
	# In this example we use Pmw.Blt.Vectors. These can mostly be used like 
	# a normal list, but changes will be updated in the graph automatically.
	# Using Pmw.Blt.Vectors is often slower, but in this case very convenient.
	vector_x = Pmw.Blt.Vector()   
	vector_y = []
	for y in range(ncurves):
	   vector_y.append(Pmw.Blt.Vector())          # make vector for y-axis
	
	g = Pmw.Blt.Graph(window)                     # make a new graph area
	g.pack(expand=1, fill='both')
	color = ['red', '#ff9900', 'blue', '#00cc00', 'black', 'grey']
	names = ['globalBest','itrBest','average', 'actualBest', 'offlinePerf']
	for c in range(ncurves):                      # for each curve...
	   curvename = names[c]          # make a curvename
	   g.line_create(curvename,                   # and create the graph
	                 xdata=vector_x,              # with x data,
	                 ydata=vector_y[c],           # and  y data
	                 color=color[c],              # and a color
	                 dashes=0,                    # and no dashed line
	                 linewidth=2,                 # and 2 pixels wide
	                 symbol='')                   # ...and no disks
	
	titleText = psoParam.guiString()
	
	#if (p.getDynamicStyle() == pso.dynamicFct):
	
	#g.configure(title=SWARM_DISPLAY[SWARMTYPE] + " " + 
	#    FUNCTION_DISPLAY[OPTFUNCTION] + ", dim="+str(DIM)+" size="+str(SWARMSIZE))          # enter a title
	g.configure(title=titleText)
	
	
	g.grid_on()

initParameter()
newPSO(True)        
        
master = Tk()                # build Tk-environment

top = Toplevel()
top2 = Toplevel()

if (DIM ==2):
    positionDisplay = Toplevel()
    initPos(positionDisplay)

initIndividualBest(top)
initVel(top2)
if (SHOWSWAPS):
	initLevel()
if (DIVERSITY):
    initDiversity()

initOpt(master)

globalBestLabel = Label(master, text="0", relief="sunken", width=30, justify="left")
globalBestLabel.pack()

sleepScale = Scale(master, from_=0, to=1, resolution=0.01, orient='horizontal')
sleepScale.pack(expand=0, fill='x')

# make s row of buttons
buttons = Pmw.ButtonBox(master, labelpos='n', label_text='Options')
buttons.pack(fill='x', expand=0, padx=10, pady=10)
buttons.add('Parameter', command=openParamWindow)
buttons.add('New',       command=newPSO)
#buttons.add('Grid',       command=g.grid_toggle)
buttons.add('Symbols',    command=symbolsOnOff)
buttons.add('Smooth',     command=smooth)
buttons.add('MAX-Y',  command=maxY)
buttons.add('Goal',    	  command=untilGoal)
buttons.add('Animate',    command=animate)
buttons.add('Stop',    command=stopExecution)
buttons.add('Quit',       command=master.destroy)


master.mainloop()                              # ...and wait for input


