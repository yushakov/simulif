rem IC      mu    sqrtD   rst   spk  thr  fnm     A       Omg    Phi

set Time=20000
..\Release\simulif.exe -t %Time% -s 1000 ^
-n 0.0 1.0    4.0 0.0 3.0 1.0 -f cos 1.084  0.4 0.0 ^
-n 0.0 1.0    4.0 0.0 3.0 1.0 -f cos 1.1393 0.53333333333333333333 0.0 ^
-n -1. 0.3665 4.0 -1. 3.0 1.0 ^
   -k 0.98 0 0.0 ^
   -k 0.98 1 0.0
mv output.txt output4by3.txt

..\Release\simulif.exe -t %Time% -s 1000 ^
-n 0.0 1.0    4.0 0.0 3.0 1.0 -f cos 1.084  0.4 0.0 ^
-n 0.0 1.0    4.0 0.0 3.0 1.0 -f cos 1.1241 0.5 0.0 ^
-n -1. 0.3665 4.0 -1. 3.0 1.0 ^
   -k 0.98 0 ^
   -k 0.98 1
mv output.txt output5by4.txt
