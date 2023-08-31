import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np
from matplotlib.ticker import MultipleLocator, AutoLocator, FixedLocator

dh = np.loadtxt('dh_mean.txt')
zy = np.loadtxt('zy_mean.txt')

year_zy = zy[:,0]
year_dh = dh[:,0]
PDR_dh = dh[:,1]
ext_dh = dh[:,2] * 1000.0
vol_dh = dh[:,3]
eff_dh = dh[:,4]

PDR_zy = zy[:,1]
ext_zy = zy[:,2] * 1000.0
vol_zy = zy[:,3]
eff_zy = zy[:,4]

time = (1, 32, 62, 93, 123, 154, 185, 215, 246, 276, 307, 338)
time_label = ('2022/03/01', '2022/04/01', '2022/05/01', '2022/06/01', '2022/07/01', '2022/08/01', '2022/09/01', '2022/010/01', '2022/11/01', '2022/12/01', '2023/01/01', '2023/02/01')
ps = 30
pp = 10
line_width = 7

fig = plt.figure(figsize=(30,25))
plt.subplot(411)
plt.plot(year_dh, ext_dh, label = "Dunhuang", c = "r", marker = 'o',markersize = pp, ls = ' ')
plt.plot(year_zy, ext_zy, label = "Zhangye", c = "b",  marker = 'o',markersize = pp, ls = ' ')
ax=plt.gca()
ax.spines['bottom'].set_linewidth(line_width)
ax.spines['left'].set_linewidth(line_width)
ax.spines['right'].set_linewidth(line_width)
ax.spines['top'].set_linewidth(line_width)
plt.ylim([0, 0.2])
plt.xlim(1, 366)
plt.yticks((0, 0.05, 0.1, 0.15, 0.2), size = ps)
plt.xticks(time)
plt.tick_params(axis='both', which = 'major', width = 5, length = 10, labelbottom = False)
# ax.set_xticks(ti)
# ax.set_xticklabels(tex)
plt.ylabel('Extinction coefficient (/km)', size = ps)
plt.legend(fontsize = ps)

plt.subplot(412)
plt.plot(year_dh, eff_dh, label = "Dunhuang", c = "r", marker = 'o',markersize = pp, ls = ' ')
plt.plot(year_zy, eff_zy, label = "Zhangye", c = "b",  marker = 'o',markersize = pp, ls = ' ')
ax=plt.gca()
ax.spines['bottom'].set_linewidth(line_width)
ax.spines['left'].set_linewidth(line_width)
ax.spines['right'].set_linewidth(line_width)
ax.spines['top'].set_linewidth(line_width)
plt.ylim([0, 2.0])
plt.xlim(1, 366)
plt.yticks((0, 0.5, 1.0, 1.5, 2.0), size = ps)
plt.xticks(time)
plt.tick_params(axis='both', which = 'major', width = 5, length = 10, labelbottom = False)
# plt.xticks((0, 0.2, 0.4, 0.6, 0.8))
# ax.set_xticks(ti)
# ax.set_xticklabels(tex)
plt.ylabel('Effective radius (\u03BCm)', size = ps)
plt.legend(fontsize = ps)

plt.subplot(413)
plt.plot(year_dh, vol_dh, label = "Dunhuang", c = "r", marker = 'o',markersize = pp, ls = ' ')
plt.plot(year_zy, vol_zy, label = "Zhangye", c = "b",  marker = 'o',markersize = pp, ls = ' ')
ax=plt.gca()
ax.spines['bottom'].set_linewidth(line_width)
ax.spines['left'].set_linewidth(line_width)
ax.spines['right'].set_linewidth(line_width)
ax.spines['top'].set_linewidth(line_width)
plt.ylim([0, 1.0])
plt.xlim(1, 366)
# plt.xticks([])
plt.yticks((0, 0.2, 0.4, 0.6, 0.8, 1.0), size = ps)
plt.xticks(time)
plt.tick_params(axis='both', which = 'major', width = 5, length = 10, labelbottom = False)
# plt.xticks((0, 0.2, 0.4, 0.6, 0.8))
# ax.set_xticks(ti)
# ax.set_xticklabels(tex)
plt.ylabel('Volume concentration\n ($\u03BCm^3$/$\u03BCm^2$)', size = ps)
plt.legend(fontsize = ps)

plt.subplot(414)
plt.plot(year_dh, PDR_dh, label = "Dunhuang", c = "r", marker = 'o',markersize = pp, ls = ' ')
plt.plot(year_zy, PDR_zy, label = "Zhangye", c = "b",  marker = 'o',markersize = pp, ls = ' ')
ax=plt.gca()
ax.spines['bottom'].set_linewidth(line_width)
ax.spines['left'].set_linewidth(line_width)
ax.spines['right'].set_linewidth(line_width)
ax.spines['top'].set_linewidth(line_width)
plt.ylim([0, 0.4])
plt.xlim(1, 366)
# plt.xticks([])
plt.yticks((0, 0.1, 0.2, 0.3, 0.4), size = ps)
plt.xticks(time)
plt.tick_params(axis='both', which = 'major', width = 5, length = 10)
# plt.xticks((0, 0.2, 0.4, 0.6, 0.8))
plt.xticks(time, time_label, size = ps)
plt.xticks(rotation = 45)
plt.ylabel('Particle depolarization\n ratio', size = ps)
plt.legend(fontsize = ps)

plt.subplots_adjust( hspace = 0.08)
plt.subplots_adjust(left=0.07,bottom=0.10,right=0.99,top=0.97)
plt.savefig("year.png")
