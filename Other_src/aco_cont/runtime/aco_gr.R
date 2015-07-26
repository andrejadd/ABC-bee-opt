source("ACO.R")
source("functions_aa.R")
for (i in 1:100) {
	print(as.integer(i))
	evals <- ACO(gr, 0.1, q=0.1, max.eval=500000)
	write(evals, file="results/gr_q0.1.dat", append=TRUE)
}
