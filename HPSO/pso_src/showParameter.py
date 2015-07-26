import psoParameter
import sys
import os
import cPickle

batchParameter = None

def loadParameter(batchfilename):
    global batchParameter
    batchfile = open(batchfilename, 'r')
    batchParameter = cPickle.load(batchfile)
    batchfile.close()


if (len(sys.argv)>1):
	batchFilename = os.path.abspath(sys.argv[1])
        loadParameter(batchFilename)
        print batchParameter.settings, "different Settings"
        print batchParameter.functionString()
        print batchParameter


else:
	print "USAGE: python showParameter.py batchParameter"
        sys.exit(-1)
