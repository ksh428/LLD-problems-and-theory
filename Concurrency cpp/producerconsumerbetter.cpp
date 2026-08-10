#include<bits/stdc++.h>
#include <thread>
#include <condition_variable>

using namespace std;
deque<int>buffer;
int max_size=2;
mutex mtx;
condition_variable not_full;
condition_variable not_empty;

void producer(int n)
{
    while(n)
    {
        // the scope brackets are an alternate to unlock, although not needed its good to scope the critical section
        {
            unique_lock<mutex>lock(mtx);
            not_full.wait(lock,[&] {return buffer.size()<max_size;});
            buffer.push_front(n);
            n--;
        }
        not_empty.notify_one();
    }
}
void consumer()
{
    while(1)
    {
        int val;
        {
            unique_lock<mutex>lock(mtx);
            not_empty.wait(lock,[&] {return buffer.size()>0;});
            val=buffer.back();
            buffer.pop_back();
        }
        cout<<val<<endl;
        not_full.notify_one();
    }
}
int main()
{
    thread t1(producer,10);
    thread t2(consumer);
    t1.join();
    t2.join();

}