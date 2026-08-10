#include<bits/stdc++.h>
#include <thread>
#include <condition_variable>

// THIS VARIANT PRODUCES ALL X ELEMENTS AT ONCE

using namespace std;
deque<int>buffer;
int max_size=100;
mutex mtx;
condition_variable cv;

void producer(int x){ // producer will try to add x elements
    unique_lock<mutex>lock(mtx);
    cv.wait(lock,[&] {return buffer.size()+x<max_size;});
    while(x--)
    {
        buffer.push_front(x);
    }
    cv.notify_one();
}
void consumer()
{
    unique_lock<mutex>lock(mtx);
    cv.wait(lock,[&] {return buffer.size()>0;});
    int val=buffer.back();
    cout<<val<<endl;
    buffer.pop_back();
    cv.notify_one();
}

int main()
{

}