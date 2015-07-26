source("ACO.R")
source("functions_aa.R")
for (i in 1:100) {
	print(as.integer(i))
	evals <- ACO(sc, 0.00001, q=0.1, max.eval=300000)
	write(evals, file="evals_sc_q0.1.dat", append=TRUE)
}
