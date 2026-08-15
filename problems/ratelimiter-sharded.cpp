#include<bits/stdc++.h>
#include<chrono>
using namespace std;

// tier, user, abstract rate limiter, concrete rate limiter, rate limiter config

enum TIER{
    FREE,
    PREMIUM
};

class User{
    string id;
    TIER t;
    public:
    User(string i,TIER tt):id(i),t(tt) {}
    string getId() const {
        return id;
    }

    TIER getTier() const {
        return t;
    }
};

class Ratelimiterconfig{
    int maxreq;
    int window;
    public:
    Ratelimiterconfig(int m,int w):maxreq(m),window(w){}
    int getnumreq() {return maxreq;}
    int getwin() {return window;}
};
class RateLimiter{
    protected:
    Ratelimiterconfig* cfg;
    public:
    RateLimiter(Ratelimiterconfig* c):cfg(c){}
    virtual bool allowrequest(const string& id)=0;
    virtual ~RateLimiter()=default;
};
class Tokenbucket:public RateLimiter{
    struct  Userstate{
        double tokens = 0;
        long long lastrefill=chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now().time_since_epoch()).count();
    };
    struct Shard{
        map<string,Userstate>users;
        mutex mtx;
    };
    vector<Shard*> shards;
    int getshardindex(string id)
    {
        return hash<string>{}(id)%shards.size();
    }
    public:
    Tokenbucket(Ratelimiterconfig* c,int numshards):RateLimiter(c) {
        shards.reserve(numshards);
        for(int i=0;i<numshards;i++)
        {
            shards.push_back(new Shard());
        }
    }
    bool allowrequest(const string& id){

        // find the shard for this user
        int index=getshardindex(id);
        Shard& sh=*shards[index];
        lock_guard<mutex>lock(sh.mtx);
        long long now = chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now().time_since_epoch()).count();
        auto it=sh.users.find(id);
        if(it==sh.users.end())
        {
            Userstate us;
            us.tokens=cfg->getnumreq();
            us.lastrefill=now;
            sh.users.emplace(id,us);
        }
        Userstate& state=sh.users[id];
        int win=cfg->getwin();
        int numreq=cfg->getnumreq();
        double refillrate =static_cast<double>(cfg->getnumreq())/ cfg->getwin();
        long long elapsed=now-state.lastrefill;
        double newtokens=state.tokens+elapsed*refillrate;
        state.tokens=min(newtokens,(double)cfg->getnumreq());
        state.lastrefill=now;
        if(state.tokens)
        {
            cout<<"Request allowed "<<id<<endl;
            state.tokens-=1.0;
            return 1;
        }
        cout<<"rate limited "<<id<<endl;
        return 0;
    }
};

class Fixedwindow:public RateLimiter{
    struct Userstate{
        long long windowstart=-1;
        int reqcount=0;
    };
    struct Shard{
        map<string,Userstate>users;
        mutex mtx;
    };
    vector<Shard*> shards;
    int getshardindex(string id)
    {
        return hash<string>{}(id)%shards.size();
    }
    public:
    Fixedwindow(Ratelimiterconfig* c,int numshards):RateLimiter(c) {
        for(int i=0;i<numshards;i++){
            shards.push_back(new Shard());
        }
    }

    bool allowrequest(const string& id){
       int index=getshardindex(id);
       Shard& sh=*shards[index];
       lock_guard<mutex>lock(sh.mtx);
       long long now =chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now().time_since_epoch()).count();
       long long winsize=cfg->getwin();
       long long currwin=now/cfg->getwin();
       auto it=sh.users.find(id);
       if(it==sh.users.end())
       {
            Userstate us;
            sh.users.emplace(id,us);
       }
       Userstate& state=sh.users[id];
       if(currwin!=state.windowstart)
       {
            state.windowstart=currwin;
            state.reqcount=1;
            cout<<"Request allowed for user "<<id<<endl;
            return 1;
       }else{
            if(state.reqcount>=cfg->getnumreq())
            {
                cout<<"not allowed "<<id<<endl;
                return 0;
            }else{
                state.reqcount+=1;
                cout<<"Request allowed"<<id<<endl;
                return 1;
            }
       }
    }
};
enum RLTYPE{
    FIXED,
    TOKEN
};
class Ratelimiterfactory{
    public:
    static RateLimiter* create(RLTYPE t,Ratelimiterconfig* c,int numshards)
    {
        switch(t)
        {
            case FIXED:
            return new Fixedwindow(c,numshards);
            case TOKEN:
            return new Tokenbucket(c,numshards);
            default:
            return new Tokenbucket(c,numshards);
        }
    }
};

int main()
{
    Ratelimiterconfig* fixedConfig=new Ratelimiterconfig(3,5);

    auto fixedLimiter =
        Ratelimiterfactory::create(
            RLTYPE::FIXED,
            fixedConfig,
            10
        );

    cout << "\n===== Fixed Window =====\n";

    User user1("user1", TIER::FREE);

    fixedLimiter->allowrequest(user1.getId());
    fixedLimiter->allowrequest(user1.getId());
    fixedLimiter->allowrequest(user1.getId());
    fixedLimiter->allowrequest(user1.getId());

     Ratelimiterconfig* tokenConfig=new Ratelimiterconfig(3,5);

    auto tokenLimiter =
        Ratelimiterfactory::create(
            RLTYPE::TOKEN,
            tokenConfig,
            10
        );

    cout << "\n===== Token Bucket =====\n";

    User user2("user2", TIER::PREMIUM);

    tokenLimiter->allowrequest(user2.getId());
    tokenLimiter->allowrequest(user2.getId());
    tokenLimiter->allowrequest(user2.getId());
    tokenLimiter->allowrequest(user2.getId());

    cout << "\nWaiting 2 seconds...\n";

    this_thread::sleep_for(
        chrono::seconds(2)
    );
    tokenLimiter->allowrequest(user2.getId());
}