
x <- scan(file="x.dat", what=numeric(0) )
y <- scan(file="y.dat", what=numeric(0) )

wilcox.test(y, x, alternative = "less", conf.level=0.95)


