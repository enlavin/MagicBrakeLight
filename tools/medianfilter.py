import matplotlib.pyplot as plt
import pandas as pd
import sys


acc = pd.read_csv(sys.argv[1], sep='\t', index_col=0)
acc['X_filtered'] = pd.rolling_median(acc['X'], 3)
acc['freno'] = (acc['X'] - acc['X_filtered']) < -60

plt.figure(1)
sensorplot = plt.subplot(311)
plt.plot(
    acc.index / 1000, acc['X'],
    # acc.index / 1000, acc['Y'],
    # acc.index / 1000, acc['Z'],
    linewidth=2)

plt.subplot(312, sharex=sensorplot)
plt.plot(acc.index / 1000, acc['X'] - acc['X_filtered'], linewidth=2, color='#00ff00')

plt.subplot(313, sharex=sensorplot)
plt.plot(acc.index / 1000, acc['freno'], linewidth=2, color='#ff0000')

plt.show()
