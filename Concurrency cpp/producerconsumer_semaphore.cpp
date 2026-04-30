// BOUNDED BUFFER PROBLEM USING SEMAPHORES

// PROBLEM STATEMENT: A producer thread produces data and puts it in a buffer.
// A consumer thread consumes data from the buffer. The producer should not produce data if the buffer is full,
// and the consumer should not cons

#include <bits/stdc++.h>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

// Counting Semaphore implementation
class Semaphore {
private:
    mutex mtx;
    condition_variable cv;
    int count;

public:
    Semaphore(int init = 0) : count(init) {}

    void acquire() {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this]() { return count > 0; });
        count--;
    }

    void release() {
        unique_lock<mutex> lock(mtx);
        count++;
        cv.notify_one();
    }
};

int buffer_size = 10;
deque<int> dq;

// Semaphores
Semaphore emptySlots(10);   // renamed from empty
Semaphore filled(0);

mutex mtx;

void producer(int n) {
    while (n > 0) {
        emptySlots.acquire();

        {
            unique_lock<mutex> lock(mtx);
            dq.push_back(n);
            cout << "Produced: " << n
                 << " (Buffer size: " << dq.size() << ")" << endl;
        }

        filled.release();
        n--;
    }
}

void consumer() {
    while (true) {
        filled.acquire();

        int val;
        {
            unique_lock<mutex> lock(mtx);
            val = dq.back();
            dq.pop_back();
            cout << "Consumed: " << val
                 << " (Buffer size: " << dq.size() << ")" << endl;
        }

        emptySlots.release();
    }
}

int main() {
    thread t1(producer, 50);
    thread t2(consumer);

    t1.join();

    // consumer runs forever, so detach it
    t2.detach();

    return 0;
}