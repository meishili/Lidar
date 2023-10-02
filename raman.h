#ifndef RAMAN_H
#define RAMAN_H
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>
#include <cmath>

void show();

class Queue{
    private:
        std::vector<std::unique_ptr<double[]>> signal532p;
        std::vector<std::unique_ptr<double[]>> signal532s;
        std::vector<std::unique_ptr<double[]>> signal1064;
        std::vector<std::unique_ptr<double[]>> signal607;
        int size;
        int begin;
        int end;
    private:
        void remove_background(std::unique_ptr<double[]> &ptr);
    public:
        Queue();
        Queue(int n);
        ~Queue();
        void push(const std::string filename);
        void pop(std::unique_ptr<double[]> &p532p, std::unique_ptr<double[]> &p532s, std::unique_ptr<double[]> &p1064, std::unique_ptr<double[]> &p607, const std::shared_ptr<double[]> overlap);
        bool isfull();
        bool isempty();
        void set_empty();
};

class Raman{
    private:
        std::unique_ptr<double[]> signal532p;
        std::unique_ptr<double[]> signal532s;
        std::unique_ptr<double[]> signal1064;
        std::unique_ptr<double[]> signal607;
        std::unique_ptr<double[]> signal532;
        std::unique_ptr<double[]> extinction;
        std::unique_ptr<double[]> backscatter;
        std::unique_ptr<double[]> lidar_ratio;
        std::unique_ptr<double[]> molocule_extinction;
        std::unique_ptr<double[]> color_ratio;
        std::unique_ptr<double[]> depolarization_ratio;
        std::unique_ptr<double[]> volume_depolarization_ratio;
        std::unique_ptr<double[]> vol_conc;
        std::unique_ptr<double[]> eff_radius;
        std::unique_ptr<double[]> distance;
    private:
        std::unique_ptr<double[]> reduce_resolution(const std::unique_ptr<double[]> &ptr);
        void get_extinction();
        void get_backscatter();
        void get_lidar_ratio();
        void get_micro();
        double slope(const std::unique_ptr<double[]> &x, const std::unique_ptr<double[]> &y, int n1, int n2);
        void smooth(std::unique_ptr<double[]> &ptr);
    public:
        Raman();
        void read_molocule(const std::shared_ptr<double[]> molo_ext);
        void get_signal(Queue &queue, const std::shared_ptr<double[]> overlap);
        void computation_coe();
        void show(std::ofstream &sout, const double time);
};


#endif