#include "lidar.h"
#include "licel.h"

Data_pre::Data_pre(const int n){
    size = n;
    ptr = std::make_unique<double[]>(size);
    distance = std::make_unique<double[]>(size);
    TABC = std::make_unique<std::unique_ptr<double[]>[]>(5);
    extinction_coefficient = std::make_unique<std::unique_ptr<double[]>[]>(5);
    for(int i = 0; i != 5; ++i){
        TABC[i] = std::make_unique<double[]>(size);
        extinction_coefficient[i] = std::make_unique<double[]>(size);
    }
}

void Data_pre::read(const std::string filename){
    Licel licel(2);
    licel.set_filename(filename);
    licel.open_file();
    licel.read_head();
    licel.read_dataset();
    auto data = std::make_unique<std::unique_ptr<double[]>[]>(2);
    data[0] = std::make_unique<double[]>(16380);
    data[1] = std::make_unique<double[]>(16380);
    licel.get_dataset(data);
    licel.close_file();
    for(int i = 0; i != 8000; ++i){
        ptr[i] = data[0][i];
        distance[i] = 3.75 * double(i);
    }
    get_time(filename);
}

void Data_pre::remove_background(){
    double background = 0.0;
    int begin = size - size / 8;
    int length = size - begin;
    for(int i = begin; i != size; ++i){
        background += ptr[i];
    }
    background /= double(length);
    for(int i = 0; i != size; ++i){
        ptr[i] -= background;
    }
}

void Data_pre::smooth(){
    double sum;
    for(int i = 10; i != size - 10; ++i){
        sum = 0.0;
        for(int j = -2; j != 3; ++j){
            sum += ptr[i + j];
        }
        ptr[i] = sum / 5.0;
    }
}

void Data_pre::fernald(const std::shared_ptr<double[]> overlap, const std::shared_ptr<double[]> molecule_extinction){
    auto x = std::make_unique<double[]>(size);
    int reference = size / 4;
    int count_t = count % 5;
    extinction_coefficient[count_t][reference] = molecule_extinction[reference];
    for(int i = 0; i != reference; ++i){
        x[i] = ptr[i] / overlap[i] * distance[i] * distance[i];
    }

    double temp1, temp2, temp3;
    double lidar_ratio = 50;
    double molecule_lidar_ratio = 8.0 * 3.1415926 / 3.0;
    extinction_coefficient[count_t][reference] = molecule_extinction[reference];

    for(int i = reference - 1; i != 10; --i){
        temp1 = x[i] * exp((lidar_ratio - molecule_lidar_ratio) * (molecule_extinction[i] + molecule_extinction[i + 1]) * 3.75 / molecule_lidar_ratio);
        temp2 = x[i + 1] / (extinction_coefficient[count_t][i + 1] + lidar_ratio / molecule_lidar_ratio * molecule_extinction[i + 1]);
        temp3 = (x[i + 1] + temp1) * 3.75;
        extinction_coefficient[count_t][i] = temp1 / (temp2 + temp3) - lidar_ratio / molecule_lidar_ratio * molecule_extinction[i];
    }
    double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
    for(int i = 1; i != 1000; ++i){
        if(i < 50){
            sum1 += extinction_coefficient[count_t][i + 30] + extinction_coefficient[count_t][i + 31] + molecule_extinction[i] + molecule_extinction[i - 1];
        }
        else{
            sum1 += extinction_coefficient[count_t][i] + extinction_coefficient[count_t][i - 1] + molecule_extinction[i] + molecule_extinction[i - 1];
            TABC[count_t][i] = (extinction_coefficient[count_t][i] / lidar_ratio + molecule_extinction[i] / molecule_lidar_ratio) * exp(-1.0 * sum1 * 3.75);
            sum2 += TABC[count_t][i];
            sum3 += x[i];
        }
    }
    double lidar_const = sum3 / sum2;
    for(int i = 0; i != 2000; ++i){
        TABC[count_t][i] = x[i] / lidar_const;
    }
    count++;
}

void Data_pre::show(std::ofstream &sout){
    sout.precision(8);
    if(count < 5){
        for(int i = 10; i != 2000; ++i){
        sout<<time<<"  "<<distance[i]<<"  "<<TABC[count][i]<<"  "<<extinction_coefficient[count][i]<<std::endl;
        }
    }else{
        double TABC_out, EXT_COE;
        for(int i = 10; i != 2000; ++i){
            TABC_out = 0.0;
            EXT_COE = 0.0;
            for(int j = 0; j != 5; ++j){
                TABC_out += TABC[j][i];
                EXT_COE += extinction_coefficient[j][i];
            }
            sout<<time<<"  "<<distance[i] / 1000.0<<"  "<<TABC_out / 5.0 * 1000.0<<std::endl;//"  "<<EXT_COE / 5.0<<std::endl;
        }
    }
}

void Data_pre::get_time(const std::string filename){
    int len = filename.length() - 16;
    int hours = std::stoi(filename.substr(len + 7, 2));
    int min = std::stoi(filename.substr(len + 10, 2));
    time = double(hours) + double(min) / 60.0;;
}