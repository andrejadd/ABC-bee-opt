source("ACO.R")
source("functions_aa.R")
q <- c(0.001, 0.1, 0.3, 0.5, 0.9)

for(j in 1:length(q)) {
      for (i in 1:5) {
	  evals <- c(q[j], as.integer(i), ACO(rn,100, q=q[j], max.eval=200000))
	  write(evals, file="q_test_rn.dat", append=TRUE)
      }
}

for(j in 1:length(q)) {
      for (i in 1:5) {
	  evals <- c(q[j], as.integer(i), ACO(gr,0.1, q=q[j], max.eval=200000))
	  write(evals, file="q_test_gr.dat", append=TRUE)
      }
}

for(j in 1:length(q)) {
      for (i in 1:5) {
	  evals <- c(q[j], as.integer(i), ACO(rg,100, q=q[j], max.eval=200000))
	  write(evals, file="q_test_rg.dat", append=TRUE)
      }
}

for(j in 1:length(q)) {
      for (i in 1:5) {
	  evals <- c(q[j], as.integer(i), ACO(ac,0.1, q=q[j], max.eval=200000))
	  write(evals, file="q_test_ac.dat", append=TRUE)
      }
}
