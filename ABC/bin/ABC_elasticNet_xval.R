
do.xval <- function(id = 61, response.idx = 2, lambdas1=c()) {

  data.prefix = "../../../../mainp.ecology/Data/SYNTHETIC/asText/"
  
  filein = paste(data.prefix, "/Data_id", id, ".Rdata.txt", sep="")
  
  #fileout = paste(data.prefix, "Data_id", id, ".transposed.Rdata.txt", sep="")
  
  data = as.matrix(read.table(filein))
  
  data = t(data)
  
  nr.training.samples = ceiling(nrow(data) * 0.7)
  train.idx = sample(1:nrow(data), nr.training.samples)
  train.set = data[train.idx,]
  
  fileout = "run.train.data.tmp"
  
  write.table(file=fileout, train.set, col.names=F, row.names=F, sep=",")

  ## for saving the MSE values for each lambda
  mse = c()
  
  for(l1 in lambdas1) {
  
    cat(".")
    cmd.string = paste("java ModelTester -f ", fileout ," -fct ElasticNet -method ABCgbest -runs 1 -pmode 4 -response ", response.idx, " -n 60 -l1 ", l1, sep="") 
  
    result = system(cmd.string, intern=T)
  
    ## delete first line
    table = as.numeric(strsplit(result[-1], " ")[[1]])
    
    ## extract coefficients
    coefs = table[2:length(table)]
  
    ## calculate MSE with crossvalidation data
    xval.idx = setdiff(c(1:nrow(data)), train.idx)
    xval.set = data[xval.idx,]  
    
    ## save response
    xval.y = xval.set[,response.idx]
    
    ## remove response
    xval.set = xval.set[,-response.idx]
    
    ## add intercept (the last coefficient in 'coefs' is the intercept weight)
    xval.set = cbind(xval.set, 1)
      
    ## do the mse
    mse = c(mse, sum((xval.set %*% coefs - xval.y )^2) / nrow(xval.set))

  }
  
  
  ## for the lowest MSE, do a run but with the whole data set
  best.run = which.min(mse)
  best.lambda = lambdas1[best.run]

  ## write transposed data
  fileout = "run.full.data.tmp"
  write.table(file=fileout, data, col.names=F, row.names=F, sep=",")
  
  cmd.string = paste("java ModelTester -f ", fileout ," -fct ElasticNet -method ABCgbest -runs 1 -pmode 4 -response ", response.idx, " -n 60 -l1 ", best.lambda, sep="") 
  
  result = system(cmd.string, intern=T)
  
  ## delete first line
  table = as.numeric(strsplit(result[-1], " ")[[1]])
  
  ## extract coefficients
  coefs = table[2:length(table)]
  
  return(list(coefficients=coefs, best.lambda=best.lambda, mses=mse))

}

runABC.elasticNet <- function() {
  
  
  par(mfrow=c(4,5))
  
  data.ids = 61

  ## FIXME: adopt to data
  all.nodes = 1:10
  
  lambdas = seq(0,20,1)
  
  for(id in data.ids) {
  
    edge.weights = matrix(0, nrow=length(all.nodes), ncol=length(all.nodes))
    
    for(response in all.nodes) {
    
      xval.res = do.xval(id, response, lambdas)
    
      ## remove intercept coefficient
      weights = xval.res$coefficients[-length(xval.res$coefficients)]
      
      ## FIXME: maybe maybe make a transformation to weight (normalization e.g.)
      # ...
      
      ## add to matrix
      predictor.idx = setdiff(all.nodes, response)
      
      edge.weights[response, predictor.idx] = weights
      
      plot(lambdas, xval.res$mses, type="l")
      

    }
  }
}