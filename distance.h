#ifndef DISTANCE_H
#define DISTANCE_H
#include <memory>
#include <iostream>
#include <fstream>
#include <vector>

class Coe_fre{
    private:
        double end;
        double begin;
        int ptr[21];
        int count;
    public:
        Coe_fre(double e, double b = 0.0);
        void add(double coe);
        double operator[](const int n);
};

class Coe_count{
    private:
        std::vector<Coe_fre> ptr;
    public:
        Coe_count(double ext_end, double eff_end, double vol_end, double PDR_end, double ext_begin = 0.0, double eff_begin = 0.0, double vol_begin = 0.0, double PDR_begin = 0.0);
        void add(double ext, double eff, double vol, double PDR);
        void show(std::ofstream &sout);
};


#endif