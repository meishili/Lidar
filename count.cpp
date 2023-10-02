#include <iostream>
#include <cmath>
#include <fstream>
#include <string>

int main(int argc, char *argv[]){
    std::ifstream sin;
    sin.open("zy_4.txt");
    double t, time, dis;
    double sum = 0.0;
    int count = 0;
    double temp;
    while(sin>>time){
        sin>>dis>>temp>>temp>>t>>temp>>temp>>temp>>temp;
        if(time > 15.3 and time < 17.5){
            if(t > 0 and t < 120 and dis > 1000 and dis < 2500){
                sum += t;
                ++count;
            }
        }
    }
    sum /= double(count);
    sin.close();
    sin.open("zy_4.txt");
    double error = 0.0;
    while(sin>>time){
        sin>>dis>>t;
        if(time > 15.3 and time < 17.5){
            if(t > 0 and t < 100 and dis > 1000 and dis < 2500)
            error += (sum - t) * (sum - t);
        }
    }
    error = sqrt(error / double(count));
    sin.close();
    std::cout<<"sum = "<<sum<<",  error = "<<error<<std::endl;
    return 0;
}