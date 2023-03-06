#include "runge.h"

Runge::Runge() : size(0)
{
	x = nullptr;
	signal532 = nullptr;
	signal607 = nullptr;
	molecule_backscatter = nullptr;
	p532p = nullptr;
	p532s = nullptr;
	p607 = nullptr;
	Range_corrected_signal = nullptr;
}

Runge::Runge(const int s, int number, double spatial_resolution) : size(s)
{
	x = new Signal(number, spatial_resolution);
	signal532 = new Signal(number, spatial_resolution);
	signal607 = new Signal(number, spatial_resolution);
	Range_corrected_signal = new Signal(number, spatial_resolution);
	molecule_backscatter = new Signal(number, spatial_resolution);
	p532p = new Signal*[size];
	p532s = new Signal*[size];
	p607 = new Signal*[size];
	for(int i = 0; i < size; i++)
	{
		p532p[i] = new Signal(number, spatial_resolution);
		p532s[i] = new Signal(number, spatial_resolution);
		p607[i]  = new Signal(number, spatial_resolution);
	}
}

Runge::~Runge()
{
	delete x;
	delete signal532;
	delete signal607;
	for(int i = 0; i < size; i++)
	{
		delete p532p[i];
		delete p532s[i];
		delete p607[i];
	}
	delete[] p532p;
	delete[] p532s;
	delete[] p607;
}

void Runge::set_wavelength(const double wavelength)
{
	if(abs(wavelength - 532.0) < 0.0001)
	{
		lamda_0 = 532.0;
		lamda_R = 607.0;
	}
	else
	{
		lamda_0 = 355.0;
		lamda_R = 387.0;
	}
}

void Runge::set_wavelength(const double wavelength_0, const double wavelength)
{
	lamda_0 = wavelength_0;
	lamda_R = wavelength;
}

void Runge::read_signal(const int n, std::ifstream &sin)
{
	double temp;
	for(int i = 0; i < x->signal_size(); i++)
	{
		sin>>temp>>temp>>(**(p607 + n))[i]>>(**(p532p + n))[i];
		sin>>temp>>(**(p532s + n))[i];
		sin>>temp;
	}
}

void Runge::read_molecule(std::ifstream &sin)
{
	for(int i = 0; i < molecule_backscatter->signal_size(); i++)
	{
		sin>>(*molecule_backscatter)[i];
		molecule_backscatter[i] = molecule_backscatter[i] / 1000.0 / molecule_lidar_ratio;
	}
}

void Runge::total_signal()
{
	for(int i = 0; i < x->signal_size(); i++)
	{
		double temp1 = 0.0;
		double temp2 = 0.0;
		for(int j = 0; j < size; j++)
		{
			temp1 = temp1 + (**(p532p + j))[i] + (**(p532s + j))[i];
			temp2 = temp2 + (**(p607 + j))[i];
		}
		(*signal532)[i] = temp1 / double(size);
		(*signal607)[i] = temp2 / double(size);
		(*x)[i] = (*signal607)[i] / (*signal532)[i];
	}
	spatial_resolution = x->get_spatial_resolution();
	(*Range_corrected_signal) = spatial_resolution * spatial_resolution * (*signal532);
}

void Runge::pretreatmen()
{
	for(int i = 0; i < size; i++)
	{
		(*(p532p + i))->remove_background();
		(*(p532p + i))->smooth();
		(*(p532s + i))->remove_background();
		(*(p532s + i))->smooth();
		(*(p607 + i))->remove_background();
		(*(p607 + i))->smooth();
	}
}

double Runge::function_integer(const double lidar_ratio, const int n, 
const double aerosol_backscatter, const double k)
{
	if(abs(k - 0.0) < pow(10,-10))
	{
		double temp1 = molecule_backscatter->get_diff(n - 3, n + 3);
		double temp2 = x->get_diff(n - 3, n + 3);
		double temp3 = temp1 / (*molecule_backscatter)[n] + (1.0 - pow(lamda_0, 4) 
		/ pow(lamda_R, 4)) * (*molecule_backscatter)[n] * molecule_lidar_ratio +
		 (1.0 - lamda_0 / lamda_R) * aerosol_backscatter * lidar_ratio - temp2 / (*x)[n];
		double temp4 = (*molecule_backscatter)[n] + aerosol_backscatter;
		return (temp3 * temp4 - temp1);
	}
	else
	{
		double temp1 = molecule_backscatter->get_diff(n - 4, n + 2);
		double temp2 = x->get_diff(n - 4, n + 2);
		double temp3 = temp1 / (*molecule_backscatter)[n - 1] + (1.0 - pow(lamda_0, 4) 
		/ pow(lamda_R, 4)) * (*molecule_backscatter)[n - 1] * molecule_lidar_ratio + 
		(1.0 - lamda_0 / lamda_R) * ((aerosol_backscatter - k) * lidar_ratio 
		* spatial_resolution) - temp2 / (*x)[n - 1];
		double temp4 = (*molecule_backscatter)[n - 1] + (aerosol_backscatter - k * spatial_resolution);
		return (temp3 * temp4 - temp1);
	}
}


double Runge::function_half(const double lidar_ratio, const int n, 
const double aerosol_backscatter, const double k)
{
	double temp1 = molecule_backscatter->get_diff(n - 3, n + 2);
	double temp2 = x->get_diff(n - 3, n + 2);
	double temp3 = temp1 / ((*molecule_backscatter)[n - 1] + (*molecule_backscatter)[n]) * 2.0
	+ (1.0 - pow(lamda_0, 4) / pow(lamda_R, 4)) * ((*molecule_backscatter)[n - 1] + (*molecule_backscatter)[n])
	/ 2.0 * molecule_lidar_ratio + (1.0 - lamda_0 / lamda_R) * (aerosol_backscatter - k * 
	spatial_resolution / 2.0) * lidar_ratio - temp2 / ((*x)[n - 1] + (*x)[n]) * 2.0;
	double temp4 = ((*molecule_backscatter)[n - 1] + (*molecule_backscatter)[n]) / 2.0 +
	aerosol_backscatter - k * spatial_resolution / 2.0;
	return (temp3 * temp4 - temp1);
}

double Runge::overlap(const double lidar_ratio, double &interal, const int n,
const int reference)
{
	double overlap;
	overlap = (*signal607)[n] * spatial_resolution * double(n) * spatial_resolution * double(n)
	* (*molecule_backscatter)[reference] / (*signal607)[reference] / spatial_resolution /
	spatial_resolution / double(reference) / double(reference) / (*molecule_backscatter)[n];
	interal += (1.0 + pow(lamda_0 / lamda_R, 4)) * ((*molecule_backscatter)[n] +
	(*molecule_backscatter)[n + 1]);
	overlap *= exp(- (interal * spatial_resolution));
	return overlap;
}

double Runge::fernald(const double lidar_ratio, const double overlap, 
const double aerosol_backscatter, const int n)
{
	(*Range_corrected_signal)[n] /= overlap;
	double temp1 = (*Range_corrected_signal)[n] * exp((lidar_ratio - molecule_lidar_ratio) *
	((*molecule_backscatter)[n] + (*molecule_backscatter)[n + 1]) * spatial_resolution);
	double temp2 = (*Range_corrected_signal)[n + 1] / (aerosol_backscatter + (*molecule_backscatter)[n + 1] /
	molecule_lidar_ratio);
	double temp3 = lidar_ratio * ((*Range_corrected_signal)[n + 1] + temp1) * spatial_resolution;
	double aerosol = temp1 / (temp2 + temp3) - (*molecule_backscatter)[n] / molecule_lidar_ratio;
	return aerosol;
}
