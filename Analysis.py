import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.axes_grid1 import make_axes_locatable

# load data
RawData = np.loadtxt('output.txt',delimiter=',')
print(f"Get {RawData.shape[0]} events")

XYCoordi = RawData[:,0:2]
TDrift = RawData[:,2]

XYCoordi = XYCoordi[TDrift!=0,:]
TDrift = TDrift[TDrift!=0]

XYSigma = np.sqrt(np.sum(np.std(XYCoordi,axis=0)**2))

print(f"XY coorinate sigma: {XYSigma*1e4:.5f} um")
print(f"Average Drift time: {np.mean(TDrift):.5f} ns, sigma: {np.std(TDrift):.5f} ns")

plt.clf()
Fig,Ax = plt.subplots(figsize=(6, 6))

Ax.plot(XYCoordi[:,0],XYCoordi[:,1],'.')
Ax.set_box_aspect(1)

Ax.set_xlabel('X [cm]')
Ax.set_ylabel('Y [cm]')
# create new Axes on the right and on the top of the current Axes
divider = make_axes_locatable(Ax)
# below height and pad are in inches
ax_histx = divider.append_axes("top", 1.2, pad=0.1, sharex=Ax)
ax_histy = divider.append_axes("right", 1.2, pad=0.1, sharey=Ax)

# make some labels invisible
ax_histx.xaxis.set_tick_params(labelbottom=False)
ax_histy.yaxis.set_tick_params(labelleft=False)

# now determine nice limits by hand:
ax_histx.hist(XYCoordi[:,0], bins=24, histtype = 'step')
ax_histy.hist(XYCoordi[:,1], bins=24, orientation='horizontal', histtype = 'step')


#ax_histy.set_ylabel('Y [cm]')

plt.savefig('fig.png')
plt.close()
