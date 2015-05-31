"""**************************************************************************
*  psd.py                                                                   *
*                                                                           *
*  Created on:   April 18, 2015                                             *
*  Author:       Yury Ushakov                                               *
*  Email:        nomah@list.ru                                              *
*                                                                           *
*                                                                           *
*  Copyright (C) 2015 by Yury Ushakov                                       *
*                                                                           *
*  This script can be used to process output of the program "simulif".      *
   Use it as following:

     python psd.py output_filename column_number

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

dt = 1.e-1
spike_value = 1.0 / dt
spike_threshold = 2
filename = sys.argv[1]
colnum   = int(sys.argv[2])

from out_proc_util import *
print("ISI array construction")
isi, _ = get_isi_array(filename, colnum, spike_threshold)
print("Sampled spike train construction")
spike_train = sampled_spike_train_from_isi(isi, dt, spike_value)

# power spectrum density calculation
print("PSD construction")
idx = 0
length = pow(2, 10)
shift  = length / 2
N = len(spike_train) / shift + 1
psd = [0.0]*length
window = hamming(length)
one_by_N = 1.0 / float(N)
spk_trn_len = len(spike_train)
for iter in range(N):
    if idx + length >= spk_trn_len:
        break
    #"""
    sig = [window[i] * s for (i, s) in enumerate(spike_train[idx:idx+length])]
    f   = abs(fft(sig))
    #"""
    #f = abs(fft(spike_train[idx:idx+length]))
    psd  = [p + f[i]*one_by_N for (i, p) in enumerate(psd)]
    idx += shift

figure()
psd_half_len = len(psd)/2
freqs = [x / float(psd_half_len) for x in range(psd_half_len)]
plot(freqs, psd[0:psd_half_len])
title("Power Spectrum Density")
xlabel("Normalized frequency (rad / pi)")
show()