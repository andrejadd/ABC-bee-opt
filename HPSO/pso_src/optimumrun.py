import pso
import os 
import sys
import cPickle
import time


SWARM_DISPLAY = ["SEQ-SWARM", "PYRAMID-SWARM"]
FUNCTION_DISPLAY = ["Sphere", "Rastrigin", "Rosenbrock"]


paramSet= [
    #dimension and function type increased
    #seqSwarm
    [0.729, 1.0, 1.494, 1.494, 31, 0, 0, 0, 0, 0],
    #pyramid
    [0.729, 1.0, 1.494, 1.494, 31, 0, 5, 2, 0, 1],
    #ring
#    [0.729, 1.0, 1.494, 1.494, 32, 0, 8, 4, 0, 2],
    #local
    [0.729, 1.0, 1.494, 1.494, 31, 0, 0, 1, 0, 3],
    ]


psoAlg = None

filename = "log"

batchParameter = []


stepsToGoalSetting = []

steps = 20000
runs = 100

logdir = "pso/optimum"



def newPSO(param):
    global psoAlg
    del psoAlg
    #step delay set to 1
    psoAlg = pso.PSO(param[0], param[1], param[2], param[3], param[4], 
        param[5], param[6], param[7], 1, param[8], param[9])

def writeStepsToGoal(functNr, first=False):
    global stepsToGoalSetting
    if (first):
        stepsToGoal = []
    else:
        f = open(LOGPATH + '/' + logdir + '/goalSteps' + str(functNr), 'r')
        stepsToGoal = cPickle.load(f)
        f.close()
        
    stepsToGoal.append(stepsToGoalSetting)
    f = open(LOGPATH + '/' + logdir + '/goalSteps' + str(functNr), 'w')
    cPickle.dump(stepsToGoal, f)
    f.close()
    print "appended to", LOGPATH + '/' + logdir + '/goalSteps' + str(functNr)

def batchExecution(startSet):
    global stepsToGoalSetting
    minDim = [[8,10,10],[4,4,3], [7,1,1], [1,1,1]]
    maxDim = [[8,16,10],[5,5,3],[7,11,8], [11,15,15]]
    functions = [1,2,4,5]
    
    settings = range(3)
    for funct in range(4):
        for set in settings:
            print "*************  SETTING " + str(set) + " *************"
            param = paramSet[set]
            #function
            param[8] = functions[funct]
            stepsToGoalSetting = []
            for dim in range(minDim[funct][set], maxDim[funct][set]):
                stepsToGoalDim = []
                for run in range(runs):
                    param[5] = dim
                    starttime = time.time()
                    
                    print "new PSO(",param,")"
                    newPSO(param)
                    
                    psoAlg.step(steps)
                    stepsToGoalDim.append(psoAlg.stat.getGoalReachedStep())
                    print "batch run completed in", str(round(time.time()-starttime,2)), "s"
                stepsToGoalSetting.append(stepsToGoalDim)
            print stepsToGoalSetting
            #append to the goalSteps file
            if (set == 0):
                writeStepsToGoal(functions[funct], True)
            else:
                writeStepsToGoal(functions[funct])


#main
if not(os.environ.has_key('LOGPATH')):
    print "environment variable LOGPATH not set !"
    sys.exit(-1)
else:  
    LOGPATH = str(os.environ.get('LOGPATH'))

batchExecution(0)
            
#p = pso.PSO(W, 1.0, C1, C2, SIZE, DIM, HEIGHT, BRANCHES, OPTFUNCTION, SWARMTYPE)


    

