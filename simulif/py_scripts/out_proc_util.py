"""**************************************************************************
*  out_proc_util.py                                                         *
*                                                                           *
*  Created on:   April 18, 2015                                             *
*  Author:       Yury Ushakov                                               *
*  Email:        nomah@list.ru                                              *
*                                                                           *
*                                                                           *
*  Copyright (C) 2015 by Yury Ushakov                                       *
*                                                                           *
*  This script contains routines for processing output of                   *
*  the program "simulif".                                                   *
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
import pylab as pl

def get_isi_array(filename, neuron_num, spike_threshold, min_isi=1.e-2):
    """isi, max_isi = get_isi_array(filename, neuron_num, spike_threshold)
       
        Creates an ISI array from the output of simulif.exe
        for specified neuron_num (starting from 0).
        min_isi - if only spikes present in the signal file (filename),
                  then two spikes are separated, if the time interval
                  between them is >= min_isi.
    """
    f = open(filename)
                  
    isi     = []
    prev_t  = 0
    max_isi = 0
    for line in f:
        s = line.strip()
        if s and s[0] != '#':
            fields   = line.split(',')
            x        = tuple([float(field) for field in fields])
            interval = x[0] - prev_t
            if spike_threshold < x[neuron_num+1] and interval >= min_isi:
                isi.append(interval)
                if max_isi < interval:
                    max_isi = interval
                prev_t = x[0]
    f.close()
    return isi, max_isi
    
def sampled_spike_train_from_isi(isi, dt, spike_value):
    """spike_train = sampled_spike_train_from_isi(isi, dt, spike_value)
        
        From the given ISI array, creates a regularly
        sampled digital spike train.
    """
    spike_train   = []
    resid_intv    = 0.0
    inv_dt        = 1.0 / dt
    spike_in_next = False
    for intv in isi:
        shorten_intv = intv - resid_intv
        N = int(shorten_intv * inv_dt)
        resid_intv = shorten_intv - float(N+1)*dt
        to_be_added = [0.0]*N
        if spike_in_next:
            to_be_added[0] = spike_value
        if resid_intv > 0.5*dt:
            to_be_added[N-1] = spike_value
            spike_in_next    = False
        else:
            spike_in_next = True
        
        spike_train.extend(to_be_added)
    return spike_train
    
def get_isi_distrib(fname, neuron_num, spk_thresh = 2, bin_num = 1000):
    """
    isi_len, density, isi = get_isi_distrib(fname, neuron_num, spk_thresh = 2, bin_num = 1000)
    """
    isi, max_isi = get_isi_array(fname, neuron_num, spk_thresh)
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
    return xvalues, distribution, isi
    
def save_isi(input_file, output_file, neuron_number, threshold=2, draw_hist=True):
    """
        Save Inter-Spike Intervals.
    """
    print(
    "Getting ISI array for neuron " + str(neuron_number) + 
    "\nfrom the file " + input_file)
    isi, _ = get_isi_array(input_file, neuron_number, threshold)
    print("Writing to file")
    f = open(output_file, "w")
    for i in isi:
        f.write(str(i)+"\n")
    f.close()
    
    if draw_hist:
        pl.hist(isi, bins=500, normed=True)
        pl.draw()
        pl.show()
        
def draw_isids(file_name, neurons_list, prefix_to_save_isi=None):
    """
        Draw Inter-Spike Interval distributions.
            neurons_list - is something like [3, 4, 5], where a neuron's
              number starts from 0.
            prefix_to_save_isi - is for saving ISIs to text files.
        Example:
            >>> draw_isids("output.txt", [3, 4, 5], "folder/isi_N")
            After completion there will be isi_N3.txt, isi_N4.txt,
            and isi_N5.txt in the "folder".
    """
    fig = pl.figure()
    fig.suptitle(file_name)
    SubPlots = []
    for i, n in enumerate(neurons_list):
        print("Getting distribution "+str(n)+"...")
        x, y, isi = get_isi_distrib(file_name, n)
        if i > 0:
            sp   = fig.add_subplot(len(neurons_list), 1, i+1, sharex=SubPlots[0])
        else:
            sp   = fig.add_subplot(len(neurons_list), 1, i+1)
        sp.plot(x, y)
        sp.set_ylabel("Neuron "+str(n))
        SubPlots.append(sp)
        if prefix_to_save_isi:
            print("Writing to file")
            f = open(prefix_to_save_isi+str(n)+".txt", "w")
            for i in isi:
                f.write(str(i)+"\n")
            f.close()
    pl.draw()
    pl.show()
    
def aggregate(dir_name, output_file_name="output.txt"):
    """
        Collects content of files "out_N.txt" from "dir_name" into
        one common file "output_file_name".
        N - are consequent numbers.
    """
    out_cnt = 0
    ofl = open(output_file_name,"w")
    while True:
        try:
            infl = open(dir_name + "out_" + str(out_cnt) + ".txt")
        except IOError as e:
            print("No more files. The last one was: "+"out_" + str(out_cnt-1) + ".txt")
            break
        else:
            for line in infl:
                ofl.write(line)
            infl.close()
            out_cnt += 1
    ofl.close()