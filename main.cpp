#include <iostream>
#include <thread>
#include "raman.h"
#include "time.h"

const std::string city = "dh";
const std::string filepath = "/home/yukikaze/lidar/c++/microphysical/data/" + city + "/";

void work(int year, int month, int day, std::shared_ptr<double[]> overlap, std::shared_ptr<double[]> mol_ext){
    std::string dir = std::to_string(month) + "_dir.txt";
    std::string outname = std::to_string(month) + "_" + city + "_1_out.txt";
    // std::string mean_name = std::to_string(month) + "-" + city + "_mean.txt";
    std::ofstream sout;
    sout.open(outname);
    Queue queue(24);
    Raman raman;
    raman.read_molocule(mol_ext);
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
        std::string cmd = "ls " + filepath + foder + "/> " + dir;
        system(cmd.c_str());
        std::cout<<"month = "<<month<<",  "<<"day = "<<i<<std::endl;
        std::ifstream sin;
        sin.open(dir);
        std::string filename;
        while(sin>>filename){
            if(get_hours(filename) < 20 and get_hours(filename) >= 6){
                queue.set_empty();
                continue;
            }else{
                queue.push(filepath + foder + "/" + filename);
                if(queue.isfull()){
                    raman.get_signal(queue, overlap);
                    double time = get_time(filename);
                    raman.computation_coe();
                    raman.show(sout, time);
                }
            }
        }
    }
    show();
    sout.close();
}

int main(int argc, char *argv[]){
   std::shared_ptr<double[]> overlap(new double[8000]);
   std::shared_ptr<double[]> mol_ext(new double[8000]); 
   std::ifstream min;
   std::ifstream oin;
   min.open(city + "_air.txt");
   oin.open(city + "_overlap.txt");
   for(int i = 0; i != 8000; ++i){
        min>>mol_ext[i];
        mol_ext[i] /= 1000.0;
        oin>>overlap[i];    
    }
    min.close();
    oin.close();

    int month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int year[12] = {23, 23, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22};
    std::vector<std::thread> thr;
    for(int i = 0; i != 12; ++i){
        thr.push_back(std::thread(work, year[i], i + 1, month[i], overlap, mol_ext));
    }
    for(auto i = thr.begin(); i != thr.end(); ++i){
        i->join();
    }
    return 0;
}