#include "bird.h"
#include <stdio.h>


Bird::Bird(PSO* pso, Swarm *swarm, const vector<double>& pos_in, const vector<double>& vel_in, int index_in) { 
	TRACEV_IN("Bird::Bird");
	index = index_in;
	mySwarm = swarm;
	psoAlgo = pso;
	pos.assign(pos_in.begin(), pos_in.end());
	bestPos.assign(pos_in.begin(), pos_in.end());
	bestValue = psoAlgo->evalFunction(bestPos);
	lastBestValue = bestValue;
	vel.assign(vel_in.begin(), vel_in.end());
	
	
	DEBUGV1("A new bird was born at ",printVec(pos));
	DEBUGV1("with velocity ",printVec(vel));
	TRACEV_OUT("Bird::Bird",0);
}

Bird::Bird(const Bird& copyBird) { 
	TRACEV_IN("Bird::Bird(const Bird&)");
	index = copyBird.index;
	mySwarm = copyBird.mySwarm;
	psoAlgo = copyBird.psoAlgo;
	//invoke copy-constructors for vectors
//	pos = vector<double>(copyBird.pos);
	pos.assign(copyBird.pos.begin(), copyBird.pos.end());
//	bestPos = vector<double>(copyBird.bestPos);
	bestPos.assign(copyBird.bestPos.begin(), copyBird.bestPos.end()); 
//	vel = vector<double>(copyBird.vel);
	vel.assign(copyBird.vel.begin(), copyBird.vel.end());

	samples.assign(copyBird.samples.begin(), copyBird.samples.end());
	//no extra evaluation
	//bestValue = psoAlgo->evalFunction(bestPos);
	bestValue = copyBird.bestValue;
	lastBestValue = copyBird.lastBestValue;
	DEBUGV1("A new bird was copied at ",printVec(pos));
	DEBUGV1("with velocity ",printVec(vel));
	TRACEV_OUT("Bird::Bird",0);
}

void Bird::initializeSidestep(double sidestep) {

	_sidestepwidth = new double[psoAlgo->get_dim()];
	for (int d = 0; d < psoAlgo->get_dim(); ++d)
		_sidestepwidth[d] = sidestep;
	
}

Bird::~Bird() {
	TRACEV_IN("Bird::~Bird");
	pos.clear();
	vel.clear();
	TRACEV_OUT("Bird::~Bird",0);
}


void Bird::move() {
	TRACEV_IN("Bird::move");
	for (int i = 0; i < vel.size(); i++) {
		//update current position
		pos[i] = pos[i] + vel[i];
	}
	TRACEV_OUT("Bird::move",0);
}

void Bird::circumvent(double alpha) {
	
	
	//the radius of the circle is given by the distance of current position to best position
	double radius = sqrt( pow(pos[0]-bestPos[0], 2.0) + pow(pos[1]-bestPos[1], 2.0));
	//angle between y and x
	double omega = asin((pos[1]-bestPos[1])/radius);
	if (pos[0] < bestPos[0])
			omega = M_PI - omega;
	
	//the new position is given on the circle defined by pso and pbest with the angle omega+alpha
	if (! isnan(omega)) {
		pos[0] = bestPos[0] + cos(omega+alpha)*radius;
		pos[1] = bestPos[1] + sin(omega+alpha)*radius;
	}

}


void Bird::updateVelocity(const vector<double>& globalBest, int level) {
	//level is only used for #PyramidSwarm with scaled w
	TRACEV_IN("Bird::updateVelocity");
	for (int i = 0; i < vel.size(); i++) {
		double newVal = psoAlgo->get_w(level) * vel[i] + psoAlgo->get_c1() * randDouble() * (bestPos[i] - pos[i]) + psoAlgo->get_c2() * randDouble() * (globalBest[i] - pos[i]);

		//apply constriction factor
		newVal = psoAlgo->get_constriction() * newVal;

		//set new velocity for following step
		vel[i] = psoAlgo->apply_vmax(newVal);

		//vel[i] = newVal;
	}
	TRACEV_OUT("Bird::updateVelocity",0);
}

double Bird::evalCurrentPos() {
	TRACEV_IN("Bird::evalCurrentPos");
	currentValue = psoAlgo->evalFunction(pos);

	//for dynamically changing functions
	//add updateFrequency

	
	//if (psoAlgo->getDynamicStyle() == dynamicFct)
	//if the global best position has been invalidated
	//dynamic functions
	
	//update called from swarm detect/response
	//if (bestPosInvalid)
	//	updateBestValue();

	if (psoAlgo->isBetter(currentValue, getBestValue())) {
		bestValue = currentValue;
		lastBestValue = currentValue;
		bestPos.assign(pos.begin(), pos.end());
	}
	TRACEV_OUT("Bird::evalCurrentPos",currentValue);
	return currentValue;
}

double Bird::getEuclideanVelocity() {
	double euclVel=0;
	double exp = 2.0;
	for (int i = 0; i < vel.size(); i++) {
		//dimension is vel.size()
		euclVel += pow(vel[i], exp);
	}
	exp = 1.0/exp;
	euclVel = pow (euclVel, exp);
	return euclVel;
}

/*void Bird::invalidateBestPos() {
	bestPosInvalid = True;	
}*/

bool Bird::updateBestValue() {
	double bestValueNew = psoAlgo->evalFunction(bestPos);
	bool change = (bestValue != bestValueNew);

	if (psoAlgo->getBestValueSamples() > 0) {
		samples.push_back(bestValueNew);
		if (samples.size() > psoAlgo->getBestValueSamples())
			samples.pop_front();
	}

	bestValue = bestValueNew;
	//bestPosInvalid = False;
	return change;
}


void Bird::sidestep(const vector<double>& nbestPos) {
	for (int d=0; d<pos.size(); d++) {
		if ((fabs(pos[d] - bestPos[d]) < _sidestepwidth[d]) 
			&& (fabs(pos[d] - nbestPos[d]) < _sidestepwidth[d]) 
			&& (fabs(vel[d]) < _sidestepwidth[d])) {
			if (vel[d] > 0)
				vel[d] -= psoAlgo->getSidestepMagnitude()*_sidestepwidth[d];
			else
				vel[d] += psoAlgo->getSidestepMagnitude()*_sidestepwidth[d];
			//cerr << "sidestep " << d << ": " << _sidestepwidth[d]*10 << endl;
			_sidestepwidth[d] /= 2.0;
		}
  }	

  

}



double Bird::getBestValue() {
/*	if (psoAlgo->getBestValueSamples() > 0) {
		if (samples.size() > 0) {
			double retVal = 0.0;
			for (int i=0; i<samples.size(); i++)
				retVal += samples[i];
			return retVal/samples.size();
		}
		else
			return bestValue;
	}
	else*/
		return bestValue;
}

double Bird::getLastBestValue() {
	return lastBestValue;
}

void Bird::setBestValue(double bestValue_in) {
	bestValue = bestValue_in;
}

void Bird::setLastBestValue(double lastBestValue_in) {
	lastBestValue = lastBestValue_in;
}

void Bird::setPos(const vector<double>& pos_in) {
	pos.assign(pos_in.begin(), pos_in.end());
}

void Bird::setBestPos(const vector<double>& bestPos_in) {
	bestPos.assign(bestPos_in.begin(), bestPos_in.end());
}

void Bird::setVel(const vector<double>& vel_in) {
	vel.assign(vel_in.begin(), vel_in.end());
}
