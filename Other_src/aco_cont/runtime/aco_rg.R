source("ACO.R")
source("functions_aa.R")
for (i in 1:100) {
	print(as.integer(i))
	evals <- ACO(rg, 100, q=0.1)
	write(evals, file="../results/rg_q0.1.dat", append=TRUE)
}
