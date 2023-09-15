#include "sbdart.h"


void Sbdart::out_head(std::ofstream &sout){
    sout<<"&INPUT"<<std::endl;
    sout<<"idatm = 2"<<std::endl;
    sout<<"wlinf = 0.25"<<std::endl;
    sout<<"wlsup = 4.0"<<std::endl;
    sout<<"wlinc = 20.0"<<std::endl;
    sout<<"time = "<<time<<std::endl;
    sout<<"alat = "<<alat<<std::endl;
    sout<<"alon = "<<alon<<std::endl;
    sout<<"albcon = "<<albcon<<std::endl;
    sout<<"uw = "<<"0.182698"<<std::endl;
    sout<<"uo3 = "<<"0.3594"<<std::endl;
}

void Sbdart::out_end(std::ofstream &sout){
    sout<<"NSTR = 4"<<std::endl;
    sout<<"zgrid1 = 0.018"<<std::endl;
    sout<<"zgrid2 = 30.00"<<std::endl;
    sout<<"ngrid = 1670"<<std::endl;
    sout<<"zout = 0, 10"<<std::endl;
    sout<<"iout = "<<iout<<std::endl;
    sout<<"/"<<std::endl;
    sout<<std::endl;
}

void Sbdart::out_aer(std::ofstream &sout, Fernald &fernald){
    sout<<"tbaer = "<<tbaer<<std::endl;
    sout<<"zbaer = ";
    fernald.show_distance(sout);
    sout<<"dbaer = ";
    fernald.show_532(sout);
    sout<<"wbaer = "<<wbaer<<std::endl;
    sout<<"gbaer = "<<gbaer<<std::endl;
}

void Sbdart::output_aer(const std::string filename, Fernald &fernald){
    std::ofstream sout;
    sout.open(filename);
    out_head(sout);
    sout<<"iaer = 5"<<std::endl;
    sout<<"wlbaer = "<<wlbaer<<std::endl;
    out_aer(sout, fernald);
    out_end(sout);
    sout.close();
}

void Sbdart::output_clear(const std::string filename){
    std::ofstream sout;
    sout.open(filename);
    out_head(sout);
    sout<<"iaer = 0"<<std::endl;
    sout<<"wlbaer = "<<wlbaer<<std::endl;
    out_end(sout);
    sout.close();
}

