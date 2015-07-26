#######################################################################
#
# ACO.R - R implementation of ACO_R algorithm for continuous and mixed-
#					variable optimization problems.
#
# Copyright (c) 2006 by Krzysztof Socha (ksocha@ulb.ac.be)
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by 
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
#
#
#	Changelog, Andrej:
#	vorzeichen von max.y ist umgedreht, da z.B. bei Sphere werte neg. sind (max problem)
#
#

# load core routines
source("routines.R")

# Ant colony optimization for continuous and mixed-variable problems
# Parameters:
#   f - function object - see functions.R for details and examples
#   e - max. error value - used when solution quality used as
#       stopping criterion
#		n.of.ants - number of ants used in each iteration >=2
#   k - size of the solution archive
#   q - locality of the search (0,1)
#   xi - convergence pressure (0,1) 
#   max.eval - maximal number of function evaluations when used as 
#              the stopping criterion
#   seed - random seed
#   ls - local search function (if used)
#
ACO <- function(f, e, n.of.ants=2, k=50, q=0.0001, xi=0.85, 
  max.eval=0, seed=0, ls=NULL) {

  # initialize random numeber generator
  if (seed>0) set.seed(seed)

  # set parameters
  e.abs <- e
  e.rel <- e
  max.value <- f$opt
  eval <- 0
  last.impr <- max.eval;
  nl <- matrix(NA,k,k-1)
  iteration <- 0

  # separate unordered and ordered from continuous variables
  range.u <- vector()
  range.o <- vector()
  n.of.x.vars <- 0
  n.of.o.vars <- 0
  n.of.u.vars <- 0
  for (i in f$d) {
    if (i[[1]]=="u") {
      range.u <- c(range.u, length(i[[2]]))
      n.of.u.vars <- n.of.u.vars + 1
    }
    if (i[[1]]=="o") {
      range.o <- c(range.o, length(i[[2]]))
      n.of.o.vars <- n.of.o.vars + 1
    }
    if (i[[1]]=="x") {
      n.of.x.vars <- n.of.x.vars + 1
    }
  }

  max.u <- rep(NA,n.of.u.vars)
  max.o <- rep(NA,n.of.o.vars)
  max.X <- rep(NA,n.of.x.vars)
  max.y <- -Inf

  p.X <- vector()
  p.u <- vector()
  p.o <- vector()
  p <- data.frame(v=numeric(),sd=numeric(),
    gr=numeric(),lr=numeric(),ld=numeric());

  # randomly choose the starting population 
  # (but based on the data given in the function definition)
  for (i in 1:k) {
    if (n.of.u.vars>0) {
      U <- vector()
      for (j in 1:n.of.u.vars) {
        U <- c(U, sample(range.u[j],1))       
      }
      U <- t(U)
    }
    else U <- NULL
    if (n.of.o.vars>0) {
      O <- vector()
      for (j in 1:n.of.o.vars) {
        O <- c(O, sample(range.o[j],1))       
      }
      O <- t(O)
    }
    X <- vector()
    for (j in 1:n.of.x.vars) {
      X <- c(X, runif(1,f$d[[n.of.u.vars+n.of.o.vars+j]][[2]][1],
        f$d[[n.of.u.vars+n.of.o.vars+j]][[2]][2]))
    }
    X <- t(X)
    if (is.function(ls)) {
      ls.results <- ls(U,O,X)
      U <- ls.results[[1]]
      O <- ls.results[[2]]
      X <- ls.results[[3]]
      y <- ls.results[[4]]
      eval <- eval + ls.results[[5]]
    } 
    else {
      y <- f$f(U,O,X)
      eval <- eval + 1
    }
    
    if (n.of.u.vars>0) p.u <- rbind(p.u,U)
    if (n.of.o.vars>0) p.o <- rbind(p.o,O)
    p.X <- rbind(p.X,X)

    new.row <- data.frame(y,0,0,0,0)
    colnames(new.row) <- colnames(p)
    p <- rbind(p,new.row)
  }
  
  p$gr <- rank(-p$v,ties="random")
  p$lr <- p$gr
  for (i in 1:k) 
    nl[i,] <- (1:k)[1:k!=i]     

  # generate and evaluate solutions until stop condition is met
  while (TRUE) {
    iteration <- iteration +1

    # generate the new points based on the chosen distributions 
    dist.mean <- cbind(p.o,p.X)   # list of means - coordinates
    if (sum(apply(dist.mean,2,sd))==0)   # population has converged!
      return(c(as.integer(iteration), as.integer(max.eval), -max.y, f$tf(max.u,max.o,max.X)))   
    dist.rank <- (p$lr + p$gr/k*k)/2   
    dim(dist.mean) <- c(length(p$v),n.of.o.vars+n.of.x.vars)
    count <- 0
    o.X <- vector()

    if (n.of.u.vars>0) {
      U <- gen.C(p.u,dist.rank,range.u,n.of.ants,q,k,xi)
      dim(U) <- c(length(U)/n.of.u.vars,n.of.u.vars)
      for (i in 1:n.of.ants) {
        idx <- vector()
        for (j in 1:dim(p.u)[1]) {
          idx[j] <- p.u[j,]==U[i,]
        }
        if (sum(idx)<k*xi | 
          sum(idx)<(n.of.u.vars+n.of.o.vars+n.of.x.vars+1)) 
            idx <- rep(TRUE,length(dist.rank))
        red.dist.mean <- dist.mean[idx,]
        dim(red.dist.mean) <- c(sum(idx),length(red.dist.mean)/sum(idx))
        red.nl <- matrix(NA,sum(idx),sum(idx)-1)
        for (j in 1:(sum(idx))) 
          red.nl[j,] <- (1:sum(idx))[1:sum(idx)!=j]  
        o.X <- rbind(o.X,gen.X(red.dist.mean,dist.rank[idx],
          red.nl,1,q,k,xi))
      }
    } else
      o.X <- gen.X(dist.mean,dist.rank,nl,n.of.ants,q,k,xi)
    
    if (length(o.X)==0)  # population has converged!
      return(c(as.integer(iteration),as.integer(max.eval), -max.y, f$tf(max.u,max.o,max.X)))   
    
    if (n.of.o.vars>0) {
      O.x <- o.X[,1:n.of.o.vars]
      dim(O.x) <- c(length(O.x)/n.of.o.vars,n.of.o.vars)
      O.x[O.x<0.5] <- 0.5 + .Machine$double.eps
      for (i in 1:n.of.o.vars) {
        o <- O.x[,i]
        o[o>range.o[i]] <- range.o[i] + 0.5 - .Machine$double.eps
        O.x[,i] <- o
      }
    }

    X <- o.X[,(n.of.o.vars+1):(n.of.o.vars+n.of.x.vars)]
    dim(X) <- c(length(X)/n.of.x.vars,n.of.x.vars)
    if (is.function(ls)) {
      ls.results <- ls(U,O.x,X)
      U <- ls.results[[1]]
      O.x <- ls.results[[2]]
      X <- ls.results[[3]]
      y <- ls.results[[4]]
      eval <- eval + ls.results[[5]]
    } 
    else {
      y <- f$f(U,O.x,X)
      eval <- eval + dim(X)[1]
      
    }
    if (n.of.u.vars>0) p.u <- rbind(p.u,U)
    if (n.of.o.vars>0) p.o <- rbind(p.o,O.x)
    p.X <- rbind(p.X,X)
    for (i in 1:dim(X)[1]) {
      
      new.row <- data.frame(y[i],0,0,0,0)
      colnames(new.row) <- colnames(p)
      p <- rbind(p,new.row)
  
#      p <- rbind(p,c(v=y[i],sd=0,gr=0,lr=0,ld=0))
    }
    p$gr <- rank(-p$v,ties="random")
    p$lr <- p$gr
  
    # remove the worst ones
    idx.worst <- p$lr==k
    if (length(idx.worst)>k) {
      c <- 1
      while (sum(idx.worst)<length(idx.worst)-k & c<k) {
        idx.worst <- p$lr>=k-c
        c <- c + 1
      }
      idx.stay <- trunc.p.random(p[idx.worst,],
        p.X[idx.worst,],k-sum(!idx.worst))
      c <- 1
      idx.final <- !idx.worst
      for (i in 1:length(idx.worst)) {
        if (idx.worst[i]==TRUE) {
          idx.final[i] <- idx.stay[c]
          c <- c + 1
        }   
      }
      p <- p[idx.final,]
      p.X <- p.X[idx.final,]
      dim(p.X) <- c(length(p.X)/n.of.x.vars,n.of.x.vars)
      if (n.of.u.vars>0) {
        p.u <- p.u[idx.final,]
        dim(p.u) <- c(length(p.u)/n.of.u.vars,n.of.u.vars)
      }
      if (n.of.o.vars>0) {
        p.o <- p.o[idx.final,]
        dim(p.o) <- c(length(p.o)/n.of.o.vars,n.of.o.vars)
      }
    }

    # recalculate the ranking
    p$gr <- rank(-p$v,ties="random")
    p$lr <- p$gr
    for (i in 1:k) 
      nl[i,] <- (1:k)[1:k!=i]   

    if (max(y)>max.y) {
      print(eval)
      print(max(y) )
      max.y <- max(y)
      max.X <- X[which.max(y),]
      if (n.of.u.vars>0) max.u <- U[which.max(y),]
      if (n.of.o.vars>0) max.o <- O.x[which.max(y),]
      last.impr <- eval
      if ((abs(max.y-max.value)<abs(e.rel*max.value+e.abs)) |
				(max.y>max.value))
          return(c(as.integer(iteration), as.integer(eval), -max.y, f$tf(max.u,max.o,max.X)))
    }

    if (max.eval>0 & eval>=max.eval) 
      return(c(as.integer(iteration),as.integer(eval), -max.y, f$tf(max.u,max.o,max.X)))
  }
}

