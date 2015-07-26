source("ACO.R")
source("functions_aa.R")
for (i in 31:100) {
	print(as.integer(i))
	filename <- paste("../results/sm", i, sep = "_")
	evals <- ACO(sm, 0.01, q=0.1, max.eval=300000, outfile=filename)
}	
