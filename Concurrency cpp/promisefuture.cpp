#include <bits/stdc++.h>
#include <thread>
#include <future>
#define ll long long

using namespace std;

void computeOddSum(promise<ll> pr)  // take by value, since we move it
{
    ll os = 0;
    for (int i = 1; i < 10000000; i++) {
        if (i & 1) os += i;
    }
    pr.set_value(os);  // set result
}

int main() {
    promise<ll> pr;                  // create promise
    future<ll> fut = pr.get_future(); // get associated future

    thread t1(computeOddSum, move(pr)); // move promise into thread

    cout << "Odd sum val: " << fut.get() << endl; // wait & get result

    t1.join(); // join thread
}
