#############################################################################
#    eACO - Ant Colony Optimization for continuous domain.
#
#                             -------------------
#    copyright            : (C) 2004 by Krzysztof Socha
#    email                : ksocha@ulb.ac.be
#############################################################################

#############################################################################
#                                                                         
#   This program is free software; you can redistribute it and/or modify  
#   it under the terms of the GNU General Public License as published by  
#   the Free Software Foundation; either version 2 of the License, or     
#   (at your option) any later version.                                   
#                                                                         
#############################################################################



#############################################################################
# Helper functions

# Returns a sum of a set of normal probability density functions
sum.dnorm <- function(x, mean, sd) {
	if (length(mean)!=length(sd)) {
		stop("Mean and Standard Deviation vectors differ in length!");
	}
	y <- 0
	for (i in 1:length(mean)) {
		 y <- y + dnorm(x, mean[i], sd[i])
	}
	return(y/length(mean))
}

# Initializes the array of distributions
init.D <- function(n.of.vars, n.of.dist, min.range, max.range) {
	D <- array(dim=c(n.of.dist,2,n.of.vars))
	for (i in 1:n.of.vars) {
		D[,1,i] <- runif(n.of.dist,min.range,max.range)
		D[,2,i] <- rep((max.range-min.range)/n.of.dist,n.of.dist)
	}
	return(D)
}

# Generates a set of points based on given distributions
gen.X <- function(distributions, n.of.points) {
	X <- array(dim=c(n.of.points,dim(distributions)[3]))
	# iterate through all the dimentions
	for (i in 1:dim(distributions)[3]) {
		t.x <- vector()
		# iterate through all the distributions
		for (j in 1:dim(distributions)[1]) { 
			t.x <- c(t.x, rnorm(n.of.points,distributions[j,1,i],distributions[j,2,i]))
		}
		dim(t.x) <- c(n.of.points,dim(distributions)[1])
		idx <- as.integer(runif(n.of.points,0,dim(distributions)[1]))+1
		n.x <- vector()
		for (j in 1:n.of.points) {
			n.x <- c(n.x,t.x[j,idx[j]])
		}
		X[,i] <- n.x
	}
	return(X)
}


#############################################################################
# eACO Algorithm
#
# Parameters:
#		min.range - the lower bound on the domain,
#		max.range - the upper bound on the domain,
#		e.abs			- the required accuracy,
#		n.of.ants	- the numeber of ants to be used,
#		n.of.dist	- the number of component normal distributions to be used - 1,
#		n.of.vars - the number of dimensions,
#		max.value - the known maximum of the evaluated function,
#		show			- whether the graphical representation of the distributions should be drawn (only for <4 dimensions)

eACOmin <- function(min.range, max.range, e.abs, n.of.ants, n.of.dist, n.of.vars, max.value, show=FALSE) {

	# paramteres
	e.rel <- e.abs

	# initialize stuff
	max.X <- vector()
	max.y <- Inf
	eval <- 0
	oldest.dist <- 1
	d <- init.D(n.of.vars,n.of.dist+1,min.range,max.range)

	# visualization makes sense only for small number of varaibles
	if (n.of.vars<4 & show) {
		layout(c(1:n.of.vars),n.of.vars,1)
	}

	# do some iterations (numeber to be perhaps parametrized)
	for (c in 1:3000) {

		# generate new points and chose only those that fit in the range
		X <- gen.X(d,n.of.ants)
		subset <- vector()
		for (i in 1:n.of.ants) {
			subset <- c(subset, all(X[i,]>min.range & X[i,]<max.range)) 
		}
		X <- X[subset,]
	
		# see if there are more than 1 point left
		if (length(X)>n.of.vars) {

			# evaluate the solutions and find the best
			eval <- eval + dim(X)[1]
			y <- f(X)
			
			# find column with minimal value
			idx <- which.min(y)
			
			# create the new distribution based on the best point
			# n.d <- c(X[idx,], rep( max( (apply(X,2,max)-apply(X,2,min))/sqrt(c),e.abs),n.of.vars))
			
			n.d <- c(X[idx,], rep( min( (apply(X,2,max)-apply(X,2,min))/sqrt(c),e.abs),n.of.vars))

			dim(n.d) <- c(n.of.vars, 2)
			
			if (y[idx]<max.y) {
				max.y <- min(y[idx],max.y)
				max.X <- X[idx,]
				d[n.of.dist+1,,] <- t(n.d)
			}
			else {
				d[oldest.dist,,] <- t(n.d)
				oldest.dist <- oldest.dist %% n.of.dist + 1
			}

			# plot the distributions if not too many variables used
			if (n.of.vars<4 & show) {
				for (j in 1:n.of.vars) {
					plot(function(x)sum.dnorm(x,d[,1,j],d[,2,j]),min.range,max.range,xlab=paste("f(",max.X[j],")=",max.y))		    
				}
			Sys.sleep(0.2)
			}
			if (abs(max.y-max.value)<abs(e.rel*max.value+e.abs)) {
				return(c(eval, max.y, max.X))
			}
		}
	}
	return(c(eval, max.y, max.X))
}
