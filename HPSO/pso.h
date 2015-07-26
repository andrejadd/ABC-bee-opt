#ifndef PSO_H
#define PSO_H

#include <stdio.h>
#include <math.h>
#include <ctime>
#include <iomanip>




typedef enum {
	seqSwarm,
	pyramidSwarm,
	ringSwarm,
	localSwarm,
	scoutSwarm,
	dynamicPyramidSwarm,
	adaptivePyramidSwarm
} swarm_type;

typedef enum {
	upToDate,
	gbestDetectChange,
	scoutDetectChange,
	noDetect,
	hierarchyChangeDetect,
	omniscientDetect
} detectionMethod_type;

typedef enum {
	noResponse,
	applyScoutResponse,
	randomize10,
	randomize10reset,
	randomize16reset,
	randomize22reset,
	subSwarmsTemporalMerge,
	subSwarmsAdaptiveMerge,
	subSwarmsTemporalMergeNoMemory,
	subSwarmsAdaptiveMergeNoMemory,
	randomize40reset,
	updateOnly,
	randomize50reset,
	resetAll
} responseMethod_type;

typedef enum {
	noAction, //worstSubtree
	bestSubtreeNoAction,
	reset,
	bestSubtreeReset
} decreaseBranchAction_type;

///How previous best values are reused in the precense of noise
typedef enum {
	noReuse, ///<The previous best value is ignored
	reuseContinuousAverage, ///<The best value becomes a continous average over all evaluations
	reuseLastBestValue ///<Only the previous and the current evaluation are the basis for the best value
} reuseOldValue_type;

typedef enum {
	nolocalsearch,
	doLocalsearch,
	sidestep
} localsearchMode_type;

#ifndef PSOGLOBALS
#include "pso_globals.h"
#endif


 
///The Particle Swarm Optimization algorithm
/**The PSO class holds all the parameters and selects the function to be optimized.*/
class PSO { 
	
 public:
 	///Constructor
  PSO(double w_in, ///<The w parameter (influence of old vel - momentum)
  		double wMin_in, ///<The minimum value for w -- decreased by time or level
  		double c1_in, ///<Influence of personal best
  		double c2_in, ///<Influence of global best
		double vmaxRatio_in, ///<vmax = vmaxRatio*xmax, with vmaxRatio=0 no vmax is enforced 
		double constriction, ///<Constriction factor, used by #Bird
  		int size_in, ///<Swarmsize
  		int dim_in, ///<Dimension of the search space
		int height_in, ///<Height of the pyramid, ignored for #seqSwarm
		int branches_in, ///<Branches per node of the pyramid, ignored for #seqSwarm
		int swapDelay, ///<Swap allowed every swapDelay steps in #PyramidSwarm, ignored for #Swarm
  		function_type funct_in, ///<#function_type selection
		noiseStyle_type noiseStyle_in, ///<#noiseStyle_type selection, set in #optFunction
		double noiseSigma_in, ///<sigma parameter for gaussian distributed noise
  		swarm_type swarmType_in, ///<#swarm_type selection
  		int nrScouts_in = 0, ///<The number of additional random moving scouts
  		int maxStep_in = 10000///<Required for calculating delta_w
  		);
  ///The allocated ressources are freed
  ~PSO();

  string printParameter(string first="");

  double* getSidestepStats();
 

  ///perform a local search around the passed bird's best position
  /**every dimension is optimized separately, the improved position is
   * written to the bird (side effect).*/
  //void localSearch(Bird* bird);
  double localSearch(vector<double> &localSearchVector, double oldValue);

  ///Set the dynamic parameters, required for dynamic functions.
  void setDynamicParameters(int moveFrequency_in, double moveDistance_in, optimumMoveStyle_type optimumMoveStyle_in,
	  updateStyle_type updateStyle_in, detectionMethod_type detectionMethod_in, responseMethod_type responseMethod_in);

  ///Set the noise parameters, required for noisy functions.
  void setNoiseParameters(int nrNoisySwapChecks_in, reuseOldValue_type noisyReuseOldValue_in, 
	  bool noisyRefineBestValue_in, bool noisyFirstComparePbestValues_in, int hierachyChangeDetectThreshold_in);

  ///Set the parameters for the AdaptivePyramidSwarm
  void setAdaptiveParameters(int decreaseBranchFrequency_in, int minBranchDegree_in, int decreaseBranchStep_in,
	  decreaseBranchAction_type decreaseBranchAction_in);

  ///Set the parameters for the local search
  void setLocalsearchParameters(localsearchMode_type mode, double localsearchParameter, double sidestepMagnitude);

  int step(int steps=1);
  
	///The dimension of the search space is returned
	int get_dim();

  ///The w parameter is returned.
  /**The optional level parameter is used for PyramidSwarm with level increasing w*/
  double get_w(int level=0);
  ///The c1 parameter is returned.
  double get_c1();
  ///The c2 parameter is returned.
  double get_c2();
  ///The constriction factor is returned
  double get_constriction();

  Swarm* getSwarm() {return swarm;};

  ///The selected swarmType
  swarm_type getSwarmType() {return swarmType;};
	
  ///The optimization function type is returned
  function_type get_functionType();
 
  ///Restrict v to vmax
  double apply_vmax(double v);
  
  ///Return the worst evaluation value, depending on (max, min) problem
  double getMax();
  
  ///Return the current step of the optimization
  int getStep();
	
  ///Comparsion function depending on the kind of optimization problem (max, min)
    /**Returns the boolean result of the expression (v1 isBetterThan v2)*/
    bool isBetter(double val1, double val2);
    
    ///Comparsion function depending on the kind of optimization problem (max, min)
      /**Returns the boolean result of the expression (v1 isBetterOrEqualThan v2)*/
      bool isBetterEqual(double val1, double val2);
      
      ///Calls Function::evaluate of the implemented optimization function
	  /**@param pos The position that is passed to the optFunction
	   * @param clean Force clean evaluation, without noise*/
  double evalFunction(const vector<double>& pos, bool clean=false);
  
  ///Called by the swarm, after an iteration is done
  /**After an iteration is done w is adapted*/
  void iterationDone();

  ///All the optimization data is kept here
  PsoStatistics *stat;

  ///The actual Function to be optimized
  Function *optFunction;


  void set_detectionMethod(detectionMethod_type detectionMethod_in);
	
  void set_responseMethod(responseMethod_type responseMethod_in);

  detectionMethod_type get_detectionMethod();
	
  responseMethod_type get_responseMethod();

  int getActualHeight();

  int getActualBranches();

	Bird* getBird(int i);
	
	double getBestValue(int bird_i);

	double getCurrValue(int bird_i);
	
	double getVel(int bird_i);
	
	double getPos(int bird_i, int dim);

	double getBestPos(int bird_i, int dim);

	double getGlobalBestPos(int dim);	

	double getOptPosition(int dim);

	///How far is the best particle away from the goal position
	double getDistanceToGoal();
	
	///Return the dim-th component of the current mean Position of the swarm
	double getMeanPosition(int dim);

	double getAverageDiversity();
	double getMedianDiversity();
	double getAverageDiversitySubtree(int subtreeNr);
	double getMedianDiversitySubtree(int subtreeNr);
	
	///If #swarmType is #PyramidSwarm the pyramid level of bird_i is returned, otherwise 0	
	int getLevel(int bird_i);
	///If #swarmType is #PyramidSwarm the pyramid subtree of bird_i is returned, otherwise 0	
	int getSubtree(int bird_i);

	dynamicStyle_type getDynamicStyle() {return optFunction->getDynamicStyle();};

	noiseStyle_type getNoiseStyle() {return optFunction->getNoiseStyle();};

	///Return offline performance from Moving Peaks function
	double getOfflinePerformance() {return get_offline_performance();};
	///Return offline error from Moving Peaks function
	double getOfflineError() {return get_offline_error();};
	///Return current error from Moving Peaks function
	double getCurrentError() {return get_current_error();};

	///Return the number of samples to take for the personal best value
	int getBestValueSamples() {return bestValueSamples;};

	///How many re-evaluations of the best position
	int getNrNoisySwapChecks() {return nrNoisySwapChecks;};
	///Include old value into re-evaluation
	reuseOldValue_type getNoisyReuseOldValue() {return noisyReuseOldValue;};
	///Store new evaluated best value in bird
	bool getNoisyRefineBestValue() {return noisyRefineBestValue;};
	///Only if pbest is better than the predecessor, the re-evaluation is done
	bool getNoisyFirstComparePbestValues() {return noisyFirstComparePbestValues;};

	///For #hierachyChangeDetect at what number of swaps a change is detected.
	int getHierachyChangeDetectThreshold() {return hierachyChangeDetectThreshold ;};

	bool isAdaptiveParametersSet() {return adaptiveParametersSet;};

	int getDecreaseBranchFrequency() {return decreaseBranchFrequency;};

	int getMinBranchDegree() {return minBranchDegree;};

	int getDecreaseBranchStep() {return decreaseBranchStep;};

	int getDecreaseBranchAction() {return decreaseBranchAction;};

	///If a change of the dynamic function has occured in the current iteration
	  /** for the omniscient detection method*/
	  bool isDynamicChange() {return dynamicChange;};

	localsearchMode_type getLocalsearchMode() {return _localsearchMode;};

	///return the initial sidestepwidth to be used
	/**depends on the optimization function ranges*/
	double getInitialSidestepWidth();

	double getSidestepMagnitude() {return _sidestepMagnitude;};

 private:
 	///The swarm doing the optimization
  Swarm *swarm;

  
  
  ///What kind of optimization (max, min)
  optimizationStyle_type optimizationStyle;
  
	
	function_type functionType;
	
	swarm_type swarmType;
	
	///Dimension of the search space
	int dim;
	
	///Swarm size
	int size;
	
	///Pyramid height, required for level-specific calculation of w
	int height;

	int branches;
	int swapDelay;
	
	///Parameter w, holds the current value of the decreasing w
  double w;
  ///Parameter delta_w -- used for adjusting w at each iteration: w = w-delta_w
  /**computed with given maxStep*/
  double delta_w;
  
  ///The input value of w
  double wOrg;
  
  ///The minimum value for w
  double wMin;
	///Parameter c1
  double c1;
	///Parameter c2  
  double c2;
  
  ///Constriction factor
  double constriction;
  
  ///The maximum number of iterations for w to decrease linearly
  int maxStep;
  
  ///vmax computed as fraction of search space
  double vmax;

  ///Test, whether setNoiseParameters has been called for noisy functions
  bool noiseParametersSet;

  ///Test, whether setDynamicParameters has been called for dynamic functions
  bool dynamicParametersSet;


  ///Store whether a dynamic change of the function has occured in the current iteration.
  bool dynamicChange;

  ///Test, whether setAdaptiveParameters has been called for adaptivePyramidSwarm
  bool adaptiveParametersSet;

  ///Test, whether setDynamicParameters has been called for dynamic functions
  bool localsearchParametersSet;

  ///The number of samples to take of the personal best value for noisy functions
  int bestValueSamples;

  ///How many re-evaluations of the best position (default: 1)
  int nrNoisySwapChecks;
  ///Include old value into re-evaluation (default: reuseLastBestValue)
  reuseOldValue_type noisyReuseOldValue;
  ///Store new evaluated best value in bird (default: true)
  bool noisyRefineBestValue;
  ///Only if pbest is better than the predecessor, the re-evaluation is done (default: false)
  /**The current pbest value (average or single evaluation) is compared with the parent,
   * if it is better, then the re-evaluation triggered by #noisyBirdIsBetter is initiated.*/
  bool noisyFirstComparePbestValues;
  	///For #hierachyChangeDetect at what number of swaps a change is detected.
	int hierachyChangeDetectThreshold;

	///At what intervals the branch degree is decreased for AdaptiveHPSO
	/**decreaseBranchFrequency = 0 to turn off*/
	int decreaseBranchFrequency;
			
	///Until what minimum branch degree for AdaptiveHPSO
	int minBranchDegree;

	///How many steps to decrease the branch degree
	int decreaseBranchStep;

	///Additional action to perform, when branch degree is decreased.
	int decreaseBranchAction;

	///How often to perfom a local search operation
	int localsearchFrequency;

	///for sidestep, initial sidestep ratio
	double _initialSidestep;
	
	///for sidestep, how far the sidestep is performed, relative to the current sidestepWidth
	double _sidestepMagnitude;

	
	///what kind of local search (sidestep) to perform
	localsearchMode_type _localsearchMode;
	

};

#endif //ifndef PSO_H
