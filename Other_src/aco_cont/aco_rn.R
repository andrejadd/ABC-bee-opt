source("ACO.R")
source("functions_aa.R")
for (i in 1:60) {
	print(as.integer(i))
	evals <- ACO(rn, 100, q=0.1, max.eval=500000)
	write(evals, file="results/rn_q0.1.dat", append=TRUE)
}
