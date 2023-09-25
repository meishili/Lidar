#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>

std::string path = "build/";

void work(const std::string city){
    int count = 0;
    std::ofstream sout;
    sout.open(city + "_mean.txt");
    for(int i = 2; i != 14; ++i){
        int month = (i % 12) + 1;
        std::string filename = path + std::to_string(month) + "_" + city + "_mean.txt";
        std::ifstream sin;
        sin.open(filename);
        int n;
        double sum1, sum2, sum3, sum4;
        while(sin>>n){
            count++;
            sin>>sum1>>sum2>>sum3>>sum4;
            sout<<count<<"  "<<sum1<<"  "<<sum2<<"  "<<sum3<<"  "<<sum4<<std::endl;
        }
        sin.close();
        std::cout<<filename<<"  "<<count<<"  "<<std::endl;
    }
    sout.close();
}

int main(int argc, char *argv[]){
    std::vector<std::thread> thr;
    thr.push_back(std::thread(work, "dh"));
    thr.push_back(std::thread(work, "zy"));
    thr[0].join();
    thr[1].join();
    return 0;
}