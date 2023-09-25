#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>
#include <thread>

std::string path = "build/";

void work(const std::string &filename, int day){
    double dis, ext_coe, Re, Nc, PDR;
    std::vector<std::unique_ptr<double[]>> distance;
    std::vector<std::unique_ptr<double[]>> extinction;
    std::vector<std::unique_ptr<double[]>> ref_radius;
    std::vector<std::unique_ptr<double[]>> vol_conc;
    std::vector<std::unique_ptr<double[]>> depolarization;
    int count1 = 0, count2 = 0;
    for(int i = 0; i != 2; ++i){
        distance.push_back(std::make_unique<double[]>(1600));
        extinction.push_back(std::make_unique<double[]>(1600));
        ref_radius.push_back(std::make_unique<double[]>(1600));
        vol_conc.push_back(std::make_unique<double[]>(1600));
        depolarization.push_back(std::make_unique<double[]>(1600));
        for(int j = 0; j != 1600; ++j){
            distance[i][j] = 0.0;
            extinction[i][j] = 0.0;
            ref_radius[i][j] = 0.0;
            vol_conc[i][j] = 0.0;
            depolarization[i][j] = 0.0;
        }
    }
    std::ifstream sin;
    sin.open(path + filename);
    double t;
    int d, hours;
    while(sin>>t){
        sin>>dis>>t>>ext_coe>>PDR>>t>>Nc>>Re>>d>>hours;
        if(d == day){
            if(hours == 11){
                int n = dis / 0.00375;
                distance[0][n] += dis;
                extinction[0][n] += ext_coe;
                ref_radius[0][n] += Re;
                vol_conc[0][n] += Nc;
                depolarization[0][n] += PDR;
                if(dis == 1.5){
                    count1++;
                }
            }else if(hours == 23){
                int n = dis / 0.00375;
                distance[1][n] += dis;
                extinction[1][n] += ext_coe;
                ref_radius[1][n] += Re;
                vol_conc[1][n] += Nc;
                depolarization[1][n] += PDR;
                if(dis == 1.5){
                    count2++;
                }
            }else{
                continue;
            }
        }
    }
    sin.close();
    std::ofstream sout;
    sout.open("time_" + filename);
    sout.precision(10);
    for(int i = 30; i != 1199; ++i){
        sout<<distance[0][i] / double(count1)<<"  "<<extinction[0][i] / double(count1)<<"  "<<ref_radius[0][i] / double(count1)<<"  "<<vol_conc[0][i] / double(count1)<<"  "<<depolarization[0][i] / double(count1)<<"  ";
        sout<<extinction[1][i] / double(count2)<<"  "<<ref_radius[1][i] / double(count2)<<"  "<<vol_conc[1][i] / double(count2)<<"  "<<depolarization[1][i] / double(count2)<<std::endl;
    }
    sout.close();
}

int main(int argc, char *argv[]){
    std::vector<std::thread> thr;
    thr.push_back(std::thread(work, "3_dh_out.txt", 8));
    thr.push_back(std::thread(work, "2_dh_out.txt", 10));
    thr.push_back(std::thread(work, "3_zy_out.txt", 9));
    thr.push_back(std::thread(work, "2_zy_out.txt", 11));
    for(auto i = thr.begin(); i != thr.end(); ++i){
        i->join();
    }
    return 0;
}