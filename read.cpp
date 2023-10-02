#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char *argv[]){
    std::ifstream sin;
    bool b;
    sin.open("build/7_dh_1_out.txt");
    std::ofstream sout;
    sout.open("dh_7.txt");
    std::string str[7];
    double time, dis;
    while(sin>>time){
        sin>>dis;
        std::cout<<time<<std::endl;
        for(int i = 0; i != 7; ++i){
            sin>>str[i];
        }
        b = true;
        if(time > 6.3 and time < 7.5){
            for(int i = 0; i != 7; ++i){
                if(str[i] == "nan" or str[i] == "-nan"){
                    b = false;
                }
            }
            if(b){
                sout<<time<<"  "<<dis<<"  ";
                for(int i = 0; i != 7; ++i){
                    sout<<str[i]<<"  ";
                }
                sout<<std::endl;
            }
        }
    }
    sout.close();
    sin.close();
    return 0;
}