import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np
from matplotlib.ticker import MultipleLocator, AutoLocator, FixedLocator

dust_spring = np.loadtxt('build/Spring_zy_dust.txt')
dust_summer = np.loadtxt('build/Summer_zy_dust.txt')
dust_autumn = np.loadtxt('build/Autumn_zy_dust.txt')
dust_winter = np.loadtxt('build/Winter_zy_dust.txt')

other_spring = np.loadtxt('build/Spring_zy_other.txt')
other_summer = np.loadtxt('build/Summer_zy_other.txt')
other_autumn = np.loadtxt('build/Autumn_zy_other.txt')
other_winter = np.loadtxt('build/Winter_zy_other.txt')

dust_spring = np.array(dust_spring)
dust_summer = np.array(dust_summer)
dust_autumn = np.array(dust_autumn)
dust_winter = np.array(dust_winter)

other_spring = np.array(other_spring)
other_summer = np.array(other_summer)
other_autumn = np.array(other_autumn)
other_winter = np.array(other_winter)

dust_spring_distance = dust_spring[:,0] / 1000.0
dust_summer_distance = dust_summer[:,0] / 1000.0
dust_autumn_distance = dust_autumn[:,0] / 1000.0
dust_winter_distance = dust_winter[:,0] / 1000.0

other_spring_distance = other_spring[:,0] / 1000.0
other_summer_distance = other_summer[:,0] / 1000.0
other_autumn_distance = other_autumn[:,0] / 1000.0
other_winter_distance = other_winter[:,0] / 1000.0

dust_spring_ext = dust_spring[:,3]
dust_summer_ext = dust_summer[:,3]
dust_autumn_ext = dust_autumn[:,3]
dust_winter_ext = dust_winter[:,3]

other_spring_ext = other_spring[:,3]
other_summer_ext = other_summer[:,3]
other_autumn_ext = other_autumn[:,3]
other_winter_ext = other_winter[:,3]


dis = (1, 2, 3)
pp = 20
ps = 15
line_width = 3
lw = 4

fig = plt.figure(figsize=(20,10))
plt.suptitle('Volume concentration in Zhangye', size = 30)
plt.subplot(141)
plt.title('Spring', size= 20)
plt.plot(dust_spring_ext, dust_spring_distance, label = "Dust", c = "g",ls = '-', linewidth = lw)
plt.plot(other_spring_ext, other_spring_distance, label = "None dust", c = "g",ls = '--', linewidth = lw)
ax=plt.gca()
ax.spines['bottom'].set_linewidth(line_width)
ax.spines['left'].set_linewidth(line_width)
ax.spines['right'].set_linewidth(line_width)
ax.spines['top'].set_linewidth(line_width)
plt.ylim([0.2, 3])
plt.xlim(0, 0.6)
plt.xticks((0, 0.2, 0.4, 0.6), size = pp)
plt.yticks(dis, size = pp)
plt.tick_params(axis='both', which = 'major', width = 3, length = 10)
plt.xlabel('Volume concentration ($\u03BCm^3$/$\u03BCm^2$)', size = pp)
plt.ylabel('Altitude (km)', size = pp)
plt.legend(fontsize = ps)

plt.subplot(142)
plt.title('Summer', size= 20)
plt.plot(dust_summer_ext, dust_summer_distance, label = "Dust", c = "r",ls = '-', linewidth = lw)
plt.plot(other_summer_ext, other_summer_distance, label = "None dust", c = "r",ls = '--', linewidth = lw)
ax=plt.gca()
ax.spines['bottom'].set_linewidth(line_width)
ax.spines['left'].set_linewidth(line_width)
ax.spines['right'].set_linewidth(line_width)
ax.spines['top'].set_linewidth(line_width)
plt.ylim([0.2, 3])
plt.xlim(0, 0.6)
plt.xticks((0, 0.2, 0.4, 0.6), size = pp)
plt.yticks(dis, size = pp)
plt.tick_params(axis='both', which = 'major', width = 3, length = 10)
plt.xlabel('Volume concentration ($\u03BCm^3$/$\u03BCm^2$)', size = pp)
plt.ylabel('Altitude (km)', size = pp)
plt.legend(fontsize = ps)

plt.subplot(143)
plt.title('Autumn', size= 20)
plt.plot(dust_autumn_ext, dust_autumn_distance, label = "Dust", c = "y",ls = '-', linewidth = lw)
plt.plot(other_autumn_ext, other_autumn_distance, label = "None dust", c = "y",ls = '--', linewidth = lw)
ax=plt.gca()
ax.spines['bottom'].set_linewidth(line_width)
ax.spines['left'].set_linewidth(line_width)
ax.spines['right'].set_linewidth(line_width)
ax.spines['top'].set_linewidth(line_width)
plt.ylim([0.2, 3])
plt.xlim(0, 0.6)
plt.xticks((0, 0.2, 0.4, 0.6), size = pp)
plt.yticks(dis, size = pp)
plt.tick_params(axis='both', which = 'major', width = 3, length = 10)
plt.xlabel('Volume concentration ($\u03BCm^3$/$\u03BCm^2$)', size = pp)
plt.ylabel('Altitude (km)', size = pp)
plt.legend(fontsize = ps)

plt.subplot(144)
plt.title('Winter', size= 20)
plt.plot(dust_winter_ext, dust_winter_distance, label = "Dust", c = "k",ls = '-', linewidth = lw)
plt.plot(other_winter_ext, other_winter_distance, label = "None dust", c = "k",ls = '--', linewidth = lw)
ax=plt.gca()
ax.spines['bottom'].set_linewidth(line_width)
ax.spines['left'].set_linewidth(line_width)
ax.spines['right'].set_linewidth(line_width)
ax.spines['top'].set_linewidth(line_width)
plt.ylim([0.2, 3])
plt.xlim(0, 0.6)
plt.xticks((0, 0.2, 0.4, 0.6), size = pp)
plt.yticks(dis, size = pp)
plt.tick_params(axis='both', which = 'major', width = 3, length = 10)
plt.xlabel('Volume concentration ($\u03BCm^3$/$\u03BCm^2$)', size = pp)
plt.ylabel('Altitude (km)', size = pp)
plt.legend(fontsize = ps)

plt.subplots_adjust(left=0.04,bottom=0.13,right=0.98,top=0.9)
plt.savefig("zy_vol.png")

