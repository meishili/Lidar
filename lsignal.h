#ifndef LSIGNAL_H
#define LSIGNAL_H

#include <iostream>
#include <cmath>

class Signal
{
	public:
		enum Mode {Analog, Photon};
	private:
		double *signal;
		int size;
		double spatial_resolution;
		Mode mode;
	public:
		Signal();
		Signal(const int s, const double sr, Mode m = Analog);
		Signal(const Signal &s);
		~Signal();
		Signal operator+(const Signal &s);
		Signal operator+(const double *s);
		friend Signal operator+(const double *s, const Signal &si);
		Signal operator-(const Signal &s);
		Signal operator-(const double *s);
		friend Signal operator-(const double *s, const Signal &si);
		Signal operator*(const Signal &s);
		Signal operator*(const double *s);
		Signal operator*(const double s);
		friend Signal operator*(const double *s, const Signal &si);
		friend Signal operator*(const double s, const Signal &si);
		Signal operator/(const Signal &s);
		Signal operator/(const double *s);
		Signal operator/(const double s);
		friend Signal operator/(const double *s, const Signal &si);
		friend Signal operator/(const double s, const Signal &si);
		friend Signal log(const Signal &s);
		friend Signal exp(const Signal &s);
		friend Signal pow(const Signal &s, const double x);
		double &operator[](int x);
		Signal &operator=(const Signal &s);
		void remove_background();
		void smooth();
		void ap_transition();
		int signal_size() const {return size;}
		double get_diff(const int n1, const int n2) const;
		double get_spatial_resolution() const {return spatial_resolution;}
};

#endif
