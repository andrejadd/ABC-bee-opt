#ifndef PSOSTATS_H
#define PSOSTATS_H

#ifndef PSOGLOBALS
#include "pso_globals.h"
#endif

#include <set>


///All of the data for evaluation is collected here
  
class PsoStatistics {
	public:
	  ///The constructor for the statistics logging
		PsoStatistics(int steps_in=1000, ///<The reserved size for the vectors
									int height=0, int branches=0);		///< Defining the size of #swappedPerLevel for the #PyramidSwarm
		~PsoStatistics();
	
		void addGlobalBest(double val);
		///For noisy functions, the value at the best perceived position
		void addActualGlobalBest(double val);
		void addIterationBest(double val);

		///Log the current function error
		void addCurrentError(double val);
		///Log the gbest in the divided DynamicPyramidSwarm
		void addGbestInSubswarm(int subswarm);
		void initAverage();
		void addAverage(double val);
		void finishAverage(int size);
		
		void swapped(int level);

		void evaluated();

		void changeDetected(bool change);
		
		double getGlobalBest(int step);
		double getActualGlobalBest(int step);
		double getIterationBest(int step);
		double getAverage(int step);
		///Get the number of all swaps in #PyramidSwarm until step
		int getNrSwapped(int step);
		///Get the number of swaps in #PyramidSwarm at step
		int getNrSwappedAt(int step);
		///Get the current number of swaps in #PyramidSwarm this iteration
		int getCurrentNrSwapped();

		///Get the number of swaps until step in the given level of #PyramidSwarm
		int getNrSwappedLevel(int step, int level);

		///The number of functions evaluations until step
		int getNrEvaluations(int step);

		///Whether a change was detected at step int
		bool isChangeDetected(int step);

		///Return the current error for step
		double getCurrentError(int step);

		///Return the gbest position in the divided DynamicPyramidSwarm, -1 otherwise
		int getGbestInSubswarm(int step);

		///Collect the diversity for the given subtree
		/**The random subtree is stored last, before that are the #branches subtrees below the root*/
		void addDiversitySubtree(int subtree, double diversity);
	
		///Return the diversity for the given subtree at step
		/**The random subtree is stored last, before that are the #branches subtrees below the root*/
		double getDiversitySubtree(int step, int subtree);

		
		///Increase #psoStep
		void psoStepDone();
		
		///Called by #PSO if the given goal for the optimization function is reached
		/**The iteration in which the goal is first reached is stored in #goalReachedStep*/
		void goalReached();
		
		///Returns the step in which the given goal was reached
		int getGoalReachedStep() {return goalReachedStep;};

		///Called by #PSO if the given optimum value for the optimization function is reached
		/**The iteration in which the optimum is first reached is stored in #goalReachedStep*/
		void optReached();
		
		///Returns the step in which the given optimum was reached
		int getOptReachedStep() {return optReachedStep;};
	
		int getCurrentStep(){return psoStep;};

		///The number of swaps performed in the current iteration
		/**Required for #hierarchyChangeDetect*/
		int currNrSwapped;

		double* calculateSidestepStats(double initSidestep, vector<Bird*> swarm, int nrBirds, int dim);

	private:
		double iterationAverage;
		
		
		int numberLevels;

		///The number of subtrees for diversitySubtree
		/**required to ignore diversitySubtree for seqSwarm etc.*/
		int subtrees;


		///The number of swaps occured per step in #PyramidSwarm
		//vector<int> nrSwapped;
		///For each level the number of swaps in #PyramidSwarm is counted cumulative
		/**Starting with the top level */
		vector <int>* swappedPerLevel;
		

		///Diversity in the different subtrees
		/**The random subtree is stored last, before that are the #branches subtrees below the root*/
		vector<double>* diversitySubtree;

		vector<double> globalBest;
		vector<double> actualGlobalBest;
		vector<double> iterationBest;
		vector<double> average;

		///Total number of function evaluations
		vector<int> nrEvaluations;

		vector<bool> detectedChanges;

		///The current Error from the global optimum
		/**Currently only used for moving_peaks*/
		vector<double> currentError;

		///In which subswarm of the divided DynamicPyramidSwarm is the gbest
		/**Only used for DynamicPyramidSwarm and only if it is divided*/
		vector<int> gbestInSubswarm;
		
		///The step in which the given goal for the optimization function is reached
		int goalReachedStep;

		///The step in which the global optimum for the optimization function is reached
		int optReachedStep;
		
		///The number of iterations done
		int psoStep;

};
#endif //ifndef PSOSTATS_H


