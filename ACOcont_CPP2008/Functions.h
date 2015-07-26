/***************************************************************************
 Function.h  -  description
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

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "config.h"
#include <vector>
#include <math.h>

class Function {
public:
  
  /// 
  /// dimension   
  ///
  int n; 

  ///
  /// domain borders (min,max)
  ///
  vector<pair<double, double> > constraint; 

  ///
  /// function that evaluates a solution vector
  ///
  virtual double evaluate(vector<double>& x) = 0; 
};

class Sphere: public Function {
public:
  Sphere(int dimension = 10, double min=-10.0, double max=10.0);
    virtual double evaluate(vector<double>& x);
};

class Griewank: public Function {
public:
    Griewank(int dimension = 10);
    virtual double evaluate(vector<double>& x);
};

class Ackley: public Function {
public:
    Ackley(int dimension = 10);
    virtual double evaluate(vector<double>& x);

};

class Rastrigin: public Function {
public:
    Rastrigin(int dimension = 10);
    virtual double evaluate(vector<double>& x);

};

#endif
