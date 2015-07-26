import cPickle
import os
import sys

from psoParameter import *

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



paramSet = PsoParameter()

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

function = 3
dim = 2
size = 31

paramSet.param = [
    #seqSwarm
    [0.4, 0.4, 1.494, 1.494, 0.0, size, dim, 0, 0, 1, function, 0],
    [0.6,   0.6,   1.7,   1.7,   0.0, size, dim, 0, 0, 1, function, 0],
    [0.729, 0.729, 1.494, 1.494, 1.0, size, dim, 0, 0, 1, function, 0],
    [0.4, 0.4, 1.494, 1.494, 1.0, size, dim, 0, 0, 1, function, 0],
    [0.6,   0.6,   1.7,   1.7,   1.0, size, dim, 0, 0, 1, function, 0],
##    [0.9,   0.4,   2.0,   2.0,   1.0, size, dim, 0, 0, 1, function, 0],
##    [0.9,   0.4,   2.0,   2.0,   1.0, size, dim, 0, 0, 1, function, 0, 10],
###    [0.729, 0.4,   1.494, 1.494, 1.0, size, dim, 0, 0, 1, function, 0],
##    #pyramidSwarm
##    #branch 2
###    [0.729, 0.729, 1.494, 1.494, 0.0, size, dim, 5, 2, 1, function, 1],
###    [0.729, 0.4,   1.494, 1.494, 1.0, size, dim, 5, 2, 1, function, 1],
###    [0.9,   0.4,   2.0,   2.0,   1.0, size, dim, 5, 2, 1, function, 1],
###    [0.4,   0.729,   1.494, 1.494, 1.0, size, dim, 5, 2, 1, function, 1],
###    [0.4,   0.9,   2.0,   2.0,   1.0, size, dim, 5, 2, 1, function, 1],
##    #branch 3
###    [0.729, 0.729, 1.494, 1.494, 0.0, size, dim, 4, 3, 1, function, 1],
###    [0.729, 0.0,   1.494, 1.494, 1.0, size, dim, 3, 3, 1, function, 1],
##    #branch 4
###    [0.729, 0.729, 1.494, 1.494, 0.0, size, dim, 3, 4, 1, function, 1],
###    [0.729, 0.0,   1.494, 1.494, 1.0, size, dim, 3, 4, 1, function, 1],
##    #branch 5
    [0.729, 0.729, 1.494, 1.494, 1.0, size, dim, 3, 5, 1, function, 1],
    [0.4, 0.4, 1.494, 1.494, 0.0, size, dim, 3, 5, 1, function, 1],
    [0.4, 0.4, 1.494, 1.494, 1.0, size, dim, 3, 5, 1, function, 1],
    [0.729, 0.4,   1.494, 1.494, 0.0, size, dim, 3, 5, 1, function, 1],
##    [0.9,   0.4,   2.0,   2.0,   1.0, size, dim, 3, 5, 1, function, 1],
    [0.4,   0.729,   1.494, 1.494, 0.0, size, dim, 3, 5, 1, function, 1]
##    [0.4,   0.9,   2.0,   2.0,   1.0, size, dim, 3, 5, 1, function, 1],
##    #branch 7
###    [0.729, 0.729, 1.494, 1.494, 0.0, size, dim, 2, 7, 1, function, 1],
##    #branch 10
###    [0.729, 0.729, 1.494, 1.494, 0.0, size, dim, 2, 10, 1, function, 1],
##    #ringSwarm
##    #[0.729, 1.0, 1.494, 1.494, 32, 2, 8, 4, 3, 2],
##    #localSwarm
##    [0.729, 0.729, 1.494, 1.494, 0.0, size, dim, 0, 1, 1, function, 3],
###    [0.729, 0.0, 1.494, 1.494, 1.0, size, dim, 0, 1, 1, function, 3],
##    [0.9,   0.4,   2.0,   2.0,   1.0, size, dim, 0, 1, 1, function, 3]
    ]

    
#logdir relative to $LOGPATH
paramSet.logdir = "pso/paper/schaffer31-decw-04"
paramSet.steps = 10000
paramSet.stepwidth = 10
paramSet.runs = 50

#attributes have to be set in run_pso.py
paramSet.attributeslist = [0,1,2,3,4]
paramSet.attributesDesc = ['Global Best', 'Number Swapped', 'Swapped Lev 0', 'Swapped Lev 1', 'Swapped Lev 2']

paramSet.description = "Introducing decreasing w\nAdditional Tests for novmax with decw pyramid and 0.4 settings\n"


print "The following parameters will be written"
print paramSet

if not os.path.exists(LOGPATH + '/' + paramSet.logdir):
    print LOGPATH + '/' + paramSet.logdir, " does not exist"
    sys.exit(-1)

print "writing", PARAMETER_FILENAME, "in", LOGPATH + '/' + paramSet.logdir
print "Ok ?(Y/N)"
a=sys.stdin.readline(1)
if (a == 'y') or (a=='Y'):

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
