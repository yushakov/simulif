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
def get_isi_array(filename, colnum, spike_threshold):
    """isi, max_isi = get_isi_array(filename, colnum, spike_threshold)
       
        Creates an ISI array from the output of simulif.exe
        at specified column.
    """
    f = open(filename)
                  
    isi     = []
    prev_t  = 0
    idx     = 0
    max_isi = 0
    print("ISI array construction")
    for line in f:
        s = line.strip()
        if s and s[0] != '#':
            fields = line.split(',')
            x      = tuple([float(field) for field in fields])
            if idx > 0 and xprev < spike_threshold and x[colnum] > spike_threshold:
                interval = x[0] - prev_t
                isi.append(interval)
                if max_isi < interval:
                    max_isi = interval
                prev_t = x[0]
                max_time = x[0]
            else:
                xprev = x[colnum]
            idx += 1
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