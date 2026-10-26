#include <bits/stdc++.h>
using namespace std;

class User{
    int id;
    map<int,int>balance;
    public:
    User(int i):id(i) {}
    int getid() {return id;}
    void updatebalance(int i,int a)
    {
        balance[i]+=a;
    }
    int gettotowing()
    {
        int ans=0;
        for(auto it:balance)
        {
            if(it.second>0) ans+=it.second;
        }
        return ans;
    }
    int gettotowed()
    {
        int ans=0;
        for(auto it:balance)
        {
            if(it.second<0) ans+=-1*it.second;
        }
        return ans;
    }
};

enum SPLITSTRATEGY{
    EQUAL,
    PERCENTAGE,
    UNEQUAL
};

class Split{
    User u;
    int amt;
    public:
    Split(User pu,int a):u(pu),amt(a) {}
    User getuser() {return u};
    int getamt() {return amt;}
};

class Splitstrategy{
    public:
    virtual vector<Split> splitbill(int amt,vector<Split>inputsplit={})=0;
    virtual ~Splitstrategy()=default;
};

class Equalsplit:public Splitstrategy{
    public:
    vector<Split> splitbill(int amt,vector<Split>inputsplit={})
    {
        vector<Split>ans;
        int no=inputsplit.size();
        int share=amt/no;
        for(auto it:inputsplit)
        {
            Split ss=Split(it.getuser(),share);
            ans.push_back(ss);
        }
        return ans;
    }
};

class Percentsplit:public Splitstrategy{
    public:
    vector<Split> splitbill(int amt,vector<Split>inputsplit={})
    {
        // here the input split amt is percent
        vector<Split>ans;
        for(auto it:inputsplit)
        {
            int share=amt*(it.getamt())/100;
            Split ss=Split(it.getuser(),share);
            ans.push_back(ss);
        }
        return ans;
    }
};

class Unequalsplit:public Splitstrategy{
    public:
    vector<Split> splitbill(int amt,vector<Split>inputsplit={})
    {
        vector<Split>ans;
        for(auto it:inputsplit)
        {
            Split ss=Split(it.getuser(),it.getamt());
            ans.push_back(ss);
        }
        return ans;
    }
};

class Splitfactory{
    public:
    static Splitstrategy* getstrategy(SPLITSTRATEGY s)
    {
        switch(s)
        {
            case EXACT:
            return new Equalsplit();
            case UNEQUAL:
            return new Unequalsplit();
            case PERCENTAGE:
            return new Percentsplit();
        }
        return new Equalsplit();
    }
};

class Expense;

class Simplifier{
    public:
    static bool comp(pair<int,int>a,pair<int,int>b)
    {
        return a.second>b.second;
    }
    static map<int,map<int,int>> simplify(map<int,map<int,int>>mp)
    {
        vector<pair<int,int>>creds,debts;// net creditors and debitors
        map<int,int>netamts;
        for(auto it:mp)
        {
            int u1=it.first;
            for(auto jt:it.second)
            {
                int u2=jt.first;
                int amt=jt.second;
                if(amt>0)
                {
                    netamts[u1]-=amt;
                    netamts[u2]+=amt;
                }
            }
        }
        // c-> 10, d->5 
        for(auto it:netamts)
        {
            if(it.second>0)
            {
                creds.push_back({it.first,it.second});
            }else debts.push_back({it.first,abs(it.second)});
        }
        sort(creds.begin(),creds.end(),comp);
        sort(debts.begin(),debts.end(),comp);
        map<int,pair<int,int>>ans;
        int i=0,j=0;
        while(i<creds.size() and j<debts.size())
        {
            int cid=creds[i].first;
            int camt=creds[i].second;
            int dif=debts[j].first;
            int damt=debts[j].second;
            int mnamt=min(damt,camt);
            creds[i].second-=mnamt;
            debts[j].second-=mnamt;
            ans[cid][did]+=mnamt;
            ans[did][cid]-=amt;
            if(creds[i].second==0)
            {
                i++;
            }
            if(debts[j].second==0)  j++;
        }
        return ans;
    }
    
};

class Group{
    static int gid;
    int id;
    vector<User*>users;
    map<int,Expense*>expenses;
    map<int,map<int,int>>grpbalances; // balance of every user with each other in this grp
    public:
    Group(vector<User*>u):users(u) {}
    void addmember(User* pu)
    {
        users.push_back(pu);
    }
    void removemember(int id)
    {
        vector<User*>temp;
        for(auto it:users)
        {
            if(it->getid()!=id) temp.push_back(it);
        }
        users=temp;
    }
    void addexpense(int id,Expense* e)
    {
        expenses[id]=e;
        auto v=e->getsplit();
        int u=e->getpaiduser();
        int a=e->getamt();
        for(auto it:v)
        {
           auto uid=it->getuser()->getid();
           if(uid!=u)
           {
               grpbalances[u][uid]-=it->getamt();
               grpbalances[uid][u]+=it->getamt();
           }
        }
        auto mp=Simplifier::simplify(grpbalances);
        grpbalances=mp;
    }
};
int Group::gid=0;


class Expense{
    int amt;
    static int eid;
    int id;
    Splitstrategy* s;
    vector<Split>splits;
    int gid;
    int payuser;
    public:
    Expense(int a,Splitstrategy* ps,vector<Split>sp,int pgid,int uid)
    {
        id=eid++;
        s=ps;
        splits=ps->splitbill(a,sp,uid);
        gid=pgid;
        payuser=uid;
    }
};
int Expense::eid=0;

class Splitwise{
    vector<Group*> groups;
    map<int,vector<Expense*>>grpexpenses;
    public:
    void creategroup(vector<User*>users)
    {
        groups.push_back(new Group(users));
    }
    void adduser(int id,int gid)
    {
        for(auto it:groups)
        {
            if(it==gid)
            {
                it->addmember(new User(id));
            }
        }
    }
    void removeuser(int id,int gid)
    {
        for(auto it:groups)
        {
            if(it==gid)
            {
                it->removemember(id);
            }
        }
    }
    // a-> 50 , b,c,d -> 
    void addexpense(Group* g,int amt,SPLITSTRATEGY s,vector<Split>splits,User* pu)
    {
        Splitstrategy* ss=Splitfactory::getstrategy(s);
        Expense* e=new Expense(amt,ss,splits,g->getid(),pu->getid());
        g->addexpense(e);
        grpexpenses[g->getid()].push_back(e);
    }
};

int main() {
	

}
