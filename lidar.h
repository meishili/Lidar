#ifndef LIDAR_H
#define LIDAR_H

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <memory>

class Data_pre{
    private:
        std::unique_ptr<double[]> ptr;
        std::unique_ptr<double[]> distance;
        std::unique_ptr<std::unique_ptr<double[]>[]> TABC;
        std::unique_ptr<std::unique_ptr<double[]>[]> extinction_coefficient;
        int size;
        int count;
        double time;
        void get_time(const std::string filename);
    public:
        Data_pre(){size = 0; count = 0;}
        Data_pre(const int n);
        ~Data_pre(){size = 0;}
        void read(const std::string filename);
        void remove_background();
        void smooth();
        void fernald(const std::shared_ptr<double[]> overlap, const std::shared_ptr<double[]> molecule_extinction);
        void show(std::ofstream &sout);
};

#endif