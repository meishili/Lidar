#ifndef MEAN_H
#define MEAN_H

#include <memory>
#include <iostream>
#include <fstream>
#include <string>

class Mean
{
private:
    std::unique_ptr<std::unique_ptr<double[]>[]> ptr;
    int count;
public:
    Mean();
    ~Mean(){}
    void add(const std::unique_ptr<double[]> &ptr1, const std::unique_ptr<double[]> &ptr2, const std::unique_ptr<double[]> &ptr3, const std::unique_ptr<double[]> &ptr4);
    void show(const std::string &filename);
    void show_count(){std::cout<<"count = "<<count<<std::endl;}
};

#endif