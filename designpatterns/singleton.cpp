#include <iostream>
#include <thread>
#include <mutex>
// #include <bits/stdc++.h>


using namespace std;

class Logger
{
    static Logger* logger;
    static int instance_count;
    Logger() {
        instance_count++;
        cout << "Logger instance created. Total instances: " << instance_count << endl; 
    } // Private constructor to prevent instantiation
public:
    static Logger* getInstance()
    {
        if(Logger::logger == nullptr)
        {
            logger = new Logger();
        }
        return logger;
    }

    void log(const string& message)
    {
        cout << "Log: " << message << endl;
    }
    void getInstanceCount() const {
        cout << "Current instance count: " << instance_count << endl;
    }
};
Logger* Logger::logger = nullptr; // Initialize static member
int Logger::instance_count = 0; // Initialize static member

void user1()
{
    Logger* logger = Logger::getInstance();
    logger->log("User 1 logging a message.");
}
void user2()
{
    Logger* logger = Logger::getInstance();
    logger->log("User 2 logging a message.");
}

int main()
{
    thread t1(user1);
    thread t2(user2);
    t1.join();
    t2.join();
    cout<< "Creating Logger instance from main thread." << endl;
    // Logger* logger = Logger::getInstance();
    cin.get();
    return 0;
}