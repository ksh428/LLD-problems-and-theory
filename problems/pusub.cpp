#include <bits/stdc++.h>
#include <mutex>
#include <thread>
#include <condition_variable>

using namespace std;

class Message {
    string msg;
public:
    Message(const string& s) : msg(s) {}
    string getmsg() const { return msg; }
};

class Subscriber {
    string name;
public:
    Subscriber(const string& s) : name(s) {}

    void consume(const Message& msg) {
        cout << name << " consuming message "
             << msg.getmsg() << endl;
    }
};

class Topic {
    string name;

    vector<Subscriber*> subscribers; // non-owning

    queue<Message> q;
    mutex mtx;
    condition_variable cv;

    thread worker;
    bool stop = false;

    // worker thread function
    void process() {
        while (true) {
            unique_lock<mutex> lock(mtx);

            cv.wait(lock, [&]() {
                return !q.empty() || stop;
            });

            if (stop && q.empty()) return;

            Message msg = q.front();
            q.pop();

            auto subs_copy = subscribers; // copy under lock
            lock.unlock();

            // process outside lock
            for (auto sub : subs_copy) {
                sub->consume(msg);
            }
        }
    }

public:
    Topic(const string& n) : name(n) {
        worker = thread(&Topic::process, this);
    }

    ~Topic() {
        {
            lock_guard<mutex> lock(mtx);
            stop = true;
        }
        cv.notify_all();
        if (worker.joinable()) worker.join();
    }

    void subscribe(Subscriber* s) {
        lock_guard<mutex> lock(mtx);
        subscribers.push_back(s);
    }

    void unsubscribe(Subscriber* s) {
        lock_guard<mutex> lock(mtx);

        vector<Subscriber*> temp;
        for (auto it : subscribers) {
            if (it != s)
                temp.push_back(it);
        }
        subscribers = temp;
    }

    void publish(const Message& msg) {
        {
            lock_guard<mutex> lock(mtx);
            q.push(msg);
        }
        cv.notify_one();
    }
};

class Pubsubsystem {
    unordered_map<string, unique_ptr<Topic>> topics;
    unordered_map<string, unique_ptr<Subscriber>> subscribers;

    mutex topic_mutex;
    mutex subscriber_mutex;

public:
    void createtopic(const string& name) {
        lock_guard<mutex> lock(topic_mutex);
        if (topics.find(name) != topics.end()) return;
        topics[name] = make_unique<Topic>(name);
    }

    void createsubscriber(const string& name) {
        lock_guard<mutex> lock(subscriber_mutex);
        if (subscribers.find(name) != subscribers.end()) return;
        subscribers[name] = make_unique<Subscriber>(name);
    }

    void subscribe(const string& topic, const string& sub) {
        Topic* t = nullptr;
        Subscriber* s = nullptr;

        {
            lock_guard<mutex> lock(topic_mutex);
            if (topics.find(topic) == topics.end()) return;
            t = topics[topic].get();
        }

        {
            lock_guard<mutex> lock(subscriber_mutex);
            if (subscribers.find(sub) == subscribers.end()) return;
            s = subscribers[sub].get();
        }

        t->subscribe(s);
    }

    void publish(const string& msg, const string& topic) {
        Topic* t = nullptr;

        {
            lock_guard<mutex> lock(topic_mutex);
            if (topics.find(topic) == topics.end()) return;
            t = topics[topic].get();
        }

        Message m(msg);
        t->publish(m);
    }
};

class Publisher {
    string name;
    Pubsubsystem& sys;

public:
    Publisher(const string& s, Pubsubsystem& ps)
        : name(s), sys(ps) {}

    void publish(const string& topic, const string& msg) {
        sys.publish(msg, topic);
    }
};

int main() {
    Pubsubsystem system;

    system.createtopic("news");

    system.createsubscriber("A");
    system.createsubscriber("B");

    system.subscribe("news", "A");
    system.subscribe("news", "B");

    Publisher p("P1", system);

    p.publish("news", "Hello World");
    p.publish("news", "Another Update");

    this_thread::sleep_for(chrono::seconds(1)); // allow worker to process

    return 0;
}