#ifndef SIGNAL_H
#define SIGNAL_H

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>

class Queue{
    private:
        std::vector<std::unique_ptr<double[]>> signal532p;
        std::vector<std::unique_ptr<double[]>> signal532s;
        std::vector<std::unique_ptr<double[]>> signal1064;
        std::vector<std::unique_ptr<double[]>> signal607;
        std::unique_ptr<double[]> distance;
        int hours;
        int size;
        int count;
    public:
        Queue(int s, int n);
        ~Queue(){hours = 0; size = 0; count = 0;}
        bool isnext(const std::string filename);
        void read_signal(const std::string filename);
        void output(std::unique_ptr<double[]> &altitude, std::unique_ptr<double[]> &p532p, std::unique_ptr<double[]> &p532s, std::unique_ptr<double[]> &p1064, std::unique_ptr<double[]> &p607);
        void reset(){count = 0;}
};


#endif