#include <bits/stdc++.h>
#include <mutex>
using namespace std;

class ElevatorRequest{
    int eid;
    int sfloor;
    int dfloor;
public:
    ElevatorRequest(int i,int sf,int df) : eid(i),sfloor(sf),dfloor(df) {}
    int getsf() {return sfloor;}
    int getdf() {return dfloor;}
};

enum DIR{
    UP,
    DOWN,
    IDLE
};

class Elevator{
    int id;
    int cfloor;
    DIR d;
    queue<ElevatorRequest*>requests;
public:
    Elevator(int pid,int cf) : id(pid),cfloor(cf) {
        d=IDLE;
    }
    DIR getdir() {return d;}
    int getid() {return id;}
    int getcf() {return cfloor;}
    void addrequest(int df)
    {
        requests.push(new ElevatorRequest(id,cfloor,df));
        handlerequests();
    }
    void setrequests(queue<ElevatorRequest*>r)
    {
        requests=r;
    }
    void handlerequests()
    {
        if (requests.empty()) {
            d = IDLE;
            return;
        }
        vector<ElevatorRequest*> temp;
        while (!requests.empty()) {
            temp.push_back(requests.front());
            requests.pop();
        }
        if (d == IDLE) {
            if (temp[0]->getdf() > cfloor)
                d = UP;
            else
                d = DOWN;
        }
        int nextFloor = -1;
        int bestDist = INT_MAX;
        for (auto &req : temp) {
            int df = req->getdf();
            if (d == UP && df >= cfloor) {
                if (df - cfloor < bestDist) {
                    bestDist = df - cfloor;
                    nextFloor = df;
                }
            }
            else if (d == DOWN && df <= cfloor) {
                if (cfloor - df < bestDist) {
                    bestDist = cfloor - df;
                    nextFloor = df;
                }
            }
        }
        if (nextFloor == -1) {
            d = (d == UP) ? DOWN : UP;
            for (auto &req : temp) {
                int df = req->getdf();
                int dist = abs(df - cfloor);

                if (dist < bestDist) {
                    bestDist = dist;
                    nextFloor = df;
                }
            }
        }
        cout << "Elevator " << id << " moving from " << cfloor 
             << " to " << nextFloor << endl;
        cfloor = nextFloor;
        for (auto &req : temp) {
            if (req->getdf() != nextFloor) {
                requests.push(req);
            }
        }
        if (requests.empty()) {
            d = IDLE;
        }
    }
};

class ElevatorSystem{
    static ElevatorSystem* es;
    static mutex mtx;
    vector<Elevator*>elevators;
    ElevatorSystem()
    {
        for(int i=0;i<5;i++)
        {
            elevators.push_back(new Elevator(i,0));
        }
    }
public:
    int getrelevantelevator(DIR d,int sf);

    ElevatorSystem& operator=(const ElevatorSystem&) = delete;
    ElevatorSystem(const ElevatorSystem&) = delete;
    static ElevatorSystem* initsystem()
    {
        lock_guard<mutex>lock(mtx);
        if(es==nullptr)
        {
            es=new ElevatorSystem();
        }
        return es;
    }
    vector<Elevator*> getelevators() {return elevators;}

    void addrequest(int sf,int df)
    {
        DIR dir;
        if(sf>df)
            dir=DOWN;
        else
            dir=UP;
        int i=getrelevantelevator(dir,sf);
        elevators[i]->addrequest(df);
    }
};
ElevatorSystem* ElevatorSystem::es=nullptr;
mutex ElevatorSystem::mtx;
int ElevatorSystem::getrelevantelevator(DIR d,int sf)
{
    int mindiff=INT_MAX;
    int ans =-1;

    for(auto it:elevators)
    {
        if(it->getdir()==d || it->getdir()==IDLE)
        {
            int diff = abs(it->getcf()-sf);
            if(diff < mindiff)
            {
                ans=it->getid();
                mindiff=diff;
            }
        }
    }
    if(ans!=-1) return ans;
    return 0;
}

int main() {
    ElevatorSystem* es = ElevatorSystem::initsystem();
    es->addrequest(0, 5);
    es->addrequest(2, 8);
    es->addrequest(7, 1);

    return 0;
}