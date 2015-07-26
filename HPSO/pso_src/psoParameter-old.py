##defines
SWARM = ["SeqSwarm", "PyramidSwarm", "RingSwarm", "LocalSwarm"]
FUNCTION = ["Sphere", "Rastrigin", "Rosenbrock", "Schaffer", "Griewank","Ackley", "Schwefel", "Levy No.5"]
DISPLAYDIGITS = 3
##end defines

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
    
    def parameterString(self, i):
        out = SWARM[self.param[i][9]] +" - w=" + str(round(self.param[i][0],DISPLAYDIGITS)) + ", c1=" + str(round(self.param[i][2],DISPLAYDIGITS))+ ", c2=" + str(round(self.param[i][3],DISPLAYDIGITS))+ ", size=" + str(self.param[i][4])+ ", height=" + str(self.param[i][6])+ ", branch=" + str(self.param[i][7])
        return out

    def functionString(self):
        out = FUNCTION[self.param[0][8]] + " (dim=" + str(self.param[0][5]) + ")"
        return out



