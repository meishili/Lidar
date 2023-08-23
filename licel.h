#ifndef LICEL_H
#define LICEL_H
#include <string>
#include <fstream>
#include <memory>
#include <iostream>
#include <cmath>

class Licel{
    public:
        enum Mode {Analog, Photon};
        struct head{
            Mode mode;
            double shot_num;
            double ana_inputrange;
            double adc_num;
            double distinction;
        };
    private:
        double time;
        int size;
        int day;
        int hours;
        int min;
        std::ifstream fin;
        std::string filename;
        std::unique_ptr<head[]> head_info;
        std::unique_ptr<std::unique_ptr<int[]>[]> ptr;
        void read_line(std::string &str);
        void analog(std::unique_ptr<double[]> &p, const int n);
        void photon(std::unique_ptr<double[]> &p, const int n);
    public:
        Licel(const int s);
        ~Licel();
        void set_filename(const std::string &str);
        bool open_file();
        bool close_file();
        double get_time();
        bool read_head();
        void save_txt(const std::string &str);
        void read_dataset();
        void get_dataset(std::unique_ptr<std::unique_ptr<double[]>[]> &p, int n = 0);
};

#endif