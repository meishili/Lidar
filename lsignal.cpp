#include "lsignal.h"

Signal::Signal() : size(0), spatial_resolution(3.75), mode(Analog)
{
	signal = nullptr;
}

Signal::Signal(const int s, const double sr, Mode m) : size(s), spatial_resolution(sr)
{
	mode = m;
	signal = new double[size];
}

Signal::Signal(const Signal &s) : size(s.size), spatial_resolution(s.spatial_resolution), mode(s.mode)
{
	signal = new double[size];
	for(int i = 0; i < size; i++)
		*(signal + i) = *(s.signal + i);
}

Signal::~Signal()
{
	delete[] signal;
}

Signal Signal::operator+(const Signal &s)
{
	Signal temp(size, spatial_resolution, mode);
	for(int i = 0; i < size; i++)
		*(temp.signal + i) = *(signal + i) + *(s.signal + i);
	return temp;
}

Signal Signal::operator+(const double *s)
{
	Signal temp(size, spatial_resolution, mode);
	for(int i = 0; i < size; i++)
		*(temp.signal + i) = *(signal + i) + *(s + i);
	return temp;
}

Signal operator+(const double *s, const Signal &si)
{
	Signal temp(si.size, si.spatial_resolution, si.mode);
	for(int i = 0; i < si.size; i++)
		*(temp.signal + i) = *(s + i) + *(si.signal + i);
	return temp;
}

Signal Signal::operator-(const Signal &s)
{
	Signal temp(size, spatial_resolution, mode);
	for(int i = 0; i < size; i++)
		*(temp.signal + i) = *(signal + i) - *(s.signal + i);
	return temp;
}

Signal Signal::operator-(const double *s)
{
	Signal temp(size, spatial_resolution, mode);
	for(int i = 0; i < size; i++)
		*(temp.signal + i) = *(signal + i) - *(s + i);
	return temp;
}

Signal operator-(const double *s, const Signal &si)
{
	Signal temp(si.size, si.spatial_resolution, si.mode);
	for(int i = 0; i < si.size; i++)
		*(temp.signal + i) = *(s + i) - *(si.signal + i);
	return temp;
}

Signal Signal::operator*(const Signal &s)
{
	Signal temp(size, spatial_resolution, mode);
	for(int i = 0; i < size; i++)
		*(temp.signal + i) = *(signal + i) + *(s.signal + i);
	return temp;
}

Signal Signal::operator*(const double *s)
{
	Signal temp(size, spatial_resolution, mode);
	for(int i = 0; i < size; i++)
		*(temp.signal + i) = *(signal + i) * *(s + i);
	return temp;
}

Signal Signal::operator*(const double s)
{
	Signal temp(size, spatial_resolution, mode);
	for(int i = 0; i < size; i++)
		*(temp.signal + i) = *(signal + i) * s;
	return temp;
}

Signal operator*(const double *s, const Signal &si)
{
	Signal temp(si.size, si.spatial_resolution, si.mode);
	for(int i = 0; i < si.size; i++)
		*(temp.signal + i) = *(s + i) * *(si.signal + i);
	return temp;
}

Signal operator*(const double s, const Signal &si)
{
	Signal temp(si.size, si.spatial_resolution, si.mode);
	for(int i = 0; i < si.size; i++)
		*(temp.signal + i) = s * *(si.signal + i);
	return temp;
}

Signal Signal::operator/(const Signal &s)
{
	Signal temp(size, spatial_resolution, mode);
	for(int i = 0; i < size; i++)
		*(temp.signal + i) = *(signal + i) / *(s.signal + i);
	return temp;
}

Signal Signal::operator/(const double *s)
{
	Signal temp(size, spatial_resolution, mode);
	for(int i = 0; i < size; i++)
		*(temp.signal + i) = *(signal + i) / *(s + i);
	return temp;
}

Signal Signal::operator/(const double s)
{
	Signal temp(size, spatial_resolution, mode);
	for(int i = 0; i < size; i++)
		*(temp.signal + i) = s * *(signal + i);
	return temp;
}

Signal operator/(const double *s, const Signal &si)
{
	Signal temp(si.size, si.spatial_resolution, si.mode);
	for(int i = 0; i < si.size; i++)
		*(temp.signal + i) = *(s + i) / *(si.signal + i);
	return temp;
}

Signal operator/(const double s, const Signal &si)
{
	Signal temp(si.size, si.spatial_resolution, si.mode);
	for(int i = 0; i < si.size; i++)
		*(temp.signal + i) = s / *(si.signal + i);
	return temp;
}

Signal log(const Signal &s)
{
	Signal temp(s.size, s.spatial_resolution, s.mode);
	for(int i = 0; i < s.size; i++)
		*(temp.signal + i) = log(*(s.signal + i));
	return temp;
}

Signal exp(const Signal &s)
{
	Signal temp(s.size, s.spatial_resolution, s.mode);
	for(int i = 0; i < s.size; i++)
		*(temp.signal + i) = exp(*(s.signal + i));
	return temp;
}

Signal pow(const Signal &s, const double x)
{
	Signal temp(s.size, s.spatial_resolution, s.mode);
	for(int i = 0; i < s.size; i++)
		*(temp.signal + i) = pow(*(s.signal + i), x);
	return temp;
}

double &Signal::operator[](int x)
{
	return *(signal + x);
}

Signal &Signal::operator=(const Signal &s)
{
	if(this == &s)
		return *this;
	delete[] signal;
	mode = s.mode;
	signal = new double[s.size];
	for(int i = 0; i < s.size; i++)
		*(signal + i) = *(s.signal + i);
	return *this;
}

void Signal::remove_background()
{
	double temp = 0.0;
	for(int i = size - 1000; i < size; i++)
		temp += *(signal + i);
	temp /= 1000.0;
	for(int i = 0; i < size; i++)
		*(signal + i) -= temp;
}

void Signal::smooth()
{
	double temp;
	for(int i = 5; i < size - 5; i++)
	{
		temp = 0.0;
		for(int j = -3; j <= 3; j++)
			temp += *(signal + i + j);
		*(signal + i) = temp / 7.0;
	}
}

void Signal::ap_transition()
{
	for(int i = 0; i < size; i++)
		*(signal + i) = -370.47693 + 376.54905 * exp(0.0000903459 * *(signal + i) / 1.0 - 0.00213 * *(signal + i));
	mode = Analog;
}
