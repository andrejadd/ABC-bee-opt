source("ACO.R")
source("functions_aa.R")
for (i in 7:100) {
	print(as.integer(i))
	filename <- paste("../results/gr", i, sep = "_")
	evals <- ACO(gr, 0.1, q=0.1, max.eval=300000, outfile=filename)
}	
