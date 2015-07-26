#ifndef TOOLS_H
#define TOOLS_H


#include <iostream>
#include <string>
#include <vector>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <math.h>

using namespace std;

//required for TRACE / DEBUG
extern int indentLevel;
extern string BLANKLINE;

//how many blanks to indent
#define INDENTATION 2
                                                                                           
#define INDENT BLANKLINE.substr(0,(indentLevel)*INDENTATION)
#define INDENT_INC BLANKLINE.substr(0,(indentLevel++)*INDENTATION)
#define INDENT_DEC BLANKLINE.substr(0,(--indentLevel)*INDENTATION)

#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define ABS(x) ((x)>0 ? (x) : -(x)) 


#ifdef STDOUT
#define OUTPUT(x) cout << INDENT << (x) <<"\n";
#define OUTPUT1(x,arg1) cout << INDENT << (x) <<arg1<<"\n";
#define OUTPUT2(x,arg1,arg2) cout << INDENT << (x) <<arg1<<" / " <<arg2<<"\n";

#else
#define OUTPUT(x) ""
#define OUTPUT1(x,arg1) ""
#define OUTPUT2(x,arg1,arg2) ""

#endif //STDOUT


#ifdef TRACE
#define TRACE_IN(x) cout << INDENT_INC << "***TRACE_IN: "<< (x) <<"()\n";
#define TRACE_IN1(x,arg1) cout << INDENT_INC << "***TRACE_IN: "<< (x) <<"("<<arg1<<")\n";
#define TRACE_OUT(x,ok) cout << INDENT_DEC << "***TRACE_OUT: "<< (x) <<"() = " << ok << "\n";

#else
#define TRACE_IN(x) ""
#define TRACE_IN1(x,arg1) ""
#define TRACE_OUT(x,ok) ""

#endif //TRACE

#ifdef TRACEV
#define TRACEV_IN(x) cout << INDENT_INC << "***TRACE_IN: "<< (x) <<"()\n";
#define TRACEV_IN1(x,arg1) cout << INDENT_INC << "***TRACE_IN: "<< (x) <<"("<<arg1<<")\n";
#define TRACEV_OUT(x,ok) cout << INDENT_DEC << "***TRACE_OUT: "<< (x) <<"() = " << ok << "\n";

#else
#define TRACEV_IN(x) ""
#define TRACEV_IN1(x,arg1) ""
#define TRACEV_OUT(x,ok) ""

#endif //TRACEV


#ifdef DBUG
#define DEBUG(x) cout << INDENT << "***DEBUG: "<< (x) <<"\n";
#define DEBUG1(x,arg1) cout << INDENT << "***DEBUG: "<< (x) <<arg1<<"\n";
#define DEBUG2(x,arg1,arg2) cout << INDENT << "***DEBUG: "<< (x) <<arg1<<" / " <<arg2<<"\n";

#else
#define DEBUG(x) ""
#define DEBUG1(x,arg1) ""
#define DEBUG2(x,arg1,arg2) ""

#endif //DBUG

//verbose debug information
//mainly exact positions (quite big with dim=30)
#ifdef DBUGV
#define DEBUGV(x) cout << "***DEBUG: "<< (x) <<"\n";
#define DEBUGV1(x,arg1) cout << "***DEBUG: "<< (x) <<arg1<<"\n";
#define DEBUGV2(x,arg1,arg2) cout << "***DEBUG: "<< (x) <<arg1<<" / " <<arg2<<"\n";

#else
#define DEBUGV(x) ""
#define DEBUGV1(x,arg1) ""
#define DEBUGV2(x,arg1,arg2) ""

#endif //DBUGV

///Little helper function calculating the maximum height
/**of a regularly built pyramid of 
  *@param minDegree minimum degree*/
int getMaxHeight(int minDegree, int swarmsize);

//fix the seed for the global RNG
void fixSeed(unsigned seed);

///Free the RNG ressources
void freeRng();
///Return gaussian distributed double from [0;1)
double randDoubleGaussian(double sigma);
///Return double from [0;1)
double randDouble();
///Wrapper to GSL Function call
/**Returns double from range [min,max)*/
double randDoubleRange(double min, double max);
///Wrapper to GSL Function call
/**Returns int from range [min,max]*/
int randIntRange(int min, int max);

string printVec(const vector<double>&);

double euclideanDistance(const vector<double>& v1, const vector<double>& v2);



///Generate a reproducable sequence
/**The seed is passed and thus the sequence is repeatable*/
class SequenceGenerator {
public:
	///Pass the seed
	SequenceGenerator(unsigned seed_in);
	///Destructor
	~SequenceGenerator();
	///Return exponentially distributed random variable, with mean mu
	double nextDoubleExponential(double mu);

	///Wrapper to GSL Function call
	/**Returns double from range [min,max)*/
	double nextDoubleRange(double min, double max);
	///Wrapper to GSL Function call
	/**Returns int from range [min,max]*/
	int nextIntRange(int min, int max);
private:
	///The random number generator
	gsl_rng*  randGen;

	///The random seed defining the sequence
	unsigned seed;
	

};

#endif //ifndef TOOLS_H
