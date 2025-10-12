// Online C++ compiler to run C++ program online
#include <bits/stdc++.h>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
using namespace std;

mutex mtx;
condition_variable cv;

int main() {
    int glo=0;
    int notified=0;
    thread consumer([&]{
        unique_lock<mutex>lock(mtx);
        if(!notified)
        {
            cout<<"consumer is sleeping"<<endl;
            cv.wait(lock);
        }
        cout<<"consumer result "<<glo<<endl;
    });
    thread producer([&]{
        unique_lock<mutex>lock(mtx);
        glo=90;
        notified =1;
        this_thread::sleep_for(chrono::seconds(5));
        cout<<"producer complete"<<endl;
        cv.notify_one(); // notify one of the threads that the lock is unlocked
    });
    consumer.join();
    producer.join();
}