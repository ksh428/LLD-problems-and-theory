
#include <bits/stdc++.h>
using namespace std;
using namespace chrono;

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

class JobScheduler {
    struct Job {
        int id;
        chrono::steady_clock::time_point executeAt;
        function<void()> task;
    };

    struct Compare {
        bool operator()(const Job& a, const Job& b) {
            return a.executeAt > b.executeAt;
        }
    };

    priority_queue<Job, vector<Job>, Compare> jobs;

    ThreadPool pool;
    thread schedulerThread;

    mutex mtx;
    condition_variable cv;
    bool stop = false;

    void run() {
        while(1)
        {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock,[this]{return !jobs.empty() or stop;});
            if(stop and jobs.empty()) return;
            auto nextTime = jobs.top().executeAt;
            // Wake if an earlier job is submitted
            cv.wait_until(lock, nextTime);
            if (jobs.empty() ||
                jobs.top().executeAt > steady_clock::now())
                continue;

            Job job = jobs.top();
            jobs.pop();

            lock.unlock();

            pool.submit(job.task);

            lock.lock();
        }
    }

public:
    JobScheduler(int n) : pool(n) {
        schedulerThread = thread([this]() {
            run();
        });
    }

    void schedule(int id, int delaySeconds,
                  function<void()> task) {
        {
            lock_guard<mutex> lock(mtx);

            if (stop)
                throw runtime_error("Scheduler stopped");

            jobs.push({
                id,
                steady_clock::now() + seconds(delaySeconds),
                move(task)
            });
        }
        cv.notify_one();
    }

    ~JobScheduler() {
        {
            lock_guard<mutex> lock(mtx);
            stop = true;
        }

        cv.notify_all();
        schedulerThread.join();
        // pool destructor runs afterward, completing
        // already-dispatched tasks
    }
};

int main() {
    JobScheduler scheduler(2);

    scheduler.schedule(1, 2, []() {
        cout << "Executing Job 1\n";
    });

    scheduler.schedule(2, 5, []() {
        cout << "Executing Job 2\n";
    });

    scheduler.schedule(3, 2, []() {
        cout << "Executing Job 3\n";
    });

    this_thread::sleep_for(seconds(7));
}
