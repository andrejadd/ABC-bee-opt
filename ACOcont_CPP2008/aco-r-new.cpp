
#include "Colony.h"
#include "Functions.h"
#include <stdio.h>
#include <iostream>

int main(int argc, char **argv) {

     // initializing the function object
  Function* f = new Griewank();
  //Function *f = new Sphere(30, -100, 100);
    int archieve_size = 50;
    int n_of_ants = 2;

    Colony *col = new Colony(f, archieve_size, n_of_ants, 0.1, 0.851);

    col->initialize();

    int iters = 300000;
    double best = 100000;
    int i = 0;

    while(i++ < iters) {
      
      col->iterate();
      
      double besttmp = col->getBestValue();
      
      //      if(besttmp < 10e-10) {
      //	cout << i << ": reached goal" << endl;
      //	break;
      //}

      if(besttmp < best) {
	best = besttmp;
	printf("%i \t %f\n", i, best);
      }
    }
    printf("%i \t %f\n", i, best);

  
}
