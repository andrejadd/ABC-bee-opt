/***************************************************************************
                          Colony.h  -  description
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

#ifndef COLONY_H
#define COLONY_H

#include "config.h"

#include <vector>
#include <map>
#include <list>
#include <cstdlib>  // For srand() and rand()
#include "Solution.h"
#include "Functions.h"

class Colony {

public:

	Colony();
	Colony(Function* my_f, int my_archive_size, int my_n_of_ants, double my_q, double my_rho);
	~Colony();

public:

        Function* f;
        Solution* bestSol;
        int archive_size;
        int n_of_ants;
        double q;
        double rho;
        list<Solution*> archive;
        int nonimpr_counter;

        int initialize();
        double iterate();
        void delete_worst();
        double gaussian(double mean, double var);
	double getBestValue();

//        friend bool sol_compare(const Solution* s1, const Solution* s2);

};

#endif
