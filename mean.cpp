#include "mean.h"

Mean::Mean(){
    ptr = std::make_unique<std::unique_ptr<double[]>[]>(4);
    for(int i = 0; i != 4; ++i){
        ptr[i] = std::make_unique<double[]>(8000);
        for(int j = 0; j != 8000; ++j){
            ptr[i][j] = 0.0;
        }
    }
    count = 0;
}

void Mean::add(const std::unique_ptr<double[]> &ptr1, const std::unique_ptr<double[]> &ptr2, const std::unique_ptr<double[]> &ptr3, const std::unique_ptr<double[]> &ptr4){
    for(int i = 30; i != 3000; ++i){
        ptr[0][i] += ptr1[i];
        ptr[1][i] += ptr2[i];
        ptr[2][i] += ptr3[i];
        ptr[3][i] += ptr4[i];
    }
    ++count;
}

void Mean::show(const std::string &filename){
    std::ofstream sout;
    sout.open(filename);
    sout.precision(10);
    for(int i = 30; i != 3000; ++i){
        sout<<3.75 * double(i)<<"  ";
        for(int j = 0; j != 4; ++j){
            sout<<ptr[j][i] / double(count)<<"  ";
        }
        sout<<std::endl;
    }
    sout.close();
}