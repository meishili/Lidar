#include "lidar.h"

std::mutex mtx;

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
    VDR = std::make_unique<double[]>(size);
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
    for(int i = 7000; i != 8000; ++i){
        background_p += signal532p[i];
        background_s += signal532s[i];
        background_C += signal1064[i];
    }
    background_C /= 1000.0;
    background_p /= 1000.0;
    background_s /= 1000.0;
    for(int i = 0; i != 8000; ++i){
        signal532p[i] -= background_p;
        signal532s[i] -= background_s;
        signal1064[i] -= background_C;
        signal532[i] = signal532p[i] + signal532s[i] / 1.1;
    }
}

void Lidar::fernald(const std::shared_ptr<double[]> overlap, const std::shared_ptr<double[]> molecule_extinction){
    int reference = 3000;
    for(int i = reference; i != 0; --i){
        TABC[i] = signal532[i] * distance[i] * distance[i] / overlap[i];
    }
    extinction[reference] = 0.0;
    double temp1, temp2, temp3;
    for(int i = reference - 1; i != 0; --i){
        temp1 = TABC[i] * exp((lidar_ratio - molecule_lidar_ratio) * (molecule_extinction[i] + molecule_extinction[i + 1]) * 3.75 / molecule_lidar_ratio );
        temp2 = TABC[i + 1] / (extinction[i + 1] + lidar_ratio / molecule_lidar_ratio * molecule_extinction[i + 1]);
        temp3 = (TABC[i + 1] + temp1) * 3.75;
        extinction[i] = temp1 / (temp2 + temp3) - molecule_lidar_ratio / lidar_ratio * molecule_extinction[i];
    }
    double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
    for(int i = 1; i != 300; ++i){
        if(i < 50){
            sum1 += extinction[i + 50] + extinction[i + 51] + molecule_extinction[i + 50] + molecule_extinction[i + 51];
        }else{
            sum1 += extinction[i] + extinction[i - 1] + molecule_extinction[i] + molecule_extinction[i - 1];
            temp1 = (extinction[i] / lidar_ratio + molecule_extinction[i] / molecule_lidar_ratio) * exp(-1.0 * sum1 * 3.75);
            sum2 += temp1;
            sum3 += TABC[i];
            
        }
    }
    double R;
    double lidar_const = sum2 / sum3;
    double cc[2] = {4.13277, 4.383524};
    lidar_const = cc[1] * pow(10.0, -14);
    // std::cout<<lidar_const<<std::endl;
    for(int i = 0; i != reference; ++i){
        TABC[i] *= lidar_const;
        R = 1.0 + extinction[i] / lidar_ratio / molecule_extinction[i] * molecule_lidar_ratio;
        VDR[i] = signal532s[i] / signal532p[i] / 1.1;
        PDR[i] = ((1.0 + molecule_depolarization) * VDR[i] * R - (1.0 + VDR[i]) * molecule_depolarization) / 
            ((1.0 + molecule_depolarization) * R - (1.0 + VDR[i]));
    }
}

void Lidar::microphysical(){
    int reference = size / 8 * 3;
    for(int i = 0; i != 3000; ++i){
        CR[i] = signal1064[i] / signal532[i] / color_correct;
        eff_rad[i] = 0.2296 + 3.3784 * PDR[i] - 0.1358 * CR[i] + 33174.9 * TABC[i];
        volume_con[i] = 0.01294 + 2.03615 * PDR[i] - 0.01456 * CR[i] - 2247.2 * TABC[i];
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
    for(int i = 30; i != reference; ++i){
        sout<<time<<"  "<<distance[i] / 1000.0<<"  "<<TABC[i] * 1000.0<<"  "<<extinction[i] * 1000.0<<"  "<<PDR[i]<<"  "<<VDR[i];
        sout<<"  "<<CR[i]<<"  "<<volume_con[i]<<"  "<<eff_rad[i]<<"  "<<day<<"  "<<hours<<std::endl;
        // sout<<distance[i]<<"  "<<extinction[i]<<"  "<<signal532s[i]<<"  "<<TABC[i]<<std::endl;
    }
}

double Lidar::mean_eff_rad(){
    double sum = 0.0;
    for(int i = 200; i != 800; ++i){
        sum += eff_rad[i];
    }
    sum /= 600.0;
    return sum;
}

double Lidar::mean_ext(){
    double sum = 0.0;
    for(int i = 200; i != 800; ++i){
        sum += extinction[i];
    }
    sum /= 600.0;
    return sum;
}

double Lidar::mean_vol_con(){
    double sum = 0.0;
    for(int i = 200; i != 800; ++i){
        sum += volume_con[i];
    }
    sum /= 600.0;
    return sum;
}

double Lidar::mean_pdr(){
    double sum = 0.0;
    for(int i = 200; i != 800; ++i){
        sum += PDR[i];
    }
    sum /= 600.0;
    return sum;
}

void Lidar::mean(std::shared_ptr<Mean> mean){
    mtx.lock();
    mean->add(extinction, eff_rad, volume_con, PDR);
    mtx.unlock();
}

