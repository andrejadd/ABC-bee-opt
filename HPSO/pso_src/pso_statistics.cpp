#include "pso_statistics.h"

PsoStatistics::PsoStatistics(int steps_in, int height, int branches) {
	TRACE_IN("PsoStatistics::PsoStatistics");
	
	globalBest.reserve(steps_in);
	actualGlobalBest.reserve(steps_in);
	iterationBest.reserve(steps_in);
	average.reserve(steps_in);
	//only used for moving_peaks
	currentError.reserve(steps_in);
	//only used for DynamicPyramidSwarm
	gbestInSubswarm.reserve(steps_in);

	if (height == 0)
		numberLevels =0;
	else
		numberLevels = height-1;
	
	if (height > 0) {
	//array of #height vector pointers
		swappedPerLevel = new vector<int>[numberLevels];
		for (int i=0; i<numberLevels; i++) {
			swappedPerLevel[i].reserve(steps_in);
			swappedPerLevel[i].push_back(0);
		}
	}

	if (branches == 0)
		subtrees = 0;
	else
		subtrees = branches+1;

#ifdef DIVERSITY
	if (subtrees > 0) {
		//all subtrees (#branches) plus the random subtree at the end
		diversitySubtree = new vector<double>[subtrees];
		for (int i=0; i<subtrees; i++) {
			diversitySubtree[i].reserve(steps_in);
			//diversitySubtree[i].push_back(0);
		}
	}
#endif

	nrEvaluations.reserve(steps_in);
	nrEvaluations.push_back(0);
	
	psoStep = 0;
	currNrSwapped = 0;

	detectedChanges.reserve(steps_in);

	
	goalReachedStep = 0;
	optReachedStep = 0;

	TRACE_OUT("PsoStatistics::PsoStatistics",0);
}

PsoStatistics::~PsoStatistics() {
	TRACE_IN("PsoStatistics::~PsoStatistics");
	
//	printVec(globalBest);
	globalBest.clear();
	actualGlobalBest.clear();
//	printVec(iterationBest);
	iterationBest.clear();
//	printVec(average);
	average.clear();
//	for (int i=0; i<numberLevels; i++) {
//		printVec(swappedPerLevel[i]);
//		swappedPerLevel[i].clear();
//	}
	OUTPUT("Evaluations: " <<nrEvaluations.back());
	if (numberLevels > 0)
		delete[] swappedPerLevel;
#ifdef DIVERSITY
	if (subtrees > 0)
		delete[] diversitySubtree;
#endif
	OUTPUT("gbestInSubswarm: " << gbestInSubswarm.size());
	OUTPUT("detectedChanges: " << detectedChanges.size());


	detectedChanges.clear();
	gbestInSubswarm.clear();
	
	TRACE_OUT("PsoStatistics::~PsoStatistics",0);
}




void PsoStatistics::evaluated() {
	TRACEV_IN("PsoStatistics::swapped");

	(nrEvaluations.back())++;

	TRACEV_OUT("PsoStatistics::swapped",0);

}

void PsoStatistics::changeDetected(bool change) {
	TRACEV_IN("PsoStatistics::changeDetected");

	detectedChanges.push_back(change);
	

	TRACEV_OUT("PsoStatistics::changeDetected",0);

}

void PsoStatistics::swapped(int level) {
	TRACEV_IN("PsoStatistics::swapped");
	
	swappedPerLevel[level].back()++;

	currNrSwapped++;
	TRACEV_OUT("PsoStatistics::swapped",0);
}

void PsoStatistics::addDiversitySubtree(int subtree, double diversity) {
	TRACEV_IN("PsoStatistics::addDiversitySubtree");
	
	diversitySubtree[subtree].push_back(diversity);

	TRACEV_OUT("PsoStatistics::addDiversitySubtree",0);
}


void PsoStatistics::addGlobalBest(double val) {
	TRACEV_IN("PsoStatistics::addGlobalBest");
	
	globalBest.push_back(val);

	TRACEV_OUT("PsoStatistics::addGlobalBest",0);
}

void PsoStatistics::addActualGlobalBest(double val) {
	TRACEV_IN("PsoStatistics::addActualGlobalBest");
	
	actualGlobalBest.push_back(val);

	TRACEV_OUT("PsoStatistics::addActualGlobalBest",0);
}

void PsoStatistics::addIterationBest(double val) {
	TRACEV_IN("PsoStatistics::addIterationBest");
	
	iterationBest.push_back(val);

	TRACEV_OUT("PsoStatistics::addIterationBest",0);
}

void PsoStatistics::addCurrentError(double val) {
	TRACEV_IN("PsoStatistics::addCurrentError");
	
	currentError.push_back(val);

	TRACEV_OUT("PsoStatistics::addCurrentError",0);
}

void PsoStatistics::addGbestInSubswarm(int subswarm) {
	TRACEV_IN("PsoStatistics::addGbestInSubswarm");
	
	gbestInSubswarm.push_back(subswarm);

	TRACEV_OUT("PsoStatistics::addGbestInSubswarm",0);
}

void PsoStatistics::initAverage() {
	TRACEV_IN("PsoStatistics::initAverage");
	
	iterationAverage = 0;

	TRACEV_OUT("PsoStatistics::initAverage",0);
}

void PsoStatistics::addAverage(double val) {
	TRACEV_IN("PsoStatistics::addAverage");
	
	iterationAverage += val;

	TRACEV_OUT("PsoStatistics::addAverage",0);
}

void PsoStatistics::finishAverage(int size) {
	TRACEV_IN("PsoStatistics::finishAverage");
	
	average.push_back(iterationAverage/size);
	
	//calc mean deviation ...

	TRACEV_OUT("PsoStatistics::finishAverage",0);
}

void PsoStatistics::psoStepDone() {
	TRACEV_IN("PsoStatistics::psoStepDone");
	
	psoStep++;
	
	for (int i=0; i<numberLevels; i++)
		//start counting for next iteration, cumulative
		swappedPerLevel[i].push_back(swappedPerLevel[i].back());

	nrEvaluations.push_back(nrEvaluations.back());

	currNrSwapped = 0;


	
	TRACEV_OUT("PsoStatistics::psoStepDone",0);
}

void PsoStatistics::goalReached() {
	//if goalReachedStep is not zero, then the goal has been reached previously
	if (goalReachedStep == 0)
		goalReachedStep = psoStep;
		
}

void PsoStatistics::optReached() {
	//if optReachedStep is not zero, then the optimum value has been reached previously
	if (optReachedStep == 0)
		optReachedStep = psoStep;
		
}

double PsoStatistics::getGlobalBest(int step) {
	if (globalBest.size() > step)
		return globalBest[step];
	else { 
		cout << "getGlobalBest: not a valid step (" << step << ")" << endl;
		return 0.0;
	}
}

double PsoStatistics::getActualGlobalBest(int step) {
	if (actualGlobalBest.size() > step)
		return actualGlobalBest[step];
	else { 
		cout << "getActualGlobalBest: not a valid step (" << step << ")" << endl;
		return 0.0;
	}
}

double PsoStatistics::getIterationBest(int step) {
	if (iterationBest.size() > step)
		return iterationBest[step];
	else { 
		cout << "getIterationBest: not a valid step (" << step << ")" << endl;
		return 0.0;
	}
}

double PsoStatistics::getAverage(int step) {
	if (average.size() > step)
		return average[step];
	else { 
		cout << "not a valid step" << endl;
		return 0.0;
	}
}

double PsoStatistics::getCurrentError(int step) {
	if (currentError.size() > step)
		return currentError[step];
	else { 
		cout << "getCurrentError: not a valid step (" << step << ")" << endl;
		return 0.0;
	}
}

int PsoStatistics::getGbestInSubswarm(int step) {
	if (gbestInSubswarm.size() > step)
		return gbestInSubswarm[step];
	else { 
	  if (gbestInSubswarm.size() > 0) {
		cout << "isChangeDetected: not a valid step (" << step << ")" << endl;
	  }
		//if nothing is entered into gbestInSubswarm
	  return -1;
	}
}

int PsoStatistics::getNrSwapped(int step) {
	int nrSwapped = 0;
	if (numberLevels == 0)
	  return 0;
	else {
		if (swappedPerLevel[0].size() > step) {
			for (int i=0; i<numberLevels; i++)
				nrSwapped += getNrSwappedLevel(step, i);
			return nrSwapped;
		}
		else { 
			cout << "getNrSwapped: not a valid step (" << step << ")" << endl;
			return 0;
		}
	}
}

int PsoStatistics::getNrSwappedAt(int step) {
	return getNrSwapped(step) - getNrSwapped(step-1);
}

int PsoStatistics::getCurrentNrSwapped() {
	return currNrSwapped;
}

int PsoStatistics::getNrSwappedLevel(int step, int level) {

  if (numberLevels-1 < level)
    return 0;
  else {
    if (swappedPerLevel[level].size() > step) {
      return swappedPerLevel[level][step];
    }
    else { 
      cout << "not a valid step" << endl;
      return 0;
    }
  }
}

double PsoStatistics::getDiversitySubtree(int step, int subtree) {

	if (subtrees <= subtree)
		return 0.0;
	else {
		if (diversitySubtree[subtree].size() > step) {
			return diversitySubtree[subtree][step];
		}
		else {
			cout << "not a valid step" << endl;
			return 0.0;
		}
	}
}


int PsoStatistics::getNrEvaluations(int step) {
	if (nrEvaluations.size() > step)
		return nrEvaluations[step];
	else { 
		//if nothing is entered into gbestInSubswarm
		return 0;
	}
}

bool PsoStatistics::isChangeDetected(int step) {
  /*  if (detectedChanges.find(step) != detectedChanges.end())
		return true;
	else { 
		return false;
		}*/
	if (detectedChanges.size() > step)
	  return detectedChanges[step];
	else { 
	  cout << "isChangeDetected: not a valid step (" << step << ")" << endl;
		return 0.0;
	}
}


double* PsoStatistics::calculateSidestepStats(double initSidestep, vector<Bird*> swarm, int nrBirds, int dim) {
	double* sidesteps;
	double* stepsPerDim = new double[dim];
	for (int d=0; d<dim; d++)
		stepsPerDim[d] = 0.0;
	double step;
	for (int k=0; k<nrBirds; k++) {
		Bird *birdPtr=swarm[k];
		sidesteps = birdPtr->getSidesteps();
		for (int d=0; d<dim; d++) {
			step = sidesteps[d];
			while (step < initSidestep) {
				stepsPerDim[d]++;
				step *= 2;
			}				
		}
	}
	for (int d=0; d<dim; d++)
		stepsPerDim[d] = stepsPerDim[d]/nrBirds;
	
	return stepsPerDim;
}
