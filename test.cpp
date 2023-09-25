#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <mutex>

std::mutex mtx;

class Count{
    private:
        int count;
    public:
        Count(){count = 0;}
        void add(){count++;}
        void show(){std::cout<<"count = "<<count<<std::endl;}
};

void work(std::vector<std::shared_ptr<Count>> ptr){
    for(int i = 0; i != 10000; ++i){
        mtx.lock();
        ptr[0]->add();
        mtx.unlock();
    }
}

int main(int argc, char *argv[]){
    std::vector<std::shared_ptr<Count>> count;
    count.push_back(std::make_shared<Count>());
    std::vector<std::thread> thr;
    for(int i = 0; i != 12; ++i){
        thr.push_back(std::thread(work, count));
    }
    for(auto i = thr.begin(); i != thr.end(); ++i){
        i->join();
    }
    count[0]->show();
    return 0;
}