source("ACO.R")
source("functions_aa.R")
for (i in 6:100) {
	print(as.integer(i))
	filename <- paste("../results/rg", i, sep = "_")
	evals <- ACO(rg, 100, q=0.1, max.eval=300000, outfile=filename)
}	
