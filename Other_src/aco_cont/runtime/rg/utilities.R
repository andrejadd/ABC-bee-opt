
#######################################################################
#
# utilities.R - various utilities and routines
#
# Copyright (c) 2006 by Krzysztof Socha (ksocha@ulb.ac.be)
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by 
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#

# Generates randomized n-dimensional rotation matrix
gen.R <- function(n) {
  # generate n, n-dimensional vectors
  vec <- runif(n^2,-1,1)
  dim(vec) <- c(n,n)
  # calculate the orthogonal rotation matrix
  R <- qr.Q(qr(vec), complete=TRUE) # rot. matrix after orthogonalization   
  if (det(R)<0) {
    R[,1] <- -R[,1]
  }
  return(R)
}

