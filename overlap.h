#ifndef OVERLAP_H
#define OVERLAP_H

#include <iostream>
#include "lsignal.h"
#include "runge.h"
#include <thread>
#include <mutex>
#include <cmath>

class Overlap
{
	private:
		Signal *overlap;
		double *error;
		double *lidar_ratio;
		Signal *raman_aerosol_backscatter;
		Signal *fernald_aerosol_backscatter;
		Runge *runge;
		int count;
		int reference;
		std::mutex overlap_mutex;
		int frequency;
	public:
		Overlap();
		Overlap(const int overlap_size, const int n, const int size, int fre,
		const double spatial_resolution, const double wavelength, const int r);
		virtual ~Overlap();
		void get_signal(const char *str);
		bool get_overlap();
};

#endif
