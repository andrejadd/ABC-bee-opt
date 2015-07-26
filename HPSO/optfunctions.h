#ifndef OPTFUNCTIONS_H
#define OPTFUNCTIONS_H

#include <stdio.h>
#include <math.h>
#include <ctime>
#include <iomanip>
#include <vector>

#ifndef TOOLS_H
#include "tools.h"
#endif

extern "C" {
#include "movpeaks.h"
}

using namespace std;

typedef enum {
	sphere,
	rastrigin,
	rosenbrock,
	schaffer,
	griewank,
	ackley,
	schwefel,
	levy5,
	freudenstein,
	quadric,
	movingGoal,
	dynamic_rosenbrock,
	dynamic_rastrigin,
	dynamic_griewank,
	moving_peaks,
	dynamic_levy,
	dynamic_ackley,
	dynamic_schwefel,
	dynamic_sinusoidal,
	zitzler3
} function_type;

typedef enum {
	optMax,
	optMin
} optimizationStyle_type;

typedef enum {
	staticFct,
	dynamicFct
} dynamicStyle_type;

typedef enum {
	noNoise,
	noisyFunctionValue,
	noisyPosition,
	noisyRelativeFunctionValue
} noiseStyle_type;

typedef enum {
	randomOptMove, ///<Random distance from [-moveDist, moveDist] for each dimension
	linearOptMove, ///<MoveDist for all dimensions
	randomLinearOptMove, ///<Same random distance from [-moveDist, moveDist] for all dimensions
	randomDistanceOptMove, ///<Same random distance from [-moveDist, moveDist] for each dimension, forward or backward
	exponentialLinearOptMove ///<Exponentially distributed random value with mean moveDistance for all dimensions, forward or backward
} optimumMoveStyle_type;

typedef enum {
	updatePeriodic,
	updateOnGoal,
	updateRandom
} updateStyle_type;

///The optimized function
/**Interface for the actual functions that inherit from Function*/
class Function {     
 public:
 	///Constructor
  Function(int dim_in, dynamicStyle_type dynamicStyle_in);
  ///Destructor
  virtual ~Function();
  ///The evaluation function has to be present, since it is the decisive part of the function
  virtual double evaluate(const vector<double>& pos);

  ///Add noise to the evaluation of position pos
  /**The respective evaluate function is called within, and the current type of noise is added*/
  double noisyEvaluate(const vector<double>& pos);

  ///Add noise to the position
  vector<double> addNoisePos(const vector<double>& pos);

  ///Add noise to the value
  double addNoiseVal(double val);

  ///Scale value with noise 
  /**Return f(x)*(1+noise)*/
  double addNoiseRelativeVal(double val);

  ///Change the function behaviour, for dynamic functions
  virtual bool update(int step, ///<The current step compared to #moveFrequency
  					bool goal=false ///<Whether the function given goal value is reached
  					);
  
  ///Returns the goal value, given by the benchmark
  double getGoal() {return goal;};
  
  ///Returns the global optimum value
  double getOpt() {return opt;}

	double get_min_x() {return min_x;}
	double get_max_x() {return max_x;}
	double get_min_v() {return min_v;}
	double get_max_v() {return max_v;}

	void setNoiseStyle(noiseStyle_type noiseStyle_in) {noiseStyle = noiseStyle_in;};

	void setNoiseSigma(double noiseSigma_in) {noiseSigma = noiseSigma_in;};

	noiseStyle_type getNoiseStyle() {return noiseStyle;};

	double getNoiseSigma() {return noiseSigma;};


	dynamicStyle_type getDynamicStyle() {return dynamicStyle;};
	double getOptPosition(int dim) {return optPos[dim];};
	
	///Set the moveFrequency for the optimization function during runtime
	void set_moveFrequency(int moveFrequency_in); 
		
	///Set the moveDistance for the optimization function during runtime
	/**The moveDistance is given as a fraction of the initialization space
	 * moving_peaks() overrides */
	virtual void set_moveDistance(double moveDistance_in);
	///Set the optimumMoveStyle for the optimization function during runtime
	void set_optMoveStyle(optimumMoveStyle_type optimumMoveStyle_in)
		{optimumMoveStyle = optimumMoveStyle_in;};
	///Set the updateStyle for the optimization function during runtime
	void set_updateStyle(updateStyle_type updateStyle_in)
		{updateStyle = updateStyle_in;};
		
	///Returns the moveFrequency for the optimization function
	int get_moveFrequency()
		{return moveFrequency;};
	///Returns the moveDistance for the optimization function 
	double get_moveDistance()
		{return moveDistance;};
	///Returns the optimumMoveStyle for the optimization function
	optimumMoveStyle_type get_optMoveStyle()
		{return optimumMoveStyle;};
	///Returns the updateStyle (periodic, onGoal) for the optimization function
	updateStyle_type get_updateStyle()
		{return updateStyle;};

	protected:
		///The move of the randomOptMove function is defined
		/**Since the sequence is seeded it is repeatable.*/
		SequenceGenerator* moveSequence;

		///Dimension of the search space, Initial Particle Positions are chosen from that range
		double min_x, max_x; 
		///Dimension of the initial velocities, Initial Particle Positions are chosen from that range
		double min_v, max_v;
		///The goal value to be reached, given by standard benchmarks
		double goal;
		///The global optimum value
		double opt;
		///The function dimension
		int dim;
		
		///The position of the optimum
		vector<double> optPos;
		///For dynamic functions the velocity of the moving optimum
		double moveDistance;
		///The frequency with which changes of the function occur
		/**every #moveFrequency steps*/
		int moveFrequency;
		///Next random move step
		int nextMoveStep;

		
		///For dynamic functions, the way the opt position is moved
		/** either random within #moveDistance, or linear in each dimension
		 * #moveDistance is added */
		optimumMoveStyle_type optimumMoveStyle;

		///Either a #staticFct or a #dynamicFct
		/**For #dynamicFct the #update method is used*/
		dynamicStyle_type dynamicStyle;

		///The kind of noise added to the function
		/**For a noisy function noisyEvaluate is called instead of evaluate from #evalFunc */
		noiseStyle_type noiseStyle;

		///The sigma parameter for the gaussian noise
		double noiseSigma;

		///The dynamic function is either updated periodically or when the function goal is reached
		/**The periodical update is done every #moveFrequency steps and the onGoal
		 * update once the given goal value is reached */
		updateStyle_type updateStyle;

};


///The Sphere function
/**f(x)=sum(x_i^2)*/
class Sphere : public Function {
 public:
 	///Constructor with dimension n
  Sphere(int n, dynamicStyle_type dynamicStyle_in);
  ///The allocated ressources are freed
  ~Sphere();
	
	///The evaluation function f(pos)  
  double evaluate(const vector<double>& pos);
};

///The Rosenbrock function
class Rosenbrock : public Function {
 public:
 	///Constructor with dimension n
  Rosenbrock(int n, dynamicStyle_type dynamicStyle_in);
  ///The allocated ressources are freed
  ~Rosenbrock();
  
	///The evaluation function f(pos)  
  double evaluate(const vector<double>& pos);
};

///The Rastrigin function
class Rastrigin : public Function {
 public:
 	///Constructor with dimension n
  Rastrigin(int n, dynamicStyle_type dynamicStyle_in);
  ///The allocated ressources are freed
  ~Rastrigin();
  
	///The evaluation function f(pos)  
  double evaluate(const vector<double>& pos);
};

///The Schaffer function
class Schaffer : public Function {
 public:
 	///Constructor with dimension n
 	/**Schaffer only defined for dimension 2<br>
 	 * otherwise an error is returned */
  Schaffer(int n, dynamicStyle_type dynamicStyle_in);
  ///The allocated ressources are freed
  ~Schaffer();
  
	///The evaluation function f(pos)  
  double evaluate(const vector<double>& pos);
};

///The Griewank function
class Griewank : public Function {
 public:
 	///Constructor with dimension n
  Griewank(int n, dynamicStyle_type dynamicStyle_in);
  ///The allocated ressources are freed
  ~Griewank();
  
	///The evaluation function f(pos)  
  double evaluate(const vector<double>& pos);
};

///The Ackley function
class Ackley : public Function {
 public:
 	///Constructor with dimension n
  Ackley(int n, dynamicStyle_type dynamicStyle_in);
  ///The allocated ressources are freed
  ~Ackley();
  
	///The evaluation function f(pos)  
  double evaluate(const vector<double>& pos);
};

///The Schwefel function
class Schwefel : public Function {
 public:
 	///Constructor with dimension n
  Schwefel(int n, dynamicStyle_type dynamicStyle_in);
  ///The allocated ressources are freed
  ~Schwefel();
  
	///The evaluation function f(pos)  
  double evaluate(const vector<double>& pos);
};

///The Levy5 function
class Levy5 : public Function {
 public:
 	///Constructor with dimension n
  Levy5(int n, dynamicStyle_type dynamicStyle_in);
  ///The allocated ressources are freed
  ~Levy5();
  
	///The evaluation function f(pos)  
  double evaluate(const vector<double>& pos);
};

///The Freudenstein function
class Freudenstein : public Function {
 public:
 	///Constructor with dimension n
  Freudenstein(int n, dynamicStyle_type dynamicStyle_in);
  ///The allocated ressources are freed
  ~Freudenstein();
  
	///The evaluation function f(pos)  
  double evaluate(const vector<double>& pos);
};

///The Quadric function
class Quadric : public Function {
 public:
 	///Constructor with dimension n
  Quadric(int n, dynamicStyle_type dynamicStyle_in);
  ///The allocated ressources are freed
  ~Quadric();
  
	///The evaluation function f(pos)  
  double evaluate(const vector<double>& pos);
};

///The MovingGoal function
class MovingGoal : public Function {
 public:
 	///Constructor with dimension n
  MovingGoal(int n, dynamicStyle_type dynamicStyle_in);
  ///The allocated ressources are freed
  ~MovingGoal();
  
	///The evaluation function f(pos)  
  double evaluate(const vector<double>& pos);
};

///The MovingPeaks function
class MovingPeaks : public Function {
 public:
 	///Constructor with dimension n
  MovingPeaks(int n, dynamicStyle_type dynamicStyle_in);
  ///The allocated ressources are freed
  ~MovingPeaks();
  
	///The evaluation function f(pos)  
  double evaluate(const vector<double>& pos);
  ///The peaks are changed
  bool update(int step, ///<The current step compared to #moveFrequency
  				bool goal=false ///<Whether the function given goal value is reached
  			);
  void set_moveDistance(double moveDistance_in);

};

///The Sinusoidal function
class Sinusoidal : public Function {
 public:
 	///Constructor with dimension n
  Sinusoidal(int n, dynamicStyle_type dynamicStyle_in);
  ///The allocated ressources are freed
  ~Sinusoidal();
  
	///The evaluation function f(pos)  
  double evaluate(const vector<double>& pos);
  bool update(int step, ///<The current step compared to #moveFrequency
  				bool goal=false ///<Whether the function given goal value is reached
  			);

};

///The second part of the multimodal Zitzler3 function
class Zitzler3 : public Function {
 public:
 	///Constructor with dimension n
  Zitzler3(int n, dynamicStyle_type dynamicStyle_in);
  ///The allocated ressources are freed
  ~Zitzler3();
  
	///The evaluation function f(pos)  
  double evaluate(const vector<double>& pos);
  bool update(int step, ///<The current step compared to #moveFrequency
  				bool goal=false ///<Whether the function given goal value is reached
  			);

};


#endif //ifndef OPTFUNCTIONS_H
