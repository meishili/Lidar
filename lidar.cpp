#include "lidar.h"

Lidar::Lidar() : size(0)
{
    p532p = nullptr;
    p532s = nullptr;
    p1064 = nullptr;
    p607 = nullptr;
    distance = nullptr;
    overlap = nullptr;
    molecule = nullptr;
    extinction = nullptr;
}

Lidar::Lidar(const int n) : size(n)
{
    p532p = new double[size];
    p532s = new double[size];
    p1064 = new double[size];
    p607 = new double[size];
    distance = new double[size];
    overlap = new double[size];
    molecule = new double[size];
    extinction = new double[size];
}

Lidar::~Lidar()
{
    delete[] p532p;
    delete[] p532s;
    delete[] p1064;
    delete[] p607;
    delete[] distance;
    delete[] overlap;
    delete[] molecule;
    delete[] extinction;
}

void Lidar::read_overlap(const char *s)
{
    std::ifstream sin;
    sin.open(s);
    for(int i = 0; i < size; i++)
        sin>>*(overlap + i);
    sin.close();
}

void Lidar::read_molecule(const char *s)
{
    std::ifstream sin;
    sin.open(s);
    for(int i = 0; i < size; i++)
    {
        sin>>*(molecule + i);
        *(molecule + i) /= 1000.0;
    }
    sin.close();
}

void Lidar::remove_background()
{
    double temp532p = 0.0;
    double temp532s = 0.0;
    double temp1064 = 0.0;
    double temp607 = 0.0;
    for(int i = size - 1000; i < size; i++)
    {
        temp532p += *(p532p + i);
        temp532s += *(p532s + i);
        temp1064 += *(p1064 + i);
        temp607 += *(p607 + i);
    }
    temp532p /= 1000.0;
    temp532s /= 1000.0;
    temp1064 /= 1000.0;
    temp607 /= 1000.0;

    for(int i = 0; i < size; i++)
    {
        *(p532p + i) -= temp532p;
        *(p532s + i) -= temp532s;
        *(p1064 + i) -= temp1064;
        *(p607 + i) -= temp607;
    }
}

void Lidar::read_signal(const char *s)
{
    std::ifstream sin;
    sin.open(s);
    for(int i = 0; i < size; i++)
    {
        sin>>*(distance + i)>>*(p532p + i)>>*(p532s + i)>>*(p1064 + i)>>*(p607 + i);
    }
    sin.close();
}

void Lidar::fernald()
{
    double x, x1;
    double temp1, temp2, temp3;
    *(extinction + 2400) = *(molecule + 2400);
    x = (*(p532p + 2400) + *(p532s + 2400) / depolarization_correct) / *(overlap + 2400) * pow(*(distance + 2400), 2);
    for(int i = 2399; i> 30; i--)
    {
        x1 = x;
        x = (*(p532p + i) + *(p532s + i) / depolarization_correct) / *(overlap + i)
        * pow(*(distance + i), 2);
        temp1 = x * exp((50.0 - molecule_lidar_ratio) * (*(molecule + i) + *(molecule + i + 1)) / molecule_lidar_ratio * 3.75);
        temp2 = x / (*(extinction + i + 1) + 50.0 / molecule_lidar_ratio * *(molecule + i + i));
        temp3 = (x1 + temp1) * 3.75;
        *(extinction + i) = temp1 / (temp2 + temp3) - 50.0 / molecule_lidar_ratio * *(molecule + i);
    }
}

void Lidar::show(std::ofstream &sout)
{
    double VDR, PDR, color_ratio, range_corrected_signal;
    for(int i = 30; i < 2400; i++)
    {
        VDR = *(p532s + i) / *(p532p + i) / depolarization_correct;
        color_ratio = *(p1064 + i) / (*(p532p + i) + *(p532s + i) / depolarization_correct) / color_ratio_correct;
        range_corrected_signal = log((*(p532p + i) + *(p532s + i) / depolarization_correct) / *(overlap + i)
        * pow(*(distance + i), 2));
        double R = (*(molecule + i) + *(extinction + i)) / *(molecule + i);
        PDR = ( (1.0 + molecule_depolarization_ratio) * VDR * R - (1.0 + VDR) * molecule_depolarization_ratio ) / 
        ( (1.0 + molecule_depolarization_ratio) * R - (1.0 + VDR) );

        sout.width(8);
        sout<<*(distance + i);
        sout.width(2);
        sout<<" ";

        sout.width(10);
        sout<<range_corrected_signal;
        sout.width(2);
        sout<<" ";

        sout.width(10);
        sout<<PDR;
        sout.width(2);
        sout<<" ";

        sout.width(10);
        sout<<color_ratio;
        sout.width(2);
        sout<<" ";

        sout.width(10);
        sout<<*(extinction + i);
        sout<<std::endl;
    }
}

void Lidar::photon_to_analog()
{
    for(int i = 0; i < size; i++)
        *(p532s + i) = -370.47693 + 376.54905 * exp(0.0000903459 * *(p532s + i) / (1.0 - 0.00213 * *(p532s + i)));
}