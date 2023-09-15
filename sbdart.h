#ifndef SBDART_H
#define SBDART_H

#include "fernald.h"
#include <iostream>
#include <fstream>
#include <string>

class Sbdart{
    private:
        std::string time;       //时间
        std::string alat;       //纬度
        std::string alon;       //经度
        double albcon;          //地表反照率
        int iaer;               //5 or 0， 0表示无气溶胶
        double wlbaer;          //波长
        double tbaer;           //光学厚度
        double wbaer;           //SSA
        double gbaer;           //不对称因子
        int iout;               //输出模式
    private:
        void out_head(std::ofstream &sout);
        void out_end(std::ofstream &sout);
        void out_aer(std::ofstream &sout, Fernald &fernald);
    public:
        Sbdart(){iout = 11;};
        void set_time(const std::string t){time = t;};
        void set_position(const std::string longitude, const std::string latitude){alat = latitude; alon = longitude;}
        void set_albcon(double a){albcon = a;}
        void set_iaer(int i){iaer = i;}
        void set_wlbaer(double w){wlbaer = w;}
        void set_tbaer(double t){tbaer = t;}
        void set_wbaer(double w){wbaer = w;}
        void set_gbaer(double g){gbaer = g;}
        void output_aer(const std::string filename, Fernald &fernald);
        void output_clear(const std::string filename);
        void set_iout(const int i){iout = i;}

};

#endif