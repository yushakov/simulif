import os
# IC      mu    sqrtD   rst   spk  thr  fnm     A       Omg    Phi
tool_str = "..\\Release\\simulif.exe"

Time = 4000
Skip_points = 1000

launch_line = """
{0} -t {1} -s {2} \
-n 0.0 1.0    4.0 0.0 3.0 1.0 cos 1.084  0.4 0.0 \
-n 0.0 1.0    4.0 0.0 3.0 1.0 cos 1.1393 0.53333333333333333333 0.0 \
-n -1. 0.3665 4.0 -1. 3.0 1.0 \
   -k 0.98 0 \
   -k 0.98 1
""".format(tool_str, Time, Skip_points)
os.system(launch_line)
os.system("mv output.txt output4by3.txt")

launch_line = """
{0} -t {1} -s {2} \
-n 0.0 1.0    4.0 0.0 3.0 1.0 cos 1.084  0.4 0.0 \
-n 0.0 1.0    4.0 0.0 3.0 1.0 cos 1.1241 0.5 0.0 \
-n -1. 0.3665 4.0 -1. 3.0 1.0 \
   -k 0.98 0 \
   -k 0.98 1
""".format(tool_str, Time, Skip_points)
os.system(launch_line)
os.system("mv output.txt output5by4.txt")
