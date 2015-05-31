"""**************************************************************************
*  draw_distr.py                                                            *
*                                                                           *
*  Created on:   March 28, 2015                                             *
*  Author:       Yury Ushakov                                               *
*  Email:        nomah@list.ru                                              *
*                                                                           *
*                                                                           *
*  Copyright (C) 2015 by Yury Ushakov                                       *
*                                                                           *
*  This script can be used to process output of the program "simulif".      *
   Use it as following:

     python draw_distr.py output_filename column_number

   where "output_filename" is a file of the following format:

    # comment 1
    # comment 2
     0.1001000,      0.1028085,      0.1040700,     -0.0017319
     0.2002000,      0.1986785,      0.2102594,     -0.0028304
     0.3003000,      0.2816654,      0.3045088,     -0.0067139
     0.4004000,      0.3526658,      0.3946321,     -0.0104221
     0.5005000,      0.4148603,      0.4644718,     -0.0104490
     0.6006000,      0.4776937,      0.5176279,     -0.0078734
     0.7007000,      0.5265563,      0.5679808,     -0.0104112
     ...

   and "column_number" is the number of the existing column.

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
from pylab import *
import sys
from out_proc_util import *

def get_isi_distrib(fname, colnum, spk_thresh = 2, bin_num = 1000):
    """
    isi_len, density = get_isi_distrib(fname, colnum, spk_thresh = 2, bin_num = 1000)
    """
    isi, max_isi = get_isi_array(fname, colnum, spk_thresh)
    len_isi = len(isi)
    step = max_isi / (bin_num)

    # get distribution
    distribution = [0.0]*bin_num
    xvalues      = [step*xv for xv in range(bin_num)]
    inv_step = 1.0 / step
    bin_incr = 1.0 / (step * len_isi)
    for interval in isi:
        dstr_idx = int(interval * inv_step)
        if interval == max_isi:
            dstr_idx = bin_num - 1
        distribution[dstr_idx] += bin_incr
    return xvalues, distribution

if len(sys.argv) >= 3:
    filename = sys.argv[1]
    colnum   = int(sys.argv[2])
    xvalues, distribution = get_isi_distrib(filename, colnum)

    S = trapz(distribution, x=xvalues)
    print("Area under the curve: {}".format(S))

    plot(xvalues, distribution)
    title("Interspike interval density")
    xlabel("ISI length")
    ylabel("Probability density")
    show()