#include "licel.h"

Licel::Licel(const int s){
    size = s;
    head_info = std::make_unique<head[]>(size);
    ptr = std::make_unique<std::unique_ptr<int[]>[]>(size);
    for(int i = 0; i < size; i++){
        ptr[i] = std::make_unique<int[]>(16380);
    }
}

Licel::~Licel(){
    size = 0;
}

bool Licel::open_file(){
    fin.open(filename, std::ios::binary);
    if(fin.is_open()){
        std::cout<<filename<<" is open!"<<std::endl;
        return true;
    }
    else{
        return false;
    }
}

bool Licel::close_file(){
    fin.close();
    return true;
}

void Licel::set_filename(const std::string &str){
    filename = str;
}

double Licel::get_time(){
    std::string temp;
    int len = filename.length() - 16;
    temp = filename.substr(len + 5,2);
    day = std::stoi(temp);
    temp = filename.substr(len + 7,2);
    hours = std::stoi(temp);
    temp = filename.substr(len + 10, 2);
    min = std::stoi(temp);
    time = double(day) + double(hours) / 24.0 + double(min) / 24.0 / 60.0;
    return time;
}

bool Licel::read_head(){
    std::string temp;
    read_line(temp);
    read_line(temp);
    read_line(temp);
    for(int i = 0; i < size; i++){
        read_line(temp);
        std::string s = temp.substr(3,1);
        if(s == "0"){
            head_info[i].mode = Analog;
        }else if(s == "1"){
            head_info[i].mode = Photon;
        }
        s = temp.substr(47,6);
        head_info[i].shot_num = std::stoi(s);
        s = temp.substr(54,5);
        head_info[i].ana_inputrange = std::stod(s);
        s = temp.substr(44,2);
        head_info[i].adc_num = std::stoi(s);
        s = temp.substr(20,4);
        head_info[i].distinction = std::stod(s);
    }
    read_line(temp);
    return true;
}

void Licel::read_line(std::string &temp){
    char prevChar = '\0';
    temp.clear();
    while(fin.peek() != EOF){
        char currentChar =fin.get();
        if(prevChar == '\r' && currentChar == '\n'){
            break;
        }else if(currentChar != '\r' && currentChar != '\n'){
            temp += currentChar;
        }
        prevChar = currentChar;
    }

}

void Licel::read_dataset(){
    std::string temp;
    for(int i = 0; i < size; i++){
        temp.clear();
        fin.read(reinterpret_cast<char*>(ptr[i].get()), 4 * 16380);
        char c;
        for(int i = 0; i < 2; i++){
            fin.get(c);
            temp += c;
        }
        if(temp != "\r\n"){
            std::cout<<"read error!"<<std::endl;
            break;
        }
    }
}

void Licel::analog(std::unique_ptr<double[]> &p, const int n){
    for(int i = 0; i < 16380; i++){
        p[i] = ptr[n][i] / head_info[n].shot_num * head_info[n].ana_inputrange * 1000.0 / pow(2.0, head_info[n].adc_num);
    }
}

void Licel::photon(std::unique_ptr<double[]> &p, const int n){
    for(int i = 0; i < 16380; i++){
        p[i] = ptr[n][i] / head_info[n].shot_num * 150.0 / head_info[n].distinction;
    }
}

void Licel::save_txt(const std::string &str){
    std::ofstream fout;
    fout.open(str);
    std::unique_ptr<std::unique_ptr<double[]>[]> p= std::make_unique<std::unique_ptr<double[]>[]>(size);
    for(int i = 0; i < size; i++){
        p[i] = std::make_unique<double[]>(16380);
        if(head_info[i].mode == Analog){
            analog(p[i], i);
        }else if(head_info[i].mode == Photon){
            photon(p[i], i);
        }
    }
    for(int i = 0; i < 8000; i++){
        for(int j = 0; j < size; j++){
            fout.width(10);
            fout<<p[j][i]<<"  ";
        }
        fout<<std::endl;
    }
    fout.close();
}

void Licel::get_dataset(std::unique_ptr<std::unique_ptr<double[]>[]> &p, int n){
    for(int i = 0; i < size; i++){
        p[i] = std::make_unique<double[]>(16380);
        if(head_info[i].mode == Analog){
            analog(p[i + n], i);
        }else if(head_info[i].mode == Photon){
            photon(p[i + n], i);
        }
    }
}