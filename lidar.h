#ifndef LIDAR_H
#define LIDAR_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <memory>

constexpr double molecule_lidar_ratio = 8.0 * 3.1415926 / 3.0;
constexpr double lidar_ratio = 50.0;
constexpr double color_correct = 0.12378;
constexpr double molecule_depolarization = 0.00376;

class Lidar{
    private:
        std::unique_ptr<double[]> signal532p;
        std::unique_ptr<double[]> signal532s;
        std::unique_ptr<double[]> signal1064;
        std::unique_ptr<double[]> signal532;
        std::unique_ptr<double[]> TABC;
        std::unique_ptr<double[]> extinction;
        std::unique_ptr<double[]> PDR;
        std::unique_ptr<double[]> CR;
        std::unique_ptr<double[]> volume_con;
        std::unique_ptr<double[]> eff_rad;
        std::unique_ptr<double[]> distance;
        std::string filename;
        const int size;
    private:
        double get_time();
        int get_day();
        int get_hours();
    public:
        Lidar() : size(0){}
        Lidar(int n);
        void set_filename(std::string f){filename = f;}
        void read();
        void remove_background();
        void fernald(const std::shared_ptr<double[]> overlap, const std::shared_ptr<double[]> molecule_extinction);
        void microphysical();
        void show(std::ofstream &sout);

};

#endif