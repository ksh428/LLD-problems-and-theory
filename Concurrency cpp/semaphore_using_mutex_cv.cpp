#include <bits/stdc++.h>
#include <mutex>
#include <condition_variable>

using namespace std;

class Binary_semaphore{
    mutex mtx;
    bool flag;
    condition_variable cv;
    public:
    Binary_semaphore(int init=0)
    {
        flag=(init==1);
    }
    void acquire()
    {
        unique_lock<mutex>lock(mtx);
        cv.wait(lock,[]{return flag;});
        flag=false;
    }
    void release()
    {
        unique_lock<mutex>lock(mtx);
        flag=true;
        cv.notify_one();
    }
};

int main() {
    
}