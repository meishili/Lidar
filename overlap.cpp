#include "overlap.h"
#include <cstring>
#include <fstream>

Overlap::Overlap()
{
	overlap = nullptr;
	error = nullptr;
	lidar_ratio = nullptr;
	raman_aerosol_backscatter = nullptr;
	fernald_aerosol_backscatter = nullptr;
	runge = nullptr;
}

Overlap::Overlap(const int overlap_size, const int n, const int size, int fre,
const double spatial_resolution, const double wavelength, const int r)
{
	count = 0;
	overlap = new Signal[overlap_size];
	error = new double[overlap_size];
	lidar_ratio = new double[overlap_size];
	raman_aerosol_backscatter = new Signal[overlap_size];
	fernald_aerosol_backscatter = new Signal[overlap_size];
	runge = new Runge(n, size, spatial_resolution);
	runge->set_wavelength(wavelength);
	reference = r;
	frequency = fre;
}

Overlap::~Overlap()
{
	delete[] overlap;
	delete[] error;
	delete[] lidar_ratio;
	delete[] raman_aerosol_backscatter;
	delete[] fernald_aerosol_backscatter;
	delete runge;
}

void Overlap::get_signal(const char *str)
{
	char *command = new char[100];
	strcpy(command, "ls ");
	strcat(command, str);
	strcat(command, ">dir.txt");
	system(command);
	delete[] command;
	std::ifstream file_in;
	file_in.open("dir.txt");
	char *filename = new char[50];
	char *path = new char[100];
	for(int i = 0; i < runge->get_size(); i++)
	{
		file_in.get(filename, 50);
		file_in.get();
		strcpy(path, str);
		strcat(path, "/");
		strcat(path, filename);
		std::ifstream signal_in;
		signal_in.open(path);
		runge->read_signal(i, signal_in);
		signal_in.close();
	}
	runge->pretreatmen();
}

bool Overlap::get_overlap()
{
	overlap_mutex.lock();
	int temp = count;
	count++;
	overlap_mutex.unlock();
	double lidar_ratio = double(temp) / double(frequency) * 100.0;
	if(count >= frequency)
		return false;
	double interal = 0.0;
	(*(overlap + temp))[reference] = 1.0;
	(*(raman_aerosol_backscatter + temp))[reference] = runge->reference_backscatter(reference);
	(*(fernald_aerosol_backscatter + temp))[reference] = runge->reference_backscatter(reference);
	*(error + temp) = 0.0;
	for(int i = reference - 1; i > 0; i--)
	{

		double k1 = runge->function_integer(lidar_ratio, i, (*(raman_aerosol_backscatter + temp))[i + 1],
			0.0);
		double k2 = runge->function_half(lidar_ratio, i, (*(raman_aerosol_backscatter + temp))[i + 1], k1);
		double k3 = runge->function_half(lidar_ratio, i, (*(raman_aerosol_backscatter + temp))[i + 1], k2);
		double k4 = runge->function_integer(lidar_ratio, i, (*(raman_aerosol_backscatter + temp))[i + 1], k3);
		(*(raman_aerosol_backscatter + temp))[i] = (*(raman_aerosol_backscatter + temp))[i + 1] - (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
		interal += (*(raman_aerosol_backscatter + temp))[i] + (*(raman_aerosol_backscatter + temp))[i + 1];
		(*(overlap + temp))[i] = runge->overlap(lidar_ratio, interal, i, reference);
		(*(fernald_aerosol_backscatter + temp))[i] = runge->fernald(lidar_ratio, (*(overlap + temp))[i],
		(*(fernald_aerosol_backscatter + temp))[i + 1], i);
	}
	for(int i = 40; i < 800; i++)
		*(error + temp) += abs((*(fernald_aerosol_backscatter + temp))[i] - (*(raman_aerosol_backscatter + temp))[i]);
}
