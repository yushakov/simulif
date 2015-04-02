from pylab import *
import sys

bin_num  = 1000
filename = sys.argv[1]
colnum   = int(sys.argv[2])

f = open(filename)
                  
# get ISI array
isi     = []
prev_t  = 0
idx     = 0
max_isi = 0
for line in f:
    s = line.strip()
    if s and s[0] != '#':
        fields = line.split(',')
        x      = tuple([float(field) for field in fields])
        if idx > 0 and xprev < 2 and x[colnum] > 2:
            interval = x[0] - prev_t
            isi.append(interval)
            if max_isi < interval:
                max_isi = interval
            prev_t = x[0]
        else:
            xprev = x[colnum]
        idx += 1
f.close()
lenx = idx
step = max_isi / (bin_num)

# get distribution
distribution = [0.0]*bin_num
xvalues      = [step*xv for xv in range(bin_num)]
for interval in isi:
	dstr_idx = int(interval / step)
	if interval == max_isi:
		dstr_idx = bin_num - 1
	distribution[dstr_idx] += 1.0 / (lenx*bin_num)

plot(xvalues, distribution)
show()