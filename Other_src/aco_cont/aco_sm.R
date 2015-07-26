source("ACO.R")
source("functions_aa.R")
for (i in 1:100) {
	print(as.integer(i))
	evals <- ACO(sm,0.01, q=0.1)
	write(evals, file="evals_sm.dat", append=TRUE)
}
