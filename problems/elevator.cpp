#include<bits/stdc++.h>
#include <mutex>
using namespace std;

class Request{
    int sf;
    int df;
    public:
    Request(int s,int d): sf(s),df(d) {}
    int getsf() {return sf;}
    int getdf() {return df;}
};

enum DIR{
    UP,
    DOWN,
    IDLE
};

class Elevator{
    int id;
    queue<Request*>requests;
    DIR d;
    int cf;
    public:
    Elevator(int i)
    {
        id=i;
        d=IDLE;
        cf=0;
    }
    int getid()
    {
        return id;
    }
    void processrequests()
    {
        if(requests.empty())
        {
            d=IDLE;
            return;
        }
        vector<Request*>temp;
        while(!requests.empty())
        {
            temp.push_back(requests.front());
            requests.pop();
        }
        int nextfloor=-1;
        if(d==IDLE)
        {
            d=(temp[0]->getdf()>cf)?UP:DOWN;
        }
        int diff=INT_MAX;
        // nearest request in the current dir
        for(auto it:temp)
        {
            int floor=it->getdf();
            if(d==UP and floor>=cf)
            {
                if(floor-cf<diff)
                {
                    diff=floor-cf;
                    nextfloor=floor;
                }
            }
            if(d==DOWN and floor<=cf)
            {
                if(cf-floor<diff)
                {
                    diff=cf-floor;
                    nextfloor=floor;
                }
            }
        }
        // if nothing found in the same dir,change and the dir and check
        if(nextfloor==-1)
        {
            d=(d==UP)?DOWN:UP;
            diff=INT_MAX;
            for(auto it:temp)
            {
                int floor=it->getdf();
                if(d==UP and floor>=cf)
                {
                    if(floor-cf<diff)
                    {
                        diff=floor-cf;
                        nextfloor=floor;
                    }
                }
                if(d==DOWN and floor<=cf)
                {
                    if(cf-floor<diff)
                    {
                        diff=cf-floor;
                        nextfloor=floor;
                    }
                }
            }

        }  
        cout<<"Elevator "<<id<<" going to "<<nextfloor<<" from "<<cf<<endl;
        cf=nextfloor;
        for(auto it:temp)
        {
            if(it->getdf()==nextfloor)
            {
                delete it;
                continue;
            }
            requests.push(it);
        }
        if(requests.empty()) d=IDLE;
    }
    DIR getdir() {return d;}
    int getcf() {return cf;}
    void addrequest(Request* r)
    {
        requests.push(r);
        while(!requests.empty())
        {
            processrequests();
        }
    }
    queue<Request*> getrequests()
    {
        return requests;
    }
};
class Elevatorsystem{
    vector<Elevator*> elevators;
    static Elevatorsystem* elevatorinstance;
    static mutex mtx;
    Elevatorsystem(int n)
    {
        for(int i=0;i<n;i++)
        {
            elevators.push_back(new Elevator(i));
        }
    }
    Elevatorsystem& operator=(const Elevatorsystem&)=delete;
    Elevatorsystem(const Elevatorsystem&)=delete;
    public:
    static Elevatorsystem* createsystem(int n)
    {
        lock_guard<mutex>lock(mtx);
        if(!elevatorinstance)
        {
            elevatorinstance=new Elevatorsystem(n);
        }
        return elevatorinstance;
    }
    int findelevator(DIR d,int cf)
    {
        int ans=0;
        int dist=INT_MAX;
        for(auto it:elevators)
        {
            if(it->getdir() == IDLE)
            {
                int cd = abs(it->getcf() - cf);
                if(cd < dist)
                {
                    dist = cd;
                    ans = it->getid();
                }
            }
            else if(it->getdir()==UP && d==UP && it->getcf()<=cf)
            {
                int cd=abs(it->getcf()-cf);
                if(cd<dist)
                {
                    dist=cd;
                    ans=it->getid();
                }
            }
           else if(it->getdir()==DOWN && d==DOWN && it->getcf()>=cf)
           {
                int cd=abs(it->getcf()-cf);
                    if(cd<dist)
                    {
                        dist=cd;
                        ans=it->getid();
                    }
           }
        }
        return ans;
    }
    void requestfloor(int cf,int tf)
    {
        DIR d;
        if(tf>cf){
            d=UP;
        }else d=DOWN;
        int id=findelevator(d,cf);
        elevators[id]->addrequest(new Request(cf,tf));
    }
};
Elevatorsystem* Elevatorsystem::elevatorinstance=nullptr;
mutex Elevatorsystem::mtx;

// press the floor where we want to go right from the outside

int main()
{
    Elevatorsystem* es = Elevatorsystem::createsystem(5);
    es->requestfloor(0, 5);
    es->requestfloor(2, 8);
    es->requestfloor(7, 1);

}