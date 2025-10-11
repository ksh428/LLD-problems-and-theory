#include <bits/stdc++.h>
#include <mutex>
using namespace std;

// ---------------- Level hierarchy ----------------
class Level {
public:
    virtual int getlevel() = 0;
    virtual ~Level() = default;
};

class Error : public Level {
public:
    int getlevel() override {
        return 2;
    }
};

class Warning : public Level {
public:
    int getlevel() override {
        return 1;
    }
};

class Debug : public Level {
public:
    int getlevel() override {
        return 0;
    }
};

// ---------------- Logrule ----------------
class Logrule {
    int levels; // bitmask of levels to log
public:
    Logrule(int r) : levels(r) {}
    int getlevels() { return levels; }
};

// ---------------- Logstruct ----------------
class Logstruct {
    Level* l;
    string msg;
    string timestamp;
public:
    Logstruct(Level* l1, string m, string t) {
        l = l1;
        msg = m;
        timestamp = t;
    }

    Level* getlevel1() { return l; }
    string getmsg() { return msg; }
    string gettimestamp() { return timestamp; }
};

// ---------------- Logdestination ----------------
class Logdestination {
public:
    bool filter(Level* l, vector<Logrule*> rules) {
        int l0 = l->getlevel();
        for (auto it : rules) {
            int l1 = it->getlevels();
            if (l1 & (1 << l0)) {
                return true;  // allowed to log
            }
        }
        return false;
    }

    virtual string getdestination() = 0;
    virtual void logg(Logstruct* l, vector<Logrule*> rules) = 0;
    virtual ~Logdestination() = default;
};

// ---------------- Filedestination ----------------
class Filedestination : public Logdestination {
    string filepath = "dummy_file_path";
public:
    string getdestination() override {
        return "file";
    }

    void logg(Logstruct* l, vector<Logrule*> rules) override {
        if (!filter(l->getlevel1(), rules)) {
            return;
        }
        cout << "Logging to file : " << filepath << endl;
        cout << "Line added to file: "
             << l->getlevel1()->getlevel() << " "
             << l->getmsg() << " " << l->gettimestamp() << endl;
    }
};

// ---------------- Portaldestination ----------------
class Portaldestination : public Logdestination {
public:
    string getdestination() override {
        return "portal";
    }

    void logg(Logstruct* l, vector<Logrule*> rules) override {
        if (!filter(l->getlevel1(), rules)) {
            return;
        }
        cout << "Logging to portal..." << endl;
        cout << "Line added to portal: "
             << l->getlevel1()->getlevel() << " "
             << l->getmsg() << " " << l->gettimestamp() << endl;
    }
};

// ---------------- Terminaldestination ----------------
class Terminaldestination : public Logdestination {
public:
    string getdestination() override {
        return "terminal";
    }

    void logg(Logstruct* l, vector<Logrule*> rules) override {
        if (!filter(l->getlevel1(), rules)) {
            return;
        }
        cout << "Logging to terminal..." << endl;
        cout << "Line added to terminal: "
             << l->getlevel1()->getlevel() << " "
             << l->getmsg() << " " << l->gettimestamp() << endl;
    }
};

// ---------------- Loggerfactory ----------------
class Loggerfactory {
public:
    static Logdestination* getlogger(string dst) {
        if (dst == "file")
            return new Filedestination();
        else if (dst == "portal")
            return new Portaldestination();
        else
            return new Terminaldestination();
    }
};

// ---------------- Config ----------------
class Config {
    static string configdata;
public:
    Config(string data) { configdata = data; }
    static string getdata() { return configdata; }
};
string Config::configdata = "7"; // bitmask: enable all levels

// ---------------- Logger (Singleton) ----------------
class Logger {
    static Logger* loggerinstance;
    Logdestination* loggertype;
    static mutex mtx;
    vector<Logrule*> rules;

    // private constructor
    Logger() {
        getrules();
    }

    void getrules() {
        string data = Config::getdata();
        int r = stoi(data);
        Logrule* rule = new Logrule(r);
        rules.push_back(rule);
    }

public:
    static Logger* getlogger() {
        lock_guard<mutex> lock(mtx);
        if (loggerinstance == nullptr) {
            loggerinstance = new Logger();
        }
        return loggerinstance;
    }

    static void setlogertype(string type) {
        if (loggerinstance == nullptr)
            loggerinstance = new Logger();
        loggerinstance->loggertype = Loggerfactory::getlogger(type);
    }

    void logg(string msg, Level* l) {
        if (!loggertype) {
            cout << "Logger type not set!\n";
            return;
        }
        Logstruct* lo = new Logstruct(l, msg, "12am");
        loggertype->logg(lo, rules);
        delete lo; // cleanup
    }

    ~Logger() {
        delete loggertype;
        for (auto r : rules) delete r;
    }
};

// static members
Logger* Logger::loggerinstance = nullptr;
mutex Logger::mtx;

// ---------------- main ----------------
int main() {
    Logger::setlogertype("terminal");
    Logger* logger = Logger::getlogger();

    Error e;
    Warning w;
    Debug d;

    logger->logg("Critical system failure", &e);
    logger->logg("Low memory warning", &w);
    logger->logg("Debug info here", &d);

    return 0;
}
