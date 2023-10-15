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
    smooth(signal1064);
    // smooth(signal532p);
    // smooth(signal532s);
    for(int i = 0; i != 8000; ++i){
        signal532p[i] -= background_p;
        signal532s[i] -= background_s;
        signal1064[i] -= background_C;
        signal532[i] = signal532p[i] + signal532s[i] / 1.1;
    }
    smooth(signal532);
}

void Lidar::fernald(const std::shared_ptr<double[]> overlap, const std::shared_ptr<double[]> molecule_extinction){
    int reference = 1600;
    for(int i = reference; i != 0; --i){
        TABC[i] = signal532[i] * distance[i] * distance[i] / overlap[i];
    }
    smooth(TABC);
    // extinction[reference] = molecule_extinction[reference];
    double temp1, temp2, temp3;
    // std::unique_ptr<double[]> back = std::make_unique<double[]>(8000);
    extinction[reference] = molecule_extinction[reference];
    for(int i = reference - 1; i != 0; --i){
        temp1 = TABC[i] * exp((lidar_ratio - molecule_lidar_ratio) * (molecule_extinction[i] + molecule_extinction[i + 1]) * 3.75 / molecule_lidar_ratio );
        temp2 = TABC[i + 1] / (extinction[i + 1] + molecule_extinction[i + 1] / molecule_lidar_ratio * lidar_ratio);
        temp3 = (TABC[i + 1] + temp1) * 3.75;
        extinction[i] = temp1 / (temp2 + temp3) - molecule_extinction[i] / molecule_lidar_ratio * lidar_ratio;
    }
    // auto TABC_fit = std::make_unique<double[]>(800);
    // double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
    // for(int i = 1; i != 300; ++i){
    //     if(i < 20){
    //         sum1 += extinction[i + 20] + extinction[i + 21] + molecule_extinction[i + 20] + molecule_extinction[i + 21];
    //     }else{
    //         sum1 += extinction[i] + extinction[i - 1] + molecule_extinction[i] + molecule_extinction[i - 1];
    //         temp1 = (extinction[i] / lidar_ratio + molecule_extinction[i] / molecule_lidar_ratio) * exp(-1.0 * sum1 * 3.75);
    //         TABC_fit[i] += temp1;
    //     }
    // }
    double R;
    // R = slope(TABC, TABC_fit);
    double lidar_const;
    // lidar_const = R;
    // lidar_const = sum2 / sum3;
    double cc[2] = {9.95127, 4.383524};
    lidar_const = cc[0] * pow(10.0, -14);
    // std::cout<<lidar_const<<std::endl;
    for(int i = 0; i != reference; ++i){
        TABC[i] *= lidar_const;
        R = 1.0 + extinction[i] / lidar_ratio / molecule_extinction[i] * molecule_lidar_ratio;
        VDR[i] = signal532s[i] / signal532p[i] / 1.1;
        PDR[i] = ((1.0 + molecule_depolarization) * VDR[i] * R - (1.0 + VDR[i]) * molecule_depolarization) / 
            ((1.0 + molecule_depolarization) * R - (1.0 + VDR[i]));
            CR[i] = signal1064[i] / signal532[i] / color_correct;
    }
    smooth(CR);
    // smooth(VDR);
}

void Lidar::microphysical(){
    int reference = 1600;
    for(int i = 0; i != reference; ++i){
        eff_rad[i] = 0.2296 + 3.3784 * PDR[i] - 0.1358 * CR[i] + 33174.9 * TABC[i];
        volume_con[i] = 0.01294 + 2.03615 * PDR[i] - 0.01456 * CR[i] - 2247.2 * TABC[i];
        // eff_rad[i] = -0.2328 + 2.3425 * VDR[i] - 0.0188 * CR[i] + 10556.6 * TABC[i];
        // volume_con[i] = 0.0308 + 0.8156 * VDR[i] + 0.0594 * CR[i] + 37.85 * TABC[i];
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
    int reference = 1400;
    for(int i = 30; i != reference; ++i){
        sout<<time<<"  "<<distance[i] / 1000.0<<"  "<<TABC[i] * 1000.0<<"  "<<extinction[i] * 1000.0<<"  "<<PDR[i];
        sout<<"  "<<CR[i]<<"  "<<volume_con[i]<<"  "<<eff_rad[i]<<"  "<<day<<"  "<<hours<<std::endl;
    }
}

double Lidar::mean_eff_rad(){
    double sum = 0.0;
    int count = 0;
    for(int i = 120; i != 600; ++i){
        if(eff_rad[i] > 0.0 and eff_rad[i] < 2.5){
            sum += eff_rad[i];
            count++;
        }
    }
    sum /= double(count);
    return sum;
}

double Lidar::mean_ext(){
    double sum = 0.0;
    int count = 0;
    for(int i = 120; i != 600; ++i){
        if(extinction[i] > 0.0 and extinction[i] < 0.0015){ 
            sum += extinction[i];
            ++count;
        }
    }
    sum /= double(count);
    return sum;
}

double Lidar::mean_vol_con(){
    double sum = 0.0;
    int count = 0;
    for(int i = 120; i != 600; ++i){
        if(volume_con[i] > 0.0 and volume_con[i] < 1.0){
            sum += volume_con[i];
            ++count;
        }
    }
    sum /= double(count);
    return sum;
}

double Lidar::mean_color_ratio(){
    double sum = 0.0;
    int count = 0;
    for(int i = 120; i != 600; ++i){
        if(CR[i] > 0.0 and CR[i] < 2.0){
            sum += CR[i];
            ++count;
        }
    }
    sum /= double(count);
    return sum;
}

double Lidar::mean_pdr(){
    double sum = 0.0;
    int count = 0;
    for(int i = 120; i != 600; ++i){
        if(PDR[i] >0.0 and PDR[i] < 0.5){
            sum += PDR[i];
            ++count;
        }
    }
    sum /= double(count);
    return sum;
}

void Lidar::mean(std::shared_ptr<Mean> mean){
    mtx.lock();
    mean->add(extinction, eff_rad, volume_con, PDR);
    mtx.unlock();
}

void Lidar::smooth(std::unique_ptr<double[]> &ptr){
    double sum;
    for(int i = 30; i != 1400; ++i){
        sum = 0.0;
        for(int j = -3; j != 4; ++j){
            sum += ptr[i + j];
        }
        ptr[i] = sum / 7.0;
    }
}

void Lidar::add_fre(std::shared_ptr<Coe_count[]> ptr){
    int n;
    for(int i = 120; i != 800; ++i){
        n = distance[i] / 1000.0;
        ptr[n].add(extinction[i] * 1000.0, eff_rad[i], volume_con[i], VDR[i]);
    }
}

double Lidar::slope(const std::unique_ptr<double[]> &x, const std::unique_ptr<double[]> &y){
    double sum1 = 0.0;
    double sum2 = 0.0;
    double sum3 = 0.0;
    double sum4 = 0.0;
    double diff;
    for(int i = 100; i != 700; ++i){
        sum1 += x[i];
        sum2 += y[i];
        sum3 += x[i] * y[i];
        sum4 += x[i] * x[i];
    }
    diff = (600.0 * sum3 - sum1 * sum2) / (600.0 * sum4 - sum1 * sum1);
    return diff;
}

bool Lidar::is_cloud(){
    double sum1 = 0.0, sum2 = 0.0;
    bool iscloud = false;
    for(int i = 200; i != 300; ++i){
        sum2 += TABC[i];
    }
    for(int i = 3; i != 9; ++i){
        int begin = 100 * i;
        int end = begin + 100;
        sum1 = 0.0;
        for(int j = begin; j != end; ++j){
            sum1 += TABC[j];
        }
        if(sum1 >= 1.5 * sum2){
            iscloud = true;
        }
    }
    return iscloud;
}
