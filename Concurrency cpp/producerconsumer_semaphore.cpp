// BOUNDED BUFFER PROBLEM USING SEMAPHORES

// PROBLEM STATEMENT: A producer thread produces data and puts it in a buffer.
// A consumer thread consumes data from the buffer. The producer should not produce data if the buffer is full,
// and the consumer should not consume data if the buffer is empty.

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

// Semaphores:
// empty: tracks number of empty slots in buffer (initially = buffer_size)
// filled: tracks number of filled slots in buffer (initially = 0)
Semaphore empty(10);  // initially 10 empty slots
Semaphore filled(0);  // initially 0 filled slots
mutex mtx;            // for mutual exclusion

void producer(int n) {
    while (n > 0) {
        empty.acquire();  // wait for empty slot
        {
            unique_lock<mutex> lock(mtx);
            dq.push_back(n);
            cout << "Produced: " << n << " (Buffer size: " << dq.size() << ")" << endl;
        }
        filled.release(); // signal that a slot is filled
        n--;
    }
}

void consumer() {
    while (true) {
        filled.acquire();  // wait for filled slot
        int val;
        {
            unique_lock<mutex> lock(mtx);
            val = dq.back();
            dq.pop_back();
            cout << "Consumed: " << val << " (Buffer size: " << dq.size() << ")" << endl;
        }
        empty.release();  // signal that a slot is empty
    }
}

int main() {
    thread t1(producer, 50);
    thread t2(consumer);

    t1.join();
    // Note: consumer thread runs indefinitely, so we don't join it

    return 0;
}
