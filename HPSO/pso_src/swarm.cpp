#include "swarm.h" 

//TODO: Statistics logging with current values, regardless of detectChange

 
Swarm::Swarm(PSO *pso_p,
						int size,
						int dim,
						double min_x_in,
						double max_x_in,
						double min_v_in,
						double max_v_in,
						int nrScouts_in
) {
	TRACE_IN("Swarm::Swarm");
	

	pso = pso_p;
	globalBestBird = NULL;
	globalBestScout = NULL;
	swarmsize = size;
	nrScouts = nrScouts_in;
	
	min_x = min_x_in;
	max_x = max_x_in;
	min_v = min_v_in;
	max_v = max_v_in;


	detectionMethod = gbestDetectChange;
	responseMethod = subSwarmsTemporalMerge;

	DEBUG("New Swarm :");
	DEBUG1("pso = ",pso_p);
	DEBUG1("size = ",size);
	DEBUG1("dim = ",dim);
	DEBUG1("min_x = ",min_x);
	DEBUG1("max_x = ",max_x);
	DEBUG1("min_v = ",min_v);
	DEBUG1("max_v = ",max_v);
	DEBUG1("nrScouts = ",nrScouts);

	
	//birds = new Bird *[size];
	//birds = vector<Bird*>;
	scouts = new Bird *[nrScouts];

	//create the birds
	createSwarm(min_x, max_x, min_v, max_v);
	createScouts(min_x, max_x, min_v, max_v);

	lastBirdIndex = swarmsize - 1;
	
	
#ifdef DIVERSITY
	for (int i=0; i<pso->get_dim(); i++)
		meanPosition.push_back(0);
	
	updateMeanPosition();	
#endif

	TRACE_OUT("Swarm::Swarm",0);
	
}

Swarm::~Swarm() {
	TRACE_IN("Swarm::~Swarm");
	
	for (int i = 0; i < swarmsize; i++) {
		delete birds[i];
	}
	birds.clear();
	for (int i = 0; i < nrScouts; i++) {
		delete scouts[i];
	}
	delete[] scouts;
	
	TRACE_OUT("Swarm::~Swarm",0);
}

void Swarm::localSearch() {
	vector<double> localSearchVector;
	//warum auch immer das nicht funktioniert???
	//localSearchVector.assign(bird->getBestPos().begin(), bird->getBestPos().end());
	for (int d=0; d<pso->get_dim(); d++)
		localSearchVector.push_back(globalBestBird->getBestPos()[d]);
	double oldValue = globalBestBird->getBestValue();

	oldValue = pso->localSearch(localSearchVector, oldValue);

	//write back the new position
	double dist= 0.0;
	for (int d=0; d<pso->get_dim(); d++)
		dist += pow(globalBestBird->getBestPos()[d] - localSearchVector[d], 2.0);
	dist = sqrt(dist);

	OUTPUT("improved from " << globalBestBird->getBestValue() << " to " << oldValue << ". Dist=" << dist);

	globalBestBird->setBestValue(oldValue);
	globalBestBird->setBestPos(localSearchVector);

}

void Swarm::printSwarm() {
	for (int i = 0; i < swarmsize; i++) {
		Bird *birdPtr=birds[i];
	    cout <<  birdPtr->getIndex() << 
		" " << birdPtr << " " << birdPtr->getIndex() << ": " << (birdPtr)->getBestValue()
    	 << endl;
	}

}

Bird* Swarm::createBird(double min_x, double max_x, double min_v, double max_v, int index)
{
	TRACE_IN("Swarm::createBird");

	vector<double> tmpPos;
	vector<double> tmpVel;
	for (int j = 0; j < pso->get_dim(); j++) {
		tmpPos.push_back(randDoubleRange(min_x, max_x));
		tmpVel.push_back(randDoubleRange(min_v, max_v));
	}
	DEBUGV1("bird spawned at ", printVec(tmpPos));
	DEBUGV1("with velocity ", printVec(tmpVel));

	TRACE_OUT("Swarm::createBird",index);

	return new Bird(pso, this, tmpPos, tmpVel, index);

	
}



Bird* Swarm::createNewBird(double min_x, double max_x, double min_v, double max_v)
{
	TRACE_IN("Swarm::createNewBird");

	lastBirdIndex ++;
	birds.push_back(createBird(min_x, max_x, min_v, max_v, lastBirdIndex));
	swarmsize++;

	TRACE_OUT("Swarm::createNewBird",lastBirdIndex);

	return birds.back();

	
}

void Swarm::removeBird(int index) {
	TRACE_IN("Swarm::removeBird");

	vector<Bird*>::iterator removeNode;
	int removed = 0;
	for ( vector<Bird*>::iterator iter = birds.begin( ) ; iter != birds.end( ) ; iter++ ) {
		if ((*iter)->getIndex() == index) {
			removeNode = iter;
			removed++;
		}
	}

	birds.erase(removeNode);
	assert(removed==1);

	swarmsize--;
	
	TRACE_OUT("Swarm::removeBird", index);
}


void Swarm::createSwarm(double min_x, double max_x, double min_v, double max_v)
{
	TRACE_IN("Swarm::createSwarm");
	for (int i=0; i<swarmsize; i++) {
		//birds[i] = createBird(min_x, max_x, min_v, max_v, i);
		birds.push_back(createBird(min_x, max_x, min_v, max_v, i));
	}
	TRACE_OUT("Swarm::createSwarm",0);
	
}

void Swarm::createScouts(double min_x, double max_x, double min_v, double max_v)
{
	TRACE_IN("Swarm::createScouts");
	for (int i=0; i<nrScouts; i++) {
		scouts[i] = createBird(min_x, max_x, min_v, max_v);
	}
	TRACE_OUT("Swarm::createScouts",0);
	
}


double Swarm::getGlobalBestValue() {
	TRACE_IN("Swarm::getGlobalBestValue");
	
	TRACE_OUT("Swarm::getGlobalBestValue",0);
	return globalBestBird->getBestValue();
}


Bird* Swarm::getBird(int i) {
	if (i<swarmsize)
		return birds[i];
	else
		return scouts[i-swarmsize];
}

bool Swarm::isScout(int i) {
	if (i<swarmsize)
		return false;
	else
		return true;
}


Bird* Swarm::evaluatePos() {
	TRACE_IN("Swarm::evaluatePos");
	double currValue;
	//new iteration best
	double itrBest = pso->getMax();
	Bird* itrBestBird = NULL;

	pso->stat->initAverage();
	for (int k=0; k<swarmsize; k++) {
		Bird *birdPtr=birds[k];
			
		//evaluate the current position
		currValue = birdPtr->evalCurrentPos();

		if (pso->isBetter(currValue, itrBest)) {
			itrBestBird = birds[k];
			itrBest = currValue;
		}
		pso->stat->addAverage(currValue);
		DEBUGV2("Bird ",k ,currValue);
#ifdef DBUGV
		cout << "at " << printVec(birdPtr->getPos()) << endl;
#endif
	}
	pso->stat->finishAverage(swarmsize);


	//protocol the iteration best bird, global best is done in compare()
	//pso->stat->addIterationBest(itrBestBird->getCurrentValue());
	pso->stat->addIterationBest(pso->evalFunction(itrBestBird->getPos(), true));
	
	DEBUG2("itrBest ", itrBestBird, itrBestBird->getCurrentValue());

	
/* test no longer required
	if (itrBest != itrBestBird->getCurrentValue()) {
		cout <<"(itrBest != itrBestBird->evalCurrentPos()) !!!!" <<endl;
		exit(-1);
	}	
	*/

	TRACE_OUT("Swarm::evaluatePos",0);
	return itrBestBird;
}

Bird* Swarm::evaluateScouts() {
	TRACE_IN("Swarm::evaluateScouts");
	//TODO: Compare Scouts with itrBestBird
	//TODO: Scout stat
	double currValue;
	//compare to swarm iteration best
	double itrBest = pso->getMax();
	Bird* itrBestScout = NULL;

	//pso->stat->initAverage();
	
	//evaluate scouts
	for(int k=0; k<nrScouts; k++) {
		Bird *birdPtr=scouts[k];
			
		//evaluate the current position
		currValue = birdPtr->evalCurrentPos();

		if (pso->isBetter(currValue, itrBest)) {
			itrBestScout = scouts[k];
			itrBest = currValue;
		}
		//pso->stat->addAverage(currValue);
		DEBUG2("Scout ",k ,currValue);
#ifdef DBUGV
		cout << "at " << printVec(birdPtr->getPos());
#endif
	}

	//pso->stat->finishAverage(swarmsize);

	
	TRACE_OUT("Swarm::evaluateScouts",0);
	return itrBestScout;

}	

void Swarm::compare(Bird* itrBestBird) {
	TRACE_IN("Swarm::compare");


	//for step 1 just pick first as global best
	if (globalBestBird == NULL) {
		globalBestBird = birds[0];
	}

	
	//evaluate all the best positions
	//with dynamic functions an old personal best position may have become global best

	for (int k=0; k<swarmsize; k++) {
		Bird *birdPtr=birds[k];

		if (pso->isBetter(birdPtr->getBestValue(), globalBestBird->getBestValue())) {
			globalBestBird = birdPtr;
		}
	}



/*changed 23/08/04
	//for step 1 just pick iteration best as global best
	if (globalBestBird == NULL) {
		globalBestBird = itrBestBird;
	}
	else {
		//pick new global best 
	    //TODO: itrBestBird->getBestValue() ? in case of dynamic optimization ...
		if (pso->isBetter(itrBestBird->getCurrentValue(), globalBestBird->getBestValue())) {
			globalBestBird = itrBestBird;
		}
	}
*/


	//protocol the global best bird (iteration best is done in evaluatePos()
	//we re-evaluate the best position of the best individual, because the bestValue
	//may be distorted by noise -- force evaluation without noise
	pso->stat->addGlobalBest(globalBestBird->getBestValue());
	pso->stat->addActualGlobalBest(pso->evalFunction(globalBestBird->getBestPos(), true));
	
	DEBUG2("gloBest ", globalBestBird, globalBestBird->getBestValue());
	

	TRACE_OUT("Swarm::compare",0);
}

void Swarm::compareScouts(Bird* itrBestScout) {
	TRACE_IN("Swarm::compareScouts");
	//for step 1 just pick iteration best as global best
	if (globalBestScout == NULL) {
		globalBestScout = itrBestScout;
	}
	else {
		//pick new global best 
		if (pso->isBetter(itrBestScout->getCurrentValue(), globalBestScout->getBestValue())) {
			globalBestScout = itrBestScout;
		}
	}

	//protocol the global best scout (iteration best is done in evaluatePos()
	//TODO:
	//pso->stat->addGlobalBestScout(globalBestScout->getBestValue());
	
	DEBUG2("bestScout ", globalBestScout, globalBestScout->getBestValue());

	TRACE_OUT("Swarm::compareScouts",0);
}

void Swarm::applyScout(bool force) {
	TRACE_IN("Swarm::applyScout");
	bool apply = false;

	if (force)
		apply = true;
	else
		apply = (pso->isBetter(globalBestScout->getBestValue(), globalBestBird->getBestValue()));

	if (apply) {
		//resest globalBestBird
		cout << "scout applied " << globalBestScout->getBestValue() << " < " <<
			globalBestBird->getBestValue() << endl;
		globalBestBird->setPos(globalBestScout->getPos());
		globalBestBird->setBestPos(globalBestScout->getBestPos());
		globalBestBird->setBestValue(globalBestScout->getBestValue());
	}
	TRACE_OUT("Swarm::applyScout", apply);	
}

void Swarm::randomizeReset(double portion, bool reset) {
	TRACE_IN("Swarm::randomizeReset");
	
	const vector<double>& bestPos = globalBestBird->getBestPos();
	bool useBestPos = false;

	if (pso->getDynamicStyle() == dynamicFct) {
		if (pso->get_functionType() == moving_peaks) {
			//moving_peaks stays within initial space
			//bestPos.clear();
			useBestPos = false;
		}
		else {
			//bestPos.assign(globalBestBird->getBestPos().begin(), globalBestBird->getBestPos().end());
			//bestPos = globalBestBird->getBestPos();
			useBestPos = true;
		}
	}
	
	//portion <= 1.0 !!!
	int nrRandomize = int(floor(portion * swarmsize));
	cout << nrRandomize << endl;
	set<int> randomizeBirds;
	int resetCount=0;
	
	while (randomizeBirds.size() < nrRandomize) {
		randomizeBirds.insert(randIntRange(0, swarmsize-1));
	}
	
	if (reset) {
		for (int i = 0; i < swarmsize; i++) {
			if (randomizeBirds.find(i) == randomizeBirds.end()) {
				//i is not in set
				Bird *birdPtr=birds[i];
				birdPtr->setBestPos(birdPtr->getPos());
						
				resetCount++;
			}
		}
	}
	
	
#ifdef STDOUT
	cout << "randomize [ ";
#endif

	for (set<int>::iterator it = randomizeBirds.begin(); it != randomizeBirds.end(); it++) {
		int k = *it;
	
		Bird *birdPtr=birds[k];
		randomizeBird(birdPtr, bestPos, useBestPos);
#ifdef STDOUT
		cout << k << " ";
#endif
	}
#ifdef STDOUT
	cout << "] - randomized " << nrRandomize << " particles " << endl;
#endif

#ifdef STDOUT
	if (reset) {
		cout << "reset " << resetCount << " particles " << endl;		
	}
#endif
	
	TRACE_OUT("Swarm::randomizeReset", nrRandomize);	
}

void Swarm::randomizeBird(Bird *birdPtr, const vector<double>& bestVector, bool useBestPos) {
	TRACE_IN("Swarm::randomizeBird");
	
	vector<double> tmpPos;
	vector<double> tmpVel;
	for (int j = 0; j < pso->get_dim(); j++) {
		if (!(useBestPos)) {
			tmpPos.push_back(randDoubleRange(min_x, max_x));
			tmpVel.push_back(randDoubleRange(min_v, max_v));
		}
		else {
			//move the randomization window along with the global best
			tmpPos.push_back(randDoubleRange(min_x + bestVector[j], max_x + bestVector[j]));
			tmpVel.push_back(randDoubleRange(min_v, max_v));
		}
	}
	birdPtr->setPos(tmpPos);
	birdPtr->setBestPos(tmpPos);
	birdPtr->setVel(tmpVel);

	TRACE_OUT("Swarm::randomizeBird", 0);	
}

void Swarm::updateVel() {
	TRACE_IN("Swarm::updateVel");
	
	for (int k=0; k<swarmsize; k++) {
		Bird *birdPtr=birds[k];
		
		birdPtr->updateVelocity(globalBestBird->getBestPos());
		
	}	

	TRACE_OUT("Swarm::updateVel",0);
}

void Swarm::updateScoutVel() {
	TRACE_IN("Swarm::updateScoutVel");
	
	for (int k=0; k<nrScouts; k++) {
		Bird *birdPtr=scouts[k];

		if (birdPtr == globalBestScout) {
			vector<double> tmpPos;
			for (int j = 0; j < pso->get_dim(); j++) {
				tmpPos.push_back(randDoubleRange(min_v, max_v));
			}
			//-1 identifies as scout
			birdPtr->updateVelocity(tmpPos, -1);
		}
		else {
			//-1 identifies as scout
			birdPtr->updateVelocity(globalBestScout->getBestPos(), -1);
		}

				
	}	

	TRACE_OUT("Swarm::updateScoutVel",0);
}

void Swarm::move() {
	TRACE_IN("Swarm::move");

	for (int k=0; k<swarmsize; k++) {
		Bird *birdPtr=birds[k];

		birdPtr->move();
	}

	TRACE_OUT("Swarm::move",0);
}

void Swarm::moveScouts() {
	TRACE_IN("Swarm::moveScouts");

	for (int k=0; k<nrScouts; k++) {
		Bird *birdPtr=scouts[k];

		birdPtr->move();
	}

	TRACE_OUT("Swarm::moveScouts",0);
}

void Swarm::updateMaxDistance() {
	TRACE_IN("Swarm::updateMaxDistance");
	
	maxDistance = 0.0;
	
	for (int i=0; i<swarmsize; i++) {
		Bird *birdPtr = birds[i];
		for (int j=i+1; j<swarmsize; j++) {
			Bird *otherBirdPtr = birds[j];
			double dist=0.0;
			for (int d=0; d<pso->get_dim(); d++)
				dist += pow(birdPtr->getPos()[d] - otherBirdPtr->getPos()[d], 2.0);
			dist = sqrt(dist);
			if (dist > maxDistance)
				maxDistance=dist;
		}
	}

	
	TRACE_OUT("Swarm::updateMaxDistance",0);
}


void Swarm::updateMeanPosition() {
	TRACE_IN("Swarm::updateMeanPosition");
	
	for (int i=0; i<pso->get_dim(); i++)
		meanPosition[i] = 0;
	
	for (int k=0; k<swarmsize; k++) {
		Bird *birdPtr = birds[k];
		for (int i=0; i<pso->get_dim(); i++)
			meanPosition[i] += birdPtr->getPos()[i];

	}

	for (int i=0; i<pso->get_dim(); i++)
		meanPosition[i] = meanPosition[i] / (double) swarmsize;
	
	TRACE_OUT("Swarm::updateMeanPosition",0);
}


void Swarm::calculateDiversity() {
	TRACE_IN("Swarm::calculateDiversity");

	//items are sorted in decreasing order
	//every inserted item is located at the right position
	priority_queue<double> distances;
	double dist;
	double average=0.0;

	for (int k=0; k<swarmsize; k++) {
		Bird *birdPtr=birds[k];

		dist = euclideanDistance(birdPtr->getPos(), meanPosition);
		//insert in sorted position
		distances.push(dist);
		average += dist;
	}
	averageDiversity = average / (double) swarmsize;

	//remove the first n/2 elements for the median element
	for (int i = 0; i < (swarmsize/2); i++) {
		distances.pop();
	}
	medianDiversity = distances.top();
	
	TRACE_OUT("Swarm::calculateDiversity",0);
}

double* Swarm::calculateDimensionVariance() {
	TRACE_IN("Swarm::calculateDimensionVariance");

		
	double* variance = new double[pso->get_dim()];
	double* avg = new double[pso->get_dim()];

	for (int k=0; k<swarmsize; k++) {
		Bird *birdPtr=birds[k];
		for (int d=0; d<pso->get_dim(); d++) {
			avg[d] += birdPtr->getBestPos()[d];
		}
	}
	for (int d=0; d<pso->get_dim(); d++) {
		avg[d] = avg[d]/swarmsize;
	}
	for (int k=0; k<swarmsize; k++) {
		Bird *birdPtr=birds[k];
		for (int d=0; d<pso->get_dim(); d++) {
			variance[d] += pow(birdPtr->getBestPos()[d] - avg[d], 2.0);
		}
	}
	for (int d=0; d<pso->get_dim(); d++) {
		variance[d] = variance[d]/swarmsize;
	}

	return variance;
	
	TRACE_OUT("Swarm::calculateDimensionVariance",0);
}


void Swarm::setInitialSidestep(double sidestep) {
	for (int k=0; k<swarmsize; k++) {
		Bird *birdPtr=birds[k];
		birdPtr->initializeSidestep(sidestep);
	}
}


int Swarm::step(int steps) {
	//new
	TRACE_IN("Swarm::step");
	int ok=1;
	
	//TODO: if (nrScouts) outside for loop, reduce comparisons
	for (int nrStep=0; nrStep<steps; nrStep++) {
		Bird *itrBestBird;
		Bird *itrBestScout;
		
		//evaluate current Positions and return iteration best bird
		itrBestBird = evaluatePos();
		
		if (nrScouts != 0)		
			//evaluate current Positions of scouts
			itrBestScout = evaluateScouts();
		
		//compare birds and adjust global best
		compare(itrBestBird);

		if (nrScouts != 0) {
			//determine globalBestScout
			compareScouts(itrBestScout);
			
			//if the globalBestScout is better than globalBestBird, the latter one
			//is reset to the scout position
			applyScout();
		}
		
		//pass the current neighbourhood to the sidestep method
		if (pso->getLocalsearchMode() == sidestep) {
			for (int k=0; k<swarmsize; k++) {
				Bird *birdPtr=birds[k];
				birdPtr->sidestep(globalBestBird->getBestPos());
			}	
		}
		
		//update velocity towards global best
		updateVel();
		
		if (nrScouts != 0)		
			//change velocity of scouts with random globalBest attractor
			updateScoutVel();

		//move with current velocity
		move();
		
		if (nrScouts != 0)
			//move scouts with current velocity
			moveScouts();
		
#ifdef DIVERSITY
		//updateMaxDistance();
		updateMeanPosition();
		calculateDiversity();	
#endif

		//iteration is done
		pso->iterationDone();

		if (pso->getDynamicStyle() == dynamicFct) {
			if (detectDynamicChange()) {
				OUTPUT1("Change detected at step ", pso->stat->getCurrentStep()-1 << " evaluate pbest positions, response ");
				respondToChange();
			}
		}
				
	}
	TRACE_OUT("Swarm::step",ok);
	return ok;
}


bool Swarm::detectDynamicChange() {
	TRACE_IN("Swarm::detectDynamicChange");
	bool change = false;
	switch (detectionMethod) {
		case noDetect: {
			break;
		}
		case gbestDetectChange: {	
			change = globalBestBird->updateBestValue();
			break;
		}
		case scoutDetectChange: {
			for (int k=0; k<nrScouts; k++) {
				Bird *birdPtr=scouts[k];
				bool scoutUpToDate = birdPtr->updateBestValue();
				//just using change || birdPtr->updateBestValue()
				//causes the execution to break, once change is true
				//therefore not necessarily all scouts are updated
				change = change || scoutUpToDate;
			}
			break;
		}
		case upToDate: {
			change = true;	
			break;
		}
		case hierarchyChangeDetect: {
			//use step-1, because iteration_done was called before detectDynamicChange
			if (pso->stat->getCurrentStep() > 10) {
				//no change if swarm is currently divided
				if ((!isDivided())&&(pso->stat->getNrSwappedAt(pso->stat->getCurrentStep()-1) >= pso->getHierachyChangeDetectThreshold())) {
					OUTPUT1("detected environment change with ", pso->stat->getNrSwappedAt(pso->stat->getCurrentStep()-1) << " swaps.");
					change = true;
				}
			}
			break;
		}
	    case omniscientDetect: {
		  change = pso->isDynamicChange();
		  break;

		}
		default: {
			cerr << "No responseMethod selected"<< endl;
			exit(-1);
			break;
		}
	}

	pso->stat->changeDetected(change);

	TRACE_OUT("Swarm::detectDynamicChange",change);
	return change;
}


void Swarm::respondToChange() {
	TRACE_IN("Swarm::respondToChange");

	if (responseMethod != noResponse) {
	  switch (responseMethod) {
	  case updateOnly: {
		break;
	  }
	  case applyScoutResponse: {
		applyScout(true);
		break;
	  }
	  case randomize10: {
		randomizeReset(0.1, false);
		break;
	  }
	  case randomize10reset: {
		randomizeReset(0.1, true);		
		break;	
	  }
	  case randomize16reset: {
		//5 of 30
		randomizeReset(0.167, true);		
		break;	
	  }
	  case randomize22reset: {
		//9 of 40
		randomizeReset(0.225, true);		
		break;	
	  }
	  case randomize40reset: {
		//16 of 40
		randomizeReset(0.4, true);		
		break;	
	  }
	  case randomize50reset: {
		randomizeReset(0.5, true);		
		break;	
	  }
	  case resetAll: {
		//reset all
		randomizeReset(0.0, true);		
		break;	
	  }
	  default: {
		cerr << "No responseMethod selected"<< endl;
		exit(-1);
		break;
	  }
	  }

	  updateBestValues();

	}

	TRACE_OUT("Swarm::respondToChange",0);		
}


void Swarm::updateBestValues() {
	TRACE_IN("Swarm::updateBestValues");
	//update the personal best values
	//that are invalidated by the environment change
	if (detectionMethod == gbestDetectChange) {
		for (int k=0; k<swarmsize; k++) {
			Bird *birdPtr=birds[k];
			//in gbestDetectChange, the global best bird already updated his bestPos
			//gbest may have been re-randomized in respondToChange
			//if (birdPtr != globalBestBird)
				birdPtr->updateBestValue();
		}
		for (int k=0; k<nrScouts; k++) {
			Bird *birdPtr=scouts[k];
			birdPtr->updateBestValue();
		}					
	}
	else if (detectionMethod == scoutDetectChange) {
		for (int k=0; k<swarmsize; k++) {
			Bird *birdPtr=birds[k];
			birdPtr->updateBestValue();
		}
		///scouts already up to date				
	}
	else {
		//no detection done before, everybody has to update
		for (int k=0; k<swarmsize; k++) {
			Bird *birdPtr=birds[k];
			birdPtr->updateBestValue();
		}
		for (int k=0; k<nrScouts; k++) {
			Bird *birdPtr=scouts[k];
			birdPtr->updateBestValue();
		}
	}

	//update the global best particle pointer
	double gbest = pso->getMax();
	double currValue;

	for (int k=0; k<swarmsize; k++) {
		Bird *birdPtr=birds[k];
		currValue = birdPtr->getBestValue();

		if (pso->isBetter(currValue, gbest)) {
			globalBestBird = birdPtr;
			gbest = currValue;
		}

	}
	DEBUG2("Updated globalBestBird: ", globalBestBird, globalBestBird->getBestValue());

	TRACE_OUT("Swarm::updateBestValues",0);		
}


bool Swarm::noisyBirdIsBetter(Bird* newBird, Bird* oldBird) {
	TRACE_IN("Swarm::noisyBirdIsBetter");
	double value=0.0;
	double value1=0.0;
	double value2=0.0;
	int nrEval=0;

	Bird* birdPtr;
	for (int i=0; i<2; i++) {
		//both birds
		if (i==0)
			birdPtr = newBird;
		else
			birdPtr = oldBird;

		if (! birdPtr->getReEvaluated()) {
			value = 0.0;
			nrEval = 0;

			if (pso->getNoisyReuseOldValue() == reuseContinuousAverage) {
				value=birdPtr->getBestValue();
				nrEval++;
			}
			else if (pso->getNoisyReuseOldValue() == reuseLastBestValue) {
				value=birdPtr->getLastBestValue();
				nrEval++;
			}

			if (pso->getNrNoisySwapChecks() != 0) {
				int evalIt=0;
				double lastEval = 0.0;
				while (evalIt < pso->getNrNoisySwapChecks()) {
					lastEval = pso->evalFunction(birdPtr->getBestPos());
					birdPtr->setReEvaluated(true);
					value += lastEval;
					evalIt++;
					nrEval++;
				}
				value = value/(double) nrEval;
				if (pso->getNoisyRefineBestValue()) {
					birdPtr->setBestValue(value);
					if (pso->getNoisyReuseOldValue() == reuseLastBestValue) {
						birdPtr->setLastBestValue(lastEval);
					}
				}
			}
		}
		//already reEvaluated
		else {
			value = birdPtr->getBestValue();
		}
		if (i==0)
			value1 = value;
		else
			value2 = value;
	} // end for 0/1

	if (pso->getNrNoisySwapChecks() != 0)
		return pso->isBetter(value1, value2);
	else
		return pso->isBetter(newBird->getBestValue(), oldBird->getBestValue());




	/*___________________________
	if (pso->getNoisyReuseOldValue() == reuseContinuousAverage) {
		value1=newBird->getBestValue();
		value2=oldBird->getBestValue();
		nrEval++;
	}
	else if (pso->getNoisyReuseOldValue() == reuseLastBestValue) {
		value1=newBird->getLastBestValue();
		value2=oldBird->getLastBestValue();
		nrEval++;
	}

	if (pso->getNrNoisySwapChecks() != 0) {
		int evalIt=0;
		double lastEval1, lastEval2 = 0.0;
		while (evalIt < pso->getNrNoisySwapChecks()) {
			lastEval1 = pso->evalFunction(newBird->getBestPos());
			newBird->setReEvaluated(true);
			lastEval2 = pso->evalFunction(oldBird->getBestPos());
			oldBird->setReEvaluated(true);
			value1 += lastEval1;
			value2 += lastEval2;
			evalIt++;
			nrEval++;
		}
		value1 = value1/(double) nrEval;
		value2 = value2/(double) nrEval;
		if (pso->getNoisyRefineBestValue()) {
			newBird->setBestValue(value1);
			oldBird->setBestValue(value2);
			if (pso->getNoisyReuseOldValue() == reuseLastBestValue) {
				newBird->setLastBestValue(lastEval1);
				oldBird->setLastBestValue(lastEval2);
			}
		}


		return pso->isBetter(value1, value2);
	}
	else
		return pso->isBetter(newBird->getBestValue(), oldBird->getBestValue());
*/

	TRACE_OUT("Swarm::noisyBirdIsBetter",0);
}

BirdContainer::BirdContainer(const BirdContainer &BC) {
	TRACEV_IN("BirdContainer::BirdContainer");
	bird = BC.bird;
	index = BC.index;

	TRACEV_OUT("BirdContainer::BirdContainer",0);
}


BirdContainer::BirdContainer(Bird* bird_in, int i) {
	TRACEV_IN("BirdContainer::BirdContainer");
	bird = bird_in;
	index = i;
	
	TRACEV_OUT("BirdContainer::BirdContainer",0);
}

BirdContainer::BirdContainer() {
	TRACEV_IN("BirdContainer::BirdContainer");
	
	TRACEV_OUT("BirdContainer::BirdContainer",0);
}


BirdContainer::~BirdContainer() {
	TRACEV_IN("BirdContainer::~BirdContainer");
	
	TRACEV_OUT("BirdContainer::~BirdContainer",0);
}


//the birds are created in Swarm::createSwarm
//Pyramid::createSwarm only creates the pyramid structure
PyramidSwarm::PyramidSwarm(PSO *pso_in, 
														int swarmsize_in,
														int dim_in,											
														double min_x,
														double max_x,
														double min_v,
														double max_v,
														int height_in, 
														int branches_in,
														int swapDelay_in,
														int nrScouts_in
														) : Swarm(pso_in, swarmsize_in, dim_in, min_x, max_x, min_v, max_v, nrScouts_in) {
	TRACE_IN("PyramidSwarm::PyramidSwarm");
	
	DEBUG("New PyramidSwarm : ");
	
	
	
	
	
	//compute the swarmsize from height and branches
	int regularSwarmsize = 0;
	swarmsize = swarmsize_in;
	branches = branches_in;
	height = height_in;
	swapDelay = swapDelay_in;
	nrScouts = nrScouts_in;


	DEBUG1("pso = ",pso_in);
	DEBUG1("size = ",swarmsize);
	DEBUG1("dim = ",dim_in);
	DEBUG1("min_x = ",min_x);
	DEBUG1("max_x = ",max_x);
	DEBUG1("min_v = ",min_v);
	DEBUG1("max_v = ",max_v);
	DEBUG1("height = ",height);
	DEBUG1("branches = ",branches);
	DEBUG1("swapDelay = ",swapDelay);
	DEBUG1("nrScouts = ",nrScouts);



	for (int i=0; i < height; i++) {
		DEBUG2("computed swarmsize",i,regularSwarmsize);
		regularSwarmsize += pow((double) branches, (double) i);
	}

	//initialize the extra nodes list
	//regular pyramid just has 0s
	//for irregular pyramid the extra nodes are added to a new level
	int bottomBranches = pow((double) branches, (double) height-1);
	for (int i=0; i<bottomBranches; i++) {
		extraNodesList.push_back(0);
	}

	
	if (regularSwarmsize == swarmsize) {
		DEBUG1("Regular PyramidSwarm of size ",regularSwarmsize);
		regularPyramid = true;
		currentHeight = height;
	}
	else {
		regularPyramid = false;		
		currentHeight = height+1;
		int extraNodes = swarmsize - regularSwarmsize;
		if (extraNodes < 0) {
			cout << "Swarm size is too small for the desired structure (height=" << height 
				<<", branches="<< branches<< ")"<<endl;
			exit(-1); 	
		}
		DEBUG2("Irregular PyramidSwarm of size with #extraNodes ",regularSwarmsize,
			extraNodes);
		

		//distribute the extra nodes
		deque<int>::iterator it=extraNodesList.begin();
		while (extraNodes > 0) {
			(*it)++;
			extraNodes--;
			it++;
			if (it == extraNodesList.end())
				it = extraNodesList.begin();
		}
		
#ifdef DBUG
		cout << "extra Nodes to be added : ";
		for (deque<int>::iterator it=extraNodesList.begin(); it != extraNodesList.end(); it++)
			cout << *it << " ";
		cout << endl;
#endif
	}
	
	//birds = new Bird *[swarmsize];
	for (int i=0; i<swarmsize; i++) {
		birdContainerLevels[i] = 0;
		birdContainerSubtree[i] = 0;
		birdContainerSubswarm[i] = 0;
	}

	for (int i=0; i<branches; i++) {
		gbestFromSubtreeTick.push_back(0);
	}
	
	swapEnabled = true;	


	//create the pyramid hierarchy over the birds
	createPyramid();
	
	updateBirdContainerLevels();
	
#ifdef DIVERSITY
	//the mean position has to be updated
	updateMeanPosition();
	for (int i=0; i<pso->get_dim(); i++)
		meanPositionSubtree.push_back(0);
	
	//store Diversity for subtrees and one random set
	averageDiversitySubtree = new double[branches+1];
	medianDiversitySubtree = new double[branches+1];	
#endif
	
	
	TRACE_OUT("PyramidSwarm::PyramidSwarm",0);
}

PyramidSwarm::~PyramidSwarm()  {
	TRACE_IN("PyramidSwarm::~PyramidSwarm");
	birdContainerLevels.clear();
	birdContainerSubtree.clear();
	birdContainerSubswarm.clear();
	extraNodesList.clear();

	TRACE_OUT("PyramidSwarm::~PyramidSwarm",0);
}

int PyramidSwarm::getActualHeight() {
	/*if (regularPyramid)
		return height;
	else
		return height+1;*/
	return currentHeight;
}

void PyramidSwarm::swap(tree<BirdContainer>::iterator it1, tree<BirdContainer>::iterator it2) {
	TRACE_IN("PyramidSwarm::swap");

	DEBUG2("swap: ",it1->getBird(),it2->getBird());


	//the higher node is stated second, it2
	//call the stat->swapped before the iterators are changed
	pso->stat->swapped(pyramid.depth(it2));


	BirdContainer tmpBC = (*it1);
	

	(*it1) = (*it2);
	(*it2) = tmpBC;
	

	DEBUG2("swap done: ",it1->getBird(),it2->getBird());
	TRACE_OUT("PyramidSwarm::swap",0);	
}



void PyramidSwarm::printPyramid() {
	string BLANK = "                                                                                                                                  ";

 	tree<BirdContainer>::iterator outIt;
	for (outIt = pyramid.begin(); outIt != pyramid.end(); ++outIt)
    cout << BLANK.substr(0,pyramid.depth(outIt)*3) << outIt->getIndex() << 
	" " << outIt->getBird() << " " << outIt->getBird()->getIndex() << ": " << (outIt->getBird())->getBestValue()
    	 << endl;

}

void PyramidSwarm::hierarchicalCompare() {
	TRACE_IN("PyramidSwarm::hierarchicalCompare");

	string BLANK = "                                                    ";

	///reset the reEvaluated state (used for noisy functions)
	for (int k=0; k<swarmsize; k++) {
		birds[k]->setReEvaluated(false);
	}

  deque<tree<BirdContainer>::iterator> nodeList;
  nodeList.push_back(pyramid.begin());
  //nodes for breadth-first search
  tree<BirdContainer>::iterator tmpNode;

  //The tree is traversed and a group consisting of node + direct children is observed
  while (!nodeList.empty()) {
  	tmpNode = nodeList.front();
   	nodeList.pop_front();
  	DEBUG("---------------");

  	if (pyramid.begin(tmpNode) == pyramid.end(tmpNode)) {
  		//No siblings
  		
  		DEBUG1("No Siblings",(tmpNode->getBird())->getCurrentValue());
  	}
  	else {
  		DEBUG2("", tmpNode->getIndex(), (tmpNode->getBird())->getCurrentValue());
			double bestVal = pso->getMax();
			tree<BirdContainer>::sibling_iterator bestSib;
			
			//remember which subtree the best sibling was from (only on the first level), required for gbestFromSubtree
			int bestSibSubtree = 0;
			int currSibSubtree = 0;
			for (tree<BirdContainer>::sibling_iterator sib = pyramid.begin(tmpNode); 
				sib != pyramid.end(tmpNode); ++sib) {
					//select best sibling
					if (pso->isBetter((sib->getBird())->getBestValue(), bestVal)) {
						bestSib = sib;
						bestVal = (sib->getBird())->getBestValue();
						bestSibSubtree = currSibSubtree;
					}
					currSibSubtree++;
		    	DEBUG2("-", BLANK.substr(0,pyramid.depth(sib)*3) << sib->getIndex(), 
		    		(sib->getBird())->getCurrentValue());
  		  	nodeList.push_back(sib);
  		}
  		
  		//if best sibling is better than predecessor node, then swap
		bool performSwap = false;
		if ((pso->getNoiseStyle() == noNoise)) {
  			performSwap = pso->isBetter((bestSib->getBird())->getBestValue(), 
  					(tmpNode->getBird())->getBestValue());
		}
		else {
			if (pso->getNoisyFirstComparePbestValues() == true) {
				if (pso->isBetter((bestSib->getBird())->getBestValue(), 
					(tmpNode->getBird())->getBestValue())) {
						performSwap = noisyBirdIsBetter(bestSib->getBird(), tmpNode->getBird());
					}
			}
			else {
			performSwap = noisyBirdIsBetter(bestSib->getBird(), tmpNode->getBird());
			}
		}
		if (swapEnabled) {
			if (performSwap) {
	  			//swap the sibling with the parent node above
  				swap(bestSib, tmpNode);
	  			DEBUG2("best Sibling : ", bestSib->getIndex(), 
						(bestSib->getBird())->getCurrentValue());
				if (pyramid.depth(tmpNode)==0) {
					//the root was exchanged
					gbestFromSubtreeTick[bestSibSubtree]=pso->stat->getCurrentStep();
				}
			}
		}
  	}

  }
	
#ifdef DBUG
	printPyramid();
#endif

	
	
	
	//The top bird of the pyramid is stored, not necessarily global best
	//CHANGE : the real global best is logged, by calling Swarm::compare()
	//pso->stat->addGlobalBest((pyramid.begin()->getBird())->getBestValue());

	DEBUG2("gloBest ", (pyramid.begin()->getBird()),
			(pyramid.begin()->getBird())->getBestValue());


	TRACE_OUT("PyramidSwarm::hierarchicalCompare",0);
}

void PyramidSwarm::updateVel() {
	TRACE_IN("PyramidSwarm::updateVel");

	string BLANK = "                                                    ";

  deque<tree<BirdContainer>::iterator> nodeList;
  nodeList.push_back(pyramid.begin());
  tree<BirdContainer>::iterator tmpNode;

	//update Top Bird with self pos
	tmpNode = pyramid.begin();
	(tmpNode->getBird())->updateVelocity((tmpNode->getBird())->getBestPos());
	DEBUG2("Updated :", tmpNode->getIndex(), tmpNode->getIndex());

	
  
  while (!nodeList.empty()) {
  	tmpNode = nodeList.front();
   	nodeList.pop_front();

  	if (pyramid.begin(tmpNode) == pyramid.end(tmpNode)) {
  		//No siblings
  		
  		//cout << (tmpNode->getBird())->getCurrentValue() << " has no siblings." << endl;
  	}
  	else {
	  for (tree<BirdContainer>::sibling_iterator sib = pyramid.begin(tmpNode); sib != pyramid.end(tmpNode); ++sib) {
					
	    //update sibling velocity according to successor
	    (sib->getBird())->updateVelocity((tmpNode->getBird())->getBestPos(), pyramid.depth(sib));
	    
	    DEBUG2("Updated :",sib->getIndex(), tmpNode->getIndex());
	    
	    nodeList.push_back(sib);
	  }
  	}
	
  }

	TRACE_OUT("PyramidSwarm::updateVel",0);
}


void PyramidSwarm::createPyramid() {
	TRACE_IN("PyramidSwarm::createPyramid");

	//build the pyramid of birdContainers
	//the birds are still stored in the array

	//list of open nodes for building the pyramid breadth-first
	deque <tree<BirdContainer>::iterator> nodeList;
	tree<BirdContainer>::iterator node, top;
	Bird* tmpBird;
	BirdContainer* bc;

	//top of the pyramid
	
	//the birds are already created by Swarm::createSwarm,
	//only the pyramid hierarchy is added
	//tmpBird = createBird(min_x, max_x, min_v, max_v, 0);
	
	tmpBird = birds[0];
	top = pyramid.begin();
	bc = new BirdContainer(tmpBird, 0);
	
	pyramid.insert(top, *bc);
	int nrBirds = 1;

	
	nodeList.push_back(pyramid.begin());
	
	while (!nodeList.empty()) {
		node = nodeList.front();
		nodeList.pop_front();
		
				
		if (pyramid.depth(node) < height-1) {
			//not reached the leaves yet
			//still further siblings added
			for (int i=0; i<branches; i++) {
				//tmpBird = createBird(min_x, max_x, min_v, max_v, nrBirds);
				tmpBird = birds[nrBirds];
				bc = new BirdContainer(tmpBird, nrBirds);
				nodeList.push_back(pyramid.append_child(node, *bc));
				nrBirds++;
			}
		}
		
		//if we are on the last level
		//and the pyramid is not balanced, then further 
		//children for node are added
		if (pyramid.depth(node) == height-1) {
			int nrExtraNodes = extraNodesList.front();
			extraNodesList.pop_front();
			for (int i=0; i<nrExtraNodes; i++) {
				//tmpBird = createBird(min_x, max_x, min_v, max_v, nrBirds);
				tmpBird = birds[nrBirds];
				bc = new BirdContainer(tmpBird, nrBirds);
				nodeList.push_back(pyramid.append_child(node, *bc));
				nrBirds++;				
			}
			DEBUG1("ExtraNodes added:",nrExtraNodes);			
		}
		
		
	}
	if (nrBirds != swarmsize) {
		DEBUG2("(nrBirds != swarmsize) could not build pyramid ",nrBirds, swarmsize);
		exit(-1);
	}
	else {
		DEBUG1("nrBirds == swarmsize == ",nrBirds);
	}
	
#ifdef DBUG
	printPyramid();
#endif

	
	
	TRACE_OUT("PyramidSwarm::createPyramid",0);
		
}

void PyramidSwarm::updateBirdContainerLevels() {
	//the pyramid is traversed and the levels and subtrees of the BirdContainers 
	//stored in an array	
		TRACE_IN("PyramidSwarm::updateBirdContainerLevels");

  deque<tree<BirdContainer>::iterator> nodeList;
  nodeList.push_back(pyramid.begin());
  tree<BirdContainer>::iterator tmpNode;

  tmpNode = pyramid.begin();
  
  int bcindex = 0;
  
  //the subtree (#branches) the bird is in
  //the root is subtree 0
  int subtree = 0;
  int subswarmCounter = 0;
  int subswarm = 0;
  int divDepth = getActualHeight()/2;
  
  
  while (!nodeList.empty()) {
    tmpNode = nodeList.front();
    nodeList.pop_front();
    
    //the subtree can be determined in this fashion, since the pyramid is traversed
    //depth-first
  
	  if (pyramid.depth(tmpNode) == 1)
		  subtree++;
	  if (pyramid.depth(tmpNode) == divDepth)
		  subswarmCounter++;

	  if (pyramid.depth(tmpNode) < divDepth)
		  subswarm = 0;
	  else
		  subswarm = subswarmCounter;
    
    
    //the level and subtree of each bird container is stored in an indexed array
    birdContainerLevels[tmpNode->getIndex()] = pyramid.depth(tmpNode);
    birdContainerSubtree[tmpNode->getIndex()] = subtree;
    birdContainerSubswarm[tmpNode->getIndex()] = subswarm;
    
    
    
    if (pyramid.begin(tmpNode) == pyramid.end(tmpNode)) {
      //No siblings
    }
    else {
      for (tree<BirdContainer>::sibling_iterator sib = pyramid.begin(tmpNode); 
	   sib != pyramid.end(tmpNode); ++sib) {
		   //depth-first
		nodeList.push_front(sib);
      }
    }
    
  }
	
	
  TRACE_OUT("PyramidSwarm::updateBirdContainerLevels",0);
		
}

void PyramidSwarm::removeBirdContainerLevelsEntry(int index) {

  TRACE_IN("PyramidSwarm::removeBirdContainerLevelsEntry");
  
  birdContainerLevels.erase(index);
  birdContainerSubtree.erase(index);
  birdContainerSubswarm.erase(index);


  TRACE_OUT("PyramidSwarm::removeBirdContainerLevelsEntry",index);

}


/*void PyramidSwarm::updateBirdContainerLevels() {
	//the pyramid is traversed and the levels and subtrees of the BirdContainers 
	//stored in an array	
		TRACE_IN("PyramidSwarm::updateBirdContainerLevels");

  deque<tree<BirdContainer>::iterator> nodeList;
  nodeList.push_back(pyramid.begin());
  tree<BirdContainer>::iterator tmpNode;

  tmpNode = pyramid.begin();
  
  int bcindex = 0;
  
  //the subtree (#branches) the bird is in
  //the root is subtree 0
  int subtree = 0;
  int subswarm = 0;
  int divDepth = getActualHeight()/2;
  int subtreeCounter = 0;
  int upperLevelsCounter =0;
  int* levelSize = new int[getActualHeight()];
  for (int i = 0; i < getActualHeight(); i++) {
    levelSize[i] = (int) pow((double) branches, (double) i);
  }
  
  while (!nodeList.empty()) {
    tmpNode = nodeList.front();
    nodeList.pop_front();
    
    //the subtree can be determined in this fashion, since the pyramid is traversed
    //breadth-first
    if (subtreeCounter == 0)
      //root
      subtree = 0;
    else {
      upperLevelsCounter = 0;

      for (int i = 0; i < pyramid.depth(tmpNode); i++) {
	upperLevelsCounter += levelSize[i];
      }
      //integer division !
	  //+1, since root is subtree 0
      subtree = ((subtreeCounter - upperLevelsCounter) / levelSize[pyramid.depth(tmpNode)-1]) + 1;
	  if (pyramid.depth(tmpNode) >= divDepth)
		subswarm = ((subtreeCounter - upperLevelsCounter) / levelSize[pyramid.depth(tmpNode)-divDepth]) + 1;
	  else
		  subswarm = 0;
    }
    subtreeCounter++;
    
    
    
    //the level and subtree of each bird container is stored in an indexed array
    birdContainerLevels[tmpNode->getIndex()] = pyramid.depth(tmpNode);
    birdContainerSubtree[tmpNode->getIndex()] = subtree;
    
    
    
    if (pyramid.begin(tmpNode) == pyramid.end(tmpNode)) {
      //No siblings
    }
    else {
      for (tree<BirdContainer>::sibling_iterator sib = pyramid.begin(tmpNode); 
	   sib != pyramid.end(tmpNode); ++sib) {
	nodeList.push_back(sib);
      }
    }
    
  }
	
	
  TRACE_OUT("PyramidSwarm::updateBirdContainerLevels",0);
		
}
*/

void PyramidSwarm::updateMeanPositionSubtree(int subtreeNr, set<int> randomSet) {
	TRACE_IN("PyramidSwarm::updateMeanPositionSubtree");
	
	
	for (int i=0; i<pso->get_dim(); i++)
		meanPositionSubtree[i] = 0;
	
	if (subtreeNr != -1) {
		int nrAverage = 0;
		for (int k=0; k<swarmsize; k++) {
			//subtree = 0 is the root node
			//therefore we shift the subtree index +1
			if (getPyramidSubtreeBird(k) == subtreeNr+1) {
				Bird *birdPtr = birds[k];
				for (int i=0; i<pso->get_dim(); i++)
					meanPositionSubtree[i] += birdPtr->getPos()[i];
				nrAverage++;
			}
		}
	
		for (int i=0; i<pso->get_dim(); i++)
			meanPositionSubtree[i] = meanPositionSubtree[i] / (double) nrAverage;
	}
	else {
		//random set of birds	
		int nrAverage = 0;
		for (set<int>::iterator it = randomSet.begin(); it != randomSet.end(); it++) {
			int k = *it;
			Bird *birdPtr = birds[k];
			for (int i=0; i<pso->get_dim(); i++)
				meanPositionSubtree[i] += birdPtr->getPos()[i];
			nrAverage++;
			}
		
	
		for (int i=0; i<pso->get_dim(); i++)
			meanPositionSubtree[i] = meanPositionSubtree[i] / (double) nrAverage;
	}
	
	TRACE_OUT("PyramidSwarm::updateMeanPositionSubtree",0);
}


void PyramidSwarm::calculateDiversitySubtree() {
	TRACE_IN("PyramidSwarm::calculateDiversitySubtree");

	//items are sorted in decreasing order
	//every inserted item is located at the right position

	priority_queue<double> distances;
	//just for compatibility
	set<int> nullset;
	double dist;
	double average;
	int nrAverage;

	for (int subtreeNr = 0; subtreeNr < branches; subtreeNr++) {
		updateMeanPositionSubtree(subtreeNr, nullset);
		average=0.0;
		nrAverage = 0;
		
		for (int k=0; k<swarmsize; k++) {
			//subtree = 0 is the root node
			//therefore we shift the subtree index +1
			if (getPyramidSubtreeBird(k) == subtreeNr+1) {
				Bird *birdPtr=birds[k];
		
				dist = euclideanDistance(birdPtr->getPos(), meanPositionSubtree);
				//insert in sorted position
				distances.push(dist);
				average += dist;
				nrAverage++;
			}
		}
		if (nrAverage != (swarmsize / branches)) {
			cout << "nrAverage != (swarmsize / BRANCHES) " << nrAverage << endl;
		}
		
		averageDiversitySubtree[subtreeNr] = average / (double) nrAverage;
		
		//remove the first n/2 elements for the median element
		for (int i = 0; i < (nrAverage/2); i++) {
			distances.pop();
		}
		medianDiversitySubtree[subtreeNr] = distances.top();
		//clear queue
		while (!(distances.empty()))
			distances.pop();
		
	}
	
	//choose random points
	set<int> randomSet;
	while (randomSet.size() < nrAverage) {
		int k = randIntRange(0, swarmsize-1);
		//do not take the root
		if (getPyramidSubtreeBird(k) != 0)
			randomSet.insert(k);
	}
	
	updateMeanPositionSubtree(-1, randomSet);
	average=0.0;
	nrAverage = 0;
	
	for (set<int>::iterator it = randomSet.begin(); it != randomSet.end(); it++) {
		int k = *it;
	
		Bird *birdPtr=birds[k];

		dist = euclideanDistance(birdPtr->getPos(), meanPositionSubtree);
		//insert in sorted position
		distances.push(dist);
		average += dist;
		nrAverage++;

	}
	if (nrAverage != (swarmsize / branches)) {
		cout << "nrAverage != (swarmsize / BRANCHES) " << nrAverage << endl;
	}
		
	//the randomSet Diversity is stored at the end of the array
	averageDiversitySubtree[branches] = average / (double) nrAverage;
	
	//remove the first n/2 elements for the median element
	for (int i = 0; i < (nrAverage/2); i++) {
		distances.pop();
	}
	medianDiversitySubtree[branches] = distances.top();
	//clear queue
	while (!(distances.empty()))
		distances.pop();
	
	TRACE_OUT("PyramidSwarm::calculateDiversitySubtree",0);
}

int PyramidSwarm::step(int steps) {
	TRACE_IN("PyramidSwarm::step");
	int ok=1;

	for (int nrStep=0; nrStep<steps; nrStep++) {
		Bird *itrBestBird;
		Bird *itrBestScout;

		if ((pso->getStep() % swapDelay) == 0) {
			swapEnabled = true;
			//cout << pso->getStep() << ": compare" << endl;
		}
		else
			swapEnabled = false;

		
		//evaluate current Positions and return iteration best bird
		itrBestBird = evaluatePos();
		
		if (nrScouts != 0)		
			//evaluate current Positions of scouts
			itrBestScout = evaluateScouts();		
		
		//call the Swarm::compare() method in order to update and log globalBestBird
		Swarm::compare(itrBestBird);
		
		//call the PyramidSwarm compare() and adjust Pyramid hierarchy
		hierarchicalCompare();

		if (nrScouts != 0) {
			//determine globalBestScout
			compareScouts(itrBestScout);
			
			//if the globalBestScout is better than globalBestBird, the latter one
			//is reset to the scout position
			applyScout();
		}
		
		//update velocity towards global best
		updateVel();

		if (nrScouts != 0)		
			//change velocity of scouts with random globalBest attractor
			updateScoutVel();

		//move with current velocity
		move();
		
		if (nrScouts != 0)
			//move scouts with current velocity
			moveScouts();
		
		//just for output purposes, update the BirdContainerLevels
		updateBirdContainerLevels();
		
#ifdef DIVERSITY
		updateMaxDistance();
		updateMeanPosition();	
		calculateDiversity();
		calculateDiversitySubtree();
		for (int i=0; i < branches+1; i++) {
			pso->stat->addDiversitySubtree(i, getAverageDiversitySubtree(i));
		}
#endif
		
		
		//iteration is done
		pso->iterationDone();

		if (pso->getDynamicStyle() == dynamicFct) {
			if (detectDynamicChange()) {
				OUTPUT1("Change detected at step ", pso->stat->getCurrentStep()-1 << " evaluate pbest positions, response ");
				respondToChange();
			}
		}

		
	}
	TRACE_OUT("PyramidSwarm::step",ok);
	return ok;
}

int PyramidSwarm::getPyramidLevelBird(int bird_i) {
	if (birdContainerLevels.count(bird_i) != 1) {
		cout << "birdContainerLevels contains index " << bird_i << " " << birdContainerLevels.count(bird_i) << " times." << endl;
		exit(-1);
	}
	return birdContainerLevels[bird_i];
}

int PyramidSwarm::getPyramidSubtreeBird(int bird_i) {
	if (birdContainerSubtree.count(bird_i) != 1) {
		cout << "birdContainerSubtree contains index " << bird_i << " " << birdContainerSubtree.count(bird_i) << " times." << endl;
		exit(-1);
	}
	return birdContainerSubtree[bird_i];
}

int PyramidSwarm::getPyramidSubswarmBird(int bird_i) {
	if (birdContainerSubswarm.count(bird_i) != 1) {
		cout << "birdContainerSubswarm contains index " << bird_i << " " << birdContainerSubswarm.count(bird_i) << " times." << endl;
		exit(-1);
	}
	return birdContainerSubswarm[bird_i];
}




DynamicPyramidSwarm::DynamicPyramidSwarm(PSO *pso_in, 
														int swarmsize_in,
														int dim_in,											
														double min_x,
														double max_x,
														double min_v,
														double max_v,
														int height_in, 
														int branches_in,
														int swapDelay_in,
														int nrScouts_in
														) 
			: PyramidSwarm(pso_in, swarmsize_in, dim_in, min_x, max_x, min_v, max_v, 
				height_in, branches_in, swapDelay_in, nrScouts_in) {
	TRACE_IN("DynamicPyramidSwarm::DynamicPyramidSwarm");
	
	DEBUG("New DynamicPyramidSwarm");
	
	nrScouts = nrScouts_in;
	
	//the scout individuals are stored in an extra array
	//scouts = new Bird*[nrScouts];


	//createScouts();

	//At which depth the hierarchy is divided after a environment change
	//divide the swarm in the middle
	divisionDepth = getActualHeight()/2;
	DEBUG1("divisionDepth = ", divisionDepth);
	dividedSwarm = false;
	divisionDuration = 0;
		
	TRACE_OUT("DynamicPyramidSwarm::DynamicPyramidSwarm",0);
}

DynamicPyramidSwarm::~DynamicPyramidSwarm()  {
	TRACE_IN("DynamicPyramidSwarm::~DynamicPyramidSwarm");
	

	TRACE_OUT("DynamicPyramidSwarm::~DynamicPyramidSwarm",0);
}

//the step method is basically identical to PyramidSwarm
//only after an environment change the swarm is divided
//and the dividedHierarchicalCompare and dividedUpdateVel
//methods are used
int DynamicPyramidSwarm::step(int steps) {
	TRACE_IN("DynamicPyramidSwarm::step");
	int ok=1;

	for (int nrStep=0; nrStep<steps; nrStep++) {
		Bird *itrBestBird;

		if ((pso->getStep() % swapDelay) == 0) {
			swapEnabled = true;
			//cout << pso->getStep() << ": compare" << endl;
		}
		else
			swapEnabled = false;

		
		//evaluate current Positions and return iteration best bird
		itrBestBird = evaluatePos();
		
		//call the Swarm::compare() method in order to update and log globalBestBird
		Swarm::compare(itrBestBird);
		
		if (dividedSwarm)
			dividedHierarchicalCompare();
		else
			//call the PyramidSwarm compare() and adjust Pyramid hierarchy
			hierarchicalCompare();
		
		if (dividedSwarm)
			dividedUpdateVel();
		else
			//update velocity towards global best
			updateVel();

		//move with current velocity
		move();
		
		//just for output purposes, update the BirdContainerLevels
		updateBirdContainerLevels();
		
#ifdef DIVERSITY
		updateMeanPosition();	
		calculateDiversity();
		calculateDiversitySubtree();		
#endif
		
		
		//iteration is done
		pso->iterationDone();
		
		if (pso->getDynamicStyle() == dynamicFct) {
			if (detectDynamicChange()) {
				OUTPUT1("Change detected at step ", pso->stat->getCurrentStep()-1 << " evaluate pbest positions, response ");
				respondToChange();
			}
		}

		if (dividedSwarm) {
			divisionDuration--;
			int gbestIndex = globalBestBird->getIndex();
			OUTPUT1("gbest ", gbestIndex <<": " << globalBestBird << \
				"lev: " << getPyramidLevelBird(gbestIndex) << " subswarm: " << getPyramidSubswarmBird(gbestIndex));

			pso->stat->addGbestInSubswarm(getPyramidSubswarmBird(gbestIndex));
			
			if ((responseMethod == subSwarmsTemporalMerge) || (responseMethod == subSwarmsTemporalMergeNoMemory)) {
				if (divisionDuration == 0) {
					dividedSwarm = false;
					OUTPUT("swarm reunited ");
				}
			}
			else if ((responseMethod == subSwarmsAdaptiveMerge) || (responseMethod == subSwarmsAdaptiveMergeNoMemory)) {
				int subswarm = getPyramidSubswarmBird(globalBestBird->getIndex());
				gbestInSubswarm[subswarm]++;
				if (gbestInSubswarm[subswarm] >= 5) {
					dividedSwarm = false;
					gbestInSubswarm.clear();
					OUTPUT1("swarm reunited after ", (10 - divisionDuration) << ", gbest in subswarm " << subswarm);
				}
			}
		}
		else {
			pso->stat->addGbestInSubswarm(-1);
		}
	}
	TRACE_OUT("DynamicPyramidSwarm::step",ok);
	return ok;
}
		
void DynamicPyramidSwarm::dividedHierarchicalCompare() {
	TRACE_IN("DynamicPyramidSwarm::dividedHierarchicalCompare");

	string BLANK = "                                                    ";

  deque<tree<BirdContainer>::iterator> nodeList;
  nodeList.push_back(pyramid.begin());
  //nodes for breadth-first search
  tree<BirdContainer>::iterator tmpNode;

  //The tree is traversed and a group consisting of node + direct children is observed
  while (!nodeList.empty()) {
  	tmpNode = nodeList.front();
   	nodeList.pop_front();
  	DEBUG("---------------");

  	if (pyramid.begin(tmpNode) == pyramid.end(tmpNode)) {
  		//No siblings
  		
  		DEBUG1("No Siblings",(tmpNode->getBird())->getCurrentValue());
  	}
  	else {
  		DEBUG2("", tmpNode->getIndex(), (tmpNode->getBird())->getCurrentValue());
			double bestVal = pso->getMax();
			tree<BirdContainer>::sibling_iterator bestSib;
			
			for (tree<BirdContainer>::sibling_iterator sib = pyramid.begin(tmpNode); 
				sib != pyramid.end(tmpNode); ++sib) {
					//select best sibling
					if (pso->isBetter((sib->getBird())->getBestValue(), bestVal)) {
						bestSib = sib;
						bestVal = (sib->getBird())->getBestValue();
					}
		    	DEBUG2("-", BLANK.substr(0,pyramid.depth(sib)*3), 
		    		(sib->getBird())->getCurrentValue());
  		  	nodeList.push_back(sib);
  		}

  		//if best sibling is better than predecessor node, then swap
		bool performSwap = false;
		if ((pso->getNoiseStyle() == noNoise)) {
  			performSwap = pso->isBetter((bestSib->getBird())->getBestValue(), 
  					(tmpNode->getBird())->getBestValue());
		}
		else {
			if (pso->getNoisyFirstComparePbestValues() == true) {
				if (pso->isBetter((bestSib->getBird())->getBestValue(), 
					(tmpNode->getBird())->getBestValue())) {
						performSwap = noisyBirdIsBetter(bestSib->getBird(), tmpNode->getBird());
					}
			}
			else {
			performSwap = noisyBirdIsBetter(bestSib->getBird(), tmpNode->getBird());
			}
		}
		if ((swapEnabled)&& (pyramid.depth(bestSib) != divisionDepth)) {
			if (performSwap) {
	  			//swap the sibling with the parent node above
  				swap(bestSib, tmpNode);
	  			DEBUG2("best Sibling : ", bestSib->getIndex(), 
						(bestSib->getBird())->getCurrentValue());
				//if (pyramid.depth(tmpNode)==0) {
					//the root was exchanged
				//	gbestFromSubtreeTick[bestSibSubtree]=pso->stat->getCurrentStep();
				//}
			}
		}
		else {
			DEBUG2("No swap between divided levels",pyramid.depth(tmpNode), pyramid.depth(bestSib));
		}


 		/* included noisy swaps
  		//if best sibling is better than predecessor node, then swap
  		if (pso->isBetter((bestSib->getBird())->getBestValue(), 
  					(tmpNode->getBird())->getBestValue())) {
			//only swap if within a sub-swarm,
			//both node and sibling are on the same side of the division line
  			if ((swapEnabled)&& (pyramid.depth(bestSib) != divisionDepth)) {
	  			//swap the sibling with the parent node above
  				swap(bestSib, tmpNode);
	  			DEBUG2("best Sibling : ", bestSib->getIndex(), 
						(bestSib->getBird())->getCurrentValue());  			
  			}
			else {
				DEBUG2("No swap between divided levels",pyramid.depth(tmpNode), pyramid.depth(bestSib));
			}
		}
					*/
	}

  }
	
#ifdef DBUG
	printPyramid();
#endif

	
	
	
	//The top bird of the pyramid is stored, not necessarily global best
	//CHANGE : the real global best is logged, by calling Swarm::compare()
	//pso->stat->addGlobalBest((pyramid.begin()->getBird())->getBestValue());
	
	DEBUG2("gloBest ", (pyramid.begin()->getBird()),
			(pyramid.begin()->getBird())->getBestValue());


	TRACE_OUT("DynamicPyramidSwarm::dividedHierarchicalCompare",0);
}



void DynamicPyramidSwarm::dividedUpdateVel() {
	TRACE_IN("DynamicPyramidSwarm::dividedUpdateVel");

	string BLANK = "                                                    ";

  deque<tree<BirdContainer>::iterator> nodeList;
  nodeList.push_back(pyramid.begin());
  tree<BirdContainer>::iterator tmpNode;

	//update Top Bird with self pos
	tmpNode = pyramid.begin();
	(tmpNode->getBird())->updateVelocity((tmpNode->getBird())->getBestPos());
	DEBUG2("Updated :", tmpNode->getIndex(), tmpNode->getIndex());

	
  
  while (!nodeList.empty()) {
  	tmpNode = nodeList.front();
   	nodeList.pop_front();

  	if (pyramid.begin(tmpNode) == pyramid.end(tmpNode)) {
  		//No siblings
  		
  		//cout << (tmpNode->getBird())->getCurrentValue() << " has no siblings." << endl;
  	}
  	else {
			for (tree<BirdContainer>::sibling_iterator sib = pyramid.begin(tmpNode); 
				sib != pyramid.end(tmpNode); ++sib) {
					if (pyramid.depth(sib) != divisionDepth) { 
						//update sibling velocity according to successor
						(sib->getBird())->updateVelocity((tmpNode->getBird())->getBestPos(),
							pyramid.depth(sib));
						DEBUG2("Updated :",sib->getIndex(), tmpNode->getIndex());
					}
					else {
						//sibling is beyond the division line and tmpNode not
						//update sibling with self pos
						(sib->getBird())->updateVelocity((sib->getBird())->getBestPos(),
							pyramid.depth(sib));
						DEBUG2("Updated :", tmpNode->getIndex(), tmpNode->getIndex());
					}
  		  		nodeList.push_back(sib);
  			}
  	}

  }

	TRACE_OUT("DynamicPyramidSwarm::dividedUpdateVel",0);
}


void DynamicPyramidSwarm::randomizeSubSwarms(bool reset, bool randomizeTopSwarm) {
	TRACE_IN("DynamicPyramidSwarm::randomizeSubSwarms");

	const vector<double>& bestPos = globalBestBird->getBestPos();
	bool useBestPos = false;

	if (pso->getDynamicStyle() == dynamicFct) {
		if (pso->get_functionType() == moving_peaks) {
			//moving_peaks stays within initial space
			//bestPos.clear();
			useBestPos = false;
		}
		else {
			//bestPos.assign(globalBestBird->getBestPos().begin(), globalBestBird->getBestPos().end());
			useBestPos = true;
		}
	}

	int nrRandomize = 0;
	int resetCount = 0;
 	tree<BirdContainer>::iterator it;
	
#ifdef STDOUT
	cout << "randomize [ ";
#endif
	for (it = pyramid.begin(); it != pyramid.end(); ++it) {
		if (pyramid.depth(it) < divisionDepth) {
			//the top sub-swarm
			if (randomizeTopSwarm) {
				//randomize and reset the topswarm
				if (pyramid.depth(it) == 0) {
					//the root
					randomizeBird(it->getBird(), bestPos, useBestPos);
					nrRandomize++;
#ifdef STDOUT
					cout << it->getIndex() << " (level " << pyramid.depth(it) << ") ";
#endif
				}
				else {
					//the lower levels of the top sub-swarm
					if (reset) {
						it->getBird()->setBestPos(it->getBird()->getPos());
						
						resetCount++;
					}
				}
			}

		}
		else if (pyramid.depth(it) == divisionDepth) {
			//the current leaders of the lower sub-swarms
			randomizeBird(it->getBird(), bestPos, useBestPos);
			nrRandomize++;
#ifdef STDOUT
			cout << it->getIndex() << " (level " << pyramid.depth(it) << ") ";
#endif
		}
		else {
			//the lower levels of the lower sub-swarms
			if (reset) {
				it->getBird()->setBestPos(it->getBird()->getPos());
						
				resetCount++;
			}
		}
	}
#ifdef STDOUT
	cout << "] - randomized " << nrRandomize << " particles " << endl;
#endif

#ifdef DBUG
	if (reset) {
		cout << "reset " << resetCount << " particles " << endl;		
	}
#endif


	TRACE_OUT("DynamicPyramidSwarm::randomizeSubSwarms", 0);	

	
}

bool DynamicPyramidSwarm::detectDynamicChange() {
	TRACE_IN("DynamicPyramidSwarm::detectDynamicChange");
	bool change;

	change = Swarm::detectDynamicChange();

	TRACE_OUT("DynamicPyramidSwarm::detectDynamicChange",change);	

	return change;
}

void DynamicPyramidSwarm::respondToChange() {
	TRACE_IN("DynamicPyramidSwarm::respondToChange");
	switch (responseMethod) {
		case subSwarmsTemporalMerge: {
			//The appropriate connections of the pyramid are disconnected
			//After a certain number of iterations they are merged.
			dividedSwarm = true;
			divisionDuration = 10;
			DEBUG1("swarm is divided at height ", divisionDepth);
			randomizeSubSwarms(true, false);
			updateBestValues();
			break;
		}
		case subSwarmsAdaptiveMerge: {
			//The appropriate connections of the pyramid are disconnected
			//If gbest is within a subswarm for a certain number of times
			dividedSwarm = true;
			divisionDuration = 10;
			DEBUG1("swarm is divided at height ", divisionDepth);
			randomizeSubSwarms(true, false);
			updateBestValues();
			break;
		}
		case subSwarmsTemporalMergeNoMemory: {
			//The appropriate connections of the pyramid are disconnected
			//After a certain number of iterations they are merged.
			dividedSwarm = true;
			divisionDuration = 10;
			DEBUG1("swarm is divided at height ", divisionDepth);
			randomizeSubSwarms(true, true);
			updateBestValues();
			break;
		}
		case subSwarmsAdaptiveMergeNoMemory: {
			//The appropriate connections of the pyramid are disconnected
			//If gbest is within a subswarm for a certain number of times
			dividedSwarm = true;
			divisionDuration = 10;
			DEBUG1("swarm is divided at height ", divisionDepth);
			randomizeSubSwarms(true, true);
			updateBestValues();
			break;
		}
		default: {
			//otherwise call the regular respond to change method
			Swarm::respondToChange();
			break;		
		}
	}

	TRACE_OUT("DynamicPyramidSwarm::respondToChange",0);		
}



AdaptivePyramidSwarm::AdaptivePyramidSwarm(PSO *pso_in, int swarmsize_in, int dim_in,											
														double min_x,
														double max_x,
														double min_v,
														double max_v,
														int height_in, 
														int branches_in,
														int swapDelay_in,
														int nrScouts_in
														) 
			: PyramidSwarm(pso_in, swarmsize_in, dim_in, min_x, max_x, min_v, max_v, 
				height_in, branches_in, swapDelay_in, nrScouts_in) {
	TRACE_IN("AdaptivePyramidSwarm::AdaptivePyramidSwarm");
	
	DEBUG("New AdaptivePyramidSwarm");
	
	nrScouts = nrScouts_in;
	//printPyramid();
	
	TRACE_OUT("AdaptivePyramidSwarm::AdaptivePyramidSwarm",0);
}

AdaptivePyramidSwarm::~AdaptivePyramidSwarm()  {
	TRACE_IN("AdaptivePyramidSwarm::~AdaptivePyramidSwarm");

#ifdef STDOUT
	printPyramid();
#endif

	TRACE_OUT("AdaptivePyramidSwarm::~AdaptivePyramidSwarm",0);
}

void AdaptivePyramidSwarm::initializeBird(Bird* newBird) {
	TRACE_IN("AdaptivePyramidSwarm::initializeBird");
	tree<BirdContainer>::iterator rootNode = pyramid.begin();

	newBird->setPos(rootNode->getBird()->getBestPos());
	newBird->setBestPos(rootNode->getBird()->getBestPos());
	newBird->setBestValue(rootNode->getBird()->getBestValue());


	TRACE_OUT("AdaptivePyramidSwarm::initializeBird",0);
}

void AdaptivePyramidSwarm::addSubtree(tree<BirdContainer>::iterator insertNode) {
	TRACE_IN("AdaptivePyramidSwarm::addSubtree");

	//build the pyramid of birdContainers
	//the birds are added to the birds vector

	//list of open nodes for building the pyramid breadth-first
	deque <tree<BirdContainer>::iterator> nodeList;
	tree<BirdContainer>::iterator node, top;
	Bird* tmpBird;
	BirdContainer* bc;

	//create the first node below insert node
	//after that the new subtree is inserted with the respective branchlevel
	tmpBird = createNewBird(min_x, max_x, min_v, max_v);
	initializeBird(tmpBird);
	bc = new BirdContainer(tmpBird, tmpBird->getIndex());
	nodeList.push_back(pyramid.append_child(insertNode, *bc));
	
	//nodeList.push_back(insertNode);
	
	while (!nodeList.empty()) {
		node = nodeList.front();
		nodeList.pop_front();
		
				
		if (pyramid.depth(node) < height-1) {
			//not reached the leaves yet
			//still further siblings added
			for (int i=0; i<branches; i++) {
				tmpBird = createNewBird(min_x, max_x, min_v, max_v);
				initializeBird(tmpBird);
				bc = new BirdContainer(tmpBird, tmpBird->getIndex());
				nodeList.push_back(pyramid.append_child(node, *bc));
			}
		}
		
		//if we are on the last level
		//and the pyramid is not balanced, then further 
		//children for node are added
		//TODO:add unbalanced subtree
/*		if (pyramid.depth(node) == height-1) {
			int nrExtraNodes = extraNodesList.front();
			extraNodesList.pop_front();
			for (int i=0; i<nrExtraNodes; i++) {
				//tmpBird = createBird(min_x, max_x, min_v, max_v, nrBirds);
				tmpBird = birds[nrBirds];
				bc = new BirdContainer(tmpBird, nrBirds);
				nodeList.push_back(pyramid.append_child(node, *bc));
				nrBirds++;				
			}
			DEBUG1("ExtraNodes added:",nrExtraNodes);			
		}*/
		
		
	}

	gbestFromSubtreeTick.push_back(0);
	for (int i=0; i<gbestFromSubtreeTick.size(); i++)
		gbestFromSubtreeTick[i]=pso->stat->getCurrentStep();


#ifdef DBUG
	printPyramid();
#endif

	
	
	TRACE_OUT("AdaptivePyramidSwarm::addSubtree",0);
}


void AdaptivePyramidSwarm::removeSubtree(tree<BirdContainer>::iterator removeNode) {
	TRACE_IN("AdaptivePyramidSwarm::removeSubtree");

  deque<tree<BirdContainer>::iterator> nodeList;
  deque<tree<BirdContainer>::iterator> removeList;
  nodeList.push_back(removeNode);
  removeList.push_back(removeNode);
  tree<BirdContainer>::iterator tmpNode;

  
  //the nodelist is built depth-first in order to remove the items later in ascending tree level order

  while (!nodeList.empty()) {
    tmpNode = nodeList.front();
    nodeList.pop_front();
    
        
    if (pyramid.begin(tmpNode) == pyramid.end(tmpNode)) {
      //No siblings
    }
    else {
      for (tree<BirdContainer>::sibling_iterator sib = pyramid.begin(tmpNode); 
	   sib != pyramid.end(tmpNode); ++sib) {
		   //depth-first
		nodeList.push_front(sib);
		removeList.push_front(sib);
      }
    }

  }

  while (!removeList.empty()) {
	tmpNode = removeList.front();
	removeList.pop_front();
	int removeIndex = tmpNode->getBird()->getIndex();
	removeBird(removeIndex);
	removeBirdContainerLevelsEntry(removeIndex);
	pyramid.erase(tmpNode);
  }

  gbestFromSubtreeTick.pop_back();
  for (int i=0; i<gbestFromSubtreeTick.size(); i++)
	  gbestFromSubtreeTick[i]=pso->stat->getCurrentStep();
	
  DEBUG1("removed subtree starting with node ", removeNode->getIndex());

  TRACE_OUT("AdaptivePyramidSwarm::removeSubtree",0);
}

tree<BirdContainer>::iterator AdaptivePyramidSwarm::getWorstSubtree() {

	TRACE_IN("AdaptivePyramidSwarm::getWorstSubtree");

	tree<BirdContainer>::iterator worstSubtree;
	double worstValue=-pso->getMax();

	tree<BirdContainer>::iterator top = pyramid.begin();

    for (tree<BirdContainer>::sibling_iterator sib = pyramid.begin(top); 
	sib != pyramid.end(top); ++sib) {
		DEBUG2("cmpWorstSubtree ", sib->getIndex(), sib->getBird()->getBestValue());
		if (pso->isBetter(worstValue, sib->getBird()->getBestValue())) {
			worstValue = sib->getBird()->getBestValue();
			worstSubtree = sib;
		}
    }

	DEBUG1("Worst Subtree starting with : ", worstSubtree->getIndex());

	return worstSubtree;
	
	TRACE_OUT("AdaptivePyramidSwarm::getWorstSubtree",0);

}


deque<BirdContainer> AdaptivePyramidSwarm::cutSubtree(tree<BirdContainer>::iterator cutNode) {
	TRACE_IN("AdaptivePyramidSwarm::cutSubtree");
	
	//tree<BirdContainer> cutTree;
	//cutTree = pyramid.subtree(pyramid.begin(cutNode), pyramid.end(cutNode));
	  deque<tree<BirdContainer>::iterator> nodeList;
	  deque<tree<BirdContainer>::iterator> cutList;
	  deque<BirdContainer> returnList;
	  nodeList.push_back(cutNode);
	  cutList.push_back(cutNode);
	  tree<BirdContainer>::iterator tmpNode;
	
	  
	  //the nodelist is built depth-first in order to cut the items later in ascending tree level order
	
	  while (!nodeList.empty()) {
	    tmpNode = nodeList.front();
	    nodeList.pop_front();
	    
	        
	    if (pyramid.begin(tmpNode) == pyramid.end(tmpNode)) {
	      //No siblings
	    }
	    else {
	      for (tree<BirdContainer>::sibling_iterator sib = pyramid.begin(tmpNode); 
		   sib != pyramid.end(tmpNode); ++sib) {
			   //depth-first
			nodeList.push_front(sib);
			cutList.push_front(sib);
	      }
	    }
	
	  }
	

	for (deque<tree<BirdContainer>::iterator>::iterator it=cutList.begin(); it!=cutList.end();
		it++) {
			pyramid.erase(*it);
			//dereference from deque-iterator to tree-iterator to BirdContainer
			returnList.push_back(*(*it));
		}

		
	  DEBUG1("cut subtree starting with node ", cutNode->getIndex());
	  
	 
	TRACE_OUT("AdaptivePyramidSwarm::cutSubtree",0);

	return returnList;

}


tree<BirdContainer>::iterator AdaptivePyramidSwarm::getWorstSubtree(tree<BirdContainer>::iterator top) {
	TRACE_IN("AdaptivePyramidSwarm::getWorstSubtree");

	tree<BirdContainer>::iterator worstSubtree;
	double worstValue = -pso->getMax();

    for (tree<BirdContainer>::sibling_iterator sib = pyramid.begin(top); 
	sib != pyramid.end(top); ++sib) {
		DEBUG2("cmpWorstSubtree ", sib->getIndex(), sib->getBird()->getBestValue());
		if (pso->isBetter(worstValue, sib->getBird()->getBestValue())) {
			worstValue = sib->getBird()->getBestValue();
			worstSubtree = sib;
		}
    }

	DEBUG1("Worst Subtree starting with : ", worstSubtree->getIndex());

	TRACE_OUT("AdaptivePyramidSwarm::getWorstSubtree",0);

	return worstSubtree;
	

}

tree<BirdContainer>::iterator AdaptivePyramidSwarm::getBestSubtree(tree<BirdContainer>::iterator top) {
	TRACE_IN("AdaptivePyramidSwarm::getBestSubtree");

	tree<BirdContainer>::iterator bestSubtree;
	double bestValue = pso->getMax();

    for (tree<BirdContainer>::sibling_iterator sib = pyramid.begin(top); 
	sib != pyramid.end(top); ++sib) {
		DEBUG2("cmpbestSubtree ", sib->getIndex(), sib->getBird()->getBestValue());
		if (pso->isBetter(sib->getBird()->getBestValue(), bestValue)) {
			bestValue = sib->getBird()->getBestValue();
			bestSubtree = sib;
		}
    }

	DEBUG1("Best Subtree starting with : ", bestSubtree->getIndex());

	TRACE_OUT("AdaptivePyramidSwarm::getBestSubtree",0);

	return bestSubtree;
	

}



tree<BirdContainer>::iterator AdaptivePyramidSwarm::getLRUSubtree() {

	TRACE_IN("AdaptivePyramidSwarm::getLRUSubtree");

	tree<BirdContainer>::iterator LRUSubtree;
	double LRUValue=MAXDOUBLE;

	tree<BirdContainer>::iterator top = pyramid.begin();

	int index=0;
    for (tree<BirdContainer>::sibling_iterator sib = pyramid.begin(top); 
	sib != pyramid.end(top); ++sib) {
		DEBUG2("cmpLRUSubtree ", sib->getIndex(), gbestFromSubtreeTick[index]);
		if (gbestFromSubtreeTick[index] < LRUValue) {
			LRUValue = gbestFromSubtreeTick[index];
			LRUSubtree = sib;
		}
		index++;
    }

	DEBUG1("LRU Subtree starting with : ", LRUSubtree->getIndex());

	TRACE_OUT("AdaptivePyramidSwarm::getLRUSubtree",0);

	return LRUSubtree;
	

}

bool AdaptivePyramidSwarm::noGbestInSubtreeSince(int threshold) {
	//how many subtrees did not produce gbest for threshold steps
	int nrNoGbestSince = 0;
	for (int i=0; i<gbestFromSubtreeTick.size(); i++) {
		if (pso->stat->getCurrentStep() - gbestFromSubtreeTick[i] > threshold) {
			nrNoGbestSince++;
		}
	}
	if (nrNoGbestSince > 0) {
		DEBUG2("in #steps gbest wasn't provided by #subtrees: ",threshold,nrNoGbestSince);
		return true;
	}
	else
		return false;

}


void AdaptivePyramidSwarm::decreaseBranchDegree() {
	TRACE_IN("AdaptivePyramidSwarm::decreaseBranchDegree");

	int newBranches = branches-1;
	
	OUTPUT1("Decreasing branch degree from ", branches << " to " << newBranches);

	//list of open nodes for building the pyramid breadth-first
	deque <tree<BirdContainer>::iterator> nodeList;
	tree<BirdContainer>::sibling_iterator node;

	deque<BirdContainer> pasteList;



	//in each level remove the worst subtree
	//this is done recursively for all groups of children larger than newBranches
	//once on the first level, newBranches times on the second level, ..., newBranches**(level-1)

	nodeList.push_back(pyramid.begin());


	
	//nodeList.push_back(insertNode);
	
	while (!nodeList.empty()) {
		node = nodeList.front();
		nodeList.pop_front();

		if (pyramid.number_of_children(node) > newBranches) {
			//cut the best or worst subtree
			deque<BirdContainer> tmpList;
			if ((pso->getDecreaseBranchAction() == bestSubtreeReset) || 
					(pso->getDecreaseBranchAction() == bestSubtreeNoAction)) {
				tmpList = cutSubtree(getBestSubtree(node));
			}
			else {
				tmpList = cutSubtree(getWorstSubtree(node));
			}
			
			for (deque<BirdContainer>::iterator it = tmpList.begin(); 
				it != tmpList.end(); it++) {
					if ((pso->getDecreaseBranchAction() == reset) ||
							(pso->getDecreaseBranchAction() == bestSubtreeReset)) {
						(*it).getBird()->setBestPos((*it).getBird()->getPos());
						//Evaluate in order to satisfy assert in pso.step
						(*it).getBird()->setBestValue(pso->evalFunction((*it).getBird()->getPos()));
					}
					pasteList.push_back(*it);
				}
		}

		for (tree<BirdContainer>::sibling_iterator sib = pyramid.begin(node); 
				sib != pyramid.end(node); ++sib) {
			nodeList.push_back(sib);	
		} 
			

		
	}





	//now the cut nodes are inserted into the new hierarchy of degree newBranches



	nodeList.clear();
	nodeList.push_back(pyramid.begin());


	
	//nodeList.push_back(insertNode);
	
	//find the node at which to start pasting
	//the first node on the second last level with the least number of children
	int minChildren = MAXINT;
	tree<BirdContainer>::iterator insertNode;
	while (!nodeList.empty()) {
		node = nodeList.front();
		nodeList.pop_front();
		//we are looking at the parentnodes of the leaves -- leaves on level height-1
		if (pyramid.depth(node) == getActualHeight()-2) {
			if (pyramid.number_of_children(node) < minChildren) {
				minChildren = pyramid.number_of_children(node);
				insertNode = node;
			}
		}
		//traverse the tree
		for (tree<BirdContainer>::sibling_iterator sib = pyramid.begin(node); 
				sib != pyramid.end(node); ++sib) {
			nodeList.push_back(sib);	
		}

	}
	DEBUG1("start inserting after node ", (*insertNode).getIndex() << " " << (*insertNode).getBird());

	bool done=false;
	//remember at what depth we started inserting
	//this is increased, in case the leaves are entirely filled
	int insertDepth = pyramid.depth(insertNode);
	//the list of nodes after which the cut nodes are pasted
	deque<tree<BirdContainer>::iterator> nodesAtInsertDepth = getAllNodesAtDepth(insertDepth);
	//start iterating at the insertNode determined before
	//then proceed, until the end of the level is reached and restart at the first node on that level
	deque<tree<BirdContainer>::iterator>::iterator depthIterator = find(nodesAtInsertDepth.begin(),nodesAtInsertDepth.end(),insertNode);
	
	while (!pasteList.empty()) {
		while (!done) {
			//whether all of the insertNodes have newBranches number of children
			done=true;
			do {
				if (!pasteList.empty()) {
					//not enough children yet
					if (pyramid.number_of_children(*depthIterator) < newBranches) {
						done=false;
						pyramid.append_child(*depthIterator, pasteList.front());
						pasteList.pop_front();
					}
				}
				else {
					//nothing to be pasted anymore
					done=true;
					break;
				}

				//next insertNode
				depthIterator++;
			
			} while (depthIterator != nodesAtInsertDepth.end());

			//if the last insertNode is reached, we return to the first node on the same level
			depthIterator = nodesAtInsertDepth.begin();
		} //while (!done)

		//all of the nodes at insertDepth have enough children
		//start a new level
		if (!pasteList.empty()) {
			insertDepth++;
			nodesAtInsertDepth = getAllNodesAtDepth(insertDepth);
			depthIterator = nodesAtInsertDepth.begin();
			currentHeight++;
			done=false;
			//resume while (!done) {}
		}

	} //while (!pasteList.empty())

#ifdef DBUG
	printPyramid();
#endif

	branches = newBranches;


	DEBUG2("New branch degree and height ", branches, getActualHeight());



	
	TRACE_OUT("AdaptivePyramidSwarm::decreaseBranchDegree",branches);
}



tree<BirdContainer>::iterator AdaptivePyramidSwarm::getFirstNodeAtDepth(int depth) {

	deque <tree<BirdContainer>::iterator> nodeList;
	tree<BirdContainer>::iterator node;



	//in each level remove the worst subtree
	//this is done recursively for all groups of children larger than newBranches
	//once on the first level, newBranches times on the second level, ..., newBranches**(level-1)

	nodeList.push_back(pyramid.begin());

	//nodeList.push_back(insertNode);
	
	while (!nodeList.empty()) {
		node = nodeList.front();
		nodeList.pop_front();

		if (pyramid.depth(node) == depth) {
			return node;
		}

		for (tree<BirdContainer>::sibling_iterator sib = pyramid.begin(node); 
				sib != pyramid.end(node); ++sib) {
			nodeList.push_back(sib);	
		} 		
	}
	//this should never be reached
	cerr << "no node at given depth " << depth << endl;
	exit(-1);

	return node;
}



deque<tree<BirdContainer>::iterator> AdaptivePyramidSwarm::getAllNodesAtDepth(int depth) {

	deque <tree<BirdContainer>::iterator> nodeList;
	deque <tree<BirdContainer>::iterator> depthList;
	tree<BirdContainer>::iterator node;



	//in each level remove the worst subtree
	//this is done recursively for all groups of children larger than newBranches
	//once on the first level, newBranches times on the second level, ..., newBranches**(level-1)

	nodeList.push_back(pyramid.begin());


	
	//nodeList.push_back(insertNode);
	
	while (!nodeList.empty()) {
		node = nodeList.front();
		nodeList.pop_front();

		if (pyramid.depth(node) == depth) {
			depthList.push_back(node);
		}

		for (tree<BirdContainer>::sibling_iterator sib = pyramid.begin(node); 
				sib != pyramid.end(node); ++sib) {
			nodeList.push_back(sib);	
		} 		
	}

	return depthList;
}



//the step method is basically identical to PyramidSwarm
int AdaptivePyramidSwarm::step(int steps) {
	TRACE_IN("AdaptivePyramidSwarm::step");
	int ok=1;

	for (int nrStep=0; nrStep<steps; nrStep++) {

		if (!(pso->isAdaptiveParametersSet())) {
			cerr << "adaptive parameters not set" << endl;
			exit(-1);
		}


		Bird *itrBestBird;

		if ((pso->getStep() % swapDelay) == 0) {
			swapEnabled = true;
		}
		else
			swapEnabled = false;

		
		//evaluate current Positions and return iteration best bird
		itrBestBird = evaluatePos();
		
		//call the Swarm::compare() method in order to update and log globalBestBird
		Swarm::compare(itrBestBird);
		
		//call the PyramidSwarm compare() and adjust Pyramid hierarchy
		hierarchicalCompare();
		
		//update velocity towards global best
		updateVel();

		//move with current velocity
		move();
		
		//just for output purposes, update the BirdContainerLevels
		updateBirdContainerLevels();
		
		
#ifdef DIVERSITY
		updateMeanPosition();	
		calculateDiversity();
		calculateDiversitySubtree();		
#endif
		
		
		if (pso->getDecreaseBranchFrequency() != 0) {
			if ((pso->getStep()!=0)&&(pso->getStep()%pso->getDecreaseBranchFrequency() == 0)) {
				for (int i=0; i<pso->getDecreaseBranchStep(); i++) {
					if (branches > pso->getMinBranchDegree()) {

		
						decreaseBranchDegree();

					}
				}
			}
		}



		addSubtree(pyramid.begin());
		///DEBUG
		// printPyramid();
 
		if (pso->getStep() % 3 == 2) {
		  //cout << "remove" << endl;
			removeSubtree(getBestSubtree(pyramid.begin()));
			//printPyramid();
			//cout << "add" << endl;
			addSubtree(pyramid.begin());
			//printPyramid();
		}
		//iteration is done
		pso->iterationDone();
		
	}
	TRACE_OUT("AdaptivePyramidSwarm::step",ok);
	return ok;
}




//the size 0 passed to the swarm constructor is corrected after computing the
//actual size of the Ring
RingSwarm::RingSwarm(PSO *pso_in, 
														int dim_in,											
														double min_x,
														double max_x,
														double min_v,
														double max_v,
														int length_in, 
														int branches_in,
														int swapDelay_in
														) : Swarm(pso_in, 0, dim_in, min_x, max_x, min_v, max_v) {
	TRACE_IN("RingSwarm::RingSwarm");
	branches = branches_in;
	length = length_in;
	//compute the swarmsize from length and branches
	swarmsize = branches*length;

	swapDelay = swapDelay_in;
	
	//birds = new Bird *[swarmsize];

	
	bestBirds = new Bird *[length];
	for (int i=0; i<length; i++)
		bestBirds[i] = NULL;
	
	
	swapEnabled = true;	


	//create the birds
	createSwarm(min_x, max_x, min_v, max_v);
	
#ifdef DBUG
	printRing();
#endif
	
	
	TRACE_OUT("RingSwarm::RingSwarm",0);
}

RingSwarm::~RingSwarm() {
	TRACE_IN("RingSwarm::~RingSwarm");
	for (int i = 0; i < length; i++) {
		delete bestBirds[i];
	}
	delete[] bestBirds;
	TRACE_OUT("RingSwarm::~RingSwarm",0);
}


void RingSwarm::printRing() {
	string BLANK = "                                                    ";

	for (int node=0; node < length; node++) {

		for (int ind=0; ind < branches; ind++) {
			int index = node*branches + ind;
			//for all contained individuals
			cout << index << " " << birds[index] << ": " << birds[index]->getBestValue() << endl;
		}
		cout << endl;
	}

}



void RingSwarm::compare(Bird* itrBestBird) {
	//itrBestBird not used !!! only for compatibility to Swarm::compare
	TRACE_IN("RingSwarm::compare");
	//for each neighbourhood determine the iteration best and compare with the
	//neighbourhood best
		
	
	
	//for each ringnode
	for (int ringnode=0; ringnode < length; ringnode++) {
		//for all contained individuals
		for (int ind=0; ind < branches; ind++) {
			//compare with the neighbourhood best
			int bird_i = ringnode*branches + ind;
			Bird *birdPtr=birds[bird_i];
			
			//for step 1 just pick the first bird as global best
			if (bestBirds[ringnode] == NULL) {
				//call the copy constructor -- only the current information must be passed
				//not the reference
				//dereference old pointer, the memory can be released
				delete bestBirds[ringnode];
				bestBirds[ringnode] = new Bird(*birdPtr);
			}
			else {
				//pick new neighbourhood best 
				if (pso->isBetter(birdPtr->getCurrentValue(), bestBirds[ringnode]->getBestValue())) {
					//dereference old pointer, the memory can be released
					delete bestBirds[ringnode];
					bestBirds[ringnode] = new Bird(*birdPtr);
				}
			}
		}
	}
	
	TRACE_OUT("RingSwarm::compare",0);
}

void RingSwarm::compareRing() {
	TRACE_IN("RingSwarm::compareRing");
	
	//the birds are compared cyclic
	
	//store the old first bestBird for later comparsion
	Bird * bestBird0 = new Bird(*bestBirds[0]);
	
	
	if (swapEnabled) {
		//compare bestBird with following ringNode's best bird
		for (int i=0; i < length-1; i++) {
			
			//if the following best bird is better, it is copied to this neighbourhood
			if (pso->isBetter(bestBirds[i+1]->getBestValue(), bestBirds[i]->getBestValue())) {
				//dereference old pointer, the memory can be released
				delete bestBirds[i];
				bestBirds[i] = new Bird(*bestBirds[i+1]);
				pso->stat->swapped(0);
			}
		}
		//to avoid comparing with the possibly new bestBirds[0] use the stored one
		if (pso->isBetter(bestBird0->getBestValue(), bestBirds[length-1]->getBestValue())) {
			//dereference old pointer, the memory can be released
			delete bestBirds[length-1];
			bestBirds[length-1] = new Bird(*bestBird0);
			pso->stat->swapped(0);
		}
	}


	//The best bird in neighbourhood 0 is chosen, not necessarily the global best at that time
	pso->stat->addGlobalBest(bestBirds[0]->getBestValue());
	
	DEBUG2("gloBest ", bestBirds[0], bestBirds[0]->getBestValue());

	//dereference old pointer, the memory can be released
	delete bestBird0;


	TRACE_OUT("RingSwarm::compareRing",0);
}

void RingSwarm::updateVel() {
	TRACE_IN("RingSwarm::updateVel");
	
	for (int node=0; node<length; node++)
		for (int ind=0; ind<branches; ind++) {
			int index = node*branches + ind;
			Bird *birdPtr=birds[index];
		
			birdPtr->updateVelocity(bestBirds[node]->getBestPos());
		
		}	

	TRACE_OUT("RingSwarm::updateVel",0);
}


int RingSwarm::step(int steps) {
	TRACE_IN("RingSwarm::step");
	int ok=1;

	for (int nrStep=0; nrStep<steps; nrStep++) {
		Bird *itrBestBird;

		if ((pso->getStep() % swapDelay) == 0) {
			swapEnabled = true;
			//cout << pso->getStep() << ": compare" << endl;
		}
		else
			swapEnabled = false;

		//evaluate current Positions and return iteration best bird
		itrBestBird = evaluatePos();

		//compare birds and adjust the neighbourhood best
		compare(itrBestBird);
		
		//compare with the succeeding neighbourhood
		compareRing();
		
		//update velocity towards global best
		updateVel();

#ifdef DIVERSITY
		updateMeanPosition();	
		calculateDiversity();	
#endif

		//move with current velocity
		move();
				
		//iteration is done
		pso->iterationDone();
		
	}
	TRACE_OUT("RingSwarm::step",ok);
	return ok;
}





LocalSwarm::LocalSwarm(PSO *pso_in, 
														int size_in,
														int dim_in,											
														double min_x,
														double max_x,
														double min_v,
														double max_v,
														int nSize_in, 
														int swapDelay_in
														) : Swarm(pso_in, size_in, dim_in, min_x, max_x, min_v, max_v) {
	TRACE_IN("LocalSwarm::LocalSwarm");
	DEBUG("New LocalSwarm :");
	DEBUG1("pso = ",pso_in);
	DEBUG1("size = ",size_in);
	DEBUG1("dim = ",dim_in);
	DEBUG1("min_x = ",min_x);
	DEBUG1("max_x = ",max_x);
	DEBUG1("min_v = ",min_v);
	DEBUG1("max_v = ",max_v);
	DEBUG1("nSize = ",nSize_in);
	DEBUG1("swapDelay = ",swapDelay_in);


	nSize = nSize_in;

	swapDelay = swapDelay_in;
	
	
	iterationBestBirds = new int[swarmsize];
	for (int i=0; i<swarmsize; i++)
		iterationBestBirds[i] = -1;
	
	
	swapEnabled = true;	


	
	
	TRACE_OUT("LocalSwarm::LocalSwarm",0);
}

LocalSwarm::~LocalSwarm() {
	TRACE_IN("LocalSwarm::~LocalSwarm");
	delete[] iterationBestBirds;
	TRACE_OUT("LocalSwarm::~LocalSwarm",0);
}




void LocalSwarm::compareLocal() {
	//itrBestBird not used !!! only for compatibility to Swarm::compare
	TRACE_IN("LocalSwarm::compareLocal");
	//for each neighbourhood determine the iteration best and store the index
	//in iterationBestBirds
		
	double bestValue;
	Bird* birdPtr;
	
	//for each bird
	for (int bird_i=0; bird_i < swarmsize; bird_i++) {
		//for the considered neighbourhood
		//including the specific bird (nIndex = 0)
		
		//the first bird is taken as reference value
		birdPtr=birds[(bird_i-nSize+swarmsize)%swarmsize];
		bestValue = birdPtr->getBestValue();
		//C does not like modulo of negative numbers

		//nSize determines the number of neighbours on ONE side
		//nSize=1 means three birds are included in the neighbourhood
		//left, self, right
		iterationBestBirds[bird_i] = (bird_i-(nSize)+swarmsize)%swarmsize;
		
//WRONG!!!		for (int nIndex=-nSize+1; nIndex <=nSize ; nIndex++) {
		//corrected for nSize on ONE side
		//<=nsize, because individual is included in neighbourhood (-> nSize+1)
		//index=1 because leftmost bird taken as reference
		for (int index=1; index <=2*nSize ; index++) {
			int nIndex=index-nSize;
			//compare with the neighbourhood best
			birdPtr=birds[(bird_i+nIndex+swarmsize)%swarmsize];
			if (pso->isBetter(birdPtr->getBestValue(), bestValue)) {
				bestValue = birdPtr->getBestValue();
				iterationBestBirds[bird_i] = (bird_i+nIndex+swarmsize)%swarmsize;		
			}
		}
	}
#ifdef DBUG
	for (int i=0; i<swarmsize; i++)
		cout << i << " -> " <<iterationBestBirds[i] << " | ";
	cout << endl;
#endif
	
	TRACE_OUT("LocalSwarm::compareLocal",0);
}

void LocalSwarm::updateVel() {
	TRACE_IN("LocalSwarm::updateVel");
	
	for (int bird_i=0; bird_i<swarmsize; bird_i++) {
		Bird *birdPtr=birds[bird_i];
		
		birdPtr->updateVelocity(birds[iterationBestBirds[bird_i]]->getBestPos());
		
	}	

	TRACE_OUT("LocalSwarm::updateVel",0);
}


int LocalSwarm::step(int steps) {
	TRACE_IN("LocalSwarm::step");
	int ok=1;

	for (int nrStep=0; nrStep<steps; nrStep++) {
		Bird *itrBestBird;

		//swapEnabled not used anywhere yet
		if ((pso->getStep() % swapDelay) == 0) {
			swapEnabled = true;
		}
		else
			swapEnabled = false;

		//evaluate current Positions and return iteration best bird
		itrBestBird = evaluatePos();

		//only called to determine and log the current global best
		//the globalBestBird is not used anywhere
		//calls Swarm::compare()
		compare(itrBestBird);
		
		//compare birds within neighbourhood and adjust the neighbourhood best
		compareLocal();
		
		//pass the current neighbourhood to the sidestep method
		if (pso->getLocalsearchMode() == sidestep) {
			for (int k=0; k<swarmsize; k++) {
				Bird *birdPtr=birds[k];
				birdPtr->sidestep(birds[iterationBestBirds[k]]->getBestPos());
			}	
		}
		
		
		//update velocity towards global best
		updateVel();

		//move with current velocity
		move();
		
#ifdef DIVERSITY
		updateMeanPosition();	
		calculateDiversity();	
#endif
				
		//iteration is done
		pso->iterationDone();
		
		if (pso->getDynamicStyle() == dynamicFct) {
			if (detectDynamicChange()) {
				OUTPUT1("Change detected at step ", pso->stat->getCurrentStep()-1 << " evaluate pbest positions, response ");
				respondToChange();
			}
		}

	}
	TRACE_OUT("LocalSwarm::step",ok);
	return ok;
}


ScoutSwarm::ScoutSwarm(PSO *pso_in, 
														int size_in,
														int dim_in,											
														double min_x,
														double max_x,
														double min_v,
														double max_v,
														int nrScouts_in,
														int nrWorkers_in,
														int swapDelay_in
														) : Swarm(pso_in, size_in, dim_in, min_x, max_x, min_v, max_v) {
	TRACE_IN("ScoutSwarm::ScoutSwarm");
	DEBUG("New ScoutSwarm :");
	DEBUG1("pso = ",pso_in);
	DEBUG1("size = ",size_in);
	DEBUG1("dim = ",dim_in);
	DEBUG1("min_x = ",min_x);
	DEBUG1("max_x = ",max_x);
	DEBUG1("min_v = ",min_v);
	DEBUG1("max_v = ",max_v);
	DEBUG1("nrScouts = ",nrScouts_in);
	DEBUG1("nrWorkers = ",nrWorkers_in);
	DEBUG1("swapDelay = ",swapDelay_in);


	nrScouts = nrScouts_in;
	nrWorkers = nrWorkers_in;

	swapDelay = swapDelay_in;
	

	//the swarm consists of #nrScouts scouts and #(nrWorkers*nrScouts) workers	
	if (nrScouts*nrWorkers + nrScouts != swarmsize) {
		cout << "nrScouts*nrWorkers + nrScouts != swarmsize\n" << 
		"The number of workers per scout does not match the given swarmsize" << endl;
		exit(-1); 	
	}


	//the birds are distributed among the scouts
	//swarmsize/nrScouts individuals per scout
	
	for (int i=0; i<swarmsize; i++) {
		if (i < nrScouts)
			//individual is a scout
			useScout.push_back( -1 );
		else
			//individual follows scout #((i-nrScouts) / nrScouts)
			//integer division
			useScout.push_back( (i-nrScouts) / nrScouts);
	}
	
	swapEnabled = false;	


	
	
	TRACE_OUT("ScoutSwarm::ScoutSwarm",0);
}

ScoutSwarm::~ScoutSwarm() {
	TRACE_IN("ScoutSwarm::~ScoutSwarm");
	useScout.clear();
	TRACE_OUT("ScoutSwarm::~ScoutSwarm",0);
}





void ScoutSwarm::updateVel() {
	TRACE_IN("ScoutSwarm::updateVel");
	
	for (int bird_i=0; bird_i<swarmsize; bird_i++) {
		Bird *birdPtr=birds[bird_i];
		
		if (useScout[bird_i] != -1)
			//worker 
			birdPtr->updateVelocity(birds[useScout[bird_i]]->getBestPos(), 1);
		else {

			//scout 

//			birdPtr->updateVelocity(globalBestBird->getBestPos(), 0);
//self-update results in stagnation
//			birdPtr->updateVelocity(birdPtr->getBestPos(), 0);

			vector<double> tmpPos;
			for (int j = 0; j < pso->get_dim(); j++) {
				tmpPos.push_back(randDoubleRange(min_v, max_v));
			}
			birdPtr->updateVelocity(tmpPos, 0);
		}
		
	}	

	TRACE_OUT("ScoutSwarm::updateVel",0);
}


int ScoutSwarm::step(int steps) {
	TRACE_IN("ScoutSwarm::step");
	int ok=0;

	for (int nrStep=0; nrStep<steps; nrStep++) {
		Bird *itrBestBird;

		//swapEnabled not used anywhere yet
		if ((pso->getStep() % swapDelay) == 0) {
			swapEnabled = true;
		}
		else
			swapEnabled = false;

		//evaluate current Positions and return iteration best bird
		itrBestBird = evaluatePos();

		//only called to determine and log the current global best
		//the globalBestBird is not used anywhere
		//calls Swarm::compare()
		compare(itrBestBird);
				
		//update velocity towards global best
		updateVel();

		//move with current velocity
		move();
		
#ifdef DIVERSITY
		updateMeanPosition();	
		calculateDiversity();	
#endif
				
		//iteration is done
		pso->iterationDone();
		
	}
	TRACE_OUT("ScoutSwarm::step",ok);
	return ok;
}

int ScoutSwarm::getUseScoutBird(int bird_i) {
	return useScout[bird_i];
}


