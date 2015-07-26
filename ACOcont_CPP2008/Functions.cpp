/***************************************************************************
 Function.cpp  -  description
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

#include "Functions.h"

Griewank::Griewank(int dimension) {
    n = dimension;
    for (int i = 0; i < n; i++) {
        pair<double, double> c;
        c.first = -600.0;
        c.second = 600.0;
        constraint.push_back(c);
    }
}

double Griewank::evaluate(vector<double>& x) {

    double result = 0.0;
    for (int i = 0; i < n; i++) {
        result = result + (x[i] * x[i]);
    }
    result = result / 4000.0;
    double term2 = 1.0;
    for (int i = 0; i < n; i++) {
        term2 = term2 * cos(x[i] / sqrt(i + 1));
    }
    result = result - term2 + 1.0;
    return result;
}

Ackley::Ackley(int dimension) {
    n = dimension;
    for (int i = 0; i < n; i++) {
        pair<double, double> c;
        c.first = -15.0;
        c.second = 30.0;
        constraint.push_back(c);
    }
}

double Ackley::evaluate(vector<double>& x) {

    double result = 20 + exp(1);
    double exp1 = 0.0;

    for (int i = 0; i < n; i++) {
        exp1 = exp1 + (x[i] * x[i]);
    }
    exp1 = exp1 * (1.0 / double(n));
    exp1 = sqrt(exp1) * (-1.0 / 5.0);
    result = result - (20 * exp(exp1));
    double exp2 = 0.0;
    for (int i = 0; i < n; i++) {
        exp2 = exp2 + cos(2 * M_PI * x[i]);
    }
    exp2 = exp2 * (-1.0 / double(n));
    result = result - exp(exp2);
    return result;
}

Rastrigin::Rastrigin(int dimension) {
    n = dimension;
    for (int i = 0; i < n; i++) {
        pair<double, double> c;
        c.first = -5.12;
        c.second = 5.12;
        constraint.push_back(c);
    }
}

double Rastrigin::evaluate(vector<double>& x) {

    double result = 10 * n;
    for (int i = 0; i < n; i++) {
        result = result + ((x[i] * x[i]) - (10 * cos(2 * M_PI * x[i])));
    }
    return result;
}


Sphere::Sphere(int dimension, double min, double max) {
    n = dimension;
    for (int i = 0; i < n; i++) {
        pair<double, double> c;
        c.first = min;
        c.second = max;
        constraint.push_back(c);
    }
}

double Sphere::evaluate(vector<double>& x) {

    double result = 0;

    for (int i = 0; i < n; i++) {
      result = result + (x[i] * x[i]);
    }

    return (result/n);
}
