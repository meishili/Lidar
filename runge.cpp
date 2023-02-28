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
}

Runge::Runge(const int s, int number, double spatial_resolution) : size(s)
{
	x = new Signal(number, spatial_resolution);
	signal532 = new Signal(number, spatial_resolution);
	signal607 = new Signal(number, spatial_resolution);
	molecule_backscatter = new Signal(number, spatial_resolution);
	p532p = new *Signal[size];
	p532s = new *Signal[size];
	p607 = new *Signal[size];
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

void Runge::read_signal(const int n, ifstream &sin)
{
	double temp;
	for(int i = 0; i < x->signal_size(); i++)
	{
		sin>>temp>>temp>>**(p607 + n)[i]>>**(p532p + n)[i]>>temp>>**(p532s + n)[i]>>temp;
	}
}

void Runge::read_molecule(ifstream &sin)
{
	for(int i = 0; i < molecule_backscatter->signal_size(), i++)
	{
		sin>>*molecule_backscatter[i];
		molecule_backscatter[i] = molecule_backscatter[i] / 1000.0 / molecule_lidar_ratio;
	}
}

void Runge::total_signal();
{
	for(int i = 0; i < x->signal_size(); i++)
	{
		double temp1 = 0.0;
		double temp2 = 0.0;
		for(int j = 0; j < size; j++)
		{
			temp1 = temp1 + **(p532p + j)[i] + **(p532s + j)[i];
			temp2 = temp2 + **(p607 + j)[i];
		}
		*signal532[i] = temp1 / double(size);
		*signal607[i] = temp2 / double(size);
		*x[i] = *signal607[i] / *signal532[i];
	}
}

void Runge::pretreatmen()
{
	for(int i = 0; i < size; i++)
	{
		*(p532p + i)->remove_background();
		*(p532p + i)->smooth();
		*(p532s + i)->remove_background();
		*(p532s + i)->smooth();
		*(p607 + i)->remove_background();
		*(p607 + i)->smooth();
	}
}
