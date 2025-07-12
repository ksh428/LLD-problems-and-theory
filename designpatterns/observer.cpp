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