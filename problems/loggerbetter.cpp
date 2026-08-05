#include<bits/stdc++.h>
#include <mutex>
using namespace std;

// enum -> log level , class log destination ->factory, class log syste -> singleton

// logger should suport logging at various levels for various destination
// you should be able to create log rules for filtering what to log

//log level, log line, timestamp

enum Level{
    WARNING,
    ERROR,
    DEBUG
};
class Logstruct{
    Level l;
    string line;
    string time;
    public:
    Logstruct(Level ll,string pl,string pt):l(ll),line(pl),time(pt){}
    void displayline()
    {
        vector<string>v={"WARNING","ERROR","DEBUG"};
        cout<<v[l]<<" "<<line<<" "<<time<<endl;
    }
    Level getlevel() {return l;}
};
class Logrule{
    int mask;
    public:
    Logrule(int m):mask(m) {}
    bool isblocked(Logstruct logline)
    {
        Level l=logline.getlevel();
        if(l==ERROR and mask&(1<<0)) return 1;
        if(l==WARNING and mask&(1<<1)) return 1;
        if(l==DEBUG and mask&(1<<2)) return 1;
        return 0;
    }
    int getrule() {return mask;}
};
class Config{
    string filename="";
    string filepath="";
    string uri="";
    Logrule* r;
    public:
    Config(string s1,string s2,string s3,int mask){
        filename=s1;
        filepath=s2;
        uri=s3;
        r=new Logrule(mask);
    }
    string getfilename() {return filename;}
    string getfilepath() {return filepath;}
    string geturi() {return uri;}
    Logrule* getrule() {return r;}
};
class Utils{
    public:
    static Logstruct getlogline(string s1,string s2,string s3)
    {
        Level l;
        if(s1=="error") l=ERROR;
        else if(s1=="warn") l=WARNING;
        else l=DEBUG;
        Logstruct ans=Logstruct(l,s2,s3);
        return ans;
    }
};
class LogDestination{
    public:
    virtual string getdestination()=0;
    virtual ~LogDestination()=default;
    virtual void log(Logstruct logline)=0;
};
class Filelogger:public LogDestination{
    Config* cfg;
    public:
    Filelogger(Config* c):cfg(c) {}
    void log(Logstruct l)
    {
        cout<<"File logger "<<endl;
        cout<<"Logging to file "<<cfg->getfilename()<<" "<<cfg->getfilepath()<<endl;
        l.displayline();
    }
    string getdestination() { return "file destination";}
};
class Otellogger:public LogDestination{
    Config* cfg;
    public:
    Otellogger(Config* c):cfg(c) {}
    void log(Logstruct l)
    {
        cout<<"otel logger"<<endl;
        cout<<"loging to ui "<<cfg->geturi();
        l.displayline();
    }
    string getdestination() {return "otel";}
};
class Terminal:public LogDestination{
    Config* cfg;
    public:
    Terminal(Config* c) :cfg(c) {}
    void log(Logstruct l)
    {
        cout<<"logging to terminal"<<endl;
        l.displayline();
    }
    string getdestination() {return "Terminal";}
};
class Logfactory{
    public:
    static LogDestination* getlogdest(string dest, Config* c)
    {
        if(dest=="terminal") return new Terminal(c);
        if(dest=="file") return new Filelogger(c);
        return new Otellogger(c);
    }
};
class Logger{
    static Logger* loggerinstance;
    static mutex mtx;
    Config *c;
    Logger& operator=(const Logger&)=delete;
    Logger(const Logger&)=delete;
    LogDestination* dest;
    Logger(string d,Config *c){
        this->c=c;
        dest=Logfactory::getlogdest(d,c);
    }
    public:
    static Logger* createlogger(string d,Config* c)
    {
        lock_guard<mutex>lock(mtx);
        if(!loggerinstance)
        {
            loggerinstance=new Logger(d,c);
        }
        return loggerinstance;
    }
    void log(string level,string line,string time)
    {
        Logstruct l=Utils::getlogline(level,line,time);
        Logrule* lr=c->getrule();
        if(lr->isblocked(l))
        {
            return ;
        }
        dest->log(l);
    }
};

Logger* Logger::loggerinstance=nullptr;
mutex Logger::mtx;
int main()
{
    Config* c= new Config("firewallonfig","d//data//fwc","www.otel.com",2);
    Logger* logger=Logger::createlogger("file",c);
    logger->log("error","firewallconfig crash","00:09");

}
// IMPROVEMENTS:

// DESTINATION SPECIFIC CONFIG, SO THE DEST HAS ONLY THE NEED INFO
// FILECONFIG,OTELCONFIG ...