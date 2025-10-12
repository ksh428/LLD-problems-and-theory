// Online C++ compiler to run C++ program online
#include <bits/stdc++.h>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
using namespace std;

mutex mtx;
condition_variable cv;
deque<int>dq;
int buffer=10;

void producer(int n)
{
    while(n>0)
    {
        unique_lock<mutex>lock(mtx);
        cv.wait(lock,[]{return dq.size()<buffer;});
        dq.push_back(n);
        n--;
        lock.unlock();
        cv.notify_one();
    }
}
void consumer()
{
    while(1)
    {
        unique_lock<mutex>lock(mtx);
        cv.wait(lock,[]{return dq.size()>0;});
        int val=dq.back();
        dq.pop_back();
        cout<<"consumed value "<<val<<endl;
        lock.unlock();
        cv.notify_one();
    }
}


int main() {
    thread t1(producer,50);
    thread t2(consumer);
    t1.join();
    t2.join();
}