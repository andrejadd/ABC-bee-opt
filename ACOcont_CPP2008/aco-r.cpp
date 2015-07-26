/***************************************************************************
 aco-r.cpp  -  description
 -------------------
 begin                : Fri Sep 26 2008
 copyright            : (C) 2008 by Christian Blum
 email                : cblum@lsi.upc.edu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define RAND_UNIFORME (double)random()/(double)RAND_MAX

#include "Timer.h"
//#include "Random.h"
#include "Solution.h"
#include "Functions.h"
#include <string>
#include <list>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()
double* archive = NULL;
double* constrains = NULL;
int dimension = 10;
int archive_size = 5;
int n_of_ants = 3;
int n_of_iterations = 100;
double q = 1.0;
double rho = 1.0;

void init() {
    if (archive)
        free(archive);
    archive = (double*) malloc(archive_size * (dimension + 1) * sizeof(double));
    constrains = (double*) malloc(2 * dimension * sizeof(double));

    for (int i = 0; i < dimension; i++) {
        constrains[2 * i] = -600;
        constrains[2 * i + 1] = 600;
    }
}

/*void read_parameters(int argc, char **argv) {
 int iarg = 1;

 while (iarg < argc) {
 if (strcmp(argv[iarg], "-archive") == 0) {
 archive_size = atoi(argv[++iarg]);
 } else if (strcmp(argv[iarg], "-ants") == 0) {
 n_of_ants = atoi(argv[++iarg]);
 } else if (strcmp(argv[iarg], "-iterations") == 0) {
 n_of_iter = atoi(argv[++iarg]);
 } else if (strcmp(argv[iarg], "-rho") == 0) {
 rho = atof(argv[++iarg]);
 }

 iarg++;
 }
 }*/

double gaussian(double mean, double var) {

    double u1 = RAND_UNIFORME;
    double u2 = RAND_UNIFORME;
    double z = sqrt(-2.0 * log(u1)) * cos(2 * M_PI * u2);
    return mean + var * z;
}

void print_best_solution(Solution bSol, Function* af, Timer& atimer, int icount) {

    /*
     cout << "(";
     for (int i = 0; i < af->n; i++) {
     if (i < (af->n - 1)) {
     cout << (bSol->x)[i] << ",";
     }
     else {
     cout << (bSol->x)[i] << ")\t";
     }
     }
     */
    //cout << icount << "\t" << bSol.value << "\t" << atimer.elapsed_time(Timer::VIRTUAL) << endl;
}

double evaluate(double* s) {
    double result = 0.0;
    for (int i = 1; i <= dimension; i++) {
        result = result + (s[i] * s[i]);
    }
    result = result / 4000.0;
    double term2 = 1.0;
    for (int i = 1; i <= dimension; i++) {
        term2 = term2 * cos(s[i] / sqrt(i + 1));
    }
    result = result - term2 + 1.0;
    s[0]=result;
    return result;
}

int main(int argc, char **argv) {

    if (argc < 2) {
        cout << "Something wrong" << endl;
        exit(1);
    } else {
       // read_parameters(argc, argv);
    }

    srand((unsigned) time(0));

    Timer timer;

    //rnd = new Random((unsigned) time(&t));
    //rnd->next();

    // initializing the function object
    Function* f = new Griewank();

    /* initializing the best solution , AA: use Solution Class instead */
    double* best_solution;
    double best_solvalue;

    // creating initial archive
    for (int i = 0; i < archive_size; i++) {
                
      for (int j = 0; j < dimension; j++) {
            double l = (f->constraint)[j].first;
            double r = (f->constraint)[j].second;
            double rnum = RAND_UNIFORME;
            double xj = l + (rnum * (r - l));
            archive[i * dimension + j + 1] = xj;
        }

        double newvalue = evaluate(&archive[i * (dimension + 1)]);

        if (i == 0) {
	  best_solution = &archive[i * (dimension + 1)];
	  best_solvalue = newvalue;
        } else {
	  if (newvalue < best_solvalue) {
	    best_solution = &archive[i * (dimension + 1)];
	    best_solvalue = newvalue;
	  }
	}
        
	//print_best_solution(best_solution, f, timer, 0);
        
	//archive[i] = new_solution;
    }

    /*AA not defined ! */
    archive.sort();

    int ic = 0;
    int iter = 1;
    bool program_stop = false;

    while (!program_stop) {

        // first we choose one of the solutions from the archive according to their ranks
        map<Solution*, double> weight;
        double baseSum = 0.0;
        int count = 1;
	double k = ((double) archive_size);

	/* calculate the gaussian weight for each solution in the archieve */
        for (list<Solution>::iterator it = archive.begin(); it != archive.end(); it++) {

	  double w = (1.0 / (k  * q * sqrt(2 * M_PI))) * exp(-1.0 * ((((double) count) - 1.0) * (((double) count) - 1.0)) / (2.0 * q * q * (k * k)));
            
	    weight[&(*it)] = w;
            baseSum = baseSum + w;
            count++;
        }

        double rand = RAND_UNIFORME;
        double wheel = 0.0;

	// probability selection of solution according to weight 	      
        list<Solution>::iterator i = archive.begin();
        while ((wheel < rand) && (i != (archive.end()))) {
            wheel = wheel + (weight[&(*i)] / baseSum);
            i++;
        }
        i--;
        Solution* chosen_solution = &(*i);

        // next we generate "n_of_ants" empty solutions
        vector<Solution> new_solutions;
        for (int j = 0; j < n_of_ants; j++) {
            Solution* nSol = new Solution();
            new_solutions.push_back(nSol);
        }

        // now we generate the contents of the new solutions
        for (int i = 0; i < f->n; i++) {

            double mean = (chosen_solution->x)[i];
            double dev = 0.0;

	    /* calculates standard deviation dev for a choosen solution */
            for (list<Solution*>::iterator it = archive.begin(); it != archive.end(); it++) {
                double add = (chosen_solution->x)[i] - ((*it)->x)[i];
                if (add < 0.0) {
                    add = add * -1.0;
                }
                dev = dev + add;
            }
            dev = dev / k;
            dev = rho * dev;

	    /* left and right border of domain */
            double l = (f->constraint)[i].first;
            double r = (f->constraint)[i].second;

	    /* get an offset from gaussian distribution inside domain borders l,r */
            for (int j = 0; j < n_of_ants; j++) {
                double res = l - 1.0;
                while ((res < l) or (res > r)) {
                    res = gaussian(mean, dev);
                }
                (new_solutions[j]->x).push_back(res);
            }
        }

        /* evaluate new solutions and push in archieve */	      
        bool improved = false;
        for (int j = 0; j < n_of_ants; j++) {
	  /* solve */
	  new_solutions[j]->value = f->evaluate(new_solutions[j]->x);
          
	  /* push on archieve */
	  archive.push_back(new_solutions[j]);

            // Attention: in case of maximization the operator should change to ">"
            if (*(new_solutions[j]) < *best_solution) {

                //    if (new_sols[j]->value < bestSol->value) {
	      /* replace best solution with new one if better */
	      *best_solution = *new_solutions[j];

                //    	  bestSol->x = new_sols[j]->x;
                //    	  bestSol->value = new_sols[j]->value;
	      print_best_solution(best_solution, f, timer, iter);
	      improved = true;
            }
        }

        new_solutions.clear();

        if (improved) {
            ic = 0;
        } else {
            ic++;
        }

       	/* sort archieve and delete worsed solutions when archieve size is bigger
		 than the original one (k) */
        archive.sort();

        list<Solution*> new_pop;
        count = 0;
        for (list<Solution*>::iterator it = archive.begin(); it != archive.end(); it++) {
            if (count < archive_size) {
                new_pop.push_back(*it);
            } else {
                delete (*it);
            }
            count++;
        }

        archive.clear();
        archive = new_pop;
        new_pop.clear();

        iter++;
        if (iter >= n_of_iter) {
            program_stop = true;
        }
    }

    //delete(rnd);
    //akedelete(instance);
}
