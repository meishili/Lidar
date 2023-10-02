#include "raman.h"
static int count1 = 0;
static int count2 = 0;

constexpr double molecule_lidar_ratio = 8.0 * 3.1415926 / 3.0;
constexpr double color_correct = 0.12378;

Queue::Queue(){
    size = 0;
    begin = 0;
    end = 0;
}

Queue::Queue(int n) : size(n + 1){
    begin = 0;
    end = 0;
    for(int i = 0; i != size; ++i){
        signal532p.push_back(std::make_unique<double[]>(8000));
        signal532s.push_back(std::make_unique<double[]>(8000));
        signal1064.push_back(std::make_unique<double[]>(8000));
        signal607.push_back(std::make_unique<double[]>(8000));
    }
}

Queue::~Queue(){
    size = 0;
    begin = 0;
    end = 0;
}

void Queue::push(const std::string filename){
    std::ifstream sin;
    sin.open(filename);
    double t;
    for(int i = 0; i != 8000; ++i){
        sin>>t>>signal532p[end][i]>>signal532s[end][i]>>signal1064[end][i]>>signal607[end][i];
    }
    sin.close();
    remove_background(signal532p[end]);
    remove_background(signal532s[end]);
    remove_background(signal1064[end]);
    remove_background(signal607[end]);
    end = (end + 1) % size;
}

void Queue::pop(std::unique_ptr<double[]> &p532p, std::unique_ptr<double[]> &p532s, std::unique_ptr<double[]> &p1064, std::unique_ptr<double[]> &p607, const std::shared_ptr<double[]> overlap){
    double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0, sum4 = 0.0;
    for(int i = 0; i != 8000; ++i){
        sum1 = 0.0;
        sum2 = 0.0;
        sum3 = 0.0;
        sum4 = 0.0;
        for(int j = 0; j < size; ++j){
            if(j == end){
                continue;
            }
            sum1 += signal532p[j][i];
            sum2 += signal532s[j][i];
            sum3 += signal1064[j][i];
            sum4 += signal607[j][i];
            // std::cout<<signal607[j][i]<<std::endl;
        }
        p532p[i] = sum1 / double(size - 1) / overlap[i];
        p532s[i] = sum2 / double(size - 1) / overlap[i];
        p1064[i] = sum3 / double(size - 1) / overlap[i];
        p607[i] = sum4 / double(size - 1) / overlap[i];
    }
    begin = (begin + 1) % size;
}

void Queue::remove_background(std::unique_ptr<double[]> &ptr){
    double background = 0.0;
    for(int i = 7000; i != 8000; ++i){
        background += ptr[i];
    }
    background /= 1000.0;
    for(int i = 0; i != 8000; ++i){
        ptr[i] -= background;
    }
}

bool Queue::isempty(){
    return begin == end;
}

bool Queue::isfull(){
    return (end + 1) % size == begin;
}

void Queue::set_empty(){
    begin = 0;
    end = 0;
}

Raman::Raman(){
    extinction = std::make_unique<double[]>(1000);
    backscatter = std::make_unique<double[]>(1000);
    lidar_ratio = std::make_unique<double[]>(1000);
    molocule_extinction = std::make_unique<double[]>(8000);
    color_ratio = std::make_unique<double[]>(1000);
    depolarization_ratio = std::make_unique<double[]>(1000);
    volume_depolarization_ratio = std::make_unique<double[]>(1000);
    vol_conc = std::make_unique<double[]>(1000);
    eff_radius = std::make_unique<double[]>(1000);
    distance = std::make_unique<double[]>(8000);
}

void Raman::read_molocule(const std::shared_ptr<double[]> molo_ext){
    for(int i = 0; i != 8000; ++i){
        molocule_extinction[i] = molo_ext[i];
        distance[i] = double(i) * 3.75;
    }
    molocule_extinction = reduce_resolution(molocule_extinction);
    distance = reduce_resolution(distance);
}

void Raman::get_signal(Queue &queue,const std::shared_ptr<double[]> overlap){
    auto p532p = std::make_unique<double[]>(8000);
    auto p532s = std::make_unique<double[]>(8000);
    auto p1064 = std::make_unique<double[]>(8000);
    auto p607 = std::make_unique<double[]>(8000);
    queue.pop(p532p, p532s, p1064, p607, overlap);
    signal532p = reduce_resolution(p532p);
    signal532s = reduce_resolution(p532s);
    signal1064 = reduce_resolution(p1064);
    signal607 = reduce_resolution(p607);
    signal532 = std::make_unique<double[]>(1000);
    for(int i = 0; i != 1000; ++i){
        signal532[i] = signal532p[i] + signal532s[i] / 1.1;
    }
}

void Raman::show(std::ofstream &sout, const double time){
    for(int i = 30; i != 250; ++i){
        sout<<time<<"  "<<distance[i]<<"  "<<extinction[i]<<"  "<<backscatter[i]<<"  ";
        sout<<lidar_ratio[i]<<"  "<<eff_radius[i]<<"  "<<vol_conc[i]<<"  ";
        sout<<depolarization_ratio[i]<<"  "<<volume_depolarization_ratio[i]<<std::endl;
    }
}

std::unique_ptr<double[]> Raman::reduce_resolution(const std::unique_ptr<double[]> &ptr){
    auto p = std::make_unique<double[]>(1000);
    double sum;
    for(int i = 0; i != 1000; ++i){
        sum = 0.0;
        for(int j = 0; j != 5; ++j){
            sum += ptr[5 * i + j];
        }
        p[i] = sum / 5.0;
    }
    return p;                                                
}

void Raman::get_extinction(){
    auto logarith = std::make_unique<double[]>(1000);
    for(int i = 10; i != 500; ++i){
        logarith[i] = signal607[i] * distance[i] * distance[i];
    }
    smooth(logarith);
    for(int i = 10; i !=500; ++i){
        logarith[i] = log(molocule_extinction[i] / logarith[i]);
    }
    for(int j = 300; j != 20; --j){
        double diff = slope(distance, logarith, j - 10, j + 10);
        extinction[j] = (diff - (1.0 + pow(532.0 / 607.0, 4)) * molocule_extinction[j]) / (1.0 + 532.0 / 607.0);
    }
}

void Raman::get_backscatter(){
    double sum1 = 0.0, sum2 = 0.0;
    double temp1 = 0.0, temp2 = 0.0;
    for(int i = -3; i <=3; ++i){
        temp1 += signal532[i + 251];
        temp2 += signal607[i + 251];
    }
    if(extinction[250] / 30.0 >= molocule_extinction[250] / molecule_lidar_ratio){
        ++count1;
    }else{
        ++count2;
    }
    for(int j = 250; j != 20; --j){
        sum1 += molocule_extinction[j] + molocule_extinction[j + 1];
        sum2 += extinction[j] + extinction[j + 1];
        backscatter[j] = (2.0 * molocule_extinction[251] / molecule_lidar_ratio) * (temp2 * signal532[j] * molocule_extinction[j])
            / (signal607[j] * temp1 * molocule_extinction[251]) * exp(((1.0 - 532.0 / 607.0) * sum2  + 
            (1.0 - pow(532.0 / 607.0, 4)) * sum1) * 18.75 / 2.0);
    }
}

void Raman::get_lidar_ratio(){
    for(int i = 20; i != 250; ++i){
        lidar_ratio[i] = extinction[i] / backscatter[i];
    }
}

void Raman::get_micro(){
    double R;
    double TABC;
    for(int i = 20; i != 250; ++i){
        TABC = signal532[i] * distance[i] * distance[i] * pow(10.0, -14) * 9.95127;
        R = 1.0 + backscatter[i] / molocule_extinction[i] * molecule_lidar_ratio;
        volume_depolarization_ratio[i] = signal532s[i] / signal532p[i] / 1.1;
        depolarization_ratio[i] = ((1.0 + 0.00376) * volume_depolarization_ratio[i] * R - (1.0 + volume_depolarization_ratio[i]) * 0.00376)/
        ((1.0 + 0.00376) * R - (1.0 + volume_depolarization_ratio[i]));
        color_ratio[i] = signal1064[i] / signal532[i] / color_correct;
        eff_radius[i] = 0.02296 + 3.3784 * depolarization_ratio[i] - 0.1358 * color_ratio[i] + 33174.9 * TABC;
        vol_conc[i] = 0.01294 + 2.03615 * depolarization_ratio[i] - 0.01456 * color_ratio[i] - 2247.2 * TABC;
    }
}

double Raman::slope(const std::unique_ptr<double[]> &x, const std::unique_ptr<double[]> &y, int n1, int n2){
    double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0, sum4 = 0.0;
    int n = n2 - n1 + 1;
    double diff;
    double range = double(n);
    for(int i = n1; i != n2 + 1; ++i){
        sum1 += x[i];
        sum2 += y[i];
        sum3 += x[i] * y[i];
        sum4 += x[i] * x[i];
    }
    diff = (range * sum3 - sum1 * sum2) / (range * sum4 - sum1 * sum1);
    return diff;
}

void Raman::smooth(std::unique_ptr<double[]> &ptr){
    double sum;
    for(int i = 450; i != 15; --i){
        sum = 0.0;
        for(int j = -5; j != 6; ++j){
            sum += ptr[i + j];
        }
        ptr[i] = sum / 11.0;
    }
}

void Raman::computation_coe(){
    get_extinction();
    get_backscatter();
    get_lidar_ratio();
    get_micro();
}

void show(){
    std::cout<<"count1 = "<<count1<<",  count2 = "<<count2<<std::endl;
}