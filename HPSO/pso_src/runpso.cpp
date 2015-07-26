#include "pso.h"
#include <getopt.h>
#include <fstream>
#include <sstream>
#include <unistd.h>

#define SETTINGS 1

int intparam[SETTINGS][3] = {
	//steps, localSearch_freq, swarm_type
	{1000,0,2},
/*	{5000,10,0},
	{9000,100,0},
	{10000,0,3},
	{5000,10,3},
	{9000,100,3}*/
};

//GLOBAL PARAMETERS
int runs=1;

int dim = 30;
int size = 40;

int fctevals = 0; 
int maxevals=500000;
int steps=maxevals/size;

int height = 3;
int branches = 5; // nsize
int swapDelay = 1;
double w = 0.729;
double wmin = 0.729;
double c1 = 1.494;
double c2 = c1;
double vmaxRatio = 0.0;
double constriction = 1.0;
int outsteps = 0;

// AA 
int PRINTMODE = 0;

//dynamic parameter
int moveFrequency = 1;
double moveDistance = 1.024;
optimumMoveStyle_type optimumMoveStyle = linearOptMove; //exponentialLinearOptMove;
updateStyle_type updateStyle = updatePeriodic;
detectionMethod_type detectionMethod = hierarchyChangeDetect; //noDetect;// upToDate; //gbestDetectChange;
responseMethod_type responseMethod = noResponse; //randomize16reset;

//noisy parameter
noiseStyle_type noiseStyle = noNoise; //noisyPosition;
double noiseSigma = 0.1;

int nrNoisySwapChecks = 1;
reuseOldValue_type noisyReuseOldValue = reuseLastBestValue;
bool noisyRefineBestValue = true;
bool noisyFirstComparePbestValues = false;
int hierachyChangeDetectThreshold =5;


/*AA specifies the type of PSO swarm here */
swarm_type swarm = pyramidSwarm;

/*AA specifies the test function here */
function_type function = sphere;

localsearchMode_type localsearch = nolocalsearch;
double localsearchParameter = 0.01;
double sidestepMagnitude = 10;


ofstream sidestepStat_out;
char* sidestepStatFile = NULL;
double* avgSidestepStats;


int readOptions(int argc, char** argv) {
	
		int c;
	
	   static struct option long_options[] =
	     {
		   /* These options set a flag. */
		   //{"verbose", no_argument,       &verbose_flag, 1},
		   /* These options don't set a flag.
		  We distinguish them by their indices. */
		   {"sidestep",     no_argument,       0, 's'},
		   {"dim",  required_argument, 0, 'd'},
		   {"pso",  required_argument, 0, 'p'},
		   {"height",  required_argument, 0, 'h'},
		   {"branches",  required_argument, 0, 'b'},
		   {"outsteps",  required_argument, 0, 'o'},
		   {"sidestepMag",  required_argument, 0, 'M'},
		   {"sidestepInit",  required_argument, 0, 'I'},
		   {"sidestepFile",  required_argument, 0, 'S'},
		   {"seed",  required_argument, 0, 'r'},
		   {"dynStyle",  required_argument, 0, 'D'},
		   {"moveFreq",  required_argument, 0, 'F'},
		   {"moveDist",  required_argument, 0, 'W'},
		   {"moveStyle",  required_argument, 0, 'Y'},
		   {"detect",  required_argument, 0, 'C'},
		   {"response",  required_argument, 0, 'R'},
		   {"noiseStyle",  required_argument, 0, 'N'},
		   {"noiseSigma",  required_argument, 0, 'O'},
		   {"noiseHThresh",  required_argument, 0, 'T'},
		   {0, 0, 0, 0}
	 };

	/* getopt_long stores the option index here. */
	int option_index = 0;
	
    while ((c = getopt_long(argc, argv, "M:I:S:n:r:h:b:d:p:f:so:D:F:W:Y:C:R:N:O:T:", long_options, &option_index)) != -1)
         switch (c)
           {
           	case 'd':
           	dim = atoi(optarg);
           	break;
           	//case 'v':
           	//sscanf(optarg, "%lf,%lf", &maxVel[0], &maxVel[1]);
           	//break;
           case 'o':
        	 outsteps = atoi(optarg);
        	 break;
           case 'n':
        	 size = atoi(optarg);
        	 break;
           case 'h':
        	 height = atoi(optarg);
        	 break;
           case 'b':
        	 branches = atoi(optarg);
        	 break;
           case 'D':
        	 updateStyle = (updateStyle_type) atoi(optarg);
        	 break;
	       case 'F':
        	 moveFrequency = atoi(optarg);
        	 break;
           case 'W':
        	 moveDistance = atof(optarg);
        	 break;
           case 'Y':
        	 optimumMoveStyle = (optimumMoveStyle_type) atoi(optarg);
        	 break;
           case 'C':
        	 detectionMethod = (detectionMethod_type) atoi(optarg);
        	 break;
           case 'R':
        	 responseMethod = (responseMethod_type) atoi(optarg);
        	 break;
           case 'N':
        	 noiseStyle = (noiseStyle_type) atoi(optarg);
        	 break;
           case 'O':
        	 noiseSigma = atof(optarg);
        	 break;
           case 'T':
        	 hierachyChangeDetectThreshold = atof(optarg);
        	 break;
           case 'M':
        	 sidestepMagnitude = atof(optarg);
        	 break;
           case 'I':
        	 localsearchParameter = atof(optarg);
        	 break;
           case 'S':
             sidestepStatFile = optarg;
             sidestepStat_out.open(sidestepStatFile, ios::out);
             break;        	 
           case 'f':
        	 function = (function_type) atoi(optarg);
        	 break;
           case 'p':
        	 swarm = (swarm_type) atoi(optarg);
        	 break;
           case 'r':
        	 fixSeed(atoll(optarg));
        	 break;
           case 's':
        	 localsearch = sidestep;
        	 break;
           case '?':
             if (isprint (optopt))
               fprintf (stderr, "Unknown option `-%c'.\n", optopt);
             else
               fprintf (stderr,
                        "Unknown option character `\\x%x'.\n",
                        optopt);
             return 1;
           default:
             abort ();
           }
  	
	
	
	if (optind + 7 != argc) {
		cerr << "usage: runpso [runs] [steps] [function_type] [swarm_type] [param_type] [PRINTMODE]" << endl;
		exit(-1);
	}
	
	
	runs = atoi(argv[optind]);
	//steps = atoi(argv[optind+1]);
	fctevals = atoi(argv[optind+1]);
	function =  (function_type) atoi(argv[optind+2]);
	swarm = (swarm_type) atoi(argv[optind+3]);
	int paramtype =  atoi(argv[optind+4]);

	/* 0: prints evals to reach the fct goal, 1: prints mean solution value per step */
	PRINTMODE = atoi(argv[optind+5]);
	size = atoi(argv[optind+6]);

	if(function == schaffer) {
	  dim = 2;
	}

	/* set -a type params for w and c1, c2 */
	if(paramtype == 0) { 
	  w = 0.6;
	  wmin = 0.6;
	  c1 = 1.7;
	  c2 = c1;
	} else if (paramtype == 1) { /* set -b type */
	  w = 0.729;
	  wmin = 0.729;
	  c1 = 1.494;
	  c2 = c1;
	} else if (paramtype == 2) { /*AA , VH-PSO */
	  w = 0.4;
	  wmin = 0.729;
	  c1 = 1.494;
	  c2 = c1;
	} else if (paramtype == 3) { /*AA  */
	  w = 0.729;
	  wmin = 0.4;
	  c1 = 1.7;
	  c2 = c1;
	} else if (paramtype == 4) { /*AA , VH-PSO, branch degree = 2, Special for Ackley */
	  w = 0.4;
	  wmin = 0.729;
	  c1 = 1.49;
	  c2 = c1;
	  branches = 2;
	} else if (paramtype == 5) { /*AA , VH-PSO */
	  w = 0.4;
	  wmin = 0.729;
	  c1 = 1.7;
	  c2 = c1;
	} 


	if (outsteps == 0) {
		outsteps = steps;
	}
	
}

int main(int argc, char** argv) {
	TRACE_IN("main");
	cout.precision(10);
	
	
	readOptions(argc, argv);

	int decBranchFreq=0;
	int minBranch=2;
	int decBranchStep=2;
	decreaseBranchAction_type decBranchAction = reset;


	if (sidestepStatFile != NULL) {
		avgSidestepStats = new double[dim];
		for (int d=0; d<dim; d++)
			avgSidestepStats[d] = 0.0;
	}

	/* number of steps needed to have evals function */
	steps = (fctevals / size);

	cout << "# steps: " << steps << " fctevals: " << fctevals << "\n";
	  
	/* best value in each step for every run, needs lots of memory */
	double lda_bestvalues[steps];
	double lia_evalcount[steps];	

	/* init PSO */
	PSO * p = new PSO(w, wmin, c1, c2, vmaxRatio, constriction, size, dim, height, branches, swapDelay, function, noiseStyle, noiseSigma, swarm);

	p->setLocalsearchParameters(localsearch, localsearchParameter, sidestepMagnitude);
	
	p->setDynamicParameters(moveFrequency, moveDistance, optimumMoveStyle, updateStyle, detectionMethod, responseMethod);
	
	p->setNoiseParameters(nrNoisySwapChecks, noisyReuseOldValue, noisyRefineBestValue, noisyFirstComparePbestValues, hierachyChangeDetectThreshold);
	
	if(swarm == (swarm_type)adaptivePyramidSwarm) {
	  p->setAdaptiveParameters(decBranchFreq,minBranch, decBranchStep, decBranchAction);
	}	
	
	cout << p->printParameter("#");
			
	if (sidestepStatFile != NULL) { 
	  sidestepStat_out << "#step | avg number sidesteps/dim | for each dim ..." << endl;
	}

	double ld_bestvalue = 1E100;
	int li_evals = 0;
	bool lb_goalreached = false;
	
	int i = 0;
	while (li_evals < fctevals) {
  	
	  p->step(1);
	  
	  /* get current global best */
	  double ld_currbest = p->stat->getGlobalBest(i++);
	  
	  if(i==0) {
	    ld_bestvalue = ld_currbest;
	  } else {
	    if(ld_currbest<ld_bestvalue) {
	      ld_bestvalue = ld_currbest;
	    }
	  }
	  
	  lda_bestvalues[i] = ld_bestvalue;
	  
	  li_evals = p->stat->getNrEvaluations(i);
	  
	  lia_evalcount[i] = li_evals;

	  if(PRINTMODE == 1) {
	    cout << li_evals << " " << ld_bestvalue << "\n";
	  }

	  /* On PRINTMODE '1' optimization runs until max steps */
	  if(PRINTMODE != 1) {
	    
	    /* check if goal was reached in this step */
	    double ld_goal = p->optFunction->getGoal();
	    if(ld_bestvalue <= ld_goal) {
	      lb_goalreached = true;
	      break;
	    }
	  }
	}
	
	delete p;
	
	if(PRINTMODE == 0) {
	  if(lb_goalreached) {
	    cout << li_evals << endl;
	  } else {
	    cout << "-1" << endl;
	  }
	}
	
	if (sidestepStatFile != NULL) {
	  sidestepStat_out << "---" << endl;
	}
	
	if (sidestepStatFile != NULL) {
	  sidestepStat_out.close();
	}
	
 	TRACE_OUT("main",0);
	return 0;
}
