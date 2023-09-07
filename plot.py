import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np
from matplotlib.ticker import MultipleLocator, AutoLocator, FixedLocator

ti = (14.0, 15.0, 16.0, 17.0, 18.0)
tex = ('14', '15', '16', '17', '18')
distance = (1, 2, 3, 4, 5)

scatter_size = 50
xray = "Local Time"
ps = 20

data = np.loadtxt("../microphysical/build/4_dh_out.txt")
data = np.array(data)
x = data[:, 0]
y = data[:, 1]
print(y)
TABC = data[:, 2]
PDR = data[:, 4]
CR = data[:, 5]
extinction = data[:, 3]
vol = data[:, 6]
Re = data[:, 7]


fig = plt.figure(figsize = (30,15))
plt.subplot(211)
plt.title('Lidar measurements in Dunhuang (40.1$^\circ$N,94.6$^\circ$E)\n2022-04',size=35)
plt.xlim([14, 19])
plt.ylim([0.2,5])
ax=plt.gca()
ax.spines['bottom'].set_linewidth(3)
ax.spines['left'].set_linewidth(3)
ax.spines['right'].set_linewidth(3)
ax.spines['top'].set_linewidth(3)
xminor=MultipleLocator(1.0 / 4.0)
xmajor_locator = MultipleLocator(0.25)
ax.xaxis.set_major_locator(xmajor_locator)
plt.xticks([])
ax.xaxis.set_minor_locator(xminor)
ax.tick_params(axis='x',labelbottom=False)
plt.yticks(distance)
plt.tick_params(which="major",width = 3,length=10)
plt.tick_params(which="minor",width = 3,length=5)
plt.text(14.2,4,'Atten. backs. coef. (532nm)',size=45,color='r')
plt.scatter(x,y,c = TABC, vmin = 0, vmax = 0.003, s = scatter_size, cmap = plt.get_cmap("jet"))
cb=plt.colorbar()
cb.ax.set_title('(/sr/km)',size=30, pad = 15)
cb.ax.tick_params(labeltop = True, labelsize = ps)
plt.tick_params(labelsize=30)
plt.ylabel('Height (km)',size=35)

plt.subplot(212)
plt.xlim([14, 19])
plt.ylim([0.2, 5])
ax=plt.gca()
ax.spines['bottom'].set_linewidth(3)
ax.spines['left'].set_linewidth(3)
ax.spines['right'].set_linewidth(3)
ax.spines['top'].set_linewidth(3)
xminor=MultipleLocator(1.0 / 4.0)
xmajor_locator = MultipleLocator(0.25)
ax.xaxis.set_major_locator(xmajor_locator)
ax.xaxis.set_minor_locator(xminor)
ax.set_xticks(ti)
ax.set_xticklabels(tex)
plt.yticks(distance)
plt.tick_params(which="major",width = 3,length=10)
plt.tick_params(which="minor",width = 3,length=5)
plt.text(14.2,4,'Volume depolarization ratio (532nm)',size=45,color='r')
plt.scatter(x,y,c = PDR, vmin = 0, vmax = 0.5, s = scatter_size, cmap = plt.get_cmap("jet"))
cb=plt.colorbar()
plt.xlabel(xray,size=35)
cb.ax.tick_params(labeltop = True, labelsize = ps)
plt.tick_params(labelsize=30)
plt.ylabel('Height (km)',size=35)
plt.subplots_adjust(hspace = 0.05)
plt.subplots_adjust(left=0.05,bottom=0.085,right=1.1,top=0.9)
plt.savefig("dh_04.png")


data = np.loadtxt("../microphysical/build/4_zy_out.txt")
data = np.array(data)
x = data[:, 0]
y = data[:, 1]
print(y)
TABC = data[:, 2]
PDR = data[:, 4]
CR = data[:, 5]
extinction = data[:, 3]
vol = data[:, 6]
Re = data[:, 7]


fig = plt.figure(figsize = (30,15))
plt.subplot(211)
plt.title('Lidar measurements in Zhangye (38.9$^\circ$N,100.5$^\circ$E)\n2022-04',size=35)
plt.xlim([14, 19])
plt.ylim([0.2,5])
ax=plt.gca()
ax.spines['bottom'].set_linewidth(3)
ax.spines['left'].set_linewidth(3)
ax.spines['right'].set_linewidth(3)
ax.spines['top'].set_linewidth(3)
xminor=MultipleLocator(1.0 / 4.0)
xmajor_locator = MultipleLocator(0.25)
ax.xaxis.set_major_locator(xmajor_locator)
plt.xticks([])
ax.xaxis.set_minor_locator(xminor)
ax.tick_params(axis='x',labelbottom=False)
plt.yticks(distance)
plt.tick_params(which="major",width = 3,length=10)
plt.tick_params(which="minor",width = 3,length=5)
plt.text(14.2,4,'Atten. backs. coef. (532nm)',size=45,color='r')
plt.scatter(x,y,c = TABC, vmin = 0, vmax = 0.003, s = scatter_size, cmap = plt.get_cmap("jet"))
cb=plt.colorbar()
cb.ax.set_title('(/sr/km)',size=30, pad = 15)
cb.ax.tick_params(labeltop = True, labelsize = ps)
plt.tick_params(labelsize=30)
plt.ylabel('Height (km)',size=35)

plt.subplot(212)
plt.xlim([14, 19])
plt.ylim([0.2, 5])
ax=plt.gca()
ax.spines['bottom'].set_linewidth(3)
ax.spines['left'].set_linewidth(3)
ax.spines['right'].set_linewidth(3)
ax.spines['top'].set_linewidth(3)
xminor=MultipleLocator(1.0 / 4.0)
xmajor_locator = MultipleLocator(0.25)
ax.xaxis.set_major_locator(xmajor_locator)
ax.xaxis.set_minor_locator(xminor)
ax.set_xticks(ti)
ax.set_xticklabels(tex)
plt.yticks(distance)
plt.tick_params(which="major",width = 3,length=10)
plt.tick_params(which="minor",width = 3,length=5)
plt.text(14.2,4,'Volume depolarization ratio (532nm)',size=45,color='r')
plt.scatter(x,y,c = PDR, vmin = 0, vmax = 0.5, s = scatter_size, cmap = plt.get_cmap("jet"))
cb=plt.colorbar()
plt.xlabel(xray,size=35)
cb.ax.tick_params(labeltop = True, labelsize = ps)
plt.tick_params(labelsize=30)
plt.ylabel('Height (km)',size=35)
plt.subplots_adjust(hspace = 0.05)
plt.subplots_adjust(left=0.05,bottom=0.085,right=1.1,top=0.9)
plt.savefig("zy_04.png")