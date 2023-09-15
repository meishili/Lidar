#include "fernald.h"

constexpr double mol_lidar_ratio = 8.0 * 3.1415926 / 3.0;
constexpr double color_correct = 0.12378;

Fernald::Fernald(int s) : size(s){
    signal532p = std::make_unique<double[]>(size);
    signal532s = std::make_unique<double[]>(size);
    signal1064 = std::make_unique<double[]>(size);
    signal607 = std::make_unique<double[]>(size);
    extinction_1064 = std::make_unique<double[]>(size);
    extinction_532 = std::make_unique<double[]>(size);
    volume_depolarization = std::make_unique<double[]>(size);
    particle_depolarization = std::make_unique<double[]>(size);
    color_ratio = std::make_unique<double[]>(size);
    vol_conc = std::make_unique<double[]>(size);
    eff_radius = std::make_unique<double[]>(size);
    distance = std::make_unique<double[]>(size);
}

void Fernald::remova_background(std::unique_ptr<double[]> &ptr){
    double sum = 0.0;
    for(int i = size / 8 * 7; i != size; ++i){
        sum += ptr[i];
    }
    sum /= double(size / 8);
    for(int i = 0; i != size; ++i){
        ptr[i] -= sum;
    }
}

void Fernald::fernald_1064(std::shared_ptr<double[]> overlap, std::shared_ptr<double[]> mol_ext, const double lidar_ratio){
    auto TABC = std::make_unique<double[]>(size);
    int reference = size / 8 * 3;
    for(int i = 0; i != reference; ++i){
        TABC[i] = signal1064[i] * distance[i] * distance[i] / overlap[i];
    }
    extinction_1064[reference] = 0.0;
    double temp1, temp2, temp3;
    double spatial = distance[10] - distance[9];
    for(int i = reference - 1; i != 0; --i){
        temp1 = TABC[i] * exp((lidar_ratio - mol_lidar_ratio) * (mol_ext[i] + mol_ext[i + 1]) * spatial / mol_lidar_ratio);
        temp2 = TABC[i + 1] / (extinction_1064[i + 1] + mol_ext[i + 1] / mol_lidar_ratio * lidar_ratio);
        temp3 = (TABC[i + 1] + temp1) * spatial;
        extinction_1064[i] = temp1 / (temp2 + temp3) - mol_ext[i] / mol_lidar_ratio * lidar_ratio;
    }
}

void Fernald::fernald_532(std::shared_ptr<double[]> overlap, std::shared_ptr<double[]> mol_ext, const double lidar_ratio){
    auto TABC = std::make_unique<double[]>(size);
    int reference = size / 8 * 3;
    for(int i = 0; i != reference; ++i){
        TABC[i] = (signal532p[i] + signal532s[i] / 1.1) * distance[i] * distance[i] / overlap[i];
    }
    extinction_532[reference] = 0.0;
    double temp1, temp2, temp3;
    double spatial = distance[10] - distance[9];
    for(int i = reference - 1; i != 0; --i){
        temp1 = TABC[i] * exp((lidar_ratio - mol_lidar_ratio) * (mol_ext[i] + mol_ext[i + 1]) * spatial / mol_lidar_ratio);
        temp2 = TABC[i + 1] / (extinction_1064[i + 1] + mol_ext[i + 1] / mol_lidar_ratio * lidar_ratio);
        temp3 = (TABC[i + 1] + temp1) * spatial;
        extinction_532[i] = temp1 / (temp2 + temp3) - mol_ext[i] / mol_lidar_ratio * lidar_ratio;
    }
}

void Fernald::get_micro(const std::shared_ptr<double[]> overlap, const std::shared_ptr<double[]> mol_ext, const double lidar_ratio){
    auto TABC = std::make_unique<double[]>(size);
    double R;
    double lidar_const = 9.95127 * pow(10.0, -14);
    int reference = size / 8 * 3;
    constexpr double mol_depolarization = 0.00376;
    for(int i = 10; i != reference; ++i){
        TABC[i] = lidar_const * (signal532p[i] + signal532s[i] / 1.1) * distance[i] * distance[i] / overlap[i];
        R = 1.0 + extinction_532[i] / lidar_ratio / mol_ext[i] * mol_lidar_ratio;
        volume_depolarization[i] = signal532s[i] / signal532p[i] / 1.1;
        particle_depolarization[i] = ((1.0 + mol_depolarization) * volume_depolarization[i] * R - (1.0 + volume_depolarization[i]) * mol_depolarization) / 
            ((1.0 + mol_depolarization) * R - (1.0 + volume_depolarization[i]));
        color_ratio[i] = signal1064[i] / (signal532p[i] + signal532s[i] / 1.1) / color_correct;
        eff_radius[i] = 0.2296 + 3.3784 * particle_depolarization[i] - 0.1358 * color_ratio[i] + 33174.9 * TABC[i];
        vol_conc[i] = 0.01294 + 2.03615 * particle_depolarization[i] - 0.01456 * color_ratio[i] - 2247.2 * TABC[i];
    }
}

void Fernald::coe_1064(std::shared_ptr<double[]> overlap, std::shared_ptr<double[]> mol_ext, const double lidar_ratio){
    fernald_1064(overlap, mol_ext, lidar_ratio);
}

void Fernald::coe_532(std::shared_ptr<double[]> overlap, std::shared_ptr<double[]> mol_ext, const double lidar_ratio){
    fernald_532(overlap, mol_ext, lidar_ratio);
    get_micro(overlap, mol_ext, lidar_ratio);
}

void Fernald::show_1064(std::ofstream &sout){
    sout.precision(8);
    int i = 10;
    while(distance[i] < 10000.0){
        sout<<extinction_1064[i] * 1000.0<<", ";
        ++i;
    }
    sout<<std::endl;
}

void Fernald::show_532(std::ofstream &sout){
    sout.precision(8);
    int i = 10;
    while(distance[i] < 10000.0){
        sout<<extinction_532[i] * 1000.0<<", ";
        ++i;
    }
    sout<<std::endl;
}

void Fernald::show_distance(std::ofstream &sout){
    sout.precision(8);
    int i = 10;
    while(distance[i] < 10000.0){
        sout<<distance[i]<<", ";
        ++i;
    }
    sout<<std::endl;
}

double Fernald::get_aer_opt_dep(const std::unique_ptr<double[]> &ptr){
    double sum = 0.0;
    int reference = size / 8 * 3;
    double spatial = distance[10] - distance[9];
    for(int i = 0; i != reference; ++i){
        if(i < 15){
            sum += ptr[i + 15];
        }else{
            sum += ptr[i];
        }
    }
    return sum * spatial;
}

double Fernald::aod_1064(){
    return get_aer_opt_dep(extinction_1064);
}

double Fernald::aod_532(){
    return get_aer_opt_dep(extinction_532);
}