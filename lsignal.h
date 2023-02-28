#ifndef LSIGNAL_H
#define LSIGNAL_H

#include <iostream>
#include <cmath>
using namespace std;

class Signal
{
	public:
		enum Mode {Analog, Photon};
	private:
		double *signal;
		const int size;
		const double spatial_resolution;
		Mode mode;
	public:
		Signal();
		Signal(const int s = 8000; const double SR = 3.75);
		Signal(const Signal &s);
		~Signal();
		Signal &operator+(const Signal &s);
		Signal &operator+(const double *s);
		friend Signal &operator+(const double *s, const Signal &si);
		Signal &operator-(const Signal &s);
		Signal &operator-(const double *s);
		friend Signal &operator-(const double *s, const Signal &si);
		Signal &operator*(const Signal &s);
		Signal &operator*(const double *s);
		Signal &operator*(double s);
		friend Signal &operator*(const double *s, const Signal &si);
		friend Signal &operator*(double s, const Signal &si);
		Signal &operator/(const Signal &s);
		Signal &operator/(const double *s);
		Signal &operator/(double s);
		friend Signal &operator/(const double *s, const Signal &si);
		friend Signal &operator/(double s, const Signal &si);
		friend Signal &log(const Signal &s);
		friend Signal &exp(const Signal &s);
		friend Signal &pow(const Signal &s, double x);
		double &operator[](double x) const;
		Signal &operator=(const Signal &s);
		void remove_background();
		void smooth();
		void ap_transition();
};

#endif
