#include <iostream>
#include <thread>
#include <memory>
#include <vector>
#include "lidar.h"

void work(std::shared_ptr<double[]> overlap, std::shared_ptr<double[]> molecule_extinction, std::vector<std::vector<std::string>> filename, int n, int day){
    Data_pre data(8000);
    std::string foder = filename[day][0].substr(4, 3);
    std::ofstream sout;
    sout.open(foder + ".txt");
    for(int i = 0; i != n; ++i){
        data.read("../licel/" + filename[day][i]);
        data.remove_background();
        data.fernald(overlap, molecule_extinction);
        data.show(sout);
    }
}

int main(int argc, char *argv[]){
    std::shared_ptr<double[]> overlap(new double[8000]);
    std::shared_ptr<double[]> molecule_extinction(new double[8000]);
    std::ifstream oin;
    oin.open("../overlap.txt");
    std::ifstream min;
    min.open("../air.txt");
    for(int i = 0; i != 8000; ++i){
        oin>>overlap[i];
        min>>molecule_extinction[i];
        molecule_extinction[i] /= 1000.0;
    }
    min.close();
    oin.close();

    std::vector<std::thread> thr;
    std::vector<std::vector<std::string>> filename(11, std::vector<std::string>(300));
    std::vector<int> count_f(11);
    std::ifstream fin;
    std::string month = "813";
    std::string licel_file;
    int count = 0;
    fin.open("dir.txt");
    int i = 0;
    while(fin>>licel_file){
        if(licel_file.substr(4, 3) != month){
            month = licel_file.substr(4, 3);
            count++;
            i = 0;
            if(count >= 11){
            break;
            }
        }
        filename[count][i] = licel_file;
        i++;
        count_f[count]++;
    }
    fin.close();
    for(int i = 0; i != 11; ++i){
        thr.push_back(std::thread(work, overlap, molecule_extinction, filename, count_f[i], i));
    }
    for(auto i = thr.begin(); i != thr.end(); ++i){
        i->join();
    }
    return 0;
}