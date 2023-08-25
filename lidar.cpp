#include "lidar.h"

Lidar::Lidar(int n) : size(n){
    signal532p = std::make_unique<double[]>(size);
    signal532s = std::make_unique<double[]>(size); 
    signal1064 = std::make_unique<double[]>(size); 
    signal532 = std::make_unique<double[]>(size); 
    distance = std::make_unique<double[]>(size); 
    TABC = std::make_unique<double[]>(size); 
    extinction = std::make_unique<double[]>(size); 
    PDR = std::make_unique<double[]>(size); 
    CR = std::make_unique<double[]>(size); 
    volume_con = std::make_unique<double[]>(size); 
    eff_rad = std::make_unique<double[]>(size); 
}

void Lidar::read(){
    std::ifstream sin;
    sin.open(filename);
    double temp;
    for(int i = 0; i != size; ++i){
        sin>>distance[i]>>signal532p[i]>>signal532s[i]>>signal1064[i]>>temp;
    }
    sin.close();
}

void Lidar::remove_background(){
    double background_p = 0.0;
    double background_s = 0.0;
    double background_C = 0.0;
    for(int i = size - size / 8000; i != 8000; ++i){
        background_p += signal532p[i];
        background_s += signal532s[i];
        background_C += signal1064[i];
    }
    background_C /= double(size / 8000);
    background_p /= double(size / 8000);
    background_s /= double(size / 8000);
    for(int i = 0; i != size; ++i){
        signal532p[i] -= background_p;
        signal532s[i] -= background_s;
        signal1064[i] -= background_C;
        signal532[i] = signal532p[i] + signal532s[i] / 1.0;
    }
}

void Lidar::fernald(const std::shared_ptr<double[]> overlap, const std::shared_ptr<double[]> molecule_extinction){
    int reference = size / 8 * 3;
    for(int i = reference; i != 0; --i){
        TABC[i] = signal532[i] * distance[i] * distance[i] / overlap[i];
    }
    extinction[reference] = molecule_extinction[reference];
    double temp1, temp2, temp3;
    for(int i = reference - 1; i != 0; --i){
        temp1 = TABC[i] * exp((lidar_ratio - molecule_lidar_ratio) * (molecule_extinction[i] + molecule_extinction[i + 1]) * 3.75 / molecule_lidar_ratio );
        temp2 = TABC[i + 1] / (extinction[i + 1] + lidar_ratio / molecule_lidar_ratio * molecule_extinction[i + 1]);
        temp3 = (TABC[i + 1] + temp1) * 3.75;
        extinction[i] = temp1 / (temp2 + temp3) - lidar_ratio / molecule_lidar_ratio * molecule_extinction[i];
    }
    double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
    for(int i = 1; i != 2000; ++i){
        if(i < 30){
            sum1 += extinction[i + 30] + extinction[i + 31] + molecule_extinction[i + 30] + molecule_extinction[i + 31];
        }else{
            sum1 += extinction[i] + extinction[i - 1] + molecule_extinction[i] + molecule_extinction[i - 1];
            temp1 = (extinction[i] / lidar_ratio + molecule_extinction[i] / molecule_lidar_ratio) * exp(-1.0 * sum1 * 3.75);
            sum2 += temp1;
            sum3 += TABC[i];
        }
    }
    double R;
    double lidar_const = sum2 / sum3;
    for(int i = 0; i != reference; ++i){
        TABC[i] *= lidar_const;
        R = 1.0 + extinction[i] / lidar_ratio / molecule_extinction[i] * molecule_lidar_ratio;
        double VDR = signal532s[i] / signal532p[i] / 1.0;
        PDR[i] = ((1.0 + molecule_depolarization) * VDR * R - (1.0 + VDR) * molecule_depolarization) / 
            ((1.0 + molecule_depolarization) * R - (1.0 + VDR));
    }
}

void Lidar::microphysical(){
    int reference = size / 8 * 3;
    for(int i = 0; i != reference; ++i){
        CR[i] = signal1064[i] / signal532[i] / color_correct;
        eff_rad[i] = 0.2296 + 3.3784 * PDR[i] - 0.1358 * CR[i] + 33.1749 * TABC[i];
        volume_con[i] = 0.01294 + 2.03615 * PDR[i] - 0.01456 * CR[i] - 2.2472 * TABC[i];
    }
}

double Lidar::get_time(){
    int len = filename.length() - 14;
    int day = std::stoi(filename.substr(len + 4, 2));
    int hours = std::stoi(filename.substr(len + 6, 2));
    int min = std::stoi(filename.substr(len + 8, 2));
    double time = double(day) + double(hours) / 24.0 + double(min) / 24.0 / 60.0;
    return time;
}

int Lidar::get_hours(){
    int len = filename.length() - 14;
    int hours = std::stoi(filename.substr(len + 6, 2));
    return hours;
}

int Lidar::get_day(){
    int len = filename.length() - 14;
    int day = std::stoi(filename.substr(len + 4, 2));
    return day;
}

void Lidar::show(std::ofstream &sout){
    int day = get_day();
    int hours = get_hours();
    double time = get_time();
    int reference = size / 8 * 3;
    for(int i = 10; i != reference; ++i){
        sout<<time<<"  "<<distance[i]<<"  "<<TABC[i]<<"  "<<extinction[i]<<"  "<<PDR[i];
        sout<<"  "<<CR[i]<<"  "<<volume_con[i]<<"  "<<eff_rad[i]<<"  "<<day<<"  "<<hours<<std::endl;
    }
}
