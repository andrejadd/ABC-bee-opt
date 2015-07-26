#ifndef BIRD_H  
#define BIRD_H

#ifndef PSOGLOBALS
#include "pso_globals.h"
#endif

#include <deque>

///The individual bird
/**A bird knows its position, its velocity and its personal best position.*/
class Bird {
	protected:

	

	///The swarm the bird belongs to.
	Swarm *mySwarm;
	///The PSO algorithm with the parameters used
	PSO *psoAlgo;
	///Current position (dimension n)
	vector<double> pos; 
	
	///Velocity in all n directions
	vector<double> vel; 	
	//Velocity vel;

	///Personal best position
	vector<double> bestPos; 

	///Function value of the personal best position
	double bestValue;

	///Remember last best value for #noisyIsBetter
	double lastBestValue;

	///Function value of the current position
	double currentValue;
	
	///The index of the bird
	/**within the birds array of the swarm*/
	int index;

	///Store #NRSAMPLES samples of bestValues
	deque<double> samples;

	///Used for noisyBirdIsBetter
	/*If the bird has already been re-evaluated in this iteration*/
	bool reEvaluated;
	
	
	double* _sidestepwidth;

	public:
	///Constructor for a new bird
	Bird(PSO *pso,///<The PSO algorithm used
				Swarm *swarm, ///<The containing swarm
				const vector<double>& pos_in, ///<The position the bird is born
				const vector<double>& vel_in, ///<The initial velocity
				int index ///<The index within the swarm, not used for scouts
				);
	///The copy constructor
	Bird(const Bird& copyBird);
	///The allocated ressources are freed
	~Bird();
	
	///Move with current velocity
	void move();
	
	///Move around the personal best position with the given angle
	void circumvent(double alpha);
	
	void initializeSidestep(double sidestep);
	

	///Update velocity according to global best
	/**The level is used for PyramidPSO with scaled w*/
	void updateVelocity(const vector<double>& globalBest, int level=0);
	
	///Evaluate the function at the current position and store new best value
	double evalCurrentPos();
	
	///Return the function value at the current position
	double getCurrentValue() {return currentValue;};
	
	///Return the current position
	vector<double> getPos() {return pos;}
	///Return the personal best position
	vector<double> getBestPos() {return bestPos;}
	///Return the velocity vector
	vector<double> getVel() {return vel;}
	
	///Set the current position
	void setPos(const vector<double>& pos_in);
	///Set the personal best position
	void setBestPos(const vector<double>& bestPos_in);
	
	///Set the personal best value
	/**used by noisyBirdIsBetter, to refine the personal best evaluation*/
	void setBestValue(double bestValue_in);
	///Set the last evaluation of the personal best value
	/**used by noisyBirdIsBetter, to refine the personal best evaluation*/
	void setLastBestValue(double lastBestValue_in);
	
	///Set the velocity vector
	void setVel(const vector<double>& vel_in);

	///Return the index
	int getIndex() {return index;};

	///Return the PSO algorithm -- for copy constructor
	PSO* getPsoAlgo() {return psoAlgo;};
	
	///Return the swarm -- for copy constructor
	Swarm* getSwarm() {return mySwarm;};
	
	///Return the function value of the personal best position
	double getBestValue();
	///Return the last function value evaluation of the personal best position (for noisy functions)
	double getLastBestValue();

	///If the bird has already been re-evaluated in this iteration
	bool getReEvaluated() {return reEvaluated;};
	///Reset the current reEvaluation status
	void setReEvaluated(bool reEvaluated_in) {reEvaluated = reEvaluated_in;};
	
	///For dynamic functions the function value of the personal best position is updated.
	/**@return True if the bestValue changed, i.e. an environment change occured*/
	bool updateBestValue();
	
	///The best position value gets invalidated
	/**It is reevaluated at the next #evaluatePosition call.
	 * Required for dynamic Functions*/
	void invalidateBestPos();
	
	///Returns the Euclidean Norm of the velocity
	/**sqrt(sum x_i^2)*/
	double getEuclideanVelocity();
	
	///check if a dimension has vanished, if so perform a side step
	void sidestep(const vector<double>& nbestPos);
	
	double* getSidesteps() {return _sidestepwidth;};
	
};


#endif //ifndef BIRD_H
