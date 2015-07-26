import pso
import os
import sys
import cPickle
import time


SWARM_DISPLAY = ["SEQ-SWARM", "PYRAMID-SWARM"]
FUNCTION_DISPLAY = ["Sphere", "Rastrigin", "Rosenbrock"]


psoAlg = None

filename = "log"

batchParameter = []


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
    out = str(psoAlg.stat.getGlobalBest(step)) + " " + str(float(psoAlg.stat.getNrSwapped(step))) + " " + str(float(psoAlg.stat.getNrSwappedLevel(step,0))) + " " + str(float(psoAlg.stat.getNrSwappedLevel(step,1))) + " " + str(float(psoAlg.stat.getNrSwappedLevel(step,2))) + "\n"
    #log nr swapped
#    out = str(psoAlg.stat.getGlobalBest(step)) + " " + str(float(psoAlg.stat.getNrSwapped(step))) + "\n"
    #only log quality
#    out = str(psoAlg.stat.getGlobalBest(step)) + "\n"
    file.write(out)

def newPSO(param):
    global psoAlg
    del psoAlg
    #old parameter
    #step delay set to 1
    #psoAlg = pso.PSO(param[0], param[1], param[2], param[3], param[4], 
    #    param[5], param[6], param[7], 1, param[8], param[9])
    #steps are set for maxSteps to adjust w
    if (len(param) == 12):
        #no vmax given
        psoAlg = pso.PSO(param[0], param[1], param[2], param[3], param[4], 
                     param[5], param[6], param[7], param[8], param[9],
                     param[10], param[11], batchParameter.steps/3)
    elif (len(param) == 13):
        #vmax ratio given
        psoAlg = pso.PSO(param[0], param[1], param[2], param[3], param[4], 
                     param[5], param[6], param[7], param[8], param[9],
                     param[10], param[11], batchParameter.steps/param[12])
        print "using vmax =",batchParameter.steps/param[12]

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
    global stepsToGoalSetting, stepsToOptSetting
    settings = range(len(batchParameter.param))[startSet:]
    for set in settings:
        print "*************  SETTING " + str(set) + " *************"
        param = getParameter(set)
        stepsToGoalSetting = []
        stepsToOptSetting = []
        for run in range(batchParameter.runs):
            #cool cpu
#            print "Break - sleep(60)"
#            time.sleep(60)
            starttime = time.time()
            print "new PSO(",param,")"
            newPSO(param)
            f = open(LOGPATH + '/' + batchParameter.logdir + '/' + filename+str(set)+"."+str(run), 'w')
            psoAlg.step(batchParameter.steps)
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
        print len(batchParameter.param), "different Settings"
        if (len(sys.argv)==2):
            print "Start execution of " + batchFilename + " ? (Y/N)"
            startSetting = 0
        else:
            #startoffset given
            startSetting = int(sys.argv[2])
            settings = range(len(batchParameter.param))[startSetting:]
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


    

