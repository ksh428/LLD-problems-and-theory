#include <bits/stdc++.h>
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

public:
    Topic(const string& n) : name(n) {}

    void subscribe(Subscriber* s) {
        subscribers.push_back(s);
    }

    void unsubscribe(Subscriber* s) {
        vector<Subscriber*> temp;
        for (auto it : subscribers) {
            if (it != s)
                temp.push_back(it);
        }
        subscribers = temp;
    }

    void publish(const Message& msg) {
        for (auto it : subscribers) {
            it->consume(msg);
        }
    }
};

class Pubsubsystem {
    unordered_map<string, unique_ptr<Topic>> topics;
    unordered_map<string, unique_ptr<Subscriber>> subscribers;

public:
    void createtopic(const string& name) {
        if (topics.find(name) != topics.end()) return;
        topics[name] = make_unique<Topic>(name);
    }

    void createsubscriber(const string& name) {
        if (subscribers.find(name) != subscribers.end()) return;
        subscribers[name] = make_unique<Subscriber>(name);
    }

    void subscribe(const string& topic, const string& sub) {
        if (topics.find(topic) == topics.end()) return;
        if (subscribers.find(sub) == subscribers.end()) return;

        topics[topic]->subscribe(subscribers[sub].get());
    }

    void publish(const string& msg, const string& topic) {
        if (topics.find(topic) == topics.end()) return;

        Message m(msg);
        topics[topic]->publish(m);
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

    return 0;
}