#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class Fre{
    private:
        int sum[100][100];
        double end1;
        double end2;
        double begin1;
        double begin2;
        int count;
    public:
        Fre(double t1, double t2, double t3, double t4);
        void add(double t1, double t2);
        void show(const std::string str);
};

Fre::Fre(double t1, double t2, double t3, double t4){
    begin1 = t1;
    end1 = t2;
    begin2 = t3;
    end2 = t4;
    for(int i = 0; i != 100; ++i){
        for(int j = 0; j != 100; ++j){
            sum[i][j] = 0;
        }
    }
    count = 0;
}

void Fre::add(double t1, double t2){
    if(t1 > end1 or t1 < begin1 or t2 < begin2 or t2 > end2){
        return;
    }else{
        int n1 = (t1 - begin1) / ((end1 - begin1) / 100.0);
        int n2 = (t2 - begin2) / ((end2  - begin2) / 100.0);
        sum[n1][n2]++;
        ++count;
    }
}

void Fre::show(const std::string str){
    std::ofstream sout;
    sout.open(str);
    double step1 = (end1 - begin1) / 100.0;
    double step2 = (end2 - begin2) / 100.0;
    for(int i = 0; i != 100; ++i){
        for(int j = 0; j != 100; ++j){
            if(double(sum[i][j]) / double(count) > 0.0005){
                sout<<begin2 + step2 * double(j)<<"  "<<begin1 + step1 * double(i)<<"  "<<double(sum[i][j]) / double(count) * 100.0<<std::endl;
            } 
        }
    }
    sout.close();
}

int main(int argc, char *argv[]){
    double time, dis, ext, lidar_ratio, eff, vol, PDR, t;
    std::ifstream sin;
    sin.open("dh_7.txt");
    std::vector<Fre> fre;
    int count = 0;
    fre.push_back(Fre(40.0, 100.0, 0.0, 0.8));
    fre.push_back(Fre(40.0, 100.0, 0.6, 1.8));
    fre.push_back(Fre(40.0, 100.0, 0.2, 0.8));
    fre.push_back(Fre(40.0, 100.0, 0.2, 0.4));
    while(sin>>time){
        sin>>dis>>ext>>t>>lidar_ratio>>eff>>vol>>PDR>>t;
        if(time < 35.0){
        if(dis > 1000.0 and dis < 3000.0){
            if(lidar_ratio >= 0.0 and lidar_ratio <= 100.0){
            ++count;
            fre[0].add(lidar_ratio, ext * 1000.0);
            fre[1].add(lidar_ratio, eff);
            fre[2].add(lidar_ratio, vol);
            fre[3].add(lidar_ratio, PDR);
            }
        }
        }
    }
    std::cout<<"count = "<<count<<std::endl;
    sin.close();
    fre[0].show("ext.txt");
    fre[1].show("eff.txt");
    fre[2].show("vol.txt");
    fre[3].show("PDR.txt");
    return 0;
}