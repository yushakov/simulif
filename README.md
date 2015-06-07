# simulif
Simulation of Leaky Integrate-and-Fire neurons network.
Do configuration in a launching script for simulif.exe. Get trajectories at the "output.txt" file.

# Processing
1. Modify setup.py (from py_scripts) so that to reflect correct path to the "py_script" folder.
2. Copy setup.py to some place, where you want to process your "output.txt".
3. From the console, enter the folder containing "setup.py".
4. From the Python console (hereafter, ">>>" means the Python's console execution):
   >>> from setup import *

5. Draw ISIDs and save ISI series:
   >>> opu.draw_isids("folder1/output.txt", [3, 5, 7], "folder2/isiN")
   (be careful, it will not ask for permission of "isiN*.txt" overwriting :)

6. Get entropies and plots for their convergence:
   >>> get_entropy("folder2/isiN5.txt")
