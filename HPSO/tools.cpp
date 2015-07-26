#include "tools.h"
#include <ctime>



//required for TRACE / DEBUG
int indentLevel=0;

string BLANKLINE = "                                                                        \
                                                                                           \
                                                                                           \
                                                                                           ";


gsl_rng * rng;  /* global generator */
time_t t1;
 
				 
int getMaxHeight(int minDegree, int swarmsize) {

	int size=0;
	//will be increased in the first iteration
	int height=0;
	while (size <= swarmsize) {
		size += pow((double) minDegree, (double) height);
		height++;
	}
	DEBUG1("Max Height for minDegree ", minDegree << " is " << height);

	return height;

}

void freeRng() {
	gsl_rng_free(rng);
	rng = NULL;
}

double randDoubleGaussian(double sigma) {
	double retVal;
	if (rng == NULL) {
		//if called for the first time - initialize
		const gsl_rng_type * T;

		gsl_rng_env_setup();

		T = gsl_rng_default;
  	rng = gsl_rng_alloc (T);

	//unsigned seed = (unsigned)time(0);
	(void) time(&t1);
	unsigned seed = (unsigned) t1;
	cout << seed << endl;


//		gsl_rng_set(rng, (unsigned)time(0));
	gsl_rng_set(rng, seed);
	//cout << "#rng seeded with "<< seed << endl;

	}
	retVal = gsl_ran_gaussian(rng,sigma);

	DEBUGV2("RandomGenerator generated Gaussian (#sigma) Double: ",sigma, retVal);
	return retVal;
}

void fixSeed(unsigned seed) {
		const gsl_rng_type * T;

		gsl_rng_env_setup();

		T = gsl_rng_default;
  	rng = gsl_rng_alloc (T);

	cerr << "FIXED random seed !!!! " << seed << endl;
	gsl_rng_set(rng, seed);
	//cout << "#rng seeded with "<< seed << endl;
}
double randDouble() {
	double retVal;
	if (rng == NULL) {
		//if called for the first time - initialize
		const gsl_rng_type * T;

		gsl_rng_env_setup();

		T = gsl_rng_default;
  	rng = gsl_rng_alloc (T);

	//unsigned seed = (unsigned)time(0);
	(void) time(&t1);
	unsigned seed = (unsigned) t1;
	cout << seed << endl;
	//unsigned seed = 1090487579;
	//cout << "FIXED random seed !!!!" << endl;


//		gsl_rng_set(rng, (unsigned)time(0));
	gsl_rng_set(rng, seed);
	//cout << "#rng seeded with "<< seed << endl;

	}
	retVal = gsl_rng_uniform(rng);
	DEBUGV1("RandomGenerator generated Double: ", retVal);
	return retVal;
}


int randIntRange(int min, int max) {
	int retVal;
	if (rng == NULL) {
		//if called for the first time - initialize
		const gsl_rng_type * T;

		gsl_rng_env_setup();

		T = gsl_rng_default;
  	rng = gsl_rng_alloc (T);

	//unsigned seed = (unsigned)time(0);
	(void) time(&t1);
	unsigned seed = (unsigned) t1;
	//cout << seed << endl;

//		gsl_rng_set(rng, (unsigned)time(0));
	gsl_rng_set(rng, seed);
	//cout << "#rng seeded with "<< seed << endl;

	}
	//determine range of values
	int range = max - min + 1;
	//value from [0,range-1]
	retVal = gsl_rng_uniform_int(rng,range);
	//change offset
	retVal +=min; 
	DEBUGV1("RandomGenerator generated Int: ", retVal);
	return retVal;
}



double randDoubleRange(double min, double max) {
	double retVal;
	if (rng == NULL) {
		//if called for the first time - initialize
		const gsl_rng_type * T;

		gsl_rng_env_setup();

		T = gsl_rng_default;
  	rng = gsl_rng_alloc (T);
	//unsigned seed = (unsigned)time(0);
	(void) time(&t1);
	unsigned seed = (unsigned) t1;
	//cout << seed << endl;

	//seed = 1130868705;
	//cout << "FIXED random seed !!!!" << endl;


//		gsl_rng_set(rng, (unsigned)time(0));
	gsl_rng_set(rng, seed);
	//cout << "#rng seeded with "<< seed << endl;

	}
	//determine range of values
	double range = max - min;
	//value from [0,1)
	double rand = gsl_rng_uniform(rng);
	//change offset
	retVal = rand*range + min;
	DEBUGV1("RandomGenerator generated Double: ", retVal);
	return retVal;
}


string printVec(const vector<double>& v) {
	string tmpString;
	//allocate memory for "(1.234, 2.345, ..)
	char valChar[255]; 
	tmpString.append("(");
	for (int i = 0; i < v.size(); i++) {
		sprintf(valChar,"%.10lf, ",v[i]);
		tmpString.append(valChar);
	}
	tmpString.erase(tmpString.size()-2,tmpString.size()-1);
	tmpString.append(")");
	
	return tmpString;
	
}

double euclideanDistance(const vector<double>& v1, const vector<double>& v2) {
	double distance=0;
	double exp = 2.0;
	for (int i = 0; i < v1.size(); i++) {
		//dimension is vel.size()
		distance += pow(ABS(v1[i]-v2[i]), exp);
	}
	exp = 1.0/exp;
	distance = pow (distance, exp);
	return distance;
	
}

SequenceGenerator::SequenceGenerator(unsigned seed_in) {
	seed = seed_in;
	//if called for the first time - initialize
	const gsl_rng_type * T;

	gsl_rng_env_setup();

	T = gsl_rng_default;
  	randGen = gsl_rng_alloc (T);

	gsl_rng_set(randGen, seed);
	//cout << "sequenceGenerator seeded with "<< seed << endl;



}

SequenceGenerator::~SequenceGenerator() {
	gsl_rng_free(randGen);
}

double SequenceGenerator::nextDoubleExponential(double mu) {
	double retVal;
	retVal = gsl_ran_exponential(randGen, mu);
	DEBUGV1("SequenceGenerator generated Double: ", retVal);
	return retVal;
}

double SequenceGenerator::nextDoubleRange(double min, double max) {
	double retVal;
	//determine range of values
	double range = max - min;
	//value from [0,1)
	double rand = gsl_rng_uniform(randGen);
	//change offset
	retVal = rand*range + min; 
	DEBUGV1("SequenceGenerator generated Double: ", retVal);
	return retVal;
}

int SequenceGenerator::nextIntRange(int min, int max) {
	int retVal;
	//determine range of values
	int range = max - min + 1;
	//value from [0,range-1]
	retVal = gsl_rng_uniform_int(randGen,range);
	//change offset
	retVal +=min; 
	DEBUGV1("SequenceGenerator generated Int: ", retVal);
	return retVal;
}
