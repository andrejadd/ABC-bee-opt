#include "lsearch.h"


LSearch::LSearch(int _dim, function_type _funct) : PSO(0.0, ///<The w parameter (influence of old vel - momentum)
  		0.0, ///<The minimum value for w -- decreased by time or level
  		0.0, ///<Influence of personal best
  		0.0, ///<Influence of global best
		0.0, ///<vmax = vmaxRatio*xmax, with vmaxRatio=0 no vmax is enforced 
		0.0, ///<Constriction factor, used by #Bird
  		0, ///<Swarmsize
  		_dim, ///<Dimension of the search space
		0, ///<Height of the pyramid, ignored for #seqSwarm
		0, ///<Branches per node of the pyramid, ignored for #seqSwarm
		0, ///<Swap allowed every swapDelay steps in #PyramidSwarm, ignored for #Swarm
  		_funct, ///<#function_type selection
		noNoise, ///<#noiseStyle_type selection, set in #optFunction
		0.0, ///<sigma parameter for gaussian distributed noise
  		seqSwarm) ///<#swarm_type selection
{

	TRACE_IN("LSearch::LSearch");
	DEBUG("New LSearch :");
	DEBUG1("dim = ",_dim);
	int a=0;

	for (int i=0; i<get_dim(); i++)
		currSolution.push_back(randDoubleRange(optFunction->get_min_x(), optFunction->get_max_x()));
	currentVal = evalFunction(currSolution);
	TRACE_OUT("LSearch::LSearch",0);
}

LSearch::~LSearch() {
	TRACE_IN("LSearch::~LSearch");
	TRACE_OUT("LSearch::~LSearch",0);
}


int LSearch::step(int steps) {
	TRACE_IN("LSearch::LSearch");

	for (int i=0; i<steps; i++) {
		currentVal = localSearch(currSolution, currentVal);

		stat->addGlobalBest(currentVal);
		stat->psoStepDone();

	}
	TRACE_OUT("LSearch::LSearch", 0);
	return 0;
}

int main(int argc, char** argv) {
	TRACE_IN("main");

	cout.precision(10);

	function_type function=sphere;
	/*if (argc >0) {
		for (int i=0; i<argc; i++)
			cout << argv[i] << endl;
	}*/
	if (argc <=1) {
		cerr << "usage: lsearch optFunction (see pso.h)" << endl;
		exit(-1);
	}
	else {
		function=(function_type) atoi(argv[1]);
	}
	//GLOBAL PARAMETERS
	int runs=10;
	int dim=30;
	int steps=7;

	for (int set=0; set<1; set++)
		for (int run=0; run<runs; run++) {
			sleep(1);
		  cout << "#" << "Setting " << set << ", Run" << run << endl;
		  LSearch* l = new LSearch(dim, function);

		  if (run==0) {
			cout << l->printParameter("#");
			cout << "#step | eval | best " << endl;
		  }

		  for (int i=0; i<steps; i++) {
			l->step(1);
			cout << i << " " << l->stat->getNrEvaluations(i) << " "<< l->stat->getGlobalBest(i) <<  endl;
		  }
			delete l;
			cout << "---" << endl;
		}
 	TRACE_OUT("main",0);
  return 0;
}
