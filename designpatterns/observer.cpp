#include<bits/stdc++.h>

using namespace std;

class ISubsciber{
    public:
    virtual void notify(string msg)=0;
};
class User:public ISubsciber{
    private:
    string username;
    public:
    User(string name)
    {
        username=name;
    }
    void notify(string msg){
        cout<<"user "<<username <<" recieved msg: "<<msg<<endl;
    }
};
class Group{
    private:
    set<ISubsciber*>subs;
    public:
    void subscribe(ISubsciber* sub){
        subs.insert(sub);
    }
    void unsubscibe(ISubsciber* sub)
    {
        subs.erase(sub);
    }
    void groupnotify(string msg)
    {
        for(auto it:subs)
        {
            it->notify(msg);
        }
    }
};
int main()
{
    Group* grp=new Group();
    User* u1=new User("om");
    User* u2=new User("ksh");
    User* u3=new User("omkssh");
    grp->subscribe(u1);
    grp->subscribe(u2);
    grp->subscribe(u3);
    
    grp->groupnotify("message");
    grp->unsubscibe(u2);
    grp->groupnotify("message2");
}

// general

#include <iostream>
#include <vector>
using namespace std;

class ISubject {
public:
    virtual void notify(string msg) = 0;
};

class User : public ISubject {
    int id;
public:
    User(int a){
        id=a;
    }
    void notify(string msg) override {
        cout << "message: " << msg <<"user id "<<id<<endl;
    }
};

class INotifier {
public:
    virtual void subscribe(ISubject* sub) = 0;
    virtual void unsubscribe(ISubject* sub) = 0;  // fixed
    virtual void sendmsg(string msg) = 0;         // fixed
};

class SMSNotifier : public INotifier {
    vector<ISubject*> v;

public:
    void subscribe(ISubject* s) override {
        v.push_back(s);
    }

    void unsubscribe(ISubject* s) override {
        vector<ISubject*> temp;
        for (auto it : v) {
            if (it != s)
                temp.push_back(it);
        }
        v = temp;
    }

    void sendmsg(string msg) override {
        for (auto it : v) {
            it->notify(msg);
        }
    }
};

int main() {
    INotifier* notifier = new SMSNotifier();
    ISubject* s1 = new User(1);
    ISubject* s2 = new User(2);
    ISubject* s3 = new User(3);

    notifier->subscribe(s1);
    notifier->subscribe(s2);
    notifier->subscribe(s3);

    notifier->sendmsg("sms msg");
    notifier->unsubscribe(s2);
    notifier->sendmsg("sms msg2");

    delete notifier;
    delete s1;
    delete s2;
    delete s3;
}
