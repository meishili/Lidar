#ifndef RUNGE_H
#define RUNGE_H

#include <fstream>
#include "lsignal.h"

class Runge
{
	private:
		const int size;
		Signal *x, *signal532, *signal607;
		Signal *molecule_backscatter;
		Signal **p532p, **p532s, **p607;
		double lamda_0, lamda_R;
	public:
		Runge();
		Runge(const int s, int number, double spatial_resolution);
		~Runge();
		void set_wavelength(const double wavelength);
		void set_wavelength(const double wavelength_0, const double wavelength);
		double function_integer(const double lidar_ratio, const double k, const int n);
		double function_half(const double lidar_ratio, const double k, const int n);
		double overlap(const double lidar_ratio, const int n);
		double fernald(const double lidar_ratio, const double overlap, const int n);
		void read_signal(const int n, ifstream &sin);
		void read_molecule(ifstream &sin);
		void total_signal();
		void pretreatmen();
};
#endif
