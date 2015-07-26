#######################################################################
# Functions
#
# Note:
#		Each function accepts as a parameter a matrix X. Each column of this matrix 
#   is a different dimiension x_i. Each row of this matrix is a single solution.
#		Hence, one call to a function may be used to evaluate multiple solutions,
#		as each function returns a vector of solutions.


# Goldstein and Price (GP) (2 variables)
f <- function(x) {
	return(-(1+(x[,1]+x[,2]+1)^2*(19-14*x[,1]+13*x[,1]^2-14*x[,2]+6*x[,1]*x[,2]+3*x[,2]^2))*(30+(2*x[,1]-3*x[,2])^2*(18-32*x[,1]+12*x[,1]^2+48*x[,2]-36*x[,1]*x[,2]+27*x[,2]^2)))
}

# Sphere Model (SM) (n variables)
f <- function(x) {
	Y <- rep(0,dim(x)[1])
	for (j in 1:(dim(x)[2]-1)) {
		Y <- Y + x[,j]^2
	}
	return(-Y)
}

# Hartmann (H3,4) (3 variables)
f <- function(x) {
	c <- c(1.0, 1.2, 3.0, 3.2)
	a <- c(c(3.0, 0.1, 3.0, 0.1), c(10, 10, 10, 10), c(30, 35, 30, 35))
	dim(a) <- c(4,3)
	p <- c(c(0.3689, 0.4699, 0.1091, 0.0381), c(0.1170, 0.4387, 0.8732, 0.5743), c(0.2673, 0.7470, 0.5547, 0.8828))
	dim(p) <- c(4,3)
	Y <- rep(0,dim(x)[1])
	for (i in 1:4) {
		y <- rep(0,dim(x)[1])
		for (j in 1:3) {
		    y <- y + a[i,j]*(x[,j]-p[i,j])^2
		}
		Y <- Y - c[i]*exp(-y)
	}
	return (-Y)
}

# Rosenbrock (Rn) (n variables)
f <- function(x) {
	Y <- rep(0,dim(x)[1])
	for (j in 1:(dim(x)[2]-1)) {
		Y <- Y + 100*(x[,j]^2-x[,j+1])^2+(x[,j]-1)^2
	}
	return(-Y)
}

# Zakharov (Zn) (n variables)
f <- function(x) {
	Y.1 <- rep(0,dim(x)[1])
	for (j in 1:dim(x)[2]) {
		Y.1 <- Y.1 + x[,j]^2
	}
	Y.2 <- rep(0,dim(x)[1])
	for (j in 1:dim(x)[2]) {
		Y.2 <- Y.2 + 0.5*j*x[,j]^2
	}
	Y.3 <- rep(0,dim(x)[1])
	for (j in 1:dim(x)[2]) {
		Y.3 <- Y.3 + 0.5*j*x[,j]^2
	}
	return(-Y.1-Y.2^2-Y.3^4)
}
