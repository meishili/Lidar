import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np
from matplotlib.ticker import MultipleLocator, AutoLocator, FixedLocator

data_dh = np.loadtxt("dh_fre.txt")
data_zy = np.loadtxt("zy_fre.txt")
data_dh = np.array(data_dh)
data_zy = np.array(data_zy)

month = data_dh[:,0]
ext_dh = data_dh[:,1]
eff_dh = data_dh[:,3]
vol_dh = data_dh[:,5]
PDR_dh = data_dh[:,7]

ext_zy = data_zy[:,1]
eff_zy = data_zy[:,3]
vol_zy = data_zy[:,5]
PDR_zy = data_zy[:,7]

x_title = ('Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec', 'Jan', 'Feb')
line_width = 5
ps = 30
bar_width = 0.3


fig = plt.figure(figsize=(30,25))
plt.subplot(411)
plt.bar(month - (bar_width / 2.0), ext_dh, width= bar_width, label = 'Dunhuang', align='center')
plt.bar(month + (bar_width / 2.0), ext_zy, width= bar_width, label = 'Zhangye', align='center')
ax=plt.gca()
ax.spines['bottom'].set_linewidth(line_width)
ax.spines['left'].set_linewidth(line_width)
ax.spines['right'].set_linewidth(line_width)
ax.spines['top'].set_linewidth(line_width)
plt.xlim([-0.5, 11.5])
plt.ylim(0, 0.15)
plt.yticks((0, 0.05, 0.1, 0.15), size = ps)
plt.xticks(month, size = ps)
plt.tick_params(axis='both', which = 'major', width = 5, length = 10, labelbottom = False)
plt.ylabel('Extinction coefficient (/km)', size = ps)
plt.legend(fontsize = ps)

plt.subplot(412)
plt.bar(month - (bar_width / 2.0), eff_dh, width= bar_width, label = 'Dunhuang', align='center')
plt.bar(month + (bar_width / 2.0), eff_zy, width= bar_width, label = 'Zhangye', align='center')
ax=plt.gca()
ax.spines['bottom'].set_linewidth(line_width)
ax.spines['left'].set_linewidth(line_width)
ax.spines['right'].set_linewidth(line_width)
ax.spines['top'].set_linewidth(line_width)
plt.xlim([-0.5, 11.5])
plt.ylim(0, 1.2)
plt.xticks(month, size = ps)
plt.tick_params(axis='both', which = 'major', width = 5, length = 10, labelbottom = False)
plt.yticks((0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2), size = ps)
plt.ylabel('Effective radius (\u03BCm)', size = ps)
plt.legend(fontsize = ps)

plt.subplot(413)
plt.bar(month - (bar_width / 2.0), vol_dh, width= bar_width, label = 'Dunhuang', align='center')
plt.bar(month + (bar_width / 2.0), vol_zy, width= bar_width, label = 'Zhangye', align='center')
ax=plt.gca()
ax.spines['bottom'].set_linewidth(line_width)
ax.spines['left'].set_linewidth(line_width)
ax.spines['right'].set_linewidth(line_width)
ax.spines['top'].set_linewidth(line_width)
plt.xlim([-0.5, 11.5])
plt.ylim(0, 0.5)
plt.xticks(month, size = ps)
plt.tick_params(axis='both', which = 'major', width = 5, length = 10, labelbottom = False)
plt.yticks((0, 0.1, 0.2, 0.3, 0.4, 0.5), size = ps)
plt.ylabel('Volume concentration\n ($\u03BCm^3$/$\u03BCm^2$)', size = ps)
plt.legend(fontsize = ps)

plt.subplot(414)
plt.bar(month - (bar_width / 2.0), PDR_dh, width= bar_width, label = 'Dunhuang', align='center')
plt.bar(month + (bar_width / 2.0), PDR_zy, width= bar_width, label = 'Zhangye', align='center')
ax=plt.gca()
ax.spines['bottom'].set_linewidth(line_width)
ax.spines['left'].set_linewidth(line_width)
ax.spines['right'].set_linewidth(line_width)
ax.spines['top'].set_linewidth(line_width)
plt.xlim([-0.5, 11.5])
plt.ylim(0, 0.2)
plt.xticks(month, x_title, size = ps)
plt.xticks(rotation = 45)
plt.yticks((0, 0.05, 0.1, 0.15, 0.2), size = ps)
plt.tick_params(axis='both', which = 'major', width = 5, length = 10, labelbottom = True)
plt.ylabel('Particle depolarization\n ratio', size = ps)
plt.legend(fontsize = ps)

plt.subplots_adjust( hspace = 0.1)
plt.subplots_adjust(left=0.08,bottom=0.10,right=0.99,top=0.97)
plt.savefig("month.png")