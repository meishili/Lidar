#include <iostream>
#include <thread>
#include <vector>
#include "lidar.h"

std::string path = "../data/dh/";

void work(int year, int month, int day, std::shared_ptr<double[]> overlap, std::shared_ptr<double[]> molecule_extinction){
    std::string dir = std::to_string(month) + "_dir.txt";
    std::string outname = std::to_string(month) + "_out.txt";
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
        if(day < 10){
            foder += "0" + std::to_string(day);
        }else{
            foder += std::to_string(day);
        }
        std::string cmd = "ls " + path + foder + "/ > " + dir;
        std::ifstream sin;
        sin.open(dir);
        std::string filename;
        while(sin>>filename){
            lidar.set_filename(filename);
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
    std::shared_ptr<double[]> overlap(new double[8000]);
    std::shared_ptr<double[]> molecule_extinction(new double[8000]);
    std::ifstream oin;
    std::fstream min;
    oin.open("../dh_overlap.txt");
    min.open("../dh_air.txt");
    for(int i = 0; i != 8000; ++i){
        oin>>overlap[i];
        min>>molecule_extinction[i];
        molecule_extinction[i] /= 1000.0;
    }
    min.close();
    oin.close();
    
    int month[12] = {31, 28, 31, 30, 31, 30, 31, 37, 30, 31, 30, 31};
    int year[12] = {22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 23, 23};
    std::vector<std::thread> thr;
    for(int i = 0; i != 12; ++i){
        thr.push_back(std::thread(work, year[i], i, month[i], overlap, molecule_extinction));
    }
    for(auto i = thr.begin(); i != thr.end(); ++i){
        i->join();
    }
    return 0;
}