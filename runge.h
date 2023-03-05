#ifndef RUNGE_H
#define RUNGE_H

#include <cmath>
#include <fstream>
#include "lsignal.h"

class Runge
{
	public:
		const double molecule_lidar_ratio = 8.0 * 3.1415926 / 3.0;
	private:
		const int size;
		Signal *x, *signal532, *signal607;
		Signal *molecule_backscatter;
		Signal **p532p, **p532s, **p607;
		double lamda_0, lamda_R;
	public:
		Runge();
		/*This constructor takes an s of type int to represent the number of
		stroed objects such as p532p, number indicates the size of the data
		stored in the Signal object, spatial_resolution indicates the spatial
		resolution of the data stored int the Signal object. */
		Runge(const int s, int number, double spatial_resolution);
		~Runge();

		/*The elastic scattering wavelength of the Raman lidar is received
		, 355 or 532, and the corresponding nitrogen Raman wavelength is
		automatically set.*/
		void set_wavelength(const double wavelength);

		/*Two parameters of type double are received, namely the elastic scattering
		wavelength of Raman Lidar and nitrogen Raman scattering wavelength.*/
		void set_wavelength(const double wavelength_0, const double wavelength);

		/*This equation return k=f(x + h, y + k), if k = 0.0, return k=f(x, y).*/
		double function_integer(const double lidar_ratio, const int n, 
		const double aerosol_backscatter, const double k = 0.0);

		/*This equation return k = f(x + h/2, y + k/2).*/
		double function_half(const double lidar_ratio, const int n, 
		const double aerosol_backscatter, const double k);

		/*This equation receives lidar coins and n, and return overlap at the
		corresponding heigth of n.*/
		double overlap(const double lidar_ratio, const int n);

		/*This function return bascatter coefficient at the corresponding heifht of n,
		 which obtain by fernald mathod.*/
		double fernald(const double lidar_ratio, const double overlap, const int n);

		/*Receives an ifstream object, and a count n, and reads the corresponding
		data contents into an array of objects such as p532p.*/
		void read_signal(const int n, std::ifstream &sin);

		/*It is the same as the previous function, except that the data type
		it receives is the name of the file.*/
		void read_signal(const int n, const char *filename);

		/*Receive an ifstream class with the extinction coefficient of air molecules
		turned on, with the original data in km, and set it to m.*/
		void read_molecule(std::ifstream &sin);

		/*Smooth data in class such as p532p to data in signal532.*/
		void total_signal();

		/*A seven-point linear smooth and background remove background will
		be performed on all data in class such as p532p.*/
		void pretreatmen();
};
#endif
