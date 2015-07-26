
# Rosenbrock (Rn) (n variables)
f <- function(x) {

	Y <- rep(0,dim(x)[1])
	for (j in 1:(dim(x)[2]-1)) {
		Y <- Y + 100*(x[,j]^2-x[,j+1])^2+(x[,j]-1)^2
	}
	return(-Y)
}