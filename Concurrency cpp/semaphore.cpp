#include <iostream>
#include <thread>
#include <semaphore>

// THIS IS VALID ONLY IN C++20 AND ABOVE

// AND THIS IS AN ALTERNATE TO MUTEX + CONDITION VARIABLE

using namespace std;

binary_semaphore sem(0); // initially unavailable (0)

void producer() {
    cout << "Producer is working...\n";
    this_thread::sleep_for(chrono::seconds(2));

    cout << "Producer done, releasing semaphore\n";
    sem.release(); // signal consumer
}

void consumer() {
    cout << "Consumer waiting...\n";
    sem.acquire(); // wait until producer releases

    cout << "Consumer received signal!\n";
}

int main() {
    thread t1(producer);
    thread t2(consumer);

    t1.join();
    t2.join();

    return 0;
}