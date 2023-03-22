#include "time.h"

Time::Time(const std::string &p)
{
    path = p;
    std::string temp = "ls " + path + " >dir.txt";
    system(temp.c_str());
    sin.open("dir.txt");
}

std::string Time::get_filename()
{
    sin>>filename;
    return (path + "/" + filename);
}

double Time::get_time()
{
    std::string temp;
    temp = filename.substr(0,4);
    year = stoi(temp);
    temp = filename.substr(4,2);
    month = stoi(temp);
    temp = filename.substr(6,2);
    day = stoi(temp);
    temp = filename.substr(8,2);
    hours = stoi(temp);
    temp = filename.substr(10,2);
    min = stoi(temp);
    double time;
    time = double(day) + hours / 24.0 + min / 60.0 / 24.0;
    return time;
}

void Time::reset(const std::string &p)
{
    path = p;
    sin.close();
    std::string temp = "ls " + path + " >dir.txt";
    system(temp.c_str());
    sin.open("dir.txt");
}

bool Time::operator<(const date &t)
{
    long temp1, temp2;
    temp1 = day + month * 100 + year * 10000;
    temp2 = t.day + t.month * 100 + t.year * 10000;
    if(temp1 < temp2)
        return true;
    else if(temp1 > temp2)
        return false;
    else
    {
        temp1 = min + hours * 100;
        temp2 = t.min + t.hours * 100;
        if(temp1 < temp2)
            return true;
        else
            return false;
    }
}

bool Time::operator==(const date &t)
{
    return (year == t.year && month == t.month && 
    day == t.day && hours == t.hours && min == t.min);
}

bool Time::operator>(const date &t)
{
    return (! (*this < t));
}