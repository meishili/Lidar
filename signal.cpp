#include "signal.h"

Queue::Queue(int s, int n){
    size = s;
    distance = std::make_unique<double[]>(size);
    hours = 0;
    count = 0;
    for(int i = 0; i != n; ++i){
        signal532p.push_back(std::make_unique<double[]>(size));
        signal532s.push_back(std::make_unique<double[]>(size));
        signal1064.push_back(std::make_unique<double[]>(size));
        signal607.push_back(std::make_unique<double[]>(size));
    }
}

bool Queue::isnext(const std::string filename){
    auto h = filename.substr(filename.length() - 16 + 4, 2);
    return std::stoi(h) == hours;
}

void Queue::read_signal(const std::string filename){
    std::ifstream sin;
    sin.open(filename);
    for(int i = 0; i != size; ++i){
        sin>>distance[i]>>signal532p[count][i]>>signal532s[count][i]>>signal1064[count][i]>>signal607[count][i];
    }
    sin.close();
    ++count;
}

void Queue::output(std::unique_ptr<double[]> &altitude, std::unique_ptr<double[]> &p532p, std::unique_ptr<double[]> &p532s, std::unique_ptr<double[]> &p1064, std::unique_ptr<double[]> &p607){
    double sum1, sum2, sum3, sum4;
    double sum = double(count);
    for(int i = 0; i != size; ++i){
        altitude[i] = distance[i];
        sum1 = 0.0;
        sum2 = 0.0;
        sum3 = 0.0;
        sum4 = 0.0;
        for(int j = 0; j != count; ++j){
            sum1 += signal532p[j][i];
            sum2 += signal532s[j][i];
            sum3 += signal1064[j][i];
            sum4 += signal607[j][i];
        }
        p532p[i] = sum1 / sum;
        p532s[i] = sum2 / sum;
        p1064[i] = sum3 / sum;
        p607[i] = sum4 / sum;
    }
}