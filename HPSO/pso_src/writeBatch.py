import cPickle
import os, shutil
import sys

from psoParameter import *


PSO = pso.seqSwarm
HPSO = pso.pyramidSwarm
LPSO = pso.localSwarm
DHPSO = pso.dynamicPyramidSwarm
AHPSO = pso.adaptivePyramidSwarm

PERIODIC = pso.updatePeriodic
ONGOAL = pso.updateOnGoal
RANDOM = pso.updateRandom

RANDMOVE = pso.randomOptMove
LINMOVE = pso.linearOptMove
RANDLINMOVE = pso.randomLinearOptMove
RANDDISTMOVE = pso.randomDistanceOptMove
EXPLINMOVE = pso.exponentialLinearOptMove

GBEST = pso.gbestDetectChange
UPTODATE = pso.upToDate
NODETECT = pso.noDetect
HIERARCHY = pso.hierarchyChangeDetect
OMNI = pso.omniscientDetect

NONOISE = pso.noNoise
NOISEVAL = pso.noisyFunctionValue
NOISEPOS = pso.noisyPosition
NOISERELVAL = pso.noisyRelativeFunctionValue

NORESP = pso.noResponse
RAND10 = pso.randomize10
RAND10RES = pso.randomize10reset
RAND16RES = pso.randomize16reset
RAND22RES = pso.randomize22reset
RAND40RES = pso.randomize40reset
SUBSWARMT = pso.subSwarmsTemporalMerge
SUBSWARMA = pso.subSwarmsAdaptiveMerge
SUBT_NOMEM = pso.subSwarmsTemporalMergeNoMemory
SUBA_NOMEM = pso.subSwarmsAdaptiveMergeNoMemory
UPDATE = pso.updateOnly

NOACT = pso.noAction
RESET = pso.reset
BESTNOACT = pso.bestSubtreeNoAction
BESTRESET = pso.bestSubtreeReset


#  PSO(double w_in, ///<The w parameter (influence of old vel - momentum)
#  		double delta_w_in, ///<w is decreased, unless delta_w = 1
#  		double c1_in, ///<Influence of personal best
#  		double c2_in, ///<Influence of global best
#  		int size_in, ///<Swarmsize, ignored for PyramidSwarm
# 		int dim_in, ///<Dimension of the search space
#			int height_in, ///<Height of the pyramid, ignored for #seqSwarm
#			int branches_in, ///<Branches per node of the pyramid, ignored for #seqSwarm
#  		function_type funct_in, ///<#function_type selection
#		-> 	sphere, rastrigin, rosenbrock, schaffer, griewank, ackley, schwefel, levy5
#  		swarm_type swarmType_in); ///<#swarm_type selection
#		-> seqSwarm, pyramidSwarm, ringSwarm


PARAMETER_FILENAME = "batchParameter"

#main
if not(os.environ.has_key('LOGPATH')):
    print "environment variable LOGPATH not set !"
    sys.exit(-1)
else:  
    LOGPATH = str(os.environ.get('LOGPATH'))



paramSet = PsoParameter3()

#old
#[0.729, 1.0, 1.494, 1.494, 31, 30, 3, 4, 4, 0],

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
##     0 - seqSwarm, 1 - pyramidSwarm, 2 - ringSwarm, 3 - localSwarm
##11 -swarm_type swarmType_in,
##     0 - sphere, 1 - rastrigin, 2 - rosenbrock, 3 - schaffer, 4 - griewank,
##     5 - ackley, 6 - schwefel, 7 - levy5, 8 - freudenstein, 9 - quadric


 



#[w, wmin, c1, c2, vmaxRatio, size, dim, height, branches, swapDelay, funct, swarm]

function = pso.moving_peaks
dim = 5
size = 40

paramSet.settings = 4

#the actual settings for the batch run
#paramSet.w =				[0.729]*2 + [0.729]*2 + [0.729]*4
paramSet.w =				[0.729]*paramSet.settings
#paramSet.wMin =				[0.729]*2 + [0.729]*2 + [0.729]*4
paramSet.wMin =				[0.729]*paramSet.settings
paramSet.c1 =				[1.494]*paramSet.settings
paramSet.c2 =				[1.494]*paramSet.settings
paramSet.vmaxRatio =		[1.0]*paramSet.settings
paramSet.constriction =		[1.0]*paramSet.settings
paramSet.swarmsize =		[size]*paramSet.settings
#paramSet.swarmsize =		([30]*3 + [47] + [30]*2)*4
paramSet.dim =				[dim]*paramSet.settings
paramSet.height =			[3]*paramSet.settings
#paramSet.branches =			[5]*paramSet.settings
#paramSet.branches =			[0,2,4]*4
paramSet.branches = 		[4]*4
paramSet.swapDelay =		[1]*paramSet.settings
paramSet.function_type =	[function]*paramSet.settings
paramSet.swarm_type =		[DHPSO]*4
#paramSet.swarm_type =		[PSO, LPSO, HPSO]*4 
#paramSet.swarm_type = 		[HPSO]*paramSet.settings
paramSet.nrScouts =			[0]*paramSet.settings
paramSet.maxStep =			[0]*paramSet.settings

paramSet.noiseStyle =		[NOISEVAL]*paramSet.settings
paramSet.noiseSigma =		[0.001]*1 + [0.01]*1 + [0.1]*1 + [1.0]*1
#paramSet.noiseSigma =		[0.0, 0.0001, 10**-3.5, 0.001, 10**-2.5, 0.01, 10**-1.5, 0.1, 10**-0.5, 1.0, 10**0.5, 10.0]
#paramSet.noiseSigma =		[0.0]*paramSet.settings
paramSet.nrNoisySwapChecks = 		[1]*paramSet.settings
paramSet.noisyReuseOldValue = 		[pso.reuseLastBestValue]*paramSet.settings
paramSet.noisyRefineBestValue =		[True]*paramSet.settings
paramSet.noisyFirstComparePbest = 	[False]*paramSet.settings
paramSet.hierarchyChangeDetectThreshold = [6]*paramSet.settings


#dynamic parameters
paramSet.moveFrequency =	[100]*paramSet.settings
paramSet.moveDistance = 	[5.0]*paramSet.settings
#paramSet.moveDistance = 	[0.1]*3 + [0.5]*3 + [1.0]*3 + [5.0]*3
paramSet.optimumMoveStyle =	[EXPLINMOVE]*paramSet.settings
paramSet.updateStyle =		[PERIODIC]*paramSet.settings
#paramSet.detectionMethod =	[GBEST]*paramSet.settings
paramSet.detectionMethod =	([HIERARCHY])*4
#paramSet.responseMethod =	[RAND22RES]*paramSet.settings
#paramSet.responseMethod =	([RAND16RES]*3 + [SUBSWARMT] + [SUBSWARMA])*5
#paramSet.responseMethod = 	[NORESP]*paramSet.settings
paramSet.responseMethod =	([SUBSWARMA])*4

#paramSet.decreaseBranchFrequency  =  ([5]*3 + [20]*3)*2 + [50]*3
paramSet.decreaseBranchFrequency  =  [0]*paramSet.settings
#paramSet.minBranchDegree =           [2]*6+[4]*9
paramSet.minBranchDegree =           [0]*paramSet.settings
#paramSet.decreaseBranchStep =        [1,2,4]*15
paramSet.decreaseBranchStep =        [0]*paramSet.settings
paramSet.decreaseBranchAction =      [NOACT]*paramSet.settings
#paramSet.decreaseBranchAction =      [BESTNOACT]*paramSet.settings

    
#logdir relative to $LOGPATH
#paramSet.logdir = "noisy-pso/check-swap/noise-movGoal/movGoal-10-100-noise-var-HPSO47"
paramSet.logdir = "noisy-pso-2/noise-dyn/movPeaks/DHPSOa-Peaks-100-5pc-noise-var-detect6"
paramSet.steps = 30000
paramSet.stepwidth = 1
paramSet.runs = 1

#attributes have to be set in run_pso.py
paramSet.attributeslist = [0, 1, 2, 3, 4]
paramSet.attributesDesc = ['Actual Best',  'Curr Error', 'nr Swap', 'nr Eval', 'detected']

#describe the log run before/after
paramSet.description = "Hierarchy Change Detect Threshold 6"
paramSet.observation = ""



print "The following parameters will be written"
print paramSet.functionString()
print paramSet

if not os.path.exists(LOGPATH + '/' + paramSet.logdir):
    print LOGPATH + '/' + paramSet.logdir, " does not exist, will be created"

print "writing", PARAMETER_FILENAME, "in", LOGPATH + '/' + paramSet.logdir
print "Ok ?(Y/N)"
a=sys.stdin.readline(1)
if (a == 'y') or (a=='Y'):
    if not os.path.exists(LOGPATH + '/' + paramSet.logdir):
        os.makedirs(LOGPATH + '/' + paramSet.logdir)
    shutil.copy("writeBatch.py", LOGPATH + '/' + paramSet.logdir)
    shutil.copy("run_pso.py", LOGPATH + '/' + paramSet.logdir)
    

##    print "Description:"
##    input=sys.stdin.read()
##    #delete first newline
##    paramSet.description = input[1:]
##    print paramSet
    f = open(LOGPATH + '/' + paramSet.logdir +'/' + PARAMETER_FILENAME, 'w')
    cPickle.dump(paramSet, f)
    f.close()
    print "\n**************\n", paramSet, "\n written in ", LOGPATH + '/' + paramSet.logdir + '/' + PARAMETER_FILENAME
else:
    print "no files written"

