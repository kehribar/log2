# -----------------------------------------------------------------------------
#
#
# -----------------------------------------------------------------------------
import sys, os
import numpy as np
import matplotlib.pyplot as plt

# Ref: https://stackoverflow.com/a/8964779
def iter_loadtxt(filename, delimiter=',', skiprows=0, dtype=float):
  
  # ...
  def iter_func():
    with open(filename, 'r') as infile:
      for _ in range(skiprows):
        next(infile)
      for line in infile:
        line = line.rstrip().split(delimiter)
        for item in line:
          yield dtype(item)
    iter_loadtxt.rowlength = len(line)

  # ...
  data = np.fromiter(iter_func(), dtype=dtype)
  data = data.reshape((-1, iter_loadtxt.rowlength))
  return data

# ...
dat = iter_loadtxt(sys.argv[1])

# ...
x = dat[:,0]
log2_fast = dat[:,1]
log2_fast_interp = dat[:,2]
log2_ref = dat[:,3]

# ...
plt.figure()
plt.subplot(2,1,1)
plt.plot(x, log2_ref)
plt.plot(x, log2_fast, "r")
plt.plot(x, log2_fast_interp, "k")
plt.subplot(2,1,2)
plt.plot(log2_ref - log2_fast)
plt.plot(log2_ref - log2_fast_interp, "k")
plt.show()
