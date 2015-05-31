from pylab import *
import sys

x = loadtxt(sys.argv[1], delimiter=",", unpack=False)

cols = len(x[0])

for col in range(cols-1):
    subplot(cols-1, 1, col+1)
    plot(x[:,0], x[:,col+1])
    if col == 0:
        title(sys.argv[1])

show()