#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include "lidar.h"
std::string city = "zy";

std::string path = "/home/yukikaze/lidar/microphysical/data/" + city + "/";

void work(int year, int month, int day, std::shared_ptr<double[]> overlap, std::shared_ptr<double[]> molecule_extinction, std::vector<std::shared_ptr<Mean>> mean_coe, std::shared_ptr<Coe_count[]> coe_count){
    std::string dir = std::to_string(month) + "_dir.txt";
    std::string outname = std::to_string(month) + "_" + city + "_out.txt";
    std::string mean_name = std::to_string(month) + "-" + city + "_mean.txt";
    std::ofstream sout, mout;
    sout.open(outname);
    mout.open(mean_name);
    mout.precision(10);
    Lidar lidar(8000);
    for(int i = 1; i != day + 1; ++i){
        if(month == 5 and day == 19){
            continue;
        }
        if(month == 5 and day == 30){
            continue;
        }
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
            // if(!lidar.is_cloud()){
            //     lidar.show(sout);
            // }
            double p = lidar.mean_pdr();
            PDR_mean += p;
            ext_mean += lidar.mean_ext();
            vol_mean += lidar.mean_vol_con();
            eff_mean += lidar.mean_eff_rad();
            ++count;
            double CR = lidar.mean_color_ratio();
            // std::cout<<eff_mean<<std::endl;
            if(p >= 0.12){
                if(!lidar.is_cloud()){
                    lidar.mean(mean_coe[0]);
                }
                if(month == 4 or month == 5 or month == 3){
                    lidar.add_fre(coe_count);
                }
            }else{
                if(!lidar.is_cloud()){
                    lidar.mean(mean_coe[1]);
                }
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
    oin.open("../" + city + "_overlap.txt");
    min.open("../" + city + "_air.txt");
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

    std::shared_ptr<Coe_count[]> coe_count(new Coe_count[3]{Coe_count(0.2, 1.5, 0.6, 0.4), Coe_count(0.2, 1.5, 0.6, 0.4), Coe_count(0.2, 1.5, 0.6, 0.4)});

    for(int i = 0; i != 12; ++i){
        // if(i == 4 and i == 5){
        //     continue;
        // }
        // if(i ==  1 or i ==  3 or i == 6 or i == 9){
            thr.push_back(std::thread(work, year[i], i + 1, month[i], overlap, molecule_extinction, mean_coe[((i + 10) / 3) % 4], coe_count));
        // }
    }
    for(auto i = thr.begin(); i != thr.end(); ++i){
        i->join();
    }
    std::ofstream coe_out1, coe_out2, coe_out3;
    coe_out1.open("../1000_" + city + ".txt");
    coe_out2.open("../2000_" + city + ".txt");
    coe_out3.open("../3000_" + city + ".txt");
    coe_count[0].show(coe_out1);
    coe_count[1].show(coe_out2);
    coe_count[2].show(coe_out3);
    coe_out1.close();
    coe_out2.close();
    coe_out3.close();

    std::vector<std::string> season;
    season.push_back("Spring");
    season.push_back("Summer");
    season.push_back("Autumn");
    season.push_back("Winter");
    for(int i = 0; i != 4; ++i){
        mean_coe[i][0]->show(season[i] + "_" + city + "_dust.txt");
        mean_coe[i][1]->show(season[i] + "_" + city + "_other.txt");
    }
    mean_coe[0][0]->show_count();
    mean_coe[0][1]->show_count();
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::minutes>(end_time - start_time);
    std::cout<<"The run time is: "<<duration.count()<<" min!"<<std::endl;
    return 0;
}