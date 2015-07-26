import pso
import os
import sys
import cPickle
import time

#which version of psoParameter.py
#1 - huge parameter matrix - deprecated (CEC03)
#2 - using proper descriptors for the individual parameters
PSOPARAMETER_STYLE = 2


SWARM_DISPLAY = ["SEQ-SWARM", "PYRAMID-SWARM"]
FUNCTION_DISPLAY = ["Sphere", "Rastrigin", "Rosenbrock"]


psoAlg = None

filename = "log"

batchParameter = None


stepsToGoalSetting = []
stepsToOptSetting = []

def loadParameter(batchfilename):
    global batchParameter
    batchfile = open(batchfilename, 'r')
    batchParameter = cPickle.load(batchfile)
    batchfile.close()

def getParameter(set):
    return batchParameter.param[set]

  
def logStep(file, step):
    #seperate different attributes with single blank " "
    #log swapped on level
#    out = str(psoAlg.stat.getGlobalBest(step)) + " " + str(float(psoAlg.stat.getNrSwapped(step))) + " " + str(float(psoAlg.stat.getNrSwappedLevel(step,0))) + " " + str(float(psoAlg.stat.getNrSwappedLevel(step,1))) + " " + str(float(psoAlg.stat.getNrSwappedLevel(step,2))) + "\n"
    #log nr swapped
#    out = str(psoAlg.stat.getGlobalBest(step)) + " " + str(float(psoAlg.stat.getNrSwapped(step))) + "\n"
    #only log quality
#    out = str(psoAlg.stat.getGlobalBest(step)) + "\n"
    #log current error for moving_peaks
#    out = str(psoAlg.stat.getGlobalBest(step)) + " " + str(psoAlg.stat.getCurrentError(step)) + "\n"
    #log gbestInSubswarm for DynamicPyramidSwarm
#    out = str(psoAlg.stat.getGlobalBest(step)) + " " + str(psoAlg.stat.getActualGlobalBest(step)) + " " + str(float(psoAlg.stat.getNrSwapped(step))) + " " + str(float(psoAlg.stat.getNrSwappedLevel(step,0))) + " " + str(float(psoAlg.stat.getNrSwappedLevel(step,1))) + "\n"

	if (batchParameter.swarm_type[set] == pso.dynamicPyramidSwarm):
            gbestInSubswarm = str(psoAlg.stat.getGbestInSubswarm(step))
        else:
            gbestInSubswarm = ""


	if (psoAlg.stat.isChangeDetected(step)):
		out = str(psoAlg.stat.getActualGlobalBest(step)) + " " + str(psoAlg.stat.getCurrentError(step)) + " " + str(psoAlg.stat.getNrSwapped(step)) + " " + str(psoAlg.stat.getNrEvaluations(step)) + " " + str(1) + " " + gbestInSubswarm + "\n"
	else:
		out = str(psoAlg.stat.getActualGlobalBest(step)) + " " + str(psoAlg.stat.getCurrentError(step)) + " " + str(psoAlg.stat.getNrSwapped(step)) + " " + str(psoAlg.stat.getNrEvaluations(step)) + " " + str(0) + " " + gbestInSubswarm +"\n"
	file.write(out)

        
def newPSO(i):
	global psoAlg
	del psoAlg
	p = batchParameter
	psoAlg = pso.PSO(p.w[i], p.wMin[i], p.c1[i], p.c2[i], p.vmaxRatio[i], p.constriction[i], p.swarmsize[i], \
		p.dim[i], p.height[i], p.branches[i], p.swapDelay[i], p.function_type[i], p.noiseStyle[i], \
		p.noiseSigma[i], p.swarm_type[i], p.nrScouts[i], p.maxStep[i])
	psoAlg.setDynamicParameters(p.moveFrequency[i], p.moveDistance[i], p.optimumMoveStyle[i], p.updateStyle[i], \
		p.detectionMethod[i], p.responseMethod[i])
	psoAlg.setNoiseParameters(p.nrNoisySwapChecks[i], p.noisyReuseOldValue[i], \
		p.noisyRefineBestValue[i], p.noisyFirstComparePbest[i], p.hierarchyChangeDetectThreshold[i])
        
        psoAlg.setAdaptiveParameters(p.decreaseBranchFrequency[i], p.minBranchDegree[i],\
        p.decreaseBranchStep[i], p.decreaseBranchAction[i])
        
	
def writeStepsToGoal(first=False):
    global stepsToGoalSetting, stepsToOptSetting
    if (first):
        stepsToGoal = []
        stepsToOpt = []
    else:
        f = open(LOGPATH + '/' + batchParameter.logdir + '/goalSteps', 'r')
        stepsToGoal = cPickle.load(f)
        f.close()
        f = open(LOGPATH + '/' + batchParameter.logdir + '/optSteps', 'r')
        stepsToOpt = cPickle.load(f)
        f.close()
        
    stepsToGoal.append(stepsToGoalSetting)
    stepsToOpt.append(stepsToOptSetting)
    f = open(LOGPATH + '/' + batchParameter.logdir + '/goalSteps', 'w')
    cPickle.dump(stepsToGoal, f)
    f.close()
    print "appended to", LOGPATH + '/' + batchParameter.logdir + '/goalSteps'
    f = open(LOGPATH + '/' + batchParameter.logdir + '/optSteps', 'w')
    cPickle.dump(stepsToOpt, f)
    f.close()
    print "appended to", LOGPATH + '/' + batchParameter.logdir + '/optSteps'

def batchExecution(startSet):
    global set, stepsToGoalSetting, stepsToOptSetting
    settings = range(batchParameter.settings)[startSet:]
    for set in settings:
        print "*************  SETTING " + str(set) + " *************"
        stepsToGoalSetting = []
        stepsToOptSetting = []
        for run in range(batchParameter.runs):
            #cool cpu
#            print "Break - sleep(60)"
#            time.sleep(60)
            starttime = time.time()
            print "new PSO(",batchParameter.parameterString(set),")"
            newPSO(set)
            f = open(LOGPATH + '/' + batchParameter.logdir + '/' + filename+str(set)+"."+str(run), 'w')
            psoAlg.step(batchParameter.steps)
            print "pso done"
            for step in range(0, batchParameter.steps, batchParameter.stepwidth):
                logStep(f, step)
            f.close()
            stepsToGoalSetting.append(psoAlg.stat.getGoalReachedStep())
            stepsToOptSetting.append(psoAlg.stat.getOptReachedStep())            
            print "batch run completed in", str(round(time.time()-starttime,2)), "s"

        #append to the goalSteps file
        if (set == 0):
            writeStepsToGoal(True)
        else:
            writeStepsToGoal()


#main
if not(os.environ.has_key('LOGPATH')):
    print "environment variable LOGPATH not set !"
    sys.exit(-1)
else:  
    LOGPATH = str(os.environ.get('LOGPATH'))


if (len(sys.argv)>1):
	batchFilename = os.path.abspath(sys.argv[1])
        loadParameter(batchFilename)
        print batchParameter.settings, "different Settings"
        if (len(sys.argv)==2):
            print "Start execution of " + batchFilename + " ? (Y/N)"
            startSetting = 0
        else:
            #startoffset given
            startSetting = int(sys.argv[2])
            #settings = range(len(batchParameter.param))[startSetting:]
            settings = range(batchParameter.settings)[startSetting:]
            print "resume for settings", settings, "?"

        #a=sys.stdin.readline(1)
        a='y'
        if (a == 'y') or (a=='Y'):
            if not os.path.exists(LOGPATH + '/' + batchParameter.logdir):
                print LOGPATH + '/' + paramSet.logdir, " does not exist"
                sys.exit(-1)
            else:
                batchExecution(startSetting)
        else:
            print "terminating"
            sys.exit(-1)


else:
	print "USAGE: python run_pso.py paramfile [startSetting]"
        sys.exit(-1)


            
#p = pso.PSO(W, 1.0, C1, C2, SIZE, DIM, HEIGHT, BRANCHES, OPTFUNCTION, SWARMTYPE)


    

