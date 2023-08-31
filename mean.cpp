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
    for(int i = 30; i != 800; ++i){
        if(ptr1[i] > 10000.00 and ptr1[i] < -100000.0)
            return;
         if(ptr2[i] > 10000.00 and ptr2[i] < -100000.0)
            return;
         if(ptr3[i] > 10000.00 and ptr3[i] < -100000.0)
            return;
         if(ptr4[i] > 10000.00 and ptr4[i] < -100000.0)
            return;
    }
    for(int i = 30; i != 800; ++i){
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
    double sum;
    for(int i = 32; i != 799; ++i){
        sout<<3.75 * double(i)<<"  ";
        for(int j = 0; j != 4; ++j){
            sum = 0.0;
            for(int k = -2; k != 3; ++k){
                sum += ptr[j][i + k];
            }
            sum /= 5.0;
            sout<<sum / double(count)<<"  ";
        }
        sout<<std::endl;
    }
    sout.close();
}

void Mean::show_count(){
    std::cout<<"count = "<<count<<std::endl;
    for(int i = 30; i != 300; ++i){
        std::cout<<3.75 * double(i)<<"  ";
        for(int j = 0; j != 4; ++j){
            std::cout<<ptr[j][i]<<"  ";
        }
        std::cout<<std::endl;
    }
}