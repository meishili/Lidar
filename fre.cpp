#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <memory>
#include <vector>
#include <cmath>

struct Fre
{
    double mean_value;
    double variance;
};

std::string city = "zy";
void work(int month, std::shared_ptr<Fre[]> fre){
    std::string filename = "build/" + std::to_string(month) + "_" + city + "_out.txt";
    std::ifstream sin;
    sin.open(filename);
    double t, ext_count = 0.0, PDR_count = 0.0, eff_count = 0.0, vol_count = 0.0;
    double ext_var = 0.0, PDR_var = 0.0, eff_var = 0.0, vol_var = 0.0;
    double ext, PDR, eff, vol;
    int count = 0;
    while(sin>>t){
        sin>>t>>t>>ext>>PDR>>t>>vol>>eff>>t>>t;
        count++;
        ext_count += ext;
        PDR_count += PDR;
        eff_count += eff;
        vol_count += vol;
    }
    sin.close();
    ext_count /= double(count);
    PDR_count /= double(count);
    eff_count /= double(count);
    vol_count /= double(count);
    sin.open(filename);
    while (sin>>t)
    {
        sin>>t>>t>>ext>>PDR>>t>>vol>>eff>>t>>t;
        ext_var += (ext - ext_count) * (ext - ext_count);
        PDR_var += (PDR - PDR_count) * (PDR- PDR_count);
        vol_var += (vol - vol_count) * (vol - vol_count);
        eff_var += (eff - eff_count) * (eff - eff_count);
    }
    sin.close();
    fre[0].mean_value = ext_count;
    fre[0].variance = sqrt(ext_var / double(count));
    fre[1].mean_value = eff_count;
    fre[1].variance = sqrt(eff_var / double(count));
    fre[2].mean_value = vol_count;
    fre[2].variance = sqrt(vol_var / double(count));
    fre[3].mean_value = PDR_count;
    fre[3].variance = sqrt(PDR_var / double(count));    
}

int main(int argc, char *argv[]){
    std::vector<std::shared_ptr<Fre[]>> fre;
    for(int i = 0; i != 12; ++i){
        fre.push_back(std::shared_ptr<Fre[]>(new Fre[4]));
    }
    std::vector<std::thread> thr;
    for(int i = 2; i != 14; ++i){
        thr.push_back(std::thread(work, (i % 12) + 1, fre[i - 2]));
    }
    for(auto i = thr.begin(); i != thr.end(); ++i){
        i->join();
    }
    std::ofstream sout;
    sout.open(city + "_fre.txt");
    for(int i = 0; i != 12; ++i){
        sout<<i<<"  ";
        for(int j = 0; j != 4; ++j){
            sout<<fre[i][j].mean_value<<"  "<<fre[i][j].variance<<"  ";
        }
        sout<<std::endl;
    }
    sout.close();
    return 0;
}