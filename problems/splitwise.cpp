/* 
requirements:
 users can join/leave group
 users can add expense in group -> split strategy (equal,%,exact)
 users can settl expense
users can add and settle individual expenses
notification to be sent when expense is crud

User,Split,Group,Splitwise,Splitstrategy,
*/

#include<bits/stdc++.h>
using namespace std;

enum SplitType{
    EQUAL,
    PERCENTAGE,
    EXACT
};
class Split{
    string userid;
    double amount;
    public:
    Split(string u,double a): userid(u),amount(a) {}
};

class Splitstrategy{
    public:
    virtual vector<Split> calculatesplit(double total,vector<string>users,vector<int>values={})=0;
    virtual ~Splitstrategy()=default;
};
class Equalsplit:public Splitstrategy{
    public:
    vector<Split> calculatesplit(double total,vector<string>users,vector<int>values={})
    {
        vector<Split> ans;
        double amt=total/(users.size());
        for(auto it:users)
        {
            ans.push_back(Split(it,amt));
        }
        return ans;
    }
};
class Percentsplit:public Splitstrategy{
    public:
    vector<Split> calculatesplit(double total,vector<string>users,vector<int>values={})
    {
        vector<Split> ans;
        for(int i=0;i<values.size();i++)
        {
            double amt=(total*values[i])/100;
            ans.push_back(Split(users[i],amt));
        }
        return ans;
    }
};
class Exactsplit:public Splitstrategy{
    public:
    vector<Split> calculatesplit(double total,vector<string>users,vector<int>values={})
    {
        vector<Split> ans;
        int i=0;
        for(auto it:users)
        {
            ans.push_back(Split(it,values[i++]));
        }
        return ans;
    }
};

class SplitFactory{
    public:
    static Splitstrategy* getstrategy(SplitType s)
    {
        switch (s)
        {
        case EQUAL:
            return new Equalsplit();
        case PERCENTAGE:
            return new Percentsplit();
        case EXACT:
            return new Exactsplit();
        default:
            return new Equalsplit();
        }
    }
};

class User{
    static int uid;
    string userid;
    string name;
    map<string,double>balances;
    public:
    User(string n)
    {
        name=n;
        userid="user_"+to_string(uid++);
    }
    string getuserid() {return userid;}
    void notify(string msg)
    {
        cout<<"msg to "<<msg<<" "<<userid<<endl;
    }
    void updatebalance(string uid,double amt)
    {
        balances[uid]+=amt;
        if(abs(balances[uid])<0.01)
        {
            balances.erase(uid);
        }
    }
    double gettotowed()
    {
        double ans=0;
        for(auto it:balances)
        {
            if(it.second<0) ans+=abs(it.second);
        }
        return ans;
    }
    double gettotowing()
    {
        double ans=0;
        for(auto it:balances)
        {
            if(it.second>0) ans+=abs(it.second);
        }
        return ans;
    }
};
int User::uid=0;
class Expense{
    static int eid;
    string expid;
    double totamt;
    string paidbyuser;
    vector<Split>splits;
    string groupid;
    public:
    Expense(double a,string uid,vector<Split>s,string gid)
    {
        expid="expense_"+to_string(eid++);
        totamt=a;
        paidbyuser=uid;
        splits=s;
        groupid=gid;
    }
};
int Expense::eid=0;

// u1,u2 :200  u1->200,u2->-200
// u1->300
//u2 -> 150
//u3 -> -450

class Debtsimplifier{
    static bool comp(pair<string,double>&a,pair<string,double>&b)
    {
        return a.second>b.second;
    }
    static map<string,map<string,double>> simplify(map<string,map<string,double>>expenses)
    {
        map<string,double>netamts;
        for(auto it:expenses)
        {
            string u1=it.first;
            for(auto jt:it.second)
            {
                string u2=jt.first;
                double amt=jt.second;
                if(amt>0)
                {
                    netamts[u1]+=amt;
                    netamts[u2]-=amt;
                }
            }
        }
        vector<pair<string,double>>creds,debts;
        for(auto it:netamts)
        {
            if(it.second>0.1)
            {
                creds.push_back({it.first,it.second});
            }else{
                debts.push_back({it.first,abs(it.second)});
            }
        }
        sort(creds.begin(),creds.end(),comp);
        sort(debts.begin(),debts.end(),comp);
        map<string,map<string,double>>ans;
        int i=0,j=0;
        while(i<creds.size() and j<debts.size())
        {
            string cid=creds[i].first;
            string did=debts[j].first;
            double camt=creds[i].second;
            double damt=debts[j].second;
            double minamt=min(camt,damt);
            creds[i].second-=minamt;
            debts[j].second-=minamt;
            ans[cid][did]=minamt;
            ans[did][cid]=-minamt;
            if(creds[i].second<0.1){
                i++;
            }
            if(debts[j].second<0.1)
            {
                j++;
            }
        }
        return ans;
    }
};
class Group{
    static int gid;
    string groupid;
    vector<User*>users;
    map<string,Expense*> expenses;
    map<string,map<string,double>>grpbalances;
    public:
    Group()
    {
        groupid="group"+to_string(gid++);
    }
    void addmember(User* u)
    {
        users.push_back(u);
        grpbalances[u->getuserid()]=map<string,double>();
    }
    bool removemember(string u)
    {
        if(!canremove(u))
        {
            cout<<"Can't remove the user yet"<<endl;
            return false;
        }
        grpbalances.erase(u);
        vector<User*> temp;
        for(auto it:users)
        {
            if(it->getuserid()==u) continue;
            temp.push_back(it);
        }
        users=temp;
        for(auto &it:grpbalances)
        {
            it.second.erase(u);
        }
        return true;
    }
    void notifymembers(string s)
    {
        for(auto it:users) it->notify(s);
    }

};
int Group::gid=0;

int main()
{

}