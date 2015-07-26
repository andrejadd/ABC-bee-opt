#include "pso.h"
#include <vector> 
#include <sstream>
#include <list>


/*
extern void init_peaks ();
extern double eval_movpeaks (double *gen);
extern void change_peaks();
extern void free_peaks();
extern void set_geno_size(int);
*/

PSO::PSO(double w_in,
	double wMin_in,
	double c1_in,
	double c2_in,
	double vmaxRatio_in,
	double constriction_in,
	int size_in,
	int dim_in,
	int height_in,
	int branches_in,
	int swapDelay_in,
	function_type funct_in,
	noiseStyle_type noiseStyle_in,
	double noiseSigma_in,
	swarm_type swarmType_in,
	int nrScouts_in,
	int maxStep_in) {

	TRACE_IN("PSO::PSO"); 
	DEBUG("New PSO :");
	DEBUG1("w = ",w_in);
	DEBUG1("wMin = ",wMin_in);
	DEBUG1("c1 = ",c1_in);
	DEBUG1("c2 = ",c2_in);
	DEBUG1("vmaxRatio = ", vmaxRatio_in);
	DEBUG1("constriction = ", constriction_in);
	DEBUG1("size = ",size_in);
	DEBUG1("dim = ",dim_in);
	DEBUG1("height = ",height_in);
	DEBUG1("branches = ",branches_in);
	DEBUG1("swapDelay = ",swapDelay_in);
	DEBUG1("function_type = ",funct_in);
	DEBUG1("noiseStyle_type = ",noiseStyle_in);
	DEBUG1("noiseSigma = ",noiseSigma_in);
	DEBUG1("swarm_type = ",swarmType_in);
	DEBUG1("nrScouts = ", nrScouts_in);
	DEBUG1("maxStep = ", maxStep_in);
	
	int i; 
	size=size_in;
	dim=dim_in;
	
	height = height_in;
	branches = branches_in;
	swapDelay = swapDelay_in;
	
	w=w_in;

	//store the original w, cause w is possibly decreased (delta_w)
	wOrg = w_in;
	wMin = wMin_in;
	c1=c1_in;
	c2=c2_in;
	constriction = constriction_in;
	
	maxStep = maxStep_in;

	//w and wMin for seqSwarm are used to decrease w over time
	//this feature is disabled, because it is not required and might only be misleading
	//delta_w = (w - wMin) / maxStep;
	delta_w = 0.0;
	DEBUG1("delta_w = ", delta_w);
	
	//printf("This is PSO\n");
	
	functionType = funct_in;

	
	switch (functionType) {
		case sphere:
			optFunction = new Sphere(dim, staticFct);
			optimizationStyle = optMin;
			break;
		case rastrigin:
			optFunction = new Rastrigin(dim, staticFct);
			optimizationStyle = optMin;
			break;
		case rosenbrock:
			optFunction = new Rosenbrock(dim, staticFct);
			optimizationStyle = optMin;
			break;
		case schaffer:
			optFunction = new Schaffer(dim, staticFct);
			optimizationStyle = optMin;
			break;
		case griewank:
			optFunction = new Griewank(dim, staticFct);
			optimizationStyle = optMin;
			break;
		case ackley:
			optFunction = new Ackley(dim, staticFct);
			optimizationStyle = optMin;
			break;
		case schwefel:
			optFunction = new Schwefel(dim, staticFct);
			optimizationStyle = optMin;
			break;
		case levy5:
			optFunction = new Levy5(dim, staticFct);
			optimizationStyle = optMin;
			break;
		case freudenstein:
			optFunction = new Freudenstein(dim, staticFct);
			optimizationStyle = optMin;
			break;
		case quadric:
			optFunction = new Quadric(dim, staticFct);
			optimizationStyle = optMin;
			break;
		case movingGoal:
			optFunction = new MovingGoal(dim, dynamicFct);
			optimizationStyle = optMin;
			break;
		case dynamic_rosenbrock:
			optFunction = new Rosenbrock(dim, dynamicFct);
			optimizationStyle = optMin;
			break;						
		case dynamic_rastrigin:
			optFunction = new Rastrigin(dim, dynamicFct);
			optimizationStyle = optMin;
			break;			
		case dynamic_griewank:
			optFunction = new Griewank(dim, dynamicFct);
			optimizationStyle = optMin;
			break;			
		case moving_peaks:
			optFunction = new MovingPeaks(dim, dynamicFct);
			optimizationStyle = optMax;
			break;			
		case dynamic_levy:
			optFunction = new Levy5(dim, dynamicFct);
			optimizationStyle = optMin;
			break;			
		case dynamic_ackley:
			optFunction = new Ackley(dim, dynamicFct);
			optimizationStyle = optMin;
			break;			
		case dynamic_schwefel:
			optFunction = new Schwefel(dim, dynamicFct);
			optimizationStyle = optMin;
			break;			
		case dynamic_sinusoidal:
			optFunction = new Sinusoidal(dim, dynamicFct);
			optimizationStyle = optMin;
			break;			
		case zitzler3:
			optFunction = new Zitzler3(dim, staticFct);
			optimizationStyle = optMin;
			break;			
		default:
			cout << "No valid function selected" << endl;
			exit(-1);
	}

	optFunction->setNoiseStyle(noiseStyle_in);
	optFunction->setNoiseSigma(noiseSigma_in);

	//the ratio of max_x is computed for vmax
	//with vmax = 0 no vmax is enforced
	vmax = optFunction->get_max_x() * vmaxRatio_in;

	if (vmax == 0.0) {
		DEBUG("No vmax enforced");
	}
	else {
		DEBUG1("vmax set to ", vmax);
	}

	swarmType = swarmType_in;

	if ((swarmType == pyramidSwarm) || (swarmType == dynamicPyramidSwarm))
		//have to give an arbitrary number of steps, in order to be able to give the second argument
		//in case the pyramid is irregular, there is an extra level with some few nodes
		//stat = new PsoStatistics(1000, swarm->getActualHeight());
		stat = new PsoStatistics(1000, height+1, branches_in);
	else if (swarmType == adaptivePyramidSwarm) {
		//minBranchDegree is set in adaptiveParameters, but we need it now, so assume minimum 2
		stat = new PsoStatistics(1000, getMaxHeight(2,size));
//		stat = new PsoStatistics(1000, 5);
	}
	else if (swarmType == ringSwarm)
		//in the ringswarm only total swaps are counted, no levels
		//levels = height - 1 => 2 
		stat = new PsoStatistics(1000, 2);
	else
		//((swarmType == seqSwarm) || (swarmType == localSwarm) || (swarmType == scoutSwarm))
		stat = new PsoStatistics();

	
	switch (swarmType) {
		case seqSwarm:
			swarm = new Swarm(this, size, dim, optFunction->get_min_x(),
				optFunction->get_max_x(), apply_vmax(optFunction->get_min_v()), 
				apply_vmax(optFunction->get_max_v()), nrScouts_in);
	 		break;
	 	case pyramidSwarm:
			swarm = new PyramidSwarm(this, size, dim, optFunction->get_min_x(),
	 			optFunction->get_max_x(), apply_vmax(optFunction->get_min_v()), 
				apply_vmax(optFunction->get_max_v()), height_in, branches_in, swapDelay_in,
				nrScouts_in);
	 		break;
	 	case ringSwarm:
			swarm = new RingSwarm(this, dim, optFunction->get_min_x(),
	 			optFunction->get_max_x(), apply_vmax(optFunction->get_min_v()), 
				apply_vmax(optFunction->get_max_v()), height_in, branches_in, swapDelay_in);
	 		break;
	 	case localSwarm:
			swarm = new LocalSwarm(this, size, dim, optFunction->get_min_x(),
	 			optFunction->get_max_x(), apply_vmax(optFunction->get_min_v()), 
				apply_vmax(optFunction->get_max_v()), branches_in, swapDelay_in);
	 		break;
	 	case scoutSwarm:
			swarm = new ScoutSwarm(this, size, dim, optFunction->get_min_x(),
	 			optFunction->get_max_x(), apply_vmax(optFunction->get_min_v()), 
				apply_vmax(optFunction->get_max_v()), height_in, branches_in, swapDelay_in);
	 		break;
		case dynamicPyramidSwarm: {
			swarm = new DynamicPyramidSwarm(this, size, dim, optFunction->get_min_x(),
	 			optFunction->get_max_x(), apply_vmax(optFunction->get_min_v()), 
				apply_vmax(optFunction->get_max_v()), height_in, branches_in, swapDelay_in,
				nrScouts_in);
	 		break;
			}
		case adaptivePyramidSwarm: {
			swarm = new AdaptivePyramidSwarm(this, size, dim, optFunction->get_min_x(),
	 			optFunction->get_max_x(), apply_vmax(optFunction->get_min_v()), 
				apply_vmax(optFunction->get_max_v()), height_in, branches_in, swapDelay_in,
				nrScouts_in);
	 		break;
			}
		default:
			cout << "No valid swarm_type selected" << endl;
			exit(-1);
	}
	

	dynamicChange = false;

	dynamicParametersSet = false;

	noiseParametersSet = false;
	adaptiveParametersSet = false;
	localsearchParametersSet = false;
	_localsearchMode = nolocalsearch;
	
	TRACE_OUT("PSO::PSO",0);
}


PSO::~PSO() {
	TRACE_IN("PSO::~PSO");
	delete swarm;
	delete stat;
	delete optFunction;
	freeRng();
	TRACE_OUT("PSO::~PSO",0);
}


string PSO::printParameter(string first) {
	ostringstream out;

	out << first;

	switch (functionType) {
		case sphere: out << "Sphere"; break;
		case rastrigin: out << "Rastrigin"; break;
		case rosenbrock: out << "Rosenbrock"; break;
		case schaffer: out << "Schaffer"; break;
		case griewank: out << "Griewank"; break;
		case ackley: out << "Ackley"; break;
		case schwefel: out << "Schwefel"; break;
		case levy5: out << "Levy 5"; break;
		case freudenstein: out << "Freudenstein"; break;
		case quadric: out << "Quadric"; break;
		case movingGoal: out << "Dyn Sphere"; break;
		case dynamic_rosenbrock: out << "Dyn Rosenbrock"; break;
		case dynamic_rastrigin: out << "Dyn Rastrigin"; break;
		case dynamic_griewank: out << "Dyn Griewank"; break;
		case moving_peaks: out << "Moving Peaks"; break;
		case dynamic_levy: out << "Dyn Levy"; break;
		case dynamic_ackley: out << "Dyn Ackley"; break;
		case dynamic_schwefel: out << "Dyn Schwefel"; break;
		default: out << "Function" << functionType; break;
	}
	out << " dim=" << dim << endl;
	

	
	if (optFunction->getDynamicStyle() == dynamicFct) {
		out << first;
		switch (optFunction->get_updateStyle()) {
			case updatePeriodic:
				out << "updatePeriodic(" << optFunction->get_moveFrequency() << ")";
				break;
			case updateOnGoal: out << "updateOnGoal"; break;
			case updateRandom: out << "updateRandom"; break;
			default: out << optFunction->get_updateStyle(); break;
		}
		out << ", moveDist=" << optFunction->get_moveDistance();
		out << ", moveStyle=";
		switch (optFunction->get_optMoveStyle()) {
			case linearOptMove: out << "lin"; break;
			case exponentialLinearOptMove: out << "expLin"; break;
			default: out << optFunction->get_optMoveStyle(); break;
		}
		out << endl;
		out << first;
		out << "detect=";
		switch (swarm->get_detectionMethod()) {
			case noDetect: out << "no"; break;
			case upToDate: out << "up2Date"; break;
			case gbestDetectChange: out << "gbest"; break;
			case hierarchyChangeDetect:	out << "hierarchy"; break;
			case omniscientDetect:	out << "omniscient"; break;
			default: out << swarm->get_detectionMethod(); break;
		}
		out << ", response=";
		switch (swarm->get_responseMethod()) {
			case noResponse: out << "no"; break;
			case randomize10: out << "randomize10"; break;
			case randomize10reset: out << "randomize10reset"; break;
			case randomize16reset: out << "randomize16reset"; break;
			case randomize22reset: out << "randomize22reset"; break;
			case randomize40reset: out << "randomize40reset"; break;
			case subSwarmsTemporalMerge: out << "PH-PSO-t"; break;
			case subSwarmsTemporalMergeNoMemory: out << "PH-PSO-a-nomem"; break;
			case subSwarmsAdaptiveMerge: out << "PH-PSO-t"; break;
			case subSwarmsAdaptiveMergeNoMemory: out << "PH-PSO-a-nomem"; break;
			case updateOnly: out << "updateOnly"; break;
			case resetAll: out << "resetAll"; break;
			default: out << swarm->get_responseMethod(); break;
		}
		out << endl;
	}
	

	if (optFunction->getNoiseStyle() != noNoise) {
		out << first;
		out << "noiseStyle=";
		switch (optFunction->getNoiseStyle()) {
			case noisyFunctionValue: out << "FctValue"; break;
			case noisyPosition: out << "Position"; break;
			case noisyRelativeFunctionValue: out << "RelFctValue"; break;
			default: out << optFunction->getNoiseStyle(); break;
		}	

		out << ", sigma=" << optFunction->getNoiseSigma() << endl;
		out << first;
		
		if ((swarmType == pyramidSwarm) || (swarmType == dynamicPyramidSwarm) || (swarmType == adaptivePyramidSwarm)) {
			out << "hierarchDetThresh=" << hierachyChangeDetectThreshold;
			out << "nrSwapChecks=" << nrNoisySwapChecks;
			if (noisyReuseOldValue == reuseLastBestValue)
				out << ", reuseLastBestValue";
			else
				out << ", reuseOldValue=" << noisyReuseOldValue;
			
			if (noisyRefineBestValue)
				out << ", refineBestValue";
			if (noisyFirstComparePbestValues)
				out << ", firstComparePbestValues";
			out << endl;
		}
	}


	out << first;
	switch (swarmType) {
		case seqSwarm: out << "gbest PSO"; break;
		case localSwarm: out << "lbest PSO"; break;
		case pyramidSwarm: out << "H-PSO"; break;
		default: out << "PSO" << swarmType; break;
	}
	out << ", size=" << size;
	if (swarmType==localSwarm)
		out << ", nSize=" << branches;
	if (swarmType==pyramidSwarm)
		out << ", height=" << height << ", branches=" << branches << ", swapDelay=" << swapDelay;
	out << endl;

	out << first;
	out << "w=" << w << ", c1=" << c1 << ", c2=" << c2;

	out << ", vmax=" << vmax << ", constriction=" << constriction;

	out << endl;

	if (_localsearchMode != nolocalsearch) {
		out << first;
		if (_localsearchMode == doLocalsearch)
			out << "lsearch_freq=" << localsearchFrequency;
		else if (_localsearchMode == sidestep) {
			out << "initSidestep=" << _initialSidestep;
			out << ", sidestepMag=" << _sidestepMagnitude;
		}
	
		out << endl;
	}


	return out.str();
	/*DEBUG1("w = ",w_in);
	DEBUG1("wMin = ",wMin_in);
	DEBUG1("c1 = ",c1_in);
	DEBUG1("c2 = ",c2_in);
	DEBUG1("vmaxRatio = ", vmaxRatio_in);
	DEBUG1("constriction = ", constriction_in);
	DEBUG1("size = ",size_in);
	DEBUG1("dim = ",dim_in);
	DEBUG1("height = ",height_in);
	DEBUG1("branches = ",branches_in);
	DEBUG1("swapDelay = ",swapDelay_in);
	DEBUG1("function_type = ",funct_in);
	DEBUG1("noiseStyle_type = ",noiseStyle_in);
	DEBUG1("noiseSigma = ",noiseSigma_in);
	DEBUG1("swarm_type = ",swarmType_in);
	DEBUG1("nrScouts = ", nrScouts_in);
	DEBUG1("maxStep = ", maxStep_in);*/

}


double* PSO::getSidestepStats() {
	return stat->calculateSidestepStats(getInitialSidestepWidth(),swarm->getBirds(), size, dim);
}

void PSO::setDynamicParameters(int moveFrequency_in, double moveDistance_in, optimumMoveStyle_type optimumMoveStyle_in,
							   updateStyle_type updateStyle_in, detectionMethod_type detectionMethod_in, 
							   responseMethod_type responseMethod_in) {
	TRACE_IN("PSO::setDynamicParameters");

	optFunction->set_optMoveStyle(optimumMoveStyle_in);
	optFunction->set_updateStyle(updateStyle_in);
	optFunction->set_moveFrequency(moveFrequency_in);
	optFunction->set_moveDistance(moveDistance_in);
	swarm->set_detectionMethod(detectionMethod_in);
	swarm->set_responseMethod(responseMethod_in);

	dynamicParametersSet = true;
	TRACE_OUT("PSO::setDynamicParameters", 0);
}

void PSO::setNoiseParameters(int nrNoisySwapChecks_in, reuseOldValue_type noisyReuseOldValue_in, 
							 bool noisyRefineBestValue_in, bool noisyFirstComparePbestValues_in,
							 int hierachyChangeDetectThreshold_in) {
	TRACE_IN("PSO::setNoiseParameters");

	nrNoisySwapChecks = nrNoisySwapChecks_in;
	noisyReuseOldValue = noisyReuseOldValue_in;
	noisyRefineBestValue = noisyRefineBestValue_in;
	noisyFirstComparePbestValues = noisyFirstComparePbestValues_in;
	bestValueSamples = 0;
	hierachyChangeDetectThreshold = hierachyChangeDetectThreshold_in;


	noiseParametersSet = true;
	TRACE_OUT("PSO::setNoiseParameters", 0);
}


void PSO::setAdaptiveParameters(int decreaseBranchFrequency_in, int minBranchDegree_in, int decreaseBranchStep_in,decreaseBranchAction_type decreaseBranchAction_in) {

	TRACE_IN("PSO::setAdaptiveParameters");

	minBranchDegree = minBranchDegree_in;
	decreaseBranchFrequency = decreaseBranchFrequency_in;
	decreaseBranchStep = decreaseBranchStep_in;
	decreaseBranchAction = decreaseBranchAction_in;

	adaptiveParametersSet = true;
	TRACE_OUT("PSO::setAdaptiveParameters", 0);

}

void PSO::setLocalsearchParameters(localsearchMode_type mode, double localsearchParameter, double sidestepMagnitude) {

	TRACE_IN("PSO::setLocalsearchParameters");

	_localsearchMode = mode;

	if (_localsearchMode == doLocalsearch)
		localsearchFrequency = localsearchParameter;
	else if (_localsearchMode == sidestep) {
		_sidestepMagnitude = sidestepMagnitude;
		_initialSidestep = localsearchParameter;
		swarm->setInitialSidestep(getInitialSidestepWidth());
	}

	localsearchParametersSet = true;
	TRACE_OUT("PSO::setLocalsearchParameters", 0);

}


int PSO::step(int steps) {
	TRACE_IN1("PSO::step",steps);
	if ((getDynamicStyle() == dynamicFct) && !(dynamicParametersSet)) {
		cerr << "dynamic parameters not set" << endl;
		exit(-1);
	}
	if ((getNoiseStyle() != noNoise) && !(noiseParametersSet)) {
		cerr << "noise parameters not set" << endl;
		exit(-1);
	}
	if (!localsearchParametersSet) {
		cerr << "local search parameters not set" << endl;
		exit(-1);
	}
	int ok = 0;
	swarm->step(steps);
	
	if ((optFunction->getNoiseStyle() == noNoise) && (swarm->get_detectionMethod() != hierarchyChangeDetect)) {
		//with noise the solution cannot be tested properly
		//hierarchyChangeDetect runs for some time with outdated values
		//Test the solution
		vector<double> tmpVector;
		
		for (int i=0; i<dim; i++) {
			tmpVector.push_back(swarm->getGlobalBestBird()->getBestPos()[i]);
		}

		if (optFunction->evaluate(tmpVector) != swarm->getGlobalBestValue()) {
			cout << "globalBestBird (" << swarm->getGlobalBestBird()->getIndex() << "): " << swarm->getGlobalBestBird() << "=" << swarm->getGlobalBestBird()->getBestValue() << endl;
			cout << "optFunction->evaluate(tmpVector) != swarm->getGlobalBestValue() (Step " << stat->getCurrentStep() << ")" << endl;
			cout << "optFunction->evaluate(tmpVector) = " << optFunction->evaluate(tmpVector) << endl;
			cout << "swarm->getGlobalBestValue() = " << swarm->getGlobalBestValue() << endl;
			cout << "swarm->getActualGlobalBestValue() = " << stat->getActualGlobalBest(stat->getCurrentStep()-1) << endl;
			swarm->printSwarm();
			exit(-1);
		}
	}

	TRACE_OUT("PSO::step",ok);
	return ok;
}

bool PSO::isBetter(double val1, double val2) {
	TRACEV_IN("PSO::isBetter");
	bool retVal;
	if (optimizationStyle == optMax)
		retVal = (val1 > val2);
	else 
		//optimizationStyle == optMin
		retVal = (val1 < val2);
	TRACEV_OUT("PSO::isBetter",retVal);
	return retVal;
}

bool PSO::isBetterEqual(double val1, double val2) {
	TRACEV_IN("PSO::isBetterEqual");
	bool retVal;
	if (optimizationStyle == optMax)
		retVal = (val1 >= val2);
	else 
		//optimizationStyle == optMin
		retVal = (val1 <= val2);
	TRACEV_OUT("PSO::isBetterEqual",retVal);
	return retVal;
}

double PSO::getMax() {
	TRACEV_IN("PSO::getMax");
	double retVal;
	if (optimizationStyle == optMax)
		retVal = -MAXDOUBLE;
	else 
		//optimizationStyle == optMin
		retVal = MAXDOUBLE;
	TRACEV_OUT("PSO::getMax",retVal);
	return retVal;
}

int PSO::getActualHeight() {
	
	return swarm->getActualHeight();

}

int PSO::getActualBranches() {
	
	return swarm->getBranches();

}


Bird* PSO::getBird(int i) {
	TRACEV_IN("PSO::getBird");

	TRACEV_OUT("PSO::getBird",0);
	return swarm->getBird(i);
}

detectionMethod_type PSO::get_detectionMethod() {
	return swarm->get_detectionMethod();	
}

responseMethod_type PSO::get_responseMethod() {
	return swarm->get_responseMethod();	
}

void PSO::set_detectionMethod(detectionMethod_type detectionMethod_in) {
	swarm->set_detectionMethod(detectionMethod_in);
};

void PSO::set_responseMethod(responseMethod_type responseMethod_in) {
	swarm->set_responseMethod(responseMethod_in);
};

double PSO::getBestValue(int bird_i) {
	return swarm->getBird(bird_i)->getBestValue();	
}

double PSO::getCurrValue(int bird_i) {
	
	return swarm->getBird(bird_i)->getCurrentValue();	
}

double PSO::getVel(int bird_i) {
	
	return swarm->getBird(bird_i)->getEuclideanVelocity();
}

double PSO::getPos(int bird_i, int dim) {
	return swarm->getBird(bird_i)->getPos()[dim];	
}

double PSO::getBestPos(int bird_i, int dim) {
	return swarm->getBird(bird_i)->getBestPos()[dim];	
}

double PSO::getGlobalBestPos(int dim) {
	return swarm->getGlobalBestBird()->getBestPos()[dim];
}

double PSO::getOptPosition(int dim) {
	return optFunction->getOptPosition(dim);	
}

double PSO::getDistanceToGoal() {
	double dist = 0.0;
	for (int i=0; i<dim; i++) {
		dist += pow(swarm->getGlobalBestBird()->getBestPos()[i] - optFunction->getOptPosition(i),2.0);
	}
	return sqrt(dist);
}


double PSO::getMeanPosition(int dim) {
	return swarm->getMeanPosition()[dim];	
}

double PSO::getAverageDiversity() {
	return swarm->getAverageDiversity();
}

double PSO::getMedianDiversity() {
	return swarm->getMedianDiversity();
}

double PSO::getAverageDiversitySubtree(int subtreeNr) {
	if ((swarmType == pyramidSwarm) || (swarmType == dynamicPyramidSwarm) || (swarmType == adaptivePyramidSwarm))
		return swarm->getAverageDiversitySubtree(subtreeNr);
	else 
		return 0.0;
}

double PSO::getMedianDiversitySubtree(int subtreeNr) {
	if ((swarmType == pyramidSwarm) || (swarmType == dynamicPyramidSwarm) || (swarmType == adaptivePyramidSwarm))
		return swarm->getMedianDiversitySubtree(subtreeNr);
	else 
		return 0.0;
}


int PSO::getLevel(int bird_i) {
	//only reasonable with pyramidSwarm
	if ((swarmType == pyramidSwarm) || (swarmType == dynamicPyramidSwarm) || (swarmType == adaptivePyramidSwarm))
		return swarm->getPyramidLevelBird(bird_i);
	else if (swarmType == scoutSwarm) {
		if (swarm->getUseScoutBird(bird_i) == -1)
			//scout
			return 0;
		else
			//worker
			return 1;
	}
	else {
		if (swarm->isScout(bird_i) == true)	
			//scout
			return 0;
		else
			return 1;
	} 
		
		return 0;	
}

int PSO::getSubtree(int bird_i) {
	//only reasonable with pyramidSwarm
	if ((swarmType == pyramidSwarm) || (swarmType == dynamicPyramidSwarm) || (swarmType == adaptivePyramidSwarm))
		return swarm->getPyramidSubtreeBird(bird_i);
	else if (swarmType == scoutSwarm)
		//scout is -1, +1 is 0
		//workers follow scout [0,nrScouts], but subtree numbering starts with 1
		return swarm->getUseScoutBird(bird_i) + 1;
	else
		return 0;	
}


double PSO::evalFunction(const vector<double>& pos, bool clean) {
	TRACEV_IN("PSO::evalFunction");
	double retVal;
	if (clean == true)
		retVal = optFunction->evaluate(pos);
	else {
		//algorithm evaluation
		if (optFunction->getNoiseStyle() == noNoise)
			retVal = optFunction->evaluate(pos);
		else if (optFunction->getNoiseStyle() == noisyPosition)
			retVal = optFunction->evaluate(optFunction->addNoisePos(pos));
		else if (optFunction->getNoiseStyle() == noisyFunctionValue)
			retVal = optFunction->addNoiseVal(optFunction->evaluate(pos));
		else if (optFunction->getNoiseStyle() == noisyRelativeFunctionValue)
			retVal = optFunction->addNoiseRelativeVal(optFunction->evaluate(pos));
		stat->evaluated();
	}
	TRACEV_OUT("PSO::evalFunction",retVal);
	return retVal;
}

void PSO::iterationDone() {
	//apply local search
	
	if ((stat->getCurrentStep() >0) &&(localsearchFrequency > 0)) {
		if (stat->getCurrentStep() % localsearchFrequency == 0)
			getSwarm()->localSearch();
	}

	bool updateGoal = false;
	if (w > wMin)
		w = w - delta_w;

	/*AA Check if goal was reached */
	/*if (isBetterEqual(stat->getActualGlobalBest(stat->getCurrentStep()), optFunction->getGoal())) {
		stat->goalReached();
		//if the dynamic function update is done for updateOnGoal -> notify
		updateGoal = true;
		}*/
        
	/*if (isBetterEqual(stat->getActualGlobalBest(stat->getCurrentStep()), optFunction->getOpt()))
		stat->optReached();
	*/
	if (functionType == moving_peaks) {
		stat->addCurrentError(get_current_error());
	}


	//updateOnGoal function reads updateGoal bool value, updatePeriodic ignores
	dynamicChange = optFunction->update(stat->getCurrentStep(), updateGoal);
	
	stat->psoStepDone();
	
		
}

int PSO::get_dim() {
	return dim;
}
double PSO::get_w(int level) {
	double retVal;
	if (level == -1) {
		//scout use the maximum weight available
		retVal = MAX(wOrg, wMin);
		DEBUGV1("get_w for scout :", retVal);
		return retVal;
	}
	else if ((swarmType == pyramidSwarm) || (swarmType == dynamicPyramidSwarm) || (swarmType == adaptivePyramidSwarm)) {
		//TODO: specific w for divided dynamicPyramidSwarm
		if (height > 1) {
			//we should expect this for a pyramid
			//the top level (0) gets wMin and the bottom level wOrg
			
			//height is 1 bigger than maxvalue for level, therefore -1
			retVal = (wOrg - wMin)*(level)/(swarm->getActualHeight()-1) + wMin;
			DEBUGV2("get_w for level", level, retVal);
			return retVal;
		}
		else {
			//never executed
			cerr << "PSO::get_w(): no proper pyramid height given" << endl;
			exit(-1);
			return wMin;
		}
	}
	else if (swarmType == scoutSwarm)  {
		//the top level (0) = scouts gets wMin 
		//and the bottom level = workers wOrg
		retVal = (wOrg - wMin)*level + wMin;
		DEBUGV2("get_w for scoutswarm (scout0, worker1)", level, retVal);
		return retVal;
	}
	else {
		DEBUGV1("get_w", w);
		return w;
	}
}
double PSO::get_c1() {
	return c1;
}
double PSO::get_c2() {
	return c2;
}

double PSO::get_constriction() {
	return constriction;
}

function_type PSO::get_functionType() {
	return functionType;
}

double PSO::apply_vmax(double v) {
	if (vmax != 0.0) {
		//enforce vmax
		if (ABS(v) > vmax)
			if (v < 0)
				return -vmax;
			else
				return vmax;
		else
			return v;
	}
	else
		//no vmax enforced
		return v;
}


int PSO::getStep() {
	return stat->getCurrentStep();
}

double PSO::getInitialSidestepWidth() {
	return _initialSidestep*(optFunction->get_max_x()-optFunction->get_min_x());
}



double PSO::localSearch(vector<double> &localSearchVector, double oldValue) {
	OUTPUT("starting local search");
	deque<int> tmpList;
	deque<int> selectList;
	for (int i=0; i<dim; i++)
		tmpList.push_back(i);
	

	int tmpRand;

	//cout << "order: ";
	for (int i=dim-1; i>=0; i--) {
		tmpRand=randIntRange(0,i);
		selectList.push_back(tmpList.at(tmpRand));
		deque<int>::iterator iter = tmpList.begin();
		iter+=tmpRand;
		tmpList.erase(iter);
		//cout << selectList.back() << " ";
	}
	//cout << endl;


	while (!selectList.empty()) {
		int searchDim = selectList.front();
		selectList.pop_front();
		

		//the local search steps
		double moveDist = (optFunction->get_max_x() - optFunction->get_min_x())/2;
		for (int k=0; k<10; k++) {
			//the previous component at dimension #searchDim in the best position vector
			double oldComponent = localSearchVector[searchDim];

			//apply local search step
			localSearchVector[searchDim] += randDoubleRange(-1,1)*moveDist;

			//eval with the newly determined component
			double newValue = evalFunction(localSearchVector);
			if (isBetter(newValue, oldValue)) {
				oldValue = newValue;
				  }
			//reset to previous position
			else {
				localSearchVector[searchDim] = oldComponent;
			}
			moveDist = moveDist/2;
		}

	

	}

	return oldValue;
}



#ifndef MAIN
#define MAIN


int main(int argc, char** argv) {
	TRACE_IN("main");
	
	cout << ABS(sqrt(1.3)-5) << endl;
	
	int dim = 30;
	int size = 31;
	int nrScouts = 0;
	int height = 3;
	int branches = 5;
	int swapDelay = 1;
	double w = 0.729;
	double wmin = 0.729;
	
	//double w = 0.729;
	//double wmin = 0.729;
	//double c1 = 2.0;
	
	double c1 = 1.494;
	double c2 = c1;
	
	double vmaxRatio = 1.0;
	double constriction = 1.0;
	function_type function = sphere;
	noiseStyle_type noiseStyle = noNoise; //noisyPosition;
	double noiseSigma = 0.1;
	swarm_type swarm = pyramidSwarm;
	int moveFrequency = 1;
	double moveDistance = 1.024;
	optimumMoveStyle_type optimumMoveStyle = linearOptMove; //exponentialLinearOptMove;
	updateStyle_type updateStyle = updatePeriodic;
	detectionMethod_type detectionMethod = hierarchyChangeDetect; //noDetect;// upToDate; //gbestDetectChange;
	responseMethod_type responseMethod = noResponse; //randomize16reset;

	int nrNoisySwapChecks = 1;
	reuseOldValue_type noisyReuseOldValue = reuseLastBestValue;
	bool noisyRefineBestValue = true;
	bool noisyFirstComparePbestValues = false;
	int hierachyChangeDetectThreshold =5;

	int decBranchFreq=0;
	int minBranch=2;
	int decBranchStep=2;
	decreaseBranchAction_type decBranchAction = reset;

	swarm_type swarmArr[] = {adaptivePyramidSwarm, localSwarm, pyramidSwarm, pyramidSwarm, seqSwarm, dynamicPyramidSwarm, dynamicPyramidSwarm};

	noiseStyle_type noiseArr[] = {noNoise, noisyFunctionValue, noisyPosition};

	double wArr[] = {0.729, 0.729, 0.729};

	responseMethod_type responseArr[] = {noResponse, noResponse, randomize16reset};
	detectionMethod_type detectArr[] = {noDetect, gbestDetectChange, hierarchyChangeDetect};

	for (int ps=0; ps<1; ps++) {

		swarm = adaptivePyramidSwarm;
		w = wArr[ps];
		noiseStyle_type noise = noiseArr[ps];
		detectionMethod = gbestDetectChange;
		responseMethod = noResponse;

		cout << "setting " << ps << endl;

		PSO* p = new PSO(w, wmin, c1, c2, vmaxRatio, constriction, size, dim, height, branches, swapDelay, function, noise, noiseSigma, swarm, nrScouts, 10000);

		p->setDynamicParameters(moveFrequency, moveDistance, optimumMoveStyle, updateStyle, detectionMethod, responseMethod);
		p->setNoiseParameters(nrNoisySwapChecks, noisyReuseOldValue, noisyRefineBestValue, noisyFirstComparePbestValues, hierachyChangeDetectThreshold);
		p->setAdaptiveParameters(decBranchFreq,minBranch, decBranchStep, decBranchAction);

		p->setLocalsearchParameters(nolocalsearch, 0.0, 0.0);

		cout << p->printParameter() << endl;
	  
		int steps = 1000;
		int stepsize = 1;
		int step = 0;
		string outbuf[size];
 	
		for (int i=0; i< steps/stepsize; i++)  {
		  cout << .;
		  /*output every x steps */
	    	  if ((i>0)&&(i % 500 == 0 )) {
		
		    cout << "step: "<< step <<", best: " <<setprecision(10)<< p->stat->getActualGlobalBest(step-1) << endl;
			
 		}

		p->step(stepsize);

		step += stepsize;

		//if (i %5 == 0) {
		//	for (int ind = 0; ind < size; ind++) {
		//		cout << outbuf[ind];
		//		for (int i=0; i<dim; i++)
		//		  cout << p->getBird(ind)->getVel()[i] << " " ;
		//		cout << endl;
		//	}
		//	cout << endl;
		//}

//		double offlinePerformance = 0;
//		for (int i=0; i < step; i++) {
//			offlinePerformance += p->stat->getGlobalBest(i);
//		}
//		offlinePerformance /= step;
//		cout << "Offline Performance after " << step << " steps : " << offlinePerformance  << endl;

 	}

	cout << "step: "<< step <<", best: " <<setprecision(10)<< p->stat->getActualGlobalBest(step-1) << endl;

	double offlinePerformance = 0;

	for (int i=0; i < step; i++) {
		offlinePerformance += p->stat->getActualGlobalBest(i);
	}

	offlinePerformance /= step;
	cout << "Offline Performance after " << step << " steps : " << offlinePerformance  << endl;
	 	
	cout << "goalStep: " << p->stat->getGoalReachedStep() << endl;
	cout << "optStep: " << p->stat->getOptReachedStep() << endl;
	
	/*
	cout << "Gbest position:" << endl;
	
	for (int i=0; i<dim; i++)
	  cout << p->getGlobalBestPos(i) << " ";
	cout << endl;
	cout << "Opt position:" << endl
	for (int i=0; i<dim; i++)
		cout << p->getOptPosition(i) << " ";
	cout << endl;
	cout << "Distance :" << p->getDistanceToGoal() << endl;
	*/

	delete p;
	}
 	TRACE_OUT("main",0);
  return 0;
} 

#endif //MAIN

/*
int main() {
	vector<double> v;
	double x = 0.0;
	for (int i=0; i<1; i++)
		v.push_back(-1808.0385745);
	//v.push_back(-1.3068);
	//v.push_back(-1.4248);
	Schwefel* g = new Schwefel(10, dynamicFct);
	cout << setprecision(30) << g->evaluate(v) << endl;
	return 0;	
}

*/
