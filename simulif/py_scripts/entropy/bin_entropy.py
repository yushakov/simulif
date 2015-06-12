# -*- coding: utf-8 -*-
"""**************************************************************************
*  bin_entropy.py                                                           *
*                                                                           *
*  Created on:   June 07, 2015                                              *
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
import entropy.bst as bst
import pylab as pl

def get_entropy(isi_file_name, draw=True):
    """
    cond_entropy, limit_entropy = get_entropy(isi_file_name, draw=True)
        Takes ISI column as input and returns conditional entropy
        and limit Shannon entropy.
        Draws plots for both, if draw=True
    """    
    isi    = pl.loadtxt(isi_file_name)
    return get_entropy_from_isi_array(isi, draw, isi_file_name)
    #return get_G_entropy_from_isi_array(isi, draw, isi_file_name)

def get_entropy_from_isi_array(isi, draw=True, file_name=""):
    T0     = 0.9*min(isi)
    
    # ISI binarization
    print("ISI binarization")
    out_bin = []
    isi_idx = 0
    Tcurr   = isi[0]
    while True:
        if T0 >= Tcurr:
            out_bin.append(1)
            isi_idx += 1
            if isi_idx >= len(isi): break
            Tcurr = isi[isi_idx] - (T0 - Tcurr)
        else:
            out_bin.append(0)
            Tcurr -= T0
    
    N    = len(out_bin)
    invN = 1.0/N
    
    # Entropy calculation
    print("Entropy calculation")
    MaxOrder = 500
    Hvals    = list()
    Hdelt    = list()
    delta    = 0.0
    del_prev = 0.0
    Hprev    = 0.0
    Intervals = (N-MaxOrder)*[0]
    for Horder in range(1, MaxOrder+1):
        tree = bst.BinarySearchTree()
        for i, intv in enumerate(Intervals):
            T = (intv << 1) | out_bin[i+Horder-1]
            Intervals[i] = T
            key = T
            value = tree.get(key)
            if value:
                tree.put(key, value+1)
            else:
                tree.put(key, 1)
        H = 0.0
        for key, val in tree.root:
            p = val * invN
            H -= p * pl.log2(p)
    
        if Horder > 1:
            delta = (H - Hprev)
            Hdelt.append(delta)
            if abs(delta - del_prev) < 1.e-5:
                break
            del_prev = delta
        Hvals.append(H)
        Hprev = H
        print("Order " + str(Horder) +" entropy: "+str(H)+", delta: "+str(delta))
        
    if draw:
        fig = pl.figure()
        fig.suptitle(file_name+", Bin interval: "+str(T0) +
        "\nH("+str(Horder)+"): "+str(H))
        sp1 = fig.add_subplot(211)
        sp1.plot([o for o in range(1, len(Hvals)+1)], Hvals)
        sp1.set_ylabel("Shannon entropy, H(N)")
        sp1.grid(True)
        
        sp2 = fig.add_subplot(212, sharex=sp1)
        sp2.plot([o for o in range(1, len(Hdelt)+1)], Hdelt)
        sp2.set_xlabel("Binary word length, N")
        sp2.set_ylabel("Conditional entropy, H(N) - H(N-1)")
        sp2.grid(True)
        pl.draw()
        pl.show()
    return delta, H
    
def get_G_entropy_from_isi_array(isi, draw=True, file_name=""):
    """
        Entropy estimator from Grossberger-2008
    """
    T0     = 0.9*min(isi)
    
    # ISI binarization
    print("ISI binarization")
    out_bin = []
    isi_idx = 0
    Tcurr   = isi[0]
    while True:
        if T0 >= Tcurr:
            out_bin.append(1)
            isi_idx += 1
            if isi_idx >= len(isi): break
            Tcurr = isi[isi_idx] - (T0 - Tcurr)
        else:
            out_bin.append(0)
            Tcurr -= T0
    
    # Entropy calculation
    print("Entropy 3 calculation")
    MaxOrder = 500
    Shift    = 5
    Hvals    = list()
    Hdelt    = list()
    delta    = 0.0
    del_prev = 0.0
    Hprev    = 0.0
    out_bin_len = len(out_bin)
    G        = update_G_until([], 2)
    max_of_max_n = 2
    for Horder in range(1, MaxOrder+1):
        tree = bst.BinarySearchTree()
        N = 0
        i = 0
        while i+Horder < out_bin_len:
            T = 0
            for j in range(Horder):
                T = (T << 1) | out_bin[i + j]
            key = T
            value = tree.get(key)
            if value:
                tree.put(key, value+1)
            else:
                tree.put(key, 1)
            N += 1
            i += min(Horder, Shift)
        H = 0.0
        n_vals = []
        for key, val in tree.root:
            n_vals.append(val)
        # Update G[] array, if necessary
        max_n = max(n_vals)
        if max_n > max_of_max_n:
            max_of_max_n = max_n
            G = update_G_until(G, max_n)
        for val in n_vals:
            H -= val * G[val]
        H /= N
        H += pl.log(N)
        H = H * pl.log2(pl.e) # to log2 format
    
        if Horder > 1:
            delta = (H - Hprev)
            Hdelt.append(delta)
            if abs(delta - del_prev) < 1.e-5:
                break
            del_prev = delta
        Hvals.append(H)
        Hprev = H
        print("Order " + str(Horder) +" entropy: "+str(H)+", delta: "+str(delta))
        
    if draw:
        fig = pl.figure()
        fig.suptitle(file_name+", Bin interval: "+str(T0) +
        "\nH("+str(Horder)+"): "+str(H))
        sp1 = fig.add_subplot(211)
        sp1.plot([o for o in range(1, len(Hvals)+1)], Hvals)
        sp1.set_ylabel("Shannon entropy, H(N)")
        sp1.grid(True)
        
        sp2 = fig.add_subplot(212, sharex=sp1)
        sp2.plot([o for o in range(1, len(Hdelt)+1)], Hdelt)
        sp2.set_xlabel("Binary word length, N")
        sp2.set_ylabel("Conditional entropy, H(N) - H(N-1)")
        sp2.grid(True)
        pl.draw()
        pl.show()
    return delta, H
    
def update_G_until(G, order):
    if order <= 3:
        gam = 0.577215
        return [0, -gam-pl.log(2.0), 2.0 - gam - pl.log(2.0), 2.0 - gam - pl.log(2.0)]
    if len(G) > order:
        return G
    last_ord  = len(G) - 1
    while last_ord < order:
        g = G[last_ord]+2.0/last_ord
        G += [g, g]
        last_ord += 2
    return G