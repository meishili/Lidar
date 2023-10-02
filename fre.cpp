#include <iostream>
#include <fstream>

int main(int argc, char *argv[]){
    std::ifstream sin;
    sin.open("zy_4.txt");
    double t, time, dis;
    double sum[21];
    int count[21];
    int s = 0;
    for(int i = 0; i != 21; ++i){
        sum[i] = 0.0;
        count[i] = 0;
    }
    double temp;
    while(sin>>time){
        sin>>dis>>temp>>temp>>t>>temp>>temp>>temp>>temp;
        if(time > 15.3 and time < 17.5){
            if(dis > 1000 and dis < 2500){
                if(t > 0 and t < 120){
                    int n = t / 5.0;
                    if(n >= 20){
                        n = 20;
                    }
                    sum[n] += t;
                    count[n]++;
                    s++;
                }
            }
        }
    }
    sin.close();
    std::ofstream sout;
    sout.open("zy_fre.txt");
    for(int i = 0; i != 21; ++i){
        sout<<i<<"  "<<double(count[i]) / double(s) * 100.0<<std::endl;
    }
    sout.close();
    return 0;
}