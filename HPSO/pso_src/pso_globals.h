#ifndef PSOGLOBALS
#define PSOGLOBALS


#define MAXDOUBLE 1.79769313486231470e+308

#define MAXINT 2147483647

#include <string>
#include <vector>
#include <iostream>

class PSO;
class PsoStatistics;
class Swarm;
class Bird;
class Velocity;
class Position;


using namespace std;

#ifndef OPTFUNCTIONS_H
#include "optfunctions.h"
#endif

#ifndef TOOLS_H
#include "tools.h"
#endif
#ifndef PSO_H
#include "pso.h"
#endif
#ifndef PSOSTATS_H
#include "pso_statistics.h"
#endif

#ifndef SWIGPYTHON
#ifndef SWARM_H
#include "swarm.h"
#endif
#endif


#ifndef BIRD_H
#include "bird.h"
#endif



#endif
