
#include <bits/stdc++.h>
using namespace std;

mutex mtx;
condition_variable notfullcv;
condition_variable notemptycv;

int buffer=10;
bool done=false;
deque<int>dq;

void producer(int n)
{
    while(n)
    {
        {
            unique_lock<mutex>lock(mtx);
            notfullcv.wait(lock,[&] {return dq.size()<buffer;});
            dq.push_back(n);
        }
        n--;
        notemptycv.notify_one();
    }
}

void consumer()
{
    while(1)
    {
        int val;
        {
            unique_lock<mutex>lock(mtx);
            notemptycv.wait(lock,[&] {return dq.size()>0 or done ;});
            if(dq.empty() and done) break;
            val=dq.front();
            dq.pop_front();
        }
        cout<<val;
        notfullcv.notify_one();
    }
}

int main()
{
    thread t1(consumer,1000);
    thread t2(producer);
    t1.join();
    {
        lock_guard<mutex>lock(mtx);
        done=true;
    }
    notemptycv.notify_all();
    t2.join();
}
