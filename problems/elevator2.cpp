// Online C++ compiler to run C++ program online
#include <bits/stdc++.h>
#include <mutex>
using namespace std;

enum STATE{
    MOVING,
    IDLE
}

class Floor{
    int no;
    public:
    Floor(int a):no(a) {}
    int getfloor() {return no;}
};
class ElevatorRequest;
class Elevator{
    Floor* currfloor;
    int id;
    STATE s;
    queue<ElevatorRequest*>requests;
    public:
    Elevator(int i){
        currfloor=new Floor(0);
        s=IDLE;
        id=i;
    }
    int getid() { return id;}
    queue<ElevatorRequest*> getrequests()
    {
        return requests;
    }
    void addrequest(ElevatorRequest* r){
        requests.insert(r);
    }
};
class ElevatorRequest{
    int eid;
    int type;
    int tfloor;//floor were the request was made
    // in case of internal request it will be to floor, in case of external request it can be anything
    public:
    ElevatorRequest(int e,int t,int tf):eid(e),type(t),tfloor(tf) {}
};
class Elevatorsystem{
    static Elevatorsystem* es;
    static mutex mtx;
    vector<Elevator>elevators;
    Elevatorsystem()
    {
        for(int i=0;i<5;i++)
        {
            elevators.push_back(new Elevator(i));
        }
    }
    public:
    Elevatorsystem& operator=(const Elevatorsystem&)=delete;
    Elevatorsystem(const Elevatorsystem&)=delete;
    Elevatorsystem* initsystem()
    {
        lock_guard<mutex>lock(mtx);
        if(es==nullptr)
        {
            es=new Elevatorsystem();
        }
        return es;
    }
    void requestfloor(int eid,int fl)
    {
        // type 2
        // internal request made inside the elevator
        Elevator e;
        for(auto it:elevators)
        {
            if(eid==it.getid())
            {
                e=id;
                break;
            }
        }
        ElevatorRequest* newreq=new ElevatorRequest(eid,2,fl);
        e.addrequest(newreq);
    }
    void requestelevator(int eid,int cf)
    {
        // type 1
        Elevator e;
        for(auto it:elevators)
        {
            if(eid==it.getid())
            {
                e=id;
                break;
            }
        }
        ElevatorRequest* newreq=new ElevatorRequest(eid,1,-1);
        e.addrequest(newreq);
    }
};
Elevatorsystem* Elevatorsystem::es=nullptr;
mutex::Elevatorsystem mtx;

int main() {
    
}