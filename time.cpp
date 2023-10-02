#include "time.h"

int get_hours(const std::string &str){
    int lenth = str.length() - 16;
    int hours = std::stoi(str.substr(6, 2));
    return hours;
}

double get_time(const std::string &str){
    int lenth = str.length() - 16;
    int hours = std::stoi(str.substr(6, 2));
    int day = std::stoi(str.substr(4, 2));
    int min = std::stoi(str.substr(8, 2));
    double time = double(day) + double(hours) / 24.0 + double(min) / 24.0 / 60.0;
    return time;
}