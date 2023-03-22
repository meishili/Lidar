#ifndef LIDAR_H
#define LIDAR_H

#include <iostream>
#include <fstream>
#include <cmath>

const double molecule_lidar_ratio = 8.0 * 3.1415926 / 3.0;
const double depolarization_correct = 1.1;
const double color_ratio_correct = 0.3;
const double molecule_depolarization_ratio = 0.00376;

class Lidar
{
    private:
        const int size;
        double *p532p;
        double *p532s;
        double *p1064;
        double *p607;
        double *distance;
        double *overlap;
        double *molecule;
        double *extinction;
    public:
        Lidar();
        Lidar(const int n);
        ~Lidar();
        void read_overlap(const char *s);
        void read_molecule(const char *s);
        void photon_to_analog();
        void remove_background();
        void fernald();
        void read_signal(const char *s);
        void show(std::ofstream &sout);
};


#endif