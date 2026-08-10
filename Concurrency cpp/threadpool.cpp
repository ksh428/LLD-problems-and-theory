#include <bits/stdc++.h>
using namespace std;

class ThreadPool {
    vector<thread> workers;
    queue<function<void()>> tasks;
    mutex mtx;
    condition_variable cv;
    bool stop = false;
public:
    ThreadPool(int n) {
        for (int i = 0; i < n; i++) {
            workers.emplace_back([this]() {
                while (true) {
                    function<void()> task;
                    {
                        unique_lock<mutex> lock(mtx);
                        cv.wait(lock, [this]() {
                            return stop || !tasks.empty();
                        });
                        // Shutdown condition
                        if (stop && tasks.empty())
                            return;
                        task = move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }
    void submit(function<void()> task) {
        {
            lock_guard<mutex> lock(mtx);

            if (stop)
                throw runtime_error("ThreadPool is stopped");

            tasks.push(move(task));
        }
        cv.notify_one();
    }
    ~ThreadPool() {
        {
            lock_guard<mutex> lock(mtx);
            stop = true;
        }
        cv.notify_all(); //this is needed for shutdown
        for (auto &worker : workers) {
            worker.join();
        }
    }
};

int main() {
    ThreadPool pool(4);
    for (int i = 0; i < 10; i++) {
        pool.submit([i]() {
            cout << "Executing task " << i
                 << " on thread "
                 << this_thread::get_id()
                 << endl;
        });
    }
}