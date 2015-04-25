"""
    Input file can contain:
    20
    15
    i.e., two periodic inputs; or
    15 5 10 10 5 15
    18 6 12 12 6 18
    i.e., two inputs of cycled interval sequences; or
    2
    3
    5
    7
    i.e., 4 periodic inputs; etc.
    
"""
import sys
import queue as q
import re
import matplotlib.pyplot as plt

fname   = sys.argv[1]
file    = open(fname)
sources = list()
for line in file:
    strp = line.strip()
    if strp[0] != "#":
        periods = re.split(" ", strp)
        sources.append(periods)
file.close()

prd_queues   = dict()
closest_prds = list()
for i, src in enumerate(sources):
    Q = q.Queue()
    for j, prd in enumerate(src):
        Q.put(int(prd))
        print("el: {}, qsize: {}".format(prd, Q.qsize()))
        if j == 0:
            closest_prds.append(Q.get())
    prd_queues.update({i: Q})
print("Section: {}".format(closest_prds))

cls_prd_len = len(closest_prds)
counter     = 0
break_flag  = False
out_periods = list()
while not break_flag:
    min_prd = min(closest_prds)
    out_periods.append(min_prd)
    zeroed = 0
    for i, clst_prd in enumerate(closest_prds):
        clst_prd -= min_prd
        if clst_prd == 0:
            corresp_q = prd_queues.get(i)
            if corresp_q.qsize() == 0:
                zeroed += 1
                for prd in sources[i]:
                    corresp_q.put(int(prd))
            clst_prd = corresp_q.get()
        closest_prds[i] = clst_prd
        if zeroed == cls_prd_len:
            break_flag = True
    counter += 1
print("Periods[{}]: {}".format(counter, out_periods))

if len(sys.argv) >= 3:
    file = open(sys.argv[2], "a")
    for i, t in enumerate(out_periods):
        file.write("{}".format(t))
        if i < counter-1:
            file.write(" ")
        else:
            file.write("\n")
    file.close()

plt.hist(out_periods, bins=100)
plt.title(fname)
plt.show()
