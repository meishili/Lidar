#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include "lidar.h"

std::string path = "/home/yukikaze/lidar/c++/microphysical/data/zy/";

void work(int year, int month, int day, std::shared_ptr<double[]> overlap, std::shared_ptr<double[]> molecule_extinction){
    std::string dir = std::to_string(month) + "_dir.txt";
    std::string outname = std::to_string(month) + "_zy_out.txt";
    std::ofstream sout;
    sout.open(outname);
    Lidar lidar(8000);
    for(int i = 1; i != day + 1; ++i){
        std::string foder = "lz" + std::to_string(year);
        if(month < 10){
            foder += "0" + std::to_string(month);
        }else{
            foder += std::to_string(month);
        }
        if(i < 10){
            foder += "0" + std::to_string(i);
        }else{
            foder += std::to_string(i);
        }
        std::string cmd = "ls " + path + foder + "/> " + dir;
        system(cmd.c_str());
        std::cout<<"month = "<<month<<",  "<<"day = "<<i<<std::endl;
        std::ifstream sin;
        sin.open(dir);
        std::string filename;
        while(sin>>filename){
            lidar.set_filename(path + foder + "/" + filename);
            lidar.read();
            lidar.remove_background();
            lidar.fernald(overlap, molecule_extinction);
            lidar.microphysical();
            lidar.show(sout);
        }
        sin.close();
    }
    sout.close();
}

int main(int argc, char *argv[]){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::shared_ptr<double[]> overlap(new double[8000]);
    std::shared_ptr<double[]> molecule_extinction(new double[8000]);
    std::ifstream oin;
    std::fstream min;
    oin.open("../zy_overlap.txt");
    min.open("../zy_air.txt");
    for(int i = 0; i != 8000; ++i){
        oin>>overlap[i];
        min>>molecule_extinction[i];
        molecule_extinction[i] /= 1000.0;
    }
    min.close();
    oin.close();

    int month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int year[12] = {23, 23, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22};
    std::vector<std::thread> thr;
    for(int i = 0; i != 12; ++i){
        thr.push_back(std::thread(work, year[i], i + 1, month[i], overlap, molecule_extinction));
    }
    for(auto i = thr.begin(); i != thr.end(); ++i){
        i->join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::minutes>(end_time - start_time);
    std::cout<<"The run time is: "<<duration.count()<<" min!"<<std::endl;
    return 0;
}