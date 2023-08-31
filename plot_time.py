import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np
from matplotlib.ticker import MultipleLocator, AutoLocator, FixedLocator

data = np.loadtxt("time_3_dh_out.txt")
data = np.array(data)
y = data[:,0]
extinction1 = data[:,1]
ref1 = data[:,2]
vol1 = data[:,3]
dep1 = data[:,4]
extinction2 = data[:,5]
ref2 = data[:,6]
vol2 = data[:,7]
dep2 = data[:,8]
dis = (1, 2, 3, 4)
ps = 15

line_width = 5
lw = 4

night_time = "11:00-12:00"
day_time = "23:00-24:00"

fig = plt.figure(figsize=(20,7))
plt.subplot(141)
plt.plot(extinction1, y, label = night_time, c = "r", ls = "-", linewidth = lw)
plt.plot(extinction2, y, label = day_time, c = "b", ls = "-", linewidth = lw)
ax=plt.gca()
ax.spines['bottom'].set_linewidth(line_width)
ax.spines['left'].set_linewidth(line_width)
ax.spines['right'].set_linewidth(line_width)
ax.spines['top'].set_linewidth(line_width)
plt.xlim([0, 1.0])
plt.ylim(0.2, 4.5)
plt.xticks((0, 0.2, 0.4, 0.6, 0.8, 1.0), size = ps)
plt.yticks(dis, size = ps)
plt.ylabel('Altitude (km)', size = ps)
plt.xlabel('Extinction coefficient (/km)', size = ps)
plt.legend(fontsize = ps)

plt.subplot(142)
plt.plot(ref1, y, label = night_time, c = "r", ls = "-", linewidth = lw)
plt.plot(ref2, y, label = day_time, c = "b", ls = "-", linewidth = lw)
ax=plt.gca()
ax.spines['bottom'].set_linewidth(line_width)
ax.spines['left'].set_linewidth(line_width)
ax.spines['right'].set_linewidth(line_width)
ax.spines['top'].set_linewidth(line_width)
plt.xticks((0, 0.5, 1.0, 1.5, 2.0), size = ps)
plt.xlim([0, 2.0])
plt.ylim(0.2, 4.5)
plt.yticks(dis, size = ps)
plt.ylabel('Altitude (km)', size = ps)
plt.xlabel('Effective radius (\u03BCm)', size = ps)
plt.legend(fontsize = ps)

plt.subplot(143)
plt.plot(vol1, y, label = night_time, c = "r", ls = "-", linewidth = lw)
plt.plot(vol2, y, label = day_time, c = "b", ls = "-", linewidth = lw)
ax=plt.gca()
ax.spines['bottom'].set_linewidth(line_width)
ax.spines['left'].set_linewidth(line_width)
ax.spines['right'].set_linewidth(line_width)
ax.spines['top'].set_linewidth(line_width)
plt.xticks((0, 0.2, 0.4, 0.6, 0.8, 1.0), size = ps)
plt.xlim([0, 1.0])
plt.ylim(0.2, 4.5)
plt.yticks(dis, size = ps)
plt.ylabel('Altitude (km)', size = ps)
plt.xlabel('Volume concentration ($\u03BCm^3$/$\u03BCm^2$)', size = ps)
plt.legend(fontsize = ps)

plt.subplot(144)
plt.plot(dep1, y, label = night_time, c = "r", ls = "-", linewidth = lw)
plt.plot(dep2, y, label = day_time, c = "b", ls = "-", linewidth = lw)
ax=plt.gca()
ax.spines['bottom'].set_linewidth(line_width)
ax.spines['left'].set_linewidth(line_width)
ax.spines['right'].set_linewidth(line_width)
ax.spines['top'].set_linewidth(line_width)
plt.xticks((0, 0.1, 0.2, 0.3, 0.4), size = ps)
plt.xlim([0, 0.4])
plt.ylim(0.2, 4.5)
plt.yticks(dis, size = ps)
plt.ylabel('Altitude (km)', size = ps)
plt.xlabel('Particle depolarization ratio', size = ps)
plt.legend(fontsize = ps)
plt.subplots_adjust(left=0.03,bottom=0.090,right=0.99,top=0.98)
plt.savefig("time_dh_03.png")