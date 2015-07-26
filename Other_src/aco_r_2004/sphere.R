# Sphere Model (SM) (n variables)
f <- function(x) {

	Y <- rep(0,dim(x)[1])
	for (j in 1:(dim(x)[2]-1)) {
		Y <- Y + x[,j]^2
	}
	return(-Y)
}
