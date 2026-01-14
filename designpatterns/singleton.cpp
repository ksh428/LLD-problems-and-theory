#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

class Logger
{
    static Logger* logger;
    static int instance_count;
    static mutex mtx;

    // Private constructor
    Logger() {
        instance_count++;
        cout << "Logger instance created. Total instances: "
             << instance_count << endl;
    }

    // ❌ Delete copy constructor
    Logger(const Logger&) = delete;

    // ❌ Delete copy assignment operator
    Logger& operator=(const Logger&) = delete;

public:
    static Logger* getInstance()
    {
        lock_guard<mutex> lock(mtx);   // RAII-based locking
        if (logger == nullptr)
        {
            logger = new Logger();
        }
        return logger;
    }

    void log(const string& message)
    {
        cout << "Log: " << message << endl;
    }

    static void getInstanceCount()
    {
        cout << "Current instance count: " << instance_count << endl;
    }
};

// Static member definitions
Logger* Logger::logger = nullptr;
int Logger::instance_count = 0;
mutex Logger::mtx;

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

    cout << "Creating Logger instance from main thread." << endl;
    Logger::getInstanceCount();

    cin.get();
    return 0;
}
