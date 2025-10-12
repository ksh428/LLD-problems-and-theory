// Online C++ compiler to run C++ program online
#include <bits/stdc++.h>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
using namespace std;

mutex mtx;
condition_variable cv;
int money=0;

void addmoney(int m)
{
    unique_lock<mutex>lock(mtx);
    money+=m;
    cout<<"money added, balance "<<money<<endl;
    cv.notify_one();// notify the waiting thread that the mutex is unlocked now
}
void withdrawmoney(int m)
{
    unique_lock<mutex>lock(mtx);
    // wait() first unlocks the mutex and checks for the condition mentioned
    // if the condition is true, it relocks it and executes the following lines
    // if the condition is false, it keeps it open and sleeps the current thread so that the addmoney thread can acuire and add the money
    // once the addmoney thread has added the money it notifies the sleeping withdrawmoney thread
    // once notified it acquires the lock again and checks for the condition again
    // now the condtion is true so it executes the following lines
    cv.wait(lock,[&]{ return (money>=m) ? true:false; });
    cout<<"withdrawing money"<<endl;
    money-=m;
    cout<<"current balance "<<money<<endl;
}

int main() {
    thread t1(withdrawmoney,50);
    thread t2(addmoney,100);
    t1.join();
    t2.join();
}