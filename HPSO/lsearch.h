#ifndef LSEARCH_H
#define LSEARCH_H

#ifndef PSOGLOBALS
#include "pso_globals.h"
#endif


class LSearch : public PSO {
	
	vector<double> currSolution;
	double currentVal;

	public:
		LSearch(int dim, function_type _funct);
		~LSearch();
	
		int step(int steps=1);
};

#endif //LSEARCH_H