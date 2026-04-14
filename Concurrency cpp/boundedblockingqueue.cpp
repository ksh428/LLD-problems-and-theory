#include <bits/stdc++.h>
#include <mutex>
#include <condition_variable>

using namespace std;

class Semaphore{
    mutex mtx;
    int count;
    condition_variable cv;
    public:
    Semaphore(int pc):count(pc) {}
    void acquire()
    {
        unique_lock<mutex>lock(mtx);
        cv.wait(lock,[]{return count>0;});
        count--;
    }
    void release()
    {
        unique_lock<mutex>lock(mtx);
        count++;
        cv.notify_one();
    }
};

class BoundedBlockingQueue{
    queue<int>q;
    int capacity;
    Semaphore empty;
    mutex mtx;
    Semaphore full;
    public:
    BoundedBlockingQueue(int c):capacity(c),empty(capacity),full(0) {}
    void enqueue(int e)
    {
        empty.acquire();
        lock_guard<mutex>lock(mtx);
        q.push(ele);
        full.release();
    }
    int dequeue()
    {
        full.acquire();
        int val;
        {
        lock_guard<mutex>lock(mtx);
        val=q.front();
        q.pop();
        }
        empty.release();
        return val;
    }
};

int main() {
    
}