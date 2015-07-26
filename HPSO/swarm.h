#ifndef SWARM_H
#define SWARM_H


#ifndef PSOGLOBALS
#include "pso_globals.h"
#endif

#ifndef tree_hh_
#include "tree.hh"
#endif

#include <deque>
#include <queue>
#include <set>
#include <map>
#include <algorithm>

#include <cassert>


const int MINUSONE = -1;	


///The Swarm class containing and controlling the individual birds
/**The actions of the single birds are initiated from here and the
 * information about the current best bird is kept here.*/
class Swarm {
	public:
	///Constructor for the swarm
	Swarm(PSO *pso_p, ///<The PSO Algorithm for the parameters used
				int size, ///<The number of birds to create
				int dim, ///<The dimension of the search space, i.e. the function to be optimized,
				double min_x, ///<Lower bound of x dimension, x0 chosen randomly
				double max_x, ///<Upper bound of x dimension, x0 chosen randomly
				double min_v, ///<Lower bound of v dimension, v0 chosen randomly
				double max_v, ///<Upper bound of v dimension, v0 chosen randomly
				int nrScouts_in = 0 ///<The number of additional scouts, default = 0
				);
	///The allocated ressources are freed
	/**Virtual destructor is used for derived Classes*/
	virtual ~Swarm();
	
	void localSearch();

	virtual void printSwarm();


	///The swarm does #steps steps
	/**Within one iteration the following actions are done for all the birds.<br>
	 * The current position is evaluated, the iteration best bird is determined 
	 * and compared to the global best, the current velocity is adjusted to the
	 * global best bird and the birds are moved.*/
	virtual int step(int steps=1);
	///Return the overall best value found
	double getGlobalBestValue();
	
	

	detectionMethod_type get_detectionMethod() {
		return detectionMethod;
	};
	
	responseMethod_type get_responseMethod() {
		return responseMethod;	
	};
	void set_detectionMethod(detectionMethod_type detectionMethod_in) {
		detectionMethod = detectionMethod_in;
	};
	
	void set_responseMethod(responseMethod_type responseMethod_in) {
		responseMethod = responseMethod_in;	
	};
	
	void setInitialSidestep(double sidestep);
	
	///Return the current mean position of the swarm
	vector<double> getMeanPosition() {return meanPosition;};
	
	double getAverageDiversity() {return averageDiversity;};
	double getMedianDiversity() {return medianDiversity;};
	
	virtual double getAverageDiversitySubtree(int subtreeNr) {return 0.0;};
	virtual double getMedianDiversitySubtree(int subtreeNr) {return 0.0;};
	
	
	///Return the bird or scout with index i
	/**@param i The index of the bird or scout (if index >= swarmsize) to be returned.
	 * @return Pointer to the indexed bird or scout
	 */
	Bird* getBird(int i);

	vector<Bird*> getBirds() {return birds;};
	
	///Return the global best bird 
	/**@return Pointer to the currently global best bird
	 */
	Bird* getGlobalBestBird() {return globalBestBird;};
	
	///Determines whether bird i is a scout
	bool isScout(int i);
	

	///Just a stub that is implemented in PyramidSwarm::getPyramidLevelBird	
	virtual int getPyramidLevelBird(int bird_i) {return 0;};
	///Just a stub that is implemented in PyramidSwarm::getPyramidLevelBird	
	virtual int getPyramidSubtreeBird(int bird_i) {return 0;};
	///Just a stub that is implemented in PyramidSwarm::getUseScoutBird	
	virtual int getUseScoutBird(int bird_i) {return 0;};
	
	///Required for #PyramidSwarm
	virtual int getActualHeight() {return 0;};

	///Required for #PyramidSwarm
	virtual int getBranches() {return 0;};

	///Required for #DynamicPyramidSwarm
	virtual bool isDivided() {return false;};

	///Implemented in #AdaptivePyramidSwarm
	virtual void decreaseBranchDegree() {};

	protected:
	///Create a single bird
	/**Called by createSwarm, index not used for scouts (index=-1)*/
	Bird* createBird(double min_x, double max_x, double min_v, double max_v, int index=MINUSONE);

	///Create the birds
	/**Called by the constructor*/
	virtual void createSwarm(double min_x, double max_x, double min_v, double max_v);

	///Create a new particle
	/**the particle is added to #birds and #swarmsize and #lastBirdIndex is increased*/
	Bird* createNewBird(double min_x, double max_x, double min_v, double max_v);
	
	///Delete the particle with the given index
	/**Swarmsize is decreased
	 *	@param index The #birds vector is search for particle with index, and the respective particle is deleted.*/
	void removeBird(int index);

	///Create the scouts
	/**Called by the constructor*/
	virtual void createScouts(double min_x, double max_x, double min_v, double max_v);

	///All the particles in the swarm evaluate their current position
	/**If the current position is better than the previous best, it is stored.
	 * @return The iteration best individual
	 */
	virtual Bird* evaluatePos();

	///Compare iteration best particle to the global best and update
	/**If iteration best is better than globalBest it becomes the new globalBest 
	 * individual.*/
	virtual void compare(Bird* itrBestBird);

	///The velocity is updated according to the globalBestBird
	virtual void updateVel();

	///All the particles in the swarm are moved
	virtual void move();
	
	
	///All the scouts evaluate their current position
	/**If the current position is better than the previous best, it is stored.
	 * @return The iteration best scout
	 */
	virtual Bird* evaluateScouts();

	///Compare iteration best scout to the currently best one and update
	/**If iteration best is better than globalBest it becomes the new globalBest 
	 * scout, used for #applyScout.*/
	virtual void compareScouts(Bird* itrBestBird);

	///The velocity is updated according to a randomly chosen position
	/**As gbest attractor a randomly chosen within the search space is selected. The
	 * pbest value is still used.*/
	virtual void updateScoutVel();

	///All the scouts are moved
	virtual void moveScouts();

	///The bird is re-randomized
	/**A new random position and random velocity is assigned
	  *@param birdPtr The particle that is randomized
	  *@param bestVector The space within which the particle is replaced can be moved
	  *@param useBestPos Whether the new position is shifted about the best position */
	void randomizeBird(Bird* birdPtr, const vector<double>& bestVector, bool useBestPos);

	///A certain portion of the swarm is re-randomized
	/**The selected individuals are assigned a random position and a random velocity.
	 * Possibly the other individuals are reset.
	 * @param portion The randomly chosen portion of the swarm that is re-randomized
	 * @param reset The other individuals are reset (forget personal best)
	 */
	void randomizeReset(double portion, bool reset);

	
	///If the best scout is better than the gbest individual it is reset
	/**The gbest particle is reset to the scout position and its best location is 
	 * also updated.
	 * @param force The reset to the best scout position is forced, even if it's worse than gbest*/
	virtual void applyScout(bool force=false);


	///Determine the current mean position of the swarm
	/**All the birds current positions are averaged */
	void updateMeanPosition();

	///Determine the maximum distance between two birds
	/**Required to scale diversity*/
	void updateMaxDistance();
	
	///Calculate the diversity
	/**The average and median distance to the swarm's mean position are calculated*/
	virtual void calculateDiversity();

	///For each dimension calculate the variance of the best positions
	double* calculateDimensionVariance();

	///Apply detection method for changed environment
	/**Depending on #detectionMethod it is checked for a change in the evalutation
	 * function.
	 * @return True if the environment changed.
	 */
	virtual bool detectDynamicChange();
	
	///Respond to the change in the environment
	virtual void respondToChange();

	///All individuals update their personal best position
	/**The personal best is invalidated by the change of the environment */
	void updateBestValues();


	///check whether the new position is better -- in the precense of noise
	/**Considering a noisy function, the decision which position is better is based on the re-evaluation
     * of the function value. In order to get a more precise estimate of the function values
	 * @param newBird The new bird, sibling in HPSO
	 * @param oldBird The old bird, predecessor in HPSO
	 * @return whether the new position is better*/
	bool noisyBirdIsBetter(Bird* newBird, Bird* oldBird);



	///The PSO algorithm with the parameters used
	PSO* pso; //the pso holds all parameters
	///The individual birds
	//Bird** birds;
	vector<Bird*> birds;
	///The additional particles used for scouting are stored separately
	Bird** scouts;
	
	
	///The number of birds
	int swarmsize;
	///The number of scouts
	int nrScouts;

	///The last index given to a new particle
	/**Only interesting if new particles are added or removed*/
	int lastBirdIndex;
	
	///The range of the initial search space
	double min_x, max_x;
	///The range of the initial velocity
	double min_v, max_v;
	
	///The current mean position of the swarm
	vector<double> meanPosition;
	
	///The current average distance to the mean position
	double averageDiversity;
	///The current median distance to the mean position
	double medianDiversity;

	///The maximum distance between two birds
	/**sort of the extension of the swarm*/
	double maxDistance;
	
	///Pointer to the current best bird
	Bird* globalBestBird;
	///Pointer to the current best scout
	Bird* globalBestScout;
	
	///How to detect dynamic changes in the evaluation function
	detectionMethod_type detectionMethod;

	///How to respond to dynamic changes in the evaluation function
	responseMethod_type responseMethod;

};



///A Container for a Bird Object to store additional information
/*A pointer to the Bird and the index within the birds vector.*/
struct BirdContainer {
	public:
		///Constructor required for sibling_iterator
		BirdContainer();
		///Copy Constructor
		BirdContainer(const BirdContainer &BC);
		///Constructor for new BirdContainer with bird pointer and index
		BirdContainer(Bird* bird_in, int i);
		///The allocated ressources are freed
		~BirdContainer();
		
		int getIndex() {return index;};
		Bird* getBird() {return bird;};
		
		
	private:
		///A pointer to the handled bird
		Bird* bird;
		///Initial index of the bird, same as the index in the #birds vector of #Swarm
		int index;
		
	
};


///The PyramidSwarm implements the hierarchical version of Swarm
/**The birds are arranged pyramid-like, with the globalbest bird on top.
 * After the evaluation of the current position birds within a N(x) neighbourhood
 * are compared and the best one moves towards the top one level.<br>
 * Irregular structures (height, branches) are built by distributing the 
 * remaining nodes at the bottom level of the pyramid*/
class PyramidSwarm : public Swarm {
	public:
		///Constructor for H-PSO, setting up the swarm and arranging it in a pyramid shape
		PyramidSwarm(PSO *pso_p, ///<The PSO Algorithm for the parameters used
									int size, ///<The number of birds to create
									int dim, ///<The dimension of the search space, i.e. the function to be optimized
									double min_x, ///<Lower bound of x dimension, x0 chosen randomly
									double max_x, ///<Upper bound of x dimension, x0 chosen randomly
									double min_v, ///<Lower bound of v dimension, v0 chosen randomly
									double max_v, ///<Upper bound of v dimension, v0 chosen randomly
									int height, ///<The height (#levels) of the pyramid
									int branches, ///<How many successors there are on the level below
									int swapDelay, ///<A swap is only allowed every swapDelay steps
									int nrScouts_in = 0 ///<The number of additional scouts, default = 0									
									);
		///The allocated ressources are freed
		~PyramidSwarm();	

	///Return the branch degree
	/**The current branch degree for #AdaptivePyramidSwarm*/
	int getBranches() {return branches;};


	///Returns the height including possibly added extra nodes for irregular pyramid
	int getActualHeight();

	///The pyramidswarm does #steps steps
	/**Within one iteration the following actions are done for all the birds.<br>
	 * The current position is evaluated, within each neighbourhood the iteration
	 * best bird is determined and compared to the bird above, the arrangment is 
	 * adjusted - the best bird per neighbourhood moves up one level, the 
	 * current velocity is adjusted to the global best bird and the birds are moved.*/
	int step(int steps=1);

	///Overwrite Swarm::printSwarm()
	void printSwarm() {printPyramid();};

	///Formatted output of the pyramid
	void printPyramid();
	
	///Two nodes of the pyramid are swapped
	void swap(tree<BirdContainer>::iterator it1, tree<BirdContainer>::iterator it2);

	///Returns the pyramid level that bird_i is on
	/**Actually birdcontainer_i is used, but since the indexing of birds and 
	 * birdcontainers is done alike, it is the same. */
	int getPyramidLevelBird(int bird_i);
	///Returns the pyramid subtree that bird_i is on
	/**Actually birdcontainer_i is used, but since the indexing of birds and 
	 * birdcontainers is done alike, it is the same. */
	int getPyramidSubtreeBird(int bird_i);
	///Returns the pyramid subswarm that bird_i is on
	/**Subswarms defined below division depth.<br>
	 * Actually birdcontainer_i is used, but since the indexing of birds and 
	 * birdcontainers is done alike, it is the same. */
	int getPyramidSubswarmBird(int bird_i);

	double getAverageDiversitySubtree(int subtreeNr) {return averageDiversitySubtree[subtreeNr]/maxDistance;};
	double getMedianDiversitySubtree(int subtreeNr) {return medianDiversitySubtree[subtreeNr];};
	

	
	protected:
		///The already created birds (Swarm::createSwarm) are put into the pyramid hierarchy
		/**Called by the constructor*/
		void createPyramid();

		///According to the pyramid hierarchy the birds are compared and swapped
		/**Each bird is compared to its direct children and is swapped with the best
		 * sibling, if this one is better than the upper level node.*/
		void hierarchicalCompare();

		///The velocities of the birds are updated
		/**The children of each node update their velocity with their parent node as
		 * global best.*/
		void updateVel();
		
		///The levels of the BirdContainers in #birdContainerLevels is updated
		/**Also for each bird the subswarm -- branch below the root -- and the
		*  subtree -- hierarchy divided in half, which section of the hierarchy.
		*  The root subtree is index 0, and the other subtrees start at division depth.
		*  used for #DynamicPyramidSwarm*/
		void updateBirdContainerLevels();
		
		///Calculate the diversity for the individual subtrees
		/**The average and median distance to the subtree's mean position are calculated*/
		void calculateDiversitySubtree();
		
		///Determine the current mean position of the subtree or the given randomSet
		/**All the birds of the subtree current positions are averaged.
		 * For a random set of birds subtreeNr = -1 is passed */
		void updateMeanPositionSubtree(int subtreeNr, set<int> randomSet);
		

		///update the BirdContainer* maps
		/**The specified element is removed from the maps to keep it consistent.*/
		void removeBirdContainerLevelsEntry(int index);
		
		
		///The height of the pyramid
		/**May differ from height for irregular pyramids, where an extra level is
		 * added for the extra nodes. Or for an AdaptivePyramidSwarm, where the
		 * hierarchy is changed.*/
		int currentHeight;


		///Controls whether in the current step a swap is allowed
		bool swapEnabled;
		
		///A swap is only allowed every swapDelay steps
		int swapDelay;

		///The number of children per node
		int branches;
		///The number of levels of the pyramid
		/**The possible extra level for irregular pyramids excluded*/
		int height;
		
		///Whether the pyramid shape is regular
		bool regularPyramid;
		
		///The main data structure for PyramidSwarm operations
		/**The bird pointers are still stored in the #birds vector. In the pyramid
		 * data structure the container objects with pointers to the birds are 
		 * moved.*/
		tree<BirdContainer> pyramid;
		
		///The levels of the different BirdContainers in the pyramid
		/**The map is accessed by the index of a specific bird.*/
		map<int, int> birdContainerLevels;
		///The subtree of the different BirdContainers in the pyramid
		map<int, int> birdContainerSubtree;
		/**The map is accessed by the index of a specific bird.*/
		///The subswarms of the different BirdContainers in the pyramid
		/**used in DynamicPyramidSwarm<br>
		  *The map is accessed by the index of a specific bird.*/
		map<int, int> birdContainerSubswarm;
		
		///The current mean position of the swarm of a single subtree
		/**The subtrees are iterated and before each evaluation 
		 * PyramidSwarm::updateMeanPositionSubtree is called.*/
		vector<double> meanPositionSubtree;
		
		///Array of the current average distance to the mean position per subtree
		double* averageDiversitySubtree;
		///Array of the current median distance to the mean position per subtree
		double* medianDiversitySubtree;
		
		
		///If the pyramid to be built is not regular (heights, branches != swarmsize)
		/**The number of extra nodes to be added to the lowest level is given.
		 * The list of numbers is computed in PyramidSwarm::PyramidSwarm*/
		deque<int> extraNodesList;
	
		///The last time tick, gbest came from subtree i
		/**The vector is initialized with branches entries and updated if subtrees are added or removed.
		 * If a change to the hierarchy occurs, all of the entries are reset to the current clock tick.*/
		vector<int> gbestFromSubtreeTick;
};

///The DynamicPyramidSwarm incorporates detection and response mechanisms for dynamic environments
/**The DynamicPyramidSwarm consists of a PyramidSwarm and several scouts that
 * monitor the environment */
class DynamicPyramidSwarm : public PyramidSwarm {
	public:
		///Constructor for H-PSO, setting up the swarm and arranging it in a pyramid shape
		DynamicPyramidSwarm(PSO *pso_p, ///<The PSO Algorithm for the parameters used
									int size, ///<The number of birds to create the PyramidSwarm
									int dim, ///<The dimension of the search space, i.e. the function to be optimized
									double min_x, ///<Lower bound of x dimension, x0 chosen randomly
									double max_x, ///<Upper bound of x dimension, x0 chosen randomly
									double min_v, ///<Lower bound of v dimension, v0 chosen randomly
									double max_v, ///<Upper bound of v dimension, v0 chosen randomly
									int height, ///<The height (#levels) of the pyramid
									int branches, ///<How many successors there are on the level below
									int swapDelay, ///<A swap is only allowed every swapDelay steps
									int nrScouts ///<The number of birds for the scouting swarm
									);
		///The allocated ressources are freed
		~DynamicPyramidSwarm();	

		///Return whether the swarm is currently divided
		bool isDivided() {return dividedSwarm;};
		
	protected:

		///The pyramidswarm does #steps steps
		/**Within one iteration the following actions are done for all the birds.<br>
		 * The current position is evaluated, within each neighbourhood the iteration
		 * best bird is determined and compared to the bird above, the arrangment is 
		 * adjusted - the best bird per neighbourhood moves up one level, the 
		 * current velocity is adjusted to the global best bird and the birds are moved.*/
		int step(int steps=1);


		///According to the pyramid hierarchy the birds are compared and swapped within the respective sub-swarm
		/**Each bird is compared to its direct children and is swapped with the best
		 * sibling, if this one is better than the upper level node.
		 * This is done on each side of the dividing line at #divisionDepth. */
		void dividedHierarchicalCompare();
	

		///The velocities of the birds are updated within the respective sub-swarm
		/**The children of each node update their velocity with their parent node as
		 * global best. This is done on each side of the dividing line at #divisionDepth. */
		void dividedUpdateVel();
	
		///The lower sub-swarms are re-randomized
		/**The leader of the lower sub-swarms is re-randomized (random position and random velocity
		* @param reset The rest of the lower sub-swarms is reset (forget personal best)
		* @param randomizeTopSwarm For SubSwarms*MergeNoMemory
		*/
		void randomizeSubSwarms(bool reset, bool randomizeTopSwarm);


		///Apply detection method for changed environment
		/**Depending on #detectionMethod it is checked for a change in the evalutation
		 * function. The #scoutDetectChange method can be used here.
		 * @return True if the environment changed.
		 */
		bool detectDynamicChange();


	
		///Respond to the change in the environment
		/**If the responseMethod dividePyramid is selected, several pyramid connections are
		 * separated and no swaps are possible. Otherwise Swarm::respondToChange is called */
		void respondToChange();
		
		///Sub-swarms are created by introducing a dividing line
		/**Particles at divisionDepth (root is depth 0) no longer receive update from their
		 * predecessors and they cannot swap upwards.*/
		int divisionDepth;

		///The swarm is still divided
		bool dividedSwarm;

		///The duration of the division, for a duration based division
		int divisionDuration;
		
		///Dictionary for counting where gbest is in the divided swarm
		/**If the swarm is divided, the count is increased for the subswarm
		 * that gbest belonged to*/
		map<int, int> gbestInSubswarm;
	
};



///The AdaptivePyramidSwarm can dynamically change its tree structure
/**Depending on the optimization behaviour, the AdaptivePyramidSwarm can add or
 * cut off certain subtrees in the hierarchy. */
class AdaptivePyramidSwarm : public PyramidSwarm {
	public:
		///Constructor for H-PSO, setting up the swarm and arranging it in a pyramid shape
		AdaptivePyramidSwarm(PSO *pso_p, ///<The PSO Algorithm for the parameters used
									int size, ///<The number of birds to create the PyramidSwarm
									int dim, ///<The dimension of the search space, i.e. the function to be optimized
									double min_x, ///<Lower bound of x dimension, x0 chosen randomly
									double max_x, ///<Upper bound of x dimension, x0 chosen randomly
									double min_v, ///<Lower bound of v dimension, v0 chosen randomly
									double max_v, ///<Upper bound of v dimension, v0 chosen randomly
									int height, ///<The height (#levels) of the pyramid
									int branches, ///<How many successors there are on the level below
									int swapDelay, ///<A swap is only allowed every swapDelay steps
									int nrScouts ///<The number of birds for the scouting swarm
									);
		///The allocated ressources are freed
		~AdaptivePyramidSwarm();	

		///Transform the tree into a tree with branch degree minus 1
		/**The worst subtree in each level is cut and the nodes are evenly
		 * distributed */
		void decreaseBranchDegree();

	protected:
		


		///The #AdaptivePyramidSwarm does #steps steps
		/**Within one iteration the following actions are done for all the particles.<br>
		 * The current position is evaluated, within each neighbourhood the iteration
		 * best bird is determined and compared to the bird above, the arrangment is 
		 * adjusted - the best bird per neighbourhood moves up one level, the 
		 * current velocity is adjusted to the global best bird and the birds are moved.*/
		int step(int steps=1);

		///Initialize the particle according to the given initialization strategy
		/**The current and best positions are set*/
		void initializeBird(Bird* newBird);

		///Insert a new subtree
		/**at the specified location*/
		void addSubtree(tree<BirdContainer>::iterator);

		///Delete the subtree
		/**starting at (including) the given position. The birds are removed from the #birds vector.*/
		void removeSubtree(tree<BirdContainer>::iterator);

		///Cut the subtree
		/**starting at (including) the given position. The birdContainers are returned.
		 * @return All the BirdContainers that were cut*/		
		deque<BirdContainer> cutSubtree(tree<BirdContainer>::iterator cutNode);
		
		///Return the worst subtree
		/**All of the nodes below the root are compared and the worst one is returned
		  * @return iterator to the top node of the worst subtree.*/
		tree<BirdContainer>::iterator getWorstSubtree();

		///Return the worst subtree below the given node
		/**All of the nodes below the given node are compared and the worst one is returned
		 * @param top The worst subtree below this node is searched.
		 * @return iterator to the top node of the worst subtree.*/
		tree<BirdContainer>::iterator getWorstSubtree(tree<BirdContainer>::iterator top);

		///Return the best subtree below the given node
		/**All of the nodes below the given node are compared and the best one is returned
		 * @param top The best subtree below this node is searched.
		 * @return iterator to the top node of the best subtree.*/
		tree<BirdContainer>::iterator getBestSubtree(tree<BirdContainer>::iterator top);

		///Return the subtree least recently used for providing gbest
		/**The subtree that hasn't produced the root node for the longest time is returned.
		  * @return iterator to the top node of the LRU subtree.*/
		tree<BirdContainer>::iterator getLRUSubtree();

		///Return whether a subtree hasn't provided the root node for threshold steps
		/** @param threshold Threshold to the number of steps
		 *  @return true, if at least one subtree took longer than threshold.*/
		bool noGbestInSubtreeSince(int threshold);


		///Return an iterator to the first child at the given depth
		tree<BirdContainer>::iterator getFirstNodeAtDepth(int depth);

		///Return all the nodes at the given depth
		/**used as a replacement for the fixed depth iterator */
		deque<tree<BirdContainer>::iterator> getAllNodesAtDepth(int depth);


};


///In the #RealPyramidSwarm considerations of a real architecture were taken into account
/**First the RealPyramidSwarm::compare operation takes place alternating for
 * odd and even levels of the pyramid. Consecutively a swap occurs only between two
 * adjacent levels in a single iteration.<br>
 * Second the birds can be disabled, by using their Bird::active variable. When a
 * swap operation is performed, the execution of Bird::evalCurrentPosition is delayed 
 * until the specified time required for a swap operation has passed.*/
//class RealPyramidSwarm : PyramidSwarm {
	
//};


///The #RingSwarm has the indivdiuals arranged in a ring
/**Each ring node consists of a neighborhood of #branches individuals and an extra storage
 * for the current global best position found so far. This global best position is 
 * updated with the adjacent ringnodes, clockwise.*/
 

class RingSwarm : public Swarm {
	public:
		///Constructor setting up the swarm and arranging it in a Ring shape
		RingSwarm(PSO *pso_p, ///<The PSO Algorithm for the parameters used
									int dim, ///<The dimension of the search space, i.e. the function to be optimized
									double min_x, ///<Lower bound of x dimension, x0 chosen randomly
									double max_x, ///<Upper bound of x dimension, x0 chosen randomly
									double min_v, ///<Lower bound of v dimension, v0 chosen randomly
									double max_v, ///<Upper bound of v dimension, v0 chosen randomly
									int length, ///<The length (#ringnodes) of the Ring
									int branches, ///<How many individuals per ringnode
									int swapDelay ///<A swap is only allowed every swapDelay steps
									);
		///The allocated ressources are freed
		~RingSwarm();	


	///The Ringswarm does #steps steps
	/**Within one iteration the following actions are done for all the birds.<br>
	 * The current position is evaluated, within each neighbourhood the iteration
	 * best bird is determined and compared to the best bird of the following ringnode.
	 * If necessary the best bird is passed on. <br>
	 * Then the current velocity is adjusted to the neighbourhood best bird and the birds are moved.*/
	int step(int steps=1);

	///Formatted output of the Ring
	void printRing();
		
	
	protected:

		///Within a ringnode the birds are compared
		/**The birds in a neighbourhood are compared to each other and the respective best Bird known*/
		void compare(Bird* itrBestBird);
		
		///Between two ring nodes the better bestBird is passed on
		/**A node compares itself with the preceeding node and if necessary receives the 
		 * bestBird[node+1]*/
		void compareRing();
		
		///The velocities of the birds are updated
		/**The birds of each ringnode update their velocity with their stored bestBird as global best.*/
		void updateVel();
		

		///Controls whether in the current step a swap is allowed
		bool swapEnabled;
		
		///A swap is only allowed every swapDelay steps
		int swapDelay;

		///The length (number ringnodes) of the Ring
		int length;
		
		///How many individuals per ringnode
		int branches;
			
		///Each ringnode neighbourhood stores the bestBird found
		/**Whether found by itself or the adjacent neighbourhood*/
		Bird** bestBirds;	
};


///The #LocalSwarm has a local neighbourhood for each indivdiual
/**The neighbourhood best is used to update the current velocity. The neighbourhood
 * size in both directions is given as a parameter.<br>
 * An extra storage is maintained, where the index of the iteration best bird for each bird  
 * is stored. The iteration best is determined by comparing personal best positions and
 * is only used for updating, not explicitly passed on, as in the #RingSwarm.
 */
 

class LocalSwarm : public Swarm {
	public:
		///Constructor setting up the swarm 
		LocalSwarm(PSO *pso_p, ///<The PSO Algorithm for the parameters used
									int size_in, ///<The swarm size
									int dim, ///<The dimension of the search space, i.e. the function to be optimized
									double min_x, ///<Lower bound of x dimension, x0 chosen randomly
									double max_x, ///<Upper bound of x dimension, x0 chosen randomly
									double min_v, ///<Lower bound of v dimension, v0 chosen randomly
									double max_v, ///<Upper bound of v dimension, v0 chosen randomly
									int nSize_in, ///<The size of the neighbourhood to the left and right, nSize=1 means 1 to the left and to the right
									int swapDelay ///<A swap is only allowed every swapDelay steps -- not used yet
									);
		///The allocated ressources are freed
		~LocalSwarm();	


	///The Localswarm does #steps steps
	/**Within one iteration the following actions are done for all the birds.<br>
	 * The current position is evaluated and within each neighbourhood the iteration
	 * best bird is determined. <br>
	 * Then the current velocity is adjusted to the neighbourhood best bird and the birds are moved.*/
	int step(int steps=1);

	
	protected:

		///For each bird all the birds in the neighbourhood, including himself, are compared
		/**The index of each best one is stored in #iterationBestBirds*/
		void compareLocal();
		
		
		///The velocities of the birds are updated
		/**Each bird updates its velocity according to the neighbourhood best bird,
		 * whose index is stored in #iterationBestBirds */
		void updateVel();
		

		///Controls whether in the current step a swap is allowed
		bool swapEnabled;
		
		///A swap is only allowed every swapDelay steps
		int swapDelay;

		
		///The neighbourhood size, i.e. the numbers of birds to left and right
		/**Total number of neighbours. nSize of 2, is one to the left and one to the right*/
		int nSize;
			
		///The index of the neighbourhood best bird of the current iteration
		/**This bird is only used for the velocity update, it is not used for further
		 * comparsion with neighbouring birds. Thus the momentary global best position 
		 * is not passed on*/
		int* iterationBestBirds;	
};




///The #ScoutSwarm uses scouts and workers for dynamic problems
/**The scouts are exploring the search space only guided by previous personal
 * experience, the workers are assigned to a specific scout and use its 
 * best Position for the velocity update */

class ScoutSwarm : public Swarm {
	public:
		///Constructor setting up the swarm 
		ScoutSwarm(PSO *pso_p, ///<The PSO Algorithm for the parameters used
									int size_in, ///<The swarm size
									int dim, ///<The dimension of the search space, i.e. the function to be optimized
									double min_x, ///<Lower bound of x dimension, x0 chosen randomly
									double max_x, ///<Upper bound of x dimension, x0 chosen randomly
									double min_v, ///<Lower bound of v dimension, v0 chosen randomly
									double max_v, ///<Upper bound of v dimension, v0 chosen randomly
									int nrScouts_in, ///<The number of scouts
									int nrWorkers_in, ///<The number of workers per scout
									int swapDelay ///<A swap is only allowed every swapDelay steps -- not used yet
									);
		///The allocated ressources are freed
		~ScoutSwarm();	


	///The Localswarm does #steps steps
	/**Within one iteration the following actions are done for all the birds.<br>
	 * The current position is evaluated and within each neighbourhood the iteration
	 * best bird is determined. <br>
	 * Then the current velocity is adjusted to the neighbourhood best bird and the birds are moved.*/
	int step(int steps=1);


	///Returns the scout that bird_i is following
	/**If bird_i happens to be a scout -1 is returned*/
	int getUseScoutBird(int bird_i);

	
	protected:

		
		
		///The velocities of the birds are updated
		/**Each bird updates its velocity according to the respective scout,
		 * whose index is stored in #useScout */
		void updateVel();
		

		///Controls whether in the current step a swap is allowed
		bool swapEnabled;
		
		///A swap is only allowed every swapDelay steps
		int swapDelay;

		///The number of scouts to be used
		int nrScouts;		

		///The number of workers per scout
		int nrWorkers;
		
		///In this vector the respective scout of bird_i is stored
		/**Scouts have an index of -1 stored*/
		vector<int> useScout;
			
};



#endif //ifndef SWARM_H

