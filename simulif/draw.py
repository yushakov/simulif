from pylab import *
import sys

#x = loadtxt("output.txt", delimiter=",", unpack=False)
x = loadtxt(sys.argv[1], delimiter=",", unpack=False)

subplot(3, 1, 1)
plot(x[:,0], x[:,1])
title(sys.argv[1])

subplot(3, 1, 2)
plot(x[:,0], x[:,2])

subplot(3, 1, 3)
plot(x[:,0], x[:,3])

show()