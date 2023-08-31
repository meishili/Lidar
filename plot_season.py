import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np
from matplotlib.ticker import MultipleLocator, AutoLocator, FixedLocator

dust_dh = np.loadtxt('build/Winter_dh_dust.txt')
dust_zy = np.loadtxt('build/Winter_zy_dust.txt')
other_dh = np.loadtxt('build/Winter_dh_other.txt')
other_zy = np.loadtxt('build/Winter_zy_other.txt')

dust_dh = np.array(dust_dh)
dust_zy = np.array(dust_zy)
other_dh = np.array(other_dh)
other_zy = np.array(other_zy)

dust_dh_distance = dust_dh[:,0] / 1000.0
dust_zy_distance = dust_zy[:,0] / 1000.0
other_dh_distance = other_dh[:,0] / 1000.0
other_zy_distance = other_zy[:,0] / 1000.0

dust_dh_ext = dust_dh[:,1] * 1000.0
dust_zy_ext = dust_zy[:,1] * 1000.0
other_dh_ext = other_dh[:,1] * 1000.0
other_zy_ext = other_zy[:,1] * 1000.0
print(dust_dh_ext)

dust_dh_eff = dust_dh[:,2]
dust_zy_eff = dust_zy[:,2]
other_dh_eff = other_dh[:,2]
other_zy_eff = other_zy[:,2]

dust_dh_vol = dust_dh[:,3]
dust_zy_vol = dust_zy[:,3]
other_dh_vol = other_dh[:,3]
other_zy_vol = other_zy[:,3]

dust_dh_dep = dust_dh[:,4]
dust_zy_dep = dust_zy[:,4]
other_dh_dep = other_dh[:,4]
other_zy_dep = other_zy[:,4]

dis = (1, 2, 3)
pp = 20
ps = 15
line_width = 3
lw = 4

fig = plt.figure(figsize=(20,7))
plt.suptitle('Lidar measurements in winter', size = 30)
plt.subplot(141)

plt.plot(dust_dh_ext, dust_dh_distance, label = "Dunhuang dust weather", c = "r",ls = '-', linewidth = lw)
# plt.plot(other_dh_ext, other_dh_distance, label = "Dunhuang other weather", c = "k", ls = '--', linewidth = lw)
plt.plot(dust_zy_ext, dust_zy_distance, label = "Zhangye dust weather", c = "b",ls = '-', linewidth = lw)
# plt.plot(other_zy_ext, other_zy_distance, label = "Zhangye other weather", c = "g", ls = '--', linewidth = lw)
ax=plt.gca()
ax.spines['bottom'].set_linewidth(line_width)
ax.spines['left'].set_linewidth(line_width)
ax.spines['right'].set_linewidth(line_width)
ax.spines['top'].set_linewidth(line_width)
plt.ylim([0.2, 3.5])
plt.xlim(0, 1.0)
plt.xticks((0, 0.2, 0.4, 0.6, 0.8, 1.0), size = pp)
plt.yticks(dis, size = pp)
plt.tick_params(axis='both', which = 'major', width = 3, length = 10)
plt.xlabel('Extinction coefficient (/km)', size = pp)
plt.ylabel('Altitude (km)', size = pp)
plt.legend(fontsize = ps)

plt.subplot(142)
plt.plot(dust_dh_eff, dust_dh_distance, label = "Dunhuang dust weather", c = "r",ls = '-', linewidth = lw)
# plt.plot(other_dh_eff, other_dh_distance, label = "Dunhuang other weather", c = "k", ls = '--', linewidth = lw)
plt.plot(dust_zy_eff, dust_zy_distance, label = "Zhangye dust weather", c = "b",ls = '-', linewidth = lw)
# plt.plot(other_zy_eff, other_zy_distance, label = "Zhangye other weather", c = "g", ls = '--', linewidth = lw)
ax=plt.gca()
ax.spines['bottom'].set_linewidth(line_width)
ax.spines['left'].set_linewidth(line_width)
ax.spines['right'].set_linewidth(line_width)
ax.spines['top'].set_linewidth(line_width)
plt.ylim([0.2, 3.5])
plt.xlim(0, 2.0)
plt.xticks((0, 0.4, 0.8, 1.2, 1.6, 2.0), size = pp)
plt.yticks(dis, size = pp)
plt.tick_params(axis='both', which = 'major', width = 3, length = 10)
plt.xlabel('Effective radius (\u03BCm)', size = pp)
plt.ylabel('Altitude (km)', size = pp)
plt.legend(fontsize = ps)

plt.subplot(143)
plt.plot(dust_dh_vol, dust_dh_distance, label = "Dunhuang dust weather", c = "r",ls = '-', linewidth = lw)
# plt.plot(other_dh_vol, other_dh_distance, label = "Dunhuang other weather", c = "k", ls = '--', linewidth = lw)
plt.plot(dust_zy_vol, dust_zy_distance, label = "Zhangye dust weather", c = "b",ls = '-', linewidth = lw)
# plt.plot(other_zy_vol, other_zy_distance, label = "Zhangye other weather", c = "g", ls = '--', linewidth = lw)
ax=plt.gca()
ax.spines['bottom'].set_linewidth(line_width)
ax.spines['left'].set_linewidth(line_width)
ax.spines['right'].set_linewidth(line_width)
ax.spines['top'].set_linewidth(line_width)
plt.ylim([0.2, 3.5])
plt.xlim(0, 2.0)
plt.xticks((0, 0.4, 0.8, 1.2, 1.6, 2.0), size = pp)
plt.yticks(dis, size = pp)
plt.tick_params(axis='both', which = 'major', width = 3, length = 10)
plt.xlabel('Volume concentration ($\u03BCm^3$/$\u03BCm^2$)', size = pp)
plt.ylabel('Altitude (km)', size = pp)
plt.legend(fontsize = ps)

plt.subplot(144)
plt.plot(dust_dh_dep, dust_dh_distance, label = "Dunhuang dust weather", c = "r",ls = '-', linewidth = lw)
# plt.plot(other_dh_dep, other_dh_distance, label = "Dunhuang other weather", c = "k", ls = '--', linewidth = lw)
plt.plot(dust_zy_dep, dust_zy_distance, label = "Zhangye dust weather", c = "b",ls = '-', linewidth = lw)
# plt.plot(other_zy_dep, other_zy_distance, label = "Zhangye other weather", c = "g", ls = '--', linewidth = lw)
ax=plt.gca()
ax.spines['bottom'].set_linewidth(line_width)
ax.spines['left'].set_linewidth(line_width)
ax.spines['right'].set_linewidth(line_width)
ax.spines['top'].set_linewidth(line_width)
plt.ylim([0.2, 3.5])
plt.xlim(0, 0.4)
plt.xticks((0, 0.1, 0.2, 0.3, 0.4), size = pp)
plt.yticks(dis, size = pp)
plt.tick_params(axis='both', which = 'major', width = 3, length = 10)
plt.xlabel('Particle depolarization ratio', size = pp)
plt.ylabel('Altitude (km)', size = pp)
plt.legend(fontsize = ps)

plt.subplots_adjust(left=0.04,bottom=0.13,right=0.99,top=0.91)
plt.savefig("winter.png")

