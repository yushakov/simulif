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