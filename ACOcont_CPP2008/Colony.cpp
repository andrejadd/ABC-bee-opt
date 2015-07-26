 /***************************************************************************
                          Colony.cpp  -  description
                             -------------------
    begin                : Sat Sep 27 2008
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

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "Colony.h"


Colony::Colony() {

  f = NULL;
  bestSol = new Solution();
  nonimpr_counter = 0;

  srand(time(NULL));
}

Colony::Colony(Function* my_f, int my_archive_size, int my_n_of_ants, double my_q, double my_rho) {

  f = my_f;
  archive_size = my_archive_size;
  n_of_ants = my_n_of_ants;
  q = my_q;
  rho = my_rho;
  bestSol = new Solution();
  nonimpr_counter = 0;

  srand(time(NULL));
}

Colony::~Colony() {

  delete(bestSol);
  if (archive.size() > 0) {
    for (list<Solution*>::iterator it = archive.begin(); it != archive.end(); it++) {
      delete(*it);
    }
    archive.clear();
  }
}

int Colony::initialize() {

  if (archive.size() > 0) {
    for (list<Solution*>::iterator it = archive.begin(); it != archive.end(); it++) {
      delete(*it);
    }
    archive.clear();
  }
  int best = 1;
  for (int i = 0; i < archive_size; i++) {
    Solution* aSol = new Solution();
    for (int j = 0; j < f->n; j++) {
      double l = (f->constraint)[j].first;
      double r = (f->constraint)[j].second;
      
      double rnum = (double)rand()/ ((double)RAND_MAX + (double) 1);
            
      double xj = l + (rnum * (r-l));
      
      (aSol->x).push_back(xj);
    }
    aSol->value = f->evaluate(aSol->x);
    if (i == 0) {
      bestSol->x = aSol->x;
      bestSol->value = aSol->value;
    }
    else {
      // Attention: in case of maximization the operator should change to ">"
      if (aSol->value < bestSol->value) {
	bestSol->x = aSol->x;
	bestSol->value = aSol->value;
        best = i+1;
      }
    }
    archive.push_back(aSol);
  }
  archive.sort();
  nonimpr_counter = 0;
  return best;
}

double Colony::iterate() {

  // first we choose one of the solutions from the archive according to their ranks
  map<Solution*,double> weight;
  double baseSum = 0.0;
  int count = 1;
  for (list<Solution*>::iterator it = archive.begin(); it != archive.end(); it++) {

    /* count is the rank */
    double w = (1.0/(((double)archive_size)*q*sqrt(2*M_PI))) * exp(-1.0 * ((((double)count)-1.0) * (((double)count)-1.0))/(2.0 * q * q * ((double)archive_size) * ((double)archive_size)));
    weight[*it] = w;
    baseSum = baseSum + w;
    count++;
  }

  double rnum = (double) rand()/ ((double)RAND_MAX + (double)1);
  //cout << rnum << "\t";
  double wheel = 0.0;

  list<Solution*>::iterator i = archive.begin();
  while ((wheel < rnum) && (i != (archive.end()))) {
    wheel = wheel + (weight[*i] / baseSum);
    i++;
  }
  i--;
  Solution* chosen = *i;

  // next we generate "n_of_ants" empty solutions
  vector<Solution*> new_sols;
  for (int j = 0; j < n_of_ants; j++) {
    Solution* nSol = new Solution();
    new_sols.push_back(nSol);
  }

  // now we generate the contents of the new solutions
  for (int i = 0; i < f->n; i++) {
    double mean = (chosen->x)[i];
    double dev = 0.0;
    for (list<Solution*>::iterator it = archive.begin(); it != archive.end(); it++) {
      double add = (chosen->x)[i] - ((*it)->x)[i];
      if (add < 0.0) {
        add = add * -1.0;
      }
      dev = dev + add;
    }
    dev = dev / ((double)archive_size);
    dev = rho * dev;

    double l = (f->constraint)[i].first;
    double r = (f->constraint)[i].second;
    for (int j = 0; j < n_of_ants; j++) {
      double res = l - 1.0;
      while ((res < l) or (res > r)) {
        res = gaussian(mean,dev);
      }
      (new_sols[j]->x).push_back(res);
    }
  }

  /* look if one of the ants has a better solution */
  int best = 1;
  bool improved = false;
  for (int j = 0; j < n_of_ants; j++) {
    new_sols[j]->value = f->evaluate(new_sols[j]->x);
    archive.push_back(new_sols[j]);

    // Attention: in case of maximization the operator should change to ">"
    if (new_sols[j]->value < bestSol->value) {
      bestSol->x = new_sols[j]->x;
      bestSol->value = new_sols[j]->value;
      improved = true;
      best = j+1;
    }
  }

  new_sols.clear();

  if (improved) {
    nonimpr_counter = 0;
  }
  else {
    nonimpr_counter++;
  }

  /* delete worsed solutions, to keep archieve in size */
  archive.sort();
  list<Solution*> new_archive;
  count = 0;
  for (list<Solution*>::iterator it = archive.begin(); it != archive.end(); it++) {
    if (count < archive_size) {
      new_archive.push_back(*it);
    }
    else {
      delete(*it);
    }
    count++;
  }
  archive.clear();
  archive = new_archive;
  new_archive.clear();
  return best;
}

void Colony::delete_worst() {

  archive.sort();
  list<Solution*>::reverse_iterator rit = archive.rbegin();
  delete(*rit);
  archive.pop_back();
}

double Colony::gaussian(double mean, double var) {

  double u1 = (double)rand()/ ((double)RAND_MAX + (double) 1);
  double u2 = (double)rand()/ ((double)RAND_MAX + (double) 1);
  double z = sqrt(-2.0*log(u1))*cos(2*M_PI*u2);
  return mean + var*z;
}

double Colony::getBestValue() {
  return bestSol->value;
   
}


