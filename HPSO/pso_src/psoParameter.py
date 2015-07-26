import pso

##defines
SWARM = ["PSO", "HPSO", "RingSwarm", "LocalSwarm", "ScoutSwarm", "DHPSO", "AHPSO"]
FUNCTION = ["Sphere", "Rastrigin", "Rosenbrock", "Schaffer", "Griewank","Ackley", "Schwefel", "Levy No.5", \
	"Freudenstein",	"Quadric", "MovingGoal", "DynamicRosenbrock", "DynamicRastrigin", "DynamicGriewank", "MovingPeaks", \
	"Dynamic Levy", "Dynamic Ackley", "Dynamic Schwefel"]

DETECT = ["upToDate", "gbestDetect", "scoutDetect", "noDetect", "hierarchy", "omniscient"]
RESPONSE = ["noResponse", "applyScout", "rand10", "rand10reset", "rand16reset", "rand22reset",\
 "subSwarmsTemporal", "subSwarmsAdaptive", "subSwarmsTemporalMergeNoMemory", "subSwarmsAdaptiveMergeNoMemory",\
 "rand40reset", "updateOnly"]
 
NOISE = ["noNoise", "noisyVal", "noisyPos", "noisyRelVal"]
REUSEVAL = ["noReuse", "reuseContinuousAverage", "reuseLastBestValue"]


MOVE = ["random", "linear", "randomLinear", "randomDistance", "exponentialLinear"]
UPDATE = ["periodic", "onGoal", "random"]

ACTION = ["noAction", "reset"]

DISPLAYDIGITS = 3
##end defines

class PsoParameter3:
	#noisy functions added
	steps = 0
	stepwidth = 1
	runs = 0
	
	settings = 1

	#the actual settings for the batch run
	w = 			[]
	wMin = 			[]
  	c1 = 			[]
  	c2 = 			[]
	vmaxRatio = 	[]
	constriction = 	[]
  	swarmsize =		[]
  	dim = 			[]
	height = 		[]
	branches = 		[]
	swapDelay = 	[]
  	function_type = []
  	noiseStyle = 	[]
  	noiseSigma = 	[]
  	swarm_type = 	[]
  	nrScouts = 		[]
  	maxStep =	 	[]
  	
	#dynamic parameters
	moveFrequency =		[]
	moveDistance = 		[]
	optimumMoveStyle = 	[]
	updateStyle =		[]
	detectionMethod = 	[]
	responseMethod = 	[]


	#noise parameters
	nrNoisySwapChecks = 	[]
	noisyReuseOldValue = 	[]
	noisyRefineBestValue =	[]
	firstComparePbest = 	[]
	hierachyChangeDetectThreshold = []
	
	#adaptive parameters
	decreaseBranchFrequency  =  []
	minBranchDegree =			[]
	decreaseBranchStep =        []
	decreaseBranchAction =      []

	logdir = ""
	#for batchmode, the different evaluation methods are given
	attributeslist = []
	attributesDesc = []
	
	#batch
	#describe the log run before/after 
	description = ""
	observation = ""

	def __init__(self):
	    pass

#	def __init__(self, w_in, wMin_in, c1_in, c2_in, vmaxRatio_in, constriction_in, size_in, dim_in, height_in, \
#	branches_in, swapDelay_in, funct_in, swarmType_in, nrScouts_in, maxStep_in):
#
#	self.set(self, w_in, wMin_in, c1_in, c2_in, vmaxRatio_in, constriction_in, size_in, dim_in, height_in, \
#	branches_in, swapDelay_in, funct_in, swarmType_in, nrScouts_in, maxStep_in)
#	
#	pass

	def set(self, w_in, wMin_in, c1_in, c2_in, vmaxRatio_in, constriction_in, size_in, dim_in, height_in, \
		branches_in, swapDelay_in, funct_in, noise_in, noiseSigma_in, swarmType_in, nrScouts_in, maxStep_in, \
		moveFrequency_in, moveDistance_in, optimumMoveStyle_in, updateStyle_in, detectionMethod_in, responseMethod_in,\
		nrNoisySwapChecks_in, noisyReuseOldValue_in, noisyRefineBestValue_in, noisyFirstComparePbest_in,
		hierachyChangeDetectThreshold_in, decreaseBranchFrequency_in, minBranchDegree_in, decreaseBranchStep_in, \
		decreaseBranchAction_in):

		self.w = 				[w_in]
		self.wMin = 			[wMin_in]
	  	self.c1 = 				[c1_in]
	  	self.c2 = 				[c2_in]
		self.vmaxRatio = 		[vmaxRatio_in]
		self.constriction = 	[constriction_in]
	  	self.swarmsize =	 	[size_in]
	  	self.dim = 				[dim_in]
		self.height = 			[height_in]
		self.branches = 		[branches_in]
		self.swapDelay = 		[swapDelay_in]
	  	self.function_type = 	[funct_in]
	  	self.noiseStyle =		[noise_in]
	  	self.noiseSigma =		[noiseSigma_in]
	  	self.swarm_type = 		[swarmType_in]
	  	self.nrScouts =	 		[nrScouts_in]
	  	self.maxStep = 			[maxStep_in]
		
		self.moveFrequency = 	[moveFrequency_in]
		self.moveDistance = 		[moveDistance_in]
		self.optimumMoveStyle = 	[optimumMoveStyle_in]
		self.updateStyle =		[updateStyle_in]
		self.detectionMethod = 	[detectionMethod_in]
		self.responseMethod = 	[responseMethod_in]

		self.nrNoisySwapChecks = 		[nrNoisySwapChecks_in]
		self.noisyReuseOldValue = 		[noisyReuseOldValue_in]
		self.noisyRefineBestValue =		[noisyRefineBestValue_in]
		self.noisyFirstComparePbest = 	[noisyFirstComparePbest_in]
		self.hierachyChangeDetectThreshold = [hierachyChangeDetectThreshold_in]
		
		self.decreaseBranchFrequency = [decreaseBranchFrequency_in]
		self.minBranchDegree = [minBranchDegree_in]
		self.decreaseBranchStep = [decreaseBranchStep_in]
		self.decreaseBranchAction = [decreaseBranchAction_in]

		pass

	
	def __str__(self):
	    out = ""
	    out += "logdir: " + self.logdir + "\n"
	    out += "runs: " + str(self.runs) + "\n"
	    out += "steps: " + str(self.steps) + "\n"
	    out += "stepwidth: " + str(self.stepwidth) + "\n"
	    out += "attributes: " + str(self.attributeslist) + "\n"
	    out += "attrDesc: " + str(self.attributesDesc) + "\n"
	    out += "param:\n[\n"
	    for i in range(self.settings):
	        out += str(i) + ": " + self.parameterString(i) + "\n  "
	        #out += self.functionParameter(i) + "\n"
	        if (self.swarm_type[i] == pso.adaptivePyramidSwarm):
	        	out += self.adaptiveParameter(i) + "\n"
	    out += "]\n"
	    out += "description:\n" + self.description + "\n"
	    out += "observation:\n" + self.observation + "\n"
	    return out
	

## New PsoParameters
## 0 - double w_in,
## 1 - double wMin_in,
## 2 - double c1_in,
## 3 - double c2_in,
## 4 -int vmaxRatio_in,
## 5 -int size_in,
## 6 -int dim_in,
## 7 -int height_in,
## 8 -int branches_in,
## 9 -int swapDelay_in,
##10 -function_type funct_in,
##11 -swarm_type swarmType_in,

	#only one parameter set included
	def guiString(self):
	    #including w-range and vmax and maxStep
		if (self.w[0]==self.wMin[0]):
			wrange = str(round(self.w[0],DISPLAYDIGITS))
		else:
			wrange = "[" + str(round(self.wMin[0],DISPLAYDIGITS)) + ", " + str(round(self.w[0],DISPLAYDIGITS)) + "]"


		if (self.vmaxRatio[0] == 0.0):
			vmax = "no"
		else:
			vmax = str(round(self.vmaxRatio[0],DISPLAYDIGITS))
		
		
		swarm = SWARM[self.swarm_type[0]]

		#identify A-HPSO and V-HPSO
		if ((self.swarm_type[0] == pso.pyramidSwarm) | (self.swarm_type[0] == pso.dynamicPyramidSwarm) \
		| (self.swarm_type[0] == pso.adaptivePyramidSwarm)):
			if (self.wMin[0] < self.w[0]):
				swarm = "A-" + swarm
			elif (self.wMin[0] > self.w[0]):
				swarm = "V-" + swarm

		out = swarm +" - size=" + str(self.swarmsize[0])+ ", nrScouts=" + str(self.nrScouts[0]) + \
			  "\nw=" + wrange + ", c1=" + str(round(self.c1[0],DISPLAYDIGITS)) + \
			  ", c2=" + str(round(self.c2[0],DISPLAYDIGITS))+ ", vmax=" + vmax


		if (self.swarm_type[0] == pso.pyramidSwarm):
			out += "\nheight=" + str(self.height[0])+ ", branch=" + str(self.branches[0])

		out += "\n" + FUNCTION[self.function_type[0]] + " (dim=" + str(self.dim[0]) + ")"

		return out
		    
	def parameterString(self, i):
	    #including w-range and vmax and maxStep
	    if (self.w[i]==self.wMin[i]):
	        wrange = str(round(self.w[i],DISPLAYDIGITS))
	    else:
	        wrange = "[" + str(round(self.wMin[i],DISPLAYDIGITS)) + ", " + str(round(self.w[i],DISPLAYDIGITS)) + "]"
	    if (self.vmaxRatio[i] == 0.0):
	        vmax = "no"
	    else:
	        vmax = str(round(self.vmaxRatio[i],DISPLAYDIGITS))
	
		swarm = SWARM[self.swarm_type[i]]

		#identify A-HPSO and V-HPSO
		if ((self.swarm_type[i] == pso.pyramidSwarm) | (self.swarm_type[i] == pso.dynamicPyramidSwarm) \
		| (self.swarm_type[i] == pso.adaptivePyramidSwarm)):
			if (self.wMin[i] < self.w[i]):
				swarm = "A-" + swarm
			elif (self.wMin[i] > self.w[i]):
				swarm = "V-" + swarm

	    out = swarm +" - w=" + wrange + ", c1=" + str(round(self.c1[i],DISPLAYDIGITS))+ \
	        ", c2=" + str(round(self.c2[i],DISPLAYDIGITS))+ ", size=" + str(self.swarmsize[i])+ ", height=" + \
	        str(self.height[i])+ ", branch=" + str(self.branches[i]) + ", vmax=" + vmax
	    out += "\n  detect:" + DETECT[self.detectionMethod[i]]
#	    if (self.detectionMethod[i] == pso.hierachyChangeDetect):
#	    	out += "(" + str(self.hierachyChangeDetectThreshold[i]) + ")"
	    out += ", response: " + RESPONSE[self.responseMethod[i]]

	    #noise
	    if (self.noiseStyle[i] != pso.noNoise):
	    	out += ", " + NOISE[self.noiseStyle[i]] + " sigma=" + str(self.noiseSigma[i])

	
	    return out
	    
	def functionString(self):
	    out = FUNCTION[self.function_type[0]] + " (dim=" + str(self.dim[0]) + ") - " + str(self.steps) +" steps"
	    return out

	def functionParameter(self,i):
	    out = "OptMove: " + MOVE[self.optimumMoveStyle[i]] + ", Severity: " + str(self.moveDistance[i]) + \
	    	", Update: " + UPDATE[self.updateStyle[i]]
	    if (self.updateStyle[i] == pso.updatePeriodic):
	    	out += " (" + str(self.moveFrequency[i]) + ")"
	    return out
	
	def adaptiveParameter(self,i):
		out = "branchDec=-" + str(self.decreaseBranchStep[i]) + " (" + str(self.decreaseBranchFrequency[i]) + ")" + \
			" to: " + str(self.minBranchDegree[i]) + " " + ACTION[self.decreaseBranchAction[i]]		
		return out
	


class PsoParameter:
    steps = 0
    stepwidth = 1
    runs = 0

    #the actual settings for the batch run
    param = []
    logdir = ""
    attributeslist = []
    attributesDesc = []

    #describe the log run before/after 
    description = ""
    observation = ""

    def __init__(self):
        pass

    def __str__(self):
        out = ""
        out += "logdir: " + self.logdir + "\n"
        out += "runs: " + str(self.runs) + "\n"
        out += "steps: " + str(self.steps) + "\n"
        out += "stepwidth: " + str(self.stepwidth) + "\n"
        out += "attributes: " + str(self.attributeslist) + "\n"
        out += "attrDesc: " + str(self.attributesDesc) + "\n"
        out += "param:\n[\n"
        for i in range(len(self.param)):
            out += "  " +str(self.param[i]) + "\n"
        out += "]\n"
        out += "description:\n" + self.description + "\n"
        out += "observation:\n" + self.observation + "\n"
        return out


## New PsoParameters
## 0 - double w_in,
## 1 - double wMin_in,
## 2 - double c1_in,
## 3 - double c2_in,
## 4 -int vmaxRatio_in,
## 5 -int size_in,
## 6 -int dim_in,
## 7 -int height_in,
## 8 -int branches_in,
## 9 -int swapDelay_in,
##10 -function_type funct_in,
##11 -swarm_type swarmType_in,



    
    def parameterString(self, i):
        if (len(self.param[i]) == 10):
            #old version
            out = SWARM[self.param[i][9]] +" - w=" + str(round(self.param[i][0],DISPLAYDIGITS)) + ", c1=" + str(round(self.param[i][2],DISPLAYDIGITS))+ ", c2=" + str(round(self.param[i][3],DISPLAYDIGITS))+ ", size=" + str(self.param[i][4])+ ", height=" + str(self.param[i][6])+ ", branch=" + str(self.param[i][7])
        else:
            #including w-range and vmax and maxStep
            if (self.param[i][0]==self.param[i][1]):
                wrange = str(round(self.param[i][0],DISPLAYDIGITS))
            else:
                wrange = "[" + str(round(self.param[i][1],DISPLAYDIGITS)) + ", " + str(round(self.param[i][0],DISPLAYDIGITS)) + "]"
            if (self.param[i][4] == 0.0):
                vmax = "no"
            else:
                vmax = str(round(self.param[i][4],DISPLAYDIGITS))
            out = SWARM[self.param[i][11]] +" - w=" + wrange + ", c1=" + str(round(self.param[i][2],DISPLAYDIGITS))+ ", c2=" + str(round(self.param[i][3],DISPLAYDIGITS))+ ", size=" + str(self.param[i][5])+ ", height=" + str(self.param[i][7])+ ", branch=" + str(self.param[i][8]) + ", vmax=" + vmax
            


        return out

    def functionString(self):
        if (len(self.param[0]) == 10):
            #old version
            out = FUNCTION[self.param[0][8]] + " (dim=" + str(self.param[0][5]) + ") - " + str(self.steps) +" steps"
        else:
            out = FUNCTION[self.param[0][10]] + " (dim=" + str(self.param[0][6]) + ") - " + str(self.steps) +" steps"
        return out



class PsoParameter2:
	steps = 0
	stepwidth = 1
	runs = 0
	
	settings = 1

	#the actual settings for the batch run
	w = 			[]
	wMin = 			[]
  	c1 = 			[]
  	c2 = 			[]
	vmaxRatio = 	[]
	constriction = 	[]
  	swarmsize =		[]
  	dim = 			[]
	height = 		[]
	branches = 		[]
	swapDelay = 	[]
  	function_type = []
  	swarm_type = 	[]
  	nrScouts = 		[]
  	maxStep =	 	[]
  	
	#dynamic parameters
	moveFrequency =		[]
	moveDistance = 		[]
	optimumMoveStyle = 	[]
	updateStyle =		[]
	detectionMethod = 	[]
	responseMethod = 	[]



	logdir = ""
	#for batchmode, the different evaluation methods are given
	attributeslist = []
	attributesDesc = []
	
	#batch
	#describe the log run before/after 
	description = ""
	observation = ""

	def __init__(self):
	    pass

#	def __init__(self, w_in, wMin_in, c1_in, c2_in, vmaxRatio_in, constriction_in, size_in, dim_in, height_in, \
#	branches_in, swapDelay_in, funct_in, swarmType_in, nrScouts_in, maxStep_in):
#
#	self.set(self, w_in, wMin_in, c1_in, c2_in, vmaxRatio_in, constriction_in, size_in, dim_in, height_in, \
#	branches_in, swapDelay_in, funct_in, swarmType_in, nrScouts_in, maxStep_in)
#	
#	pass

	def set(self, w_in, wMin_in, c1_in, c2_in, vmaxRatio_in, constriction_in, size_in, dim_in, height_in, \
		branches_in, swapDelay_in, funct_in, swarmType_in, nrScouts_in, maxStep_in, moveFrequency_in, \
		moveDistance_in, optimumMoveStyle_in, updateStyle_in, detectionMethod_in, responseMethod_in):

		self.w = 				[w_in]
		self.wMin = 			[wMin_in]
	  	self.c1 = 				[c1_in]
	  	self.c2 = 				[c2_in]
		self.vmaxRatio = 		[vmaxRatio_in]
		self.constriction = 	[constriction_in]
	  	self.swarmsize =	 	[size_in]
	  	self.dim = 				[dim_in]
		self.height = 			[height_in]
		self.branches = 		[branches_in]
		self.swapDelay = 		[swapDelay_in]
	  	self.function_type = 	[funct_in]
	  	self.swarm_type = 		[swarmType_in]
	  	self.nrScouts =	 		[nrScouts_in]
	  	self.maxStep = 			[maxStep_in]
		
		self.moveFrequency = 	[moveFrequency_in]
		self.moveDistance = 		[moveDistance_in]
		self.optimumMoveStyle = 	[optimumMoveStyle_in]
		self.updateStyle =		[updateStyle_in]
		self.detectionMethod = 	[detectionMethod_in]
		self.responseMethod = 	[responseMethod_in]

		pass

	
	def __str__(self):
	    out = ""
	    out += "logdir: " + self.logdir + "\n"
	    out += "runs: " + str(self.runs) + "\n"
	    out += "steps: " + str(self.steps) + "\n"
	    out += "stepwidth: " + str(self.stepwidth) + "\n"
	    out += "attributes: " + str(self.attributeslist) + "\n"
	    out += "attrDesc: " + str(self.attributesDesc) + "\n"
	    out += "param:\n[\n"
	    for i in range(self.settings):
	        out += self.parameterString(i) + "\n"
	    out += "]\n"
	    out += "description:\n" + self.description + "\n"
	    out += "observation:\n" + self.observation + "\n"
	    return out
	

## New PsoParameters
## 0 - double w_in,
## 1 - double wMin_in,
## 2 - double c1_in,
## 3 - double c2_in,
## 4 -int vmaxRatio_in,
## 5 -int size_in,
## 6 -int dim_in,
## 7 -int height_in,
## 8 -int branches_in,
## 9 -int swapDelay_in,
##10 -function_type funct_in,
##11 -swarm_type swarmType_in,

	#only one parameter set included
	def guiString(self):
	    #including w-range and vmax and maxStep
		if (self.w[0]==self.wMin[0]):
			wrange = str(round(self.w[0],DISPLAYDIGITS))
		else:
			wrange = "[" + str(round(self.wMin[0],DISPLAYDIGITS)) + ", " + str(round(self.w[0],DISPLAYDIGITS)) + "]"


		if (self.vmaxRatio[0] == 0.0):
			vmax = "no"
		else:
			vmax = str(round(self.vmaxRatio[0],DISPLAYDIGITS))
		
		
		swarm = SWARM[self.swarm_type[0]]

		#identify A-HPSO and V-HPSO
		if ((self.swarm_type[0] == pso.pyramidSwarm) | (self.swarm_type[0] == pso.dynamicPyramidSwarm)):
			if (self.wMin[0] < self.w[0]):
				swarm = "A-" + swarm
			elif (self.wMin[0] > self.w[0]):
				swarm = "V-" + swarm

		out = swarm +" - size=" + str(self.swarmsize[0])+ ", nrScouts=" + str(self.nrScouts[0]) + \
			  "\nw=" + wrange + ", c1=" + str(round(self.c1[0],DISPLAYDIGITS)) + \
			  ", c2=" + str(round(self.c2[0],DISPLAYDIGITS))+ ", vmax=" + vmax


		if (self.swarm_type[0] == pso.pyramidSwarm):
			out += "\nheight=" + str(self.height[0])+ ", branch=" + str(self.branches[0])

		out += "\n" + FUNCTION[self.function_type[0]] + " (dim=" + str(self.dim[0]) + ")"

		return out
		    
	def parameterString(self, i):
	    #including w-range and vmax and maxStep
	    if (self.w[i]==self.wMin[i]):
	        wrange = str(round(self.w[i],DISPLAYDIGITS))
	    else:
	        wrange = "[" + str(round(self.wMin[i],DISPLAYDIGITS)) + ", " + str(round(self.w[i],DISPLAYDIGITS)) + "]"
	    if (self.vmaxRatio[i] == 0.0):
	        vmax = "no"
	    else:
	        vmax = str(round(self.vmaxRatio[i],DISPLAYDIGITS))
	
		swarm = SWARM[self.swarm_type[i]]

		#identify A-HPSO and V-HPSO
		if ((self.swarm_type[i] == pso.pyramidSwarm) | (self.swarm_type[i] == pso.dynamicPyramidSwarm)):
			if (self.wMin[i] < self.w[i]):
				swarm = "A-" + swarm
			elif (self.wMin[i] > self.w[i]):
				swarm = "V-" + swarm

	    out = swarm +" - w=" + wrange + ", c1=" + str(round(self.c1[i],DISPLAYDIGITS))+ \
	        ", c2=" + str(round(self.c2[i],DISPLAYDIGITS))+ ", size=" + str(self.swarmsize[i])+ ", height=" + \
	        str(self.height[i])+ ", branch=" + str(self.branches[i]) + ", vmax=" + vmax
	    out += "\ndetect:" + DETECT[self.detectionMethod[i]] + ", response: " + RESPONSE[self.responseMethod[i]]
	        
	
	    return out
	    
	def functionString(self):
	    out = FUNCTION[self.function_type[0]] + " (dim=" + str(self.dim[0]) + ") - " + str(self.steps) +" steps"
	    out += "\nOptMove: " + MOVE[self.optimumMoveStyle[0]] + ", Severity: " + str(self.moveDistance[0]) + \
	    	", Update: " + UPDATE[self.updateStyle[0]]
	    if (self.updateStyle[0] == pso.updatePeriodic):
	    	out += " (" + str(self.moveFrequency[0]) + ")"

	    return out
	
	
	
