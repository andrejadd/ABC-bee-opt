#include "optfunctions.h"

Function::Function(int dim_in, dynamicStyle_type dynamicStyle_in) {
	TRACE_IN("Function::Function");
	dynamicStyle = dynamicStyle_in;
	dim = dim_in;
	optimumMoveStyle = linearOptMove;
	updateStyle = updatePeriodic;

	//the seed for sequences defining the optfunction move
	//do not use the same seed as for rng
	unsigned seed = (unsigned)time(0)+1;
	moveSequence = new SequenceGenerator(seed);

  	for (int i = 0; i < dim; i++) {
		optPos.push_back(0.0);
	}


	TRACE_OUT("Function::Function",0);
}

Function::~Function() {
	TRACE_IN("Function::~Function");
	delete moveSequence;
	TRACE_OUT("Function::~Function",0);
}

double Function::evaluate(const vector<double>& pos) {
	TRACEV_IN("Function::evaluate");
	TRACEV_OUT("Function::evaluate",0);
	return 0.0;
}

double Function::noisyEvaluate(const vector<double>& pos) {
	TRACEV_IN("Function::noisyEvaluate");
	double retVal = evaluate(pos);
	TRACEV_OUT("Function::noisyEvaluate",retVal);
	return retVal;
}

vector<double> Function::addNoisePos(const vector<double>& pos) {
	//TODO: add a proper random distribution (more sphere like) -- uniform distributed angle and gaussian distributed length
	TRACEV_IN("Function::addNoisePos");
	vector<double> tmpVector;
	for (int i=0; i< pos.size(); i++)
		tmpVector.push_back(pos[i] + randDoubleGaussian(noiseSigma));
	
	TRACEV_OUT("Function::addNoisePos",0);
	return tmpVector;
}
double Function::addNoiseVal(double val) {
	TRACEV_IN("Function::addNoiseVal");
	double noise = randDoubleGaussian(noiseSigma);
	double retVal = val + noise;
	
	TRACEV_OUT("Function::addNoiseVal",noise);
	return retVal;
}

double Function::addNoiseRelativeVal(double val) {
	TRACEV_IN("Function::addNoiseRelativeVal");
	double noise = randDoubleGaussian(noiseSigma);
	double retVal = val *(1 + noise);
	
	TRACEV_OUT("Function::addNoiseRelativeVal",noise);
	return retVal;
}

bool Function::update(int step, bool goal) {
	TRACE_IN("Function:update");

	bool update = false;

	if (updateStyle == updatePeriodic) {
		if ((dynamicStyle == dynamicFct) && (moveFrequency != 0))
			//no update on first step
			if ((step % moveFrequency == 0) && (step != 0))
				update = true;
	}
	else if (updateStyle == updateOnGoal) {
		if (goal == true) {
			update = true;
			DEBUG1("updateonGoal at step ", step);
		}
	}
	else if (updateStyle == updateRandom) {
		if (step == nextMoveStep) {
			update = true;
			DEBUG1("random update move step ", step);
			nextMoveStep = step + moveSequence->nextIntRange(1,moveFrequency);
		}
	}
			
	
	if (update) {
		OUTPUT2("UpdateFunction with moveStyle / moveDistance", optimumMoveStyle, moveDistance << " at step " << step);
		double tmpMove;
		double stepDist;

		if ((optimumMoveStyle == randomLinearOptMove) || (optimumMoveStyle == randomDistanceOptMove))
			stepDist = moveSequence->nextDoubleRange(-1,1) * moveDistance;
		else if (optimumMoveStyle == exponentialLinearOptMove)
			//exponential random number pos or neg
			stepDist = ((moveSequence->nextIntRange(0,1))*2 -1) * moveSequence->nextDoubleExponential(moveDistance);

#ifdef DBUG
		cout << "Move Optimum : [ ";
#endif
	  	for (int i = 0; i < dim; i++) {
	  		if (optimumMoveStyle == randomOptMove)
				tmpMove = moveSequence->nextDoubleRange(-1,1) * moveDistance;
			else if (optimumMoveStyle == linearOptMove)
				tmpMove = moveDistance;
			else if ((optimumMoveStyle == randomLinearOptMove) || (optimumMoveStyle == exponentialLinearOptMove)) {
				tmpMove = stepDist;
			}
			else if (optimumMoveStyle == randomDistanceOptMove) {
				//move the optimum[i] for the given distance fwd (+1) or bwd (-1)
				//rand produces either +1 or -1
				tmpMove = ((moveSequence->nextIntRange(0,1))*2 -1) * stepDist;
			}

			optPos[i] += tmpMove;
#ifdef DBUG
			cout << tmpMove << " ";
#endif
		}
		
#ifdef DBUG
		cout << "]" << endl;
#endif
	}

		
	TRACE_OUT("Function:update",update);

	return update;
}

void Function::set_moveFrequency(int moveFrequency_in) {

	moveFrequency = moveFrequency_in;

	if (updateStyle == updateRandom) {
		//called at step 0
		nextMoveStep = moveSequence->nextIntRange(1,moveFrequency);
	}
}

void Function::set_moveDistance(double moveDistance_in) {
		moveDistance = moveDistance_in*(max_x-min_x);
}


Sphere::Sphere(int dim_in, dynamicStyle_type dynamicStyle_in)
		: Function(dim_in, dynamicStyle_in) {
	TRACE_IN("Sphere::Sphere");
	min_x = -100.0;
 	max_x = 100.0;
 	min_v = -100.0;
 	max_v = 100.0;
 	goal = 0.01;
	opt = 0.0;

  	moveFrequency = 1;
  	moveDistance = 10.0;


 	
	TRACE_OUT("Sphere::Sphere",0);
} 	

Sphere::~Sphere() {
	TRACE_IN("Sphere::~Sphere");
 	TRACE_OUT("Sphere::~Sphere",0);
} 	

double Sphere::evaluate(const vector<double>& pos) {
	TRACEV_IN("Sphere:evaluate");
	double val=0.0;
	if (dynamicStyle == staticFct) {
		for (int i=0; i<dim; i++)
		  val += pow(pos[i],2);
	}
	else {
		for (int i=0; i<dim; i++)
		  val += pow(pos[i] - optPos[i],2);
	}
	TRACEV_OUT("Sphere:evaluate",val);
	return val;
}



Rosenbrock::Rosenbrock(int dim_in, dynamicStyle_type dynamicStyle_in)
		: Function(dim_in, dynamicStyle_in) {
	TRACE_IN("Rosenbrock::Rosenbrock");
 	min_x = -30.0;
 	max_x = 30.0;
 	min_v = -30.0;
 	max_v = 30.0;
 	goal = 100;
	opt = 0.0;


  	moveFrequency = 100;
  	moveDistance = 1.0;



	TRACE_OUT("Rosenbrock::Rosenbrock",0);
} 	

Rosenbrock::~Rosenbrock() {
	TRACE_IN("Rosenbrock::~Rosenbrock");
 	TRACE_OUT("Rosenbrock::~Rosenbrock",0);
} 	

double Rosenbrock::evaluate(const vector<double>& pos) {
	TRACEV_IN("Rosenbrock:evaluate");
	double val=0.0;
	if (dynamicStyle == staticFct) {
		for (int i=0; i<dim-1; i++)
			val += 100 * pow(pos[i+1] - pow(pos[i],2),2) + pow((pos[i] - 1),2);
	}
	else {
		for (int i=0; i<dim-1; i++)
			val += 100 * pow((pos[i+1]-optPos[i+1]) - 
				pow(pos[i]-optPos[i],2),2) + pow(((pos[i]-optPos[i]) - 1),2);
	}
	TRACEV_OUT("Rosenbrock:evaluate",val);
	return val;
}

Rastrigin::Rastrigin(int dim_in, dynamicStyle_type dynamicStyle_in)
		: Function(dim_in, dynamicStyle_in) {
	TRACE_IN("Rastrigin::Rastrigin");
 	min_x = -5.12;
 	max_x = 5.12;
 	min_v = -5.12;
 	max_v = 5.12;
 	goal = 100;
	opt = 0.0;


  	moveFrequency = 200;
  	moveDistance = 1.0;


	TRACE_OUT("Rastrigin::Rastrigin",0);
} 	

Rastrigin::~Rastrigin() {
	TRACE_IN("Rastrigin::~Rastrigin");
 	TRACE_OUT("Rastrigin::~Rastrigin",0);
} 	

double Rastrigin::evaluate(const vector<double>& pos) {
	TRACEV_IN("Rastrigin:evaluate");
	double val=0.0;
	if (dynamicStyle == staticFct) {
		for (int i=0; i<dim; i++)
			val += ( pow(pos[i],2) - 10 * cos(2*M_PI*pos[i]) + 10 );
	}
	else {
		for (int i=0; i<dim; i++)
			val += ( pow((pos[i]-optPos[i]),2) - 10 * cos(2*M_PI*(pos[i]-optPos[i])) + 10 );		
	}
	TRACEV_OUT("Rastrigin:evaluate",val);
	return val;
}


Schaffer::Schaffer(int dim_in, dynamicStyle_type dynamicStyle_in)
		: Function(dim_in, dynamicStyle_in) {
	TRACE_IN("Schaffer::Schaffer");
 	min_x = -100;
 	max_x = 100;
 	min_v = -100;
 	max_v = 100;
 	goal = 0.00001;
	opt = 0.0;

  if (dim != 2) {
  	cout << "Schaffer is only defined for dim=2 !" << endl;
  	exit(-1);
  }
	TRACE_OUT("Schaffer::Schaffer",0);
} 	

Schaffer::~Schaffer() {
	TRACE_IN("Schaffer::~Schaffer");
 	TRACE_OUT("Schaffer::~Schaffer",0);
} 	

double Schaffer::evaluate(const vector<double>& pos) {
	TRACEV_IN("Schaffer:evaluate");
	double val=0.0;		
	val = 0.5 + ((pow( sin( sqrt( pow(pos[0],2.0) + pow(pos[1],2.0)) ) ,2.0) -0.5) /
					(pow(1.0 + 0.001 * (pow(pos[0],2.0) + pow(pos[1],2.0)),2.0)) );
	TRACEV_OUT("Schaffer:evaluate",val);
	return val;
}


Griewank::Griewank(int dim_in, dynamicStyle_type dynamicStyle_in)
		: Function(dim_in, dynamicStyle_in) {
	TRACE_IN("Griewank::Griewank");
 	min_x = -600;
 	max_x = 600;
 	min_v = -600;
 	max_v = 600;
 	goal = 0.1;
	opt = 0.0;

	TRACE_OUT("Griewank::Griewank",0);
} 	

Griewank::~Griewank() {
	TRACE_IN("Griewank::~Griewank");
 	TRACE_OUT("Griewank::~Griewank",0);
} 	

double Griewank::evaluate(const vector<double>& pos) {
	TRACEV_IN("Griewank:evaluate");
	double val=0.0;
	double val2=1.0;
	
	if (dynamicStyle == staticFct) {
		for (int i=0; i<dim; i++)
			val += pow(pos[i],2.0);
	}
	else {
		for (int i=0; i<dim; i++)
			val += pow((pos[i]-optPos[i]),2.0);
	}
	val = val/4000;

	double index = 1.0;	
	if (dynamicStyle == staticFct) {
		for (int i=0; i<dim; i++) {
			//square root is taken from the index
			//but the vector index starts from 0, not from 1
			//therefore sqrt(i+1)
			val2 *= cos(pos[i]/sqrt(index));
			index++;
		}
	}
	else {
		for (int i=0; i<dim; i++) {
			//square root is taken from the index
			//but the vector index starts from 0, not from 1
			//therefore sqrt(i+1)
			val2 *= cos((pos[i]-optPos[i])/sqrt(index));
			index++;
		}
	}


	val = val - val2 + 1.0;
	
	TRACEV_OUT("Griewank:evaluate",val);
	return val;
}


Ackley::Ackley(int dim_in, dynamicStyle_type dynamicStyle_in)
		: Function(dim_in, dynamicStyle_in) {
	TRACE_IN("Ackley::Ackley");
 	min_x = -32;
 	max_x = 32;
 	min_v = -32;
 	max_v = 32;
 	goal = 0.1;
	opt = 0.0;

	TRACE_OUT("Ackley::Ackley",0);
} 	

Ackley::~Ackley() {
	TRACE_IN("Ackley::~Ackley");
 	TRACE_OUT("Ackley::~Ackley",0);
} 	

double Ackley::evaluate(const vector<double>& pos) {
	TRACEV_IN("Ackley:evaluate");
	double sum1=0.0;
	double sum2=0.0;
	double val=0.0;
	
	for (int i=0; i<dim; i++)
		sum1 += pow(pos[i]-optPos[i],2.0);
	for (int i=0; i<dim; i++)
		sum2 += cos(2*M_PI*(pos[i]-optPos[i]));
	
	//tmpVal required, otherwise overflow ? (f(0)!=0)
	double tmpVal1 = exp(1.0) -  exp(sum2/(double)dim);
	double tmpVal2 = 20 - 20*exp( -0.2* sqrt(sum1/(double)dim));
	
	val = tmpVal1 + tmpVal2;
	
	TRACEV_OUT("Ackley:evaluate",val);
	return val;
}


Schwefel::Schwefel(int dim_in, dynamicStyle_type dynamicStyle_in) 
		: Function(dim_in, dynamicStyle_in) {
	TRACE_IN("Schwefel::Schwefel");
 	min_x = -500;
 	max_x = 500;
 	min_v = -500;
 	max_v = 500;
 	goal = 0.1;
	opt = 1.272783748618167e-05*dim_in; //global opt

	TRACE_OUT("Schwefel::Schwefel",0);
} 	

Schwefel::~Schwefel() {
	TRACE_IN("Schwefel::~Schwefel");
 	TRACE_OUT("Schwefel::~Schwefel",0);
} 	

double Schwefel::evaluate(const vector<double>& pos) {
	TRACEV_IN("Schwefel:evaluate");
	double sum1=0.0;
	double val=0.0;
	
	for (int i=0; i<dim; i++)
		sum1 -= (pos[i]-optPos[i])*sin(sqrt(ABS(pos[i]-optPos[i])));
	
	val = sum1 + 418.9829*dim;
	
	TRACEV_OUT("Schwefel:evaluate",val);
	return val;
}


Levy5::Levy5(int dim_in, dynamicStyle_type dynamicStyle_in) 
		: Function(dim_in, dynamicStyle_in) {
	TRACE_IN("Levy5::Levy5");
 	min_x = -2;
 	max_x = 2;
 	min_v = -2;
 	max_v = 2;
 	goal = -176.13657123796191;
	opt = -176.13757123796191; //global opt

  if (dim != 2) {
  	cout << "Levy Nr. 5 is only defined for dim=2 !" << endl;
  	exit(-1);
  }
  
	TRACE_OUT("Levy5::Levy5",0);
} 	

Levy5::~Levy5() {
	TRACE_IN("Levy5::~Levy5");
 	TRACE_OUT("Levy5::~Levy5",0);
} 	

double Levy5::evaluate(const vector<double>& pos) {
	TRACEV_IN("Levy5:evaluate");
	double sum1=0.0;
	double sum2=0.0;
	double val=0.0;
	
	for (int i=1; i<6; i++)
		sum1 += i*cos( (i-1)*(pos[0]-optPos[0]) + i );

	for (int i=1; i<6; i++) 
		sum2 += i*cos( (i+1)*(pos[1]-optPos[1]) + i );

	val = sum1 * sum2 + pow(pos[0] - optPos[0] + 1.42513, 2.0) + pow(pos[1] - optPos[1] + 0.80032, 2.0);
		
	TRACEV_OUT("Levy5:evaluate",val);
	return val;
}


Freudenstein::Freudenstein(int dim_in, dynamicStyle_type dynamicStyle_in) 
		: Function(dim_in, dynamicStyle_in) {
	TRACE_IN("Freudenstein::Freudenstein");
 	min_x = -5;
 	max_x = 5;
 	min_v = -5;
 	max_v = 5;
 	goal = 0.1;
	opt = 0.0; //global opt

  if (dim != 2) {
  	cout << "Freudenstein is only defined for dim=2 !" << endl;
  	exit(-1);
  }
  
	TRACE_OUT("Freudenstein::Freudenstein",0);
} 	

Freudenstein::~Freudenstein() {
	TRACE_IN("Freudenstein::~Freudenstein");
 	TRACE_OUT("Freudenstein::~Freudenstein",0);
} 	

double Freudenstein::evaluate(const vector<double>& pos) {
	TRACEV_IN("Freudenstein:evaluate");
	double val=0.0;
	
	double tmpval1 = pow(-13.0 + pos[0] + ((5 - pos[1])*pos[1] - 2)*pos[1],2.0);

	double tmpval2 = pow(-29.0 + pos[0] + ((pos[1] + 1)*pos[1] - 14)*pos[1],2.0);

	val = tmpval1 + tmpval2;
		
	TRACEV_OUT("Freudenstein:evaluate",val);
	return val;
}


Quadric::Quadric(int dim_in, dynamicStyle_type dynamicStyle_in) 
		: Function(dim_in, dynamicStyle_in) {
	TRACE_IN("Quadric::Quadric");
 	min_x = -100;
 	max_x = 100;
 	min_v = -100;
 	max_v = 100;
 	goal = 0.1;
	opt = 0.0; //global opt

  
	TRACE_OUT("Quadric::Quadric",0);
} 	

Quadric::~Quadric() {
	TRACE_IN("Quadric::~Quadric");
 	TRACE_OUT("Quadric::~Quadric",0);
} 	

double Quadric::evaluate(const vector<double>& pos) {
	TRACEV_IN("Quadric:evaluate");
	double val=0.0;
	double sum=0.0;
	
	for (int i=0; i<dim; i++) {
		for (int j=0; j<=i; j++) {
			sum += pos[j];
		}
		val += pow(sum, 2.0);
		sum=0.0;
	}
		
	TRACEV_OUT("Quadric:evaluate",val);
	return val;
}


MovingGoal::MovingGoal(int dim_in, dynamicStyle_type dynamicStyle_in) 
		: Function(dim_in, dynamicStyle_in) {
	TRACE_IN("MovingGoal::MovingGoal");
 	min_x = -50;
 	max_x = 50;
 	min_v = -50;
 	max_v = 50;
 	goal = 0.0001;
	opt = 0.0; //global opt

  	moveFrequency = 300;
  	moveDistance = 10.0;

  
	
  
	TRACE_OUT("MovingGoal::MovingGoal",0);
} 	

MovingGoal::~MovingGoal() {
	TRACE_IN("MovingGoal::~MovingGoal");
 	TRACE_OUT("MovingGoal::~MovingGoal",0);
} 	

double MovingGoal::evaluate(const vector<double>& pos) {
	TRACEV_IN("MovingGoal:evaluate");
	double val=0.0;
	double sum=0.0;
	
	for (int i=0; i<dim; i++) {
		sum += pow(optPos[i] - pos[i], 2.0);
	}
	val = sqrt(sum);
		
	TRACEV_OUT("MovingGoal:evaluate",val);
	return val;
}


MovingPeaks::MovingPeaks(int dim_in, dynamicStyle_type dynamicStyle_in) 
		: Function(dim_in, dynamicStyle_in) {
	TRACE_IN("MovingPeaks::MovingPeaks");
 	min_x = 0;
 	max_x = 100;
 	min_v = 0;
 	max_v = 100;
 	goal = 0.1;
	opt = 0.0; //global opt

  	moveFrequency = 10;
  	moveDistance = 1.0;

  
	set_geno_size(dim_in);
	set_number_peaks(50);
	set_vlength(moveDistance);
	
	init_peaks();
  
	TRACE_OUT("MovingPeaks::MovingPeaks",0);
} 	

MovingPeaks::~MovingPeaks() {
	TRACE_IN("MovingPeaks::~MovingPeaks");

	cout << "get_offline_error() = " << get_offline_error() << endl;
  	cout << "get_offline_performance() = " << get_offline_performance() << endl;
	cout << "get_number_of_evals() = " << get_number_of_evals() << endl;
	free_peaks();
	
 	TRACE_OUT("MovingPeaks::~MovingPeaks",0);
} 	

void MovingPeaks::set_moveDistance(double moveDistance_in) {
	moveDistance = moveDistance_in;
	set_vlength(moveDistance);
};


double MovingPeaks::evaluate(const vector<double>& pos) {
	TRACEV_IN("MovingPeaks:evaluate");
	
	double val=0.0;
	//eval_movepeaks does not like const vector
	vector<double> tmpPos;
	tmpPos.assign(pos.begin(), pos.end());
	val = eval_movpeaks(&tmpPos[0]);
		
	TRACEV_OUT("MovingPeaks:evaluate",val);
	return val;
}

bool MovingPeaks::update(int step, bool goal) {
	TRACE_IN("MovingPeaks:update");
	bool update = false;

	if (updateStyle == updatePeriodic) {
		if (dynamicStyle == dynamicFct)
			//no update on first step
			if ((step % moveFrequency == 0) && (step != 0))
				update = true;
	}
	else if (updateStyle == updateOnGoal) {
		if (goal == true) {
			update = true;
			DEBUG1("updateonGoal at step ", step);
		}
	}
			

	
	if (update) {
		DEBUG("UpdateFunction calling move_peaks()");
		change_peaks();			
	}
		
	TRACE_OUT("MovingPeaks:update",0);

	return update;
}

Sinusoidal::Sinusoidal(int dim_in, dynamicStyle_type dynamicStyle_in)
		: Function(dim_in, dynamicStyle_in) {
	TRACE_IN("Sinusoidal::Sinusoidal");
 	min_x = -1.0;
 	max_x = 1.0;
 	min_v = -1.0;
 	max_v = 1.0;
 	goal = 100;
	opt = 0.0;


  	moveFrequency = 1;
  	moveDistance = 1.0;


	TRACE_OUT("Sinusoidal::Sinusoidal",0);
} 	

Sinusoidal::~Sinusoidal() {
	TRACE_IN("Sinusoidal::~Sinusoidal");
 	TRACE_OUT("Sinusoidal::~Sinusoidal",0);
} 	

double Sinusoidal::evaluate(const vector<double>& pos) {
	TRACEV_IN("Sinusoidal:evaluate");
	double val=0.0;
	if (dynamicStyle == staticFct) {
		for (int i=0; i<dim; i++)
		  val += pow(pos[i],2);
	}
	else {
		for (int i=0; i<dim; i++)
		  val += pow(pos[i] - optPos[i],2);
	}
	TRACEV_OUT("Sinusoidal:evaluate",val);
	return val;
}

bool Sinusoidal::update(int step, bool goal) {
	TRACE_IN("Sinusoidal:update");
	bool update = false;

	if (updateStyle == updatePeriodic) {
		if (dynamicStyle == dynamicFct)
			//no update on first step
			if ((step % moveFrequency == 0) && (step != 0))
				update = true;
	}
	else if (updateStyle == updateOnGoal) {
		if (goal == true) {
			update = true;
			DEBUG1("updateonGoal at step ", step);
		}
	}
			

	
	if (update) {
		for (int i=0; i<dim; i++)
			optPos[i]=0.7*sin(step*M_PI*(i+1)/1000);
	}
		
	TRACE_OUT("Sinusoidal:update",0);

	return update;
}





Zitzler3::Zitzler3(int dim_in, dynamicStyle_type dynamicStyle_in)
		: Function(dim_in, dynamicStyle_in) {
	TRACE_IN("Zitzler3::Zitzler3");
 	min_x = 0.0;
 	max_x = 1.0;
 	min_v = 0.0;
 	max_v = 1.0;
 	goal = 100;
	opt = 0.0;


  	moveFrequency = 1;
  	moveDistance = 1.0;


	TRACE_OUT("Zitzler3::Zitzler3",0);
} 	

Zitzler3::~Zitzler3() {
	TRACE_IN("Zitzler3::~Zitzler3");
 	TRACE_OUT("Zitzler3::~Zitzler3",0);
} 	

double Zitzler3::evaluate(const vector<double>& pos) {
	TRACEV_IN("Zitzler3:evaluate");
	double val=0.0;
	if (dynamicStyle == staticFct) {
		double g = 0.0;
        for (int i = 1; i < dim; i++) {
			g+=pos[i];
		}
		g=9.0*g/(dim-1.0) + 1;
		val = g*(1 - (pos[0]/g)*(pos[0]/g));
		//val = g*(1 - pow(pos[0]/g, 2.0));
	}
	else {
		cerr << "only defined static" << endl;
	}
	
    for (int i = 0; i < dim; i++) {
            if ((pos[i]<0) || (pos[i]>1))
                    val=17;
    }
	
	TRACEV_OUT("Zitzler3:evaluate",val);
	return val;
}

bool Zitzler3::update(int step, bool goal) {
	TRACE_IN("Zitzler3:update");
	bool update = false;

	if (updateStyle == updatePeriodic) {
		if (dynamicStyle == dynamicFct)
			//no update on first step
			if ((step % moveFrequency == 0) && (step != 0))
				update = true;
	}
	else if (updateStyle == updateOnGoal) {
		if (goal == true) {
			update = true;
			DEBUG1("updateonGoal at step ", step);
		}
	}
			

	
	if (update) {
		for (int i=0; i<dim; i++)
			optPos[i]=0.7*sin(step*M_PI*(i+1)/1000);
	}
		
	TRACE_OUT("Zitzler3:update",0);

	return update;
}



