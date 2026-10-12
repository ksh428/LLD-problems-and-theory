#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

// here the compiler handles concurrency of the logger instance if declared with 
// static keyword, while compiling it adds the locks as required and makes sure its created only once


class Logger
{
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
public:
    static Logger* getInstance()
    {
        static Logger* instance; 
        return instance;
    }
    void log(const string& message)
    {
        cout << "Log: " << message << endl;
    }
};

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
    cin.get();
    return 0;
}
