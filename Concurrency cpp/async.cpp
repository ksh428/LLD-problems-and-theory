#include <bits/stdc++.h>
#include <future>
#define ll long long

using namespace std;

ll computeOddSum(ll n) {
    ll sum = 0;
    for (ll i = 1; i <= n; i += 2) {
        sum += i;
    }
    cout << "[Worker] Computed sum of odds up to " << n << endl;
    return sum;
}

int main() {
    ll N = 10000000;

    // Deferred launch: no new thread starts here
    future<ll> fut = async(std::launch::deferred, computeOddSum, N);

    cout << "[Main] Doing some work before asking for result..." << endl;

    // The computation starts *now*, when .get() is called
    ll result = fut.get();

    cout << "[Main] Sum of odd numbers up to " << N << " = " << result << endl;

    return 0;
}
