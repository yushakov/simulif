"""**************************************************************************
*  proc_noiseless.py                                                                 *
*                                                                           *
*  Created on:   April 18, 2015                                             *
*  Author:       Yury Ushakov                                               *
*  Email:        nomah@list.ru                                              *
*                                                                           *
*                                                                           *
*  Copyright (C) 2015 by Yury Ushakov                                       *
*                                                                           *
*                                                                           *
*  This script is free software: you can redistribute it and/or modify      *
*  it under the terms of the GNU General Public License as published by     *
*  the Free Software Foundation, either version 3 of the License, or        *
*  (at your option) any later version.                                      *
*                                                                           *
*  This program is distributed in the hope that it will be useful,          *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
*  GNU General Public License for more details.                             *
*                                                                           *
*  You should have received a copy of the GNU General Public License        *
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
*                                                                           *
**************************************************************************"""
"""
    Requires two parameters:
        input filename,
        output filename (optional parameter).
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
astr = " "
fname    = sys.argv[1]
file     = open(fname)
sources  = list()
src_lens = list()
src_idxs = list()
closest_prds = list()
for line in file:
    strp = line.strip()
    if strp[0] != "#":
        periods = re.split(" ", strp)
        ilst = [int(ix) for ix in periods]
        sources.append(ilst)
        src_lens.append(len(ilst))
        src_idxs.append(1)
        closest_prds.append(ilst[0])
file.close()

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
            if src_idxs[i] >= src_lens[i]:
                zeroed += 1
                src_idxs[i] = 0
            clst_prd  = sources[i][src_idxs[i]]
            src_idxs[i] += 1
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
plt.title("{} {}".format(fname, astr))
plt.show()
