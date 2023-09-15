#ifndef FERNALD_H
#define FERNALD_h

#include <iostream>
#include <cmath>
#include <memory>
#include <fstream>
#include "signal.h"

class Fernald{
    private:
        std::unique_ptr<double[]> signal532p;
        std::unique_ptr<double[]> signal532s;
        std::unique_ptr<double[]> signal1064;
        std::unique_ptr<double[]> signal607;
        std::unique_ptr<double[]> extinction_1064;
        std::unique_ptr<double[]> extinction_532;
        std::unique_ptr<double[]> volume_depolarization;
        std::unique_ptr<double[]> particle_depolarization;
        std::unique_ptr<double[]> color_ratio;
        std::unique_ptr<double[]> vol_conc;
        std::unique_ptr<double[]> eff_radius;
        std::unique_ptr<double[]> distance;
        int size;
    private:
        void remova_background(std::unique_ptr<double[]> &ptr);
        void fernald_1064(std::shared_ptr<double[]> overlap, std::shared_ptr<double[]> mol_ext, const double lidar_ratio);
        void fernald_532(std::shared_ptr<double[]> overlap, std::shared_ptr<double[]> mol_ext, const double lidar_ratio);
        void get_micro(const std::shared_ptr<double[]> overlap, const std::shared_ptr<double[]> mol_ext, double lidar_ratio);
        double get_aer_opt_dep(const std::unique_ptr<double[]> &ptr);
    public:
        Fernald(int s);
        void read_signal(Queue &queue);
        void coe_1064(std::shared_ptr<double[]> overlap, std::shared_ptr<double[]> mol_ext, const double lidar_ratio);
        void coe_532(std::shared_ptr<double[]> overlap, std::shared_ptr<double[]> mol_ext, const double lidar_ratio);
        void show_532(std::ofstream &sout);
        void show_1064(std::ofstream &sout);
        void show_distance(std::ofstream &sout);
        double aod_532();
        double aod_1064();

};

#endif