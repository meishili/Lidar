#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include "lidar.h"

std::string path = "/home/yukikaze/lidar/c++/microphysical/data/zy/";

void work(int year, int month, int day, std::shared_ptr<double[]> overlap, std::shared_ptr<double[]> molecule_extinction, std::vector<std::shared_ptr<Mean>> mean_coe){
    std::string dir = std::to_string(month) + "_dir.txt";
    std::string outname = std::to_string(month) + "_zy_out.txt";
    std::string mean_name = std::to_string(month) + "_zy_mean.txt";
    std::ofstream sout, mout;
    sout.open(outname);
    mout.open(mean_name);
    mout.precision(10);
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
        double PDR_mean = 0.0, ext_mean = 0.0, vol_mean = 0.0, eff_mean = 0.0;
        int count = 0;
        while(sin>>filename){
            lidar.set_filename(path + foder + "/" + filename);
            lidar.read();
            lidar.remove_background();
            lidar.fernald(overlap, molecule_extinction);
            lidar.microphysical();
            lidar.show(sout);
            PDR_mean += lidar.mean_pdr();
            double p = PDR_mean;
            ext_mean += lidar.mean_ext();
            vol_mean += lidar.mean_vol_con();
            eff_mean += lidar.mean_eff_rad();
            ++count;
            // std::cout<<eff_mean<<std::endl;
            if(p >= 0.15){
                lidar.mean(mean_coe[0]);
            }else{
                lidar.mean(mean_coe[1]);
            }
            // return;
        }
        mout<<i<<"  "<<PDR_mean / double(count)<<"  "<<ext_mean / double(count)<<"  "<<vol_mean / double(count)<<"  "<<eff_mean / double(count)<<std::endl;
        sin.close();
    }
    sout.close();
    mout.close();
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

    std::vector<std::vector<std::shared_ptr<Mean>>> mean_coe(4);
    for(int i = 0; i != 4; ++i){
        mean_coe[i].push_back(std::make_shared<Mean>());
        mean_coe[i].push_back(std::make_shared<Mean>());
    }

    for(int i = 0; i != 12; ++i){
        thr.push_back(std::thread(work, year[i], i + 1, month[i], overlap, molecule_extinction, mean_coe[((i + 10) / 3) % 4]));
    }
    for(auto i = thr.begin(); i != thr.end(); ++i){
        i->join();
    }

    std::vector<std::string> season;
    season.push_back("Spring");
    season.push_back("Summer");
    season.push_back("Autumn");
    season.push_back("Winter");
    for(int i = 0; i != 4; ++i){
        mean_coe[i][0]->show(season[i] + "_zy_dust.txt");
        mean_coe[i][1]->show(season[i] + "_zy_other.txt");
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::minutes>(end_time - start_time);
    std::cout<<"The run time is: "<<duration.count()<<" min!"<<std::endl;
    return 0;
}