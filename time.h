#ifndef TIME_H
#define TIME_H

#include <iostream>
#include <string>
#include <fstream>

struct date
{
    int year;
    int month;
    int day;
    int hours;
    int min;
};

class Time
{
    private:
        std::string path;
        std::string filename;
        int year;
        int month;
        int day;
        int hours;
        int min;
        std::ifstream sin;
    public:
        Time(const std::string &p);
        std::string get_filename();
        double get_time();
        void reset(const std::string &p);
        bool operator<(const date &t);
        bool operator==(const date &t);
        bool operator>(const date &t);
};


#endif