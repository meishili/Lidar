#include "distance.h"

Coe_fre::Coe_fre(double e, double b){
    end = e;
    begin = b;
    count = 0;
    for(auto &i : ptr){
        i = 0;
    }
}

void Coe_fre::add(double coe){
    double step = (end - begin) / 20.0;
    int n = (coe - begin) / step;
    if(n >= 20){
        n = 20;
    }
    if(n <= 0){
        n = 0;
    }
    ptr[n]++;
    ++count;
}

double Coe_fre::operator[](int n){
    return ptr[n] / double(count);
}

Coe_count::Coe_count(double ext_end, double eff_end, double vol_end, double PDR_end, double ext_begin, double eff_begin, double vol_begin, double PDR_begin){
    ptr.push_back(Coe_fre(ext_end, ext_begin));
    ptr.push_back(Coe_fre(eff_end, eff_begin));
    ptr.push_back(Coe_fre(vol_end, vol_begin));
    ptr.push_back(Coe_fre(PDR_end, PDR_begin));
}

void Coe_count::add(double ext, double eff, double vol, double PDR){
    ptr[0].add(ext);
    ptr[1].add(eff);
    ptr[2].add(vol);
    ptr[3].add(PDR);
}

void Coe_count::show(std::ofstream &sout){
    for(int i = 0; i != 20; ++i){
        sout<<i<<"  "<<ptr[0][i] * 100.0<<"  "<<ptr[1][i] * 100.0<<"  "<<ptr[2][i] * 100.0<<"  "<<ptr[3][i] * 100.0<<std::endl;
    }
}