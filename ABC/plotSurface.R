
require(rgl)  ## for 3D surface plotting

plotValues <- function(filename) {

data.abc = read.table(filename)
# remove duplicates
data.abc = unique(data.abc)

best.solution = data.abc[which.min(data.abc[,1]),]

cat("best solution: ")
print.table(best.solution)


x = data.abc[, 2]
xlab = "beta_1"

z = data.abc[, 3]
zlab = "beta_2"

y = log(data.abc[,1])
ylab = "score"


## option 1
open3d()
plot3d(x, y, z, col=rainbow(1000),xlab=xlab, zlab=zlab, ylab=ylab)                      

}


return(0)

plotValues("abc_results_resp2.txt")


## option 2
open3d()
scale.up = 5
rgl.points(x=x, z=z, y=(y*scale.up), alpha=0.75, back="lines", color=rainbow(1000))



## plot surface 
# rgl.surface(x=data.abc[,2], z=data.abc[,3], y=data.abc[,1], alpha=0.75, back="lines", color="black")
# rgl.points(x=data.abc[filter.idx, 2], z=data.abc[filter.idx, 4], y=data.abc[filter.idx,1], alpha=0.75, back="lines", color="black")

# exclude duplicates

y = matrix(NaN, nrow=length(x), ncol=length(z))


for(i in 1:length(x)) {
  
  for(j in 1:length(z)) {
    
    z.values = which(data.abc[,3] == x[i] & data.abc[,4] == z[j])
    
    if(length(z.values) == 0)  y[i,j] = NaN # browser()
    else y[i,j] = log(z.values[1 ])
  
  }
  cat(".")
}



open3d()
#rgl.points(x=x, z=z, y=y, alpha=0.75, back="lines", color="black")


ylim <- range(y[!is.nan(y)])
ylen <- ylim[2] - ylim[1] + 1
colorlut <- terrain.colors(ylen) # height color lookup table
col <- colorlut[ y-ylim[1]+1 ] # assign colors to heights for each point

open3d()
rgl.surface(x, z, y, color=col, back="lines")





x = c(1:3)
z = c(1:3)
y = matrix(NaN, nrow=length(x), ncol=length(z))

y[1,1] = 0.3
y[1,2] = 0.6
y[1,3] = 0.9

y[2,1] = 2
y[2,2] = 1
y[2,3] = 1.5

y[3,1] = 4
y[3,2] = 3
y[3,3] = 5


ylim <- range(y[!is.nan(y)])
ylen <- ylim[2] - ylim[1] + 1
colorlut <- terrain.colors(ylen) # height color lookup table
col <- colorlut[ y-ylim[1]+1 ] # assign colors to heights for each point

rgl.surface(x, z, y, color=col, back="lines")


## draw data points we trained with
rgl.points(x=1:10, z=1:10, y=1:10, col="red")
