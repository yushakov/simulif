"""**************************************************************************
*  get_C.py                                                                 *
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
""" Check how many ambiguous dots
    should be removed from the Common segment,
    if we put dots periodically with Periods."""
    
Periods = [2, 3, 5, 7]
Common  = 210

#Periods = [15, 18, 20, 24]
#Common  = 360

#Periods = [3, 4, 5]
#Common  = 60

l = []
C = 0

for i, T in enumerate(Periods):
    for t in range(T, Common, T):
        if i == 0:
            l.append(t)
        else:
            if l.__contains__(t):
                C += 1
            else:
                l.append(t)

print("C = {}".format(C))