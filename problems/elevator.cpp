#include<bits/stdc++.h>

using namespace std;

// entities
enum State{
    MOVING
    STOPPED,
    IDLE
};
enum Direction{
    UP,
    DOWN
};
class Floor{
    int number;
    public:
    //getters and setters
};
class Building{
    int id;
    int numfloors;
    ElevatorController controller;
    public:
    //getters and setters
};
class Elevator{
    State state;
    Direction dir;
    vector<StateObserver*>observers;
    int currFloor;
    int id;
    queue<ElevatorRequest>requests;// process the elevator requests
    vector<StateObserver*>observers;// observe for change in state of a elevator, can be used to track/display in the screens
    public:
    // notify method
    // getters and setters
    // add delete observers
    // add request
    //move to next step as decided by the scheduling strategy
    void movetonextstep(int cfloor){
        while(currfloor!=cfloor){
            if(direction==UP){
                currfloor++;
            }else currfloor--;
            nofifyfloorchange(currfloor);
            if(currfloor==cfloor){
                arrive();
            }
        }
    }
    void arrive()
    {
            setState(ElevatorState.STOPPED);
        // If no more requests, set state to IDLE
        if (requests.isEmpty()) {
          direction = Direction.IDLE;
          setState(ElevatorState.IDLE);
        } else {
          // Otherwise, continue moving after a brief stop
          setState(ElevatorState.MOVING);
        }
    }
};

class ElevatorController{
    vector<Elevator>elevators;
    int curr_elevator_id;
    SchedulingStrategy strategy;
    public:
    // getters and setters and constructor
    void requestElevator(int id,int cfloor,Direction dir){
        // external request
        Elevator e;
        for(auto e1: elevators){
            if(e.getid()== e1.getid()){
                e=e1;
                break;
            }
        }
        ElevatorRequest req=ElevatorRequest(id,cfloor,false,dir);
        e.addrequests(req);
    }
    void requestFloor(int id,int cfloor){
        // external request
        Elevator e;
        for(auto e1: elevators){
            if(e.getid()== e1.getid()){
                e=e1;
                break;
            }
        }
        // set the direction
        ElevatorRequest req=ElevatorRequest(id,cfloor,true,dir);
        e.addrequests(req);
    }
    
};
// done entities

// observers
class StateObserver{
    public:
    virtual void onstatechange(Elevator e,State start,State end)=0;
    virtual void onfloorchange(Elevator e,Floor start,Floor end)=0;
};
class ElevatorDisplay:public StateObserver{
    void onstatechange(Elevator e,State start,State end){
        cout<<"state changed for the elevator "<<e.getId()<<"from state to to state:"<<start<<" "<<endl; 
    }
    void onfloorchange(Elevator e,Floor start,Floor end){
        cout<<"floor changed for the elevator "<<e.getId()<<"from state to to state:"<<start<<" "<<end; 
    }
};
// done observers

// create ElevatorRequest

class ElevatorRequest{
    int currentfloor;// floor where the request is made
    // for ext req currentfloor is the floor in which it is pressed
    Direction reqdir;
    ElevatorController ec; // handles the requests
    int id;// id of the elevator;
    int isinternalrequest;// int or ext request
    public:
    ElevatroRequest(int floor,int dir,int id,int type){
        currentfloor=floor;
        reqdir=dir;
        id=id;
        isinternalrequest=type;
        ec=ElevatorController();
    }
    void executeRequest()
    {
        if(isinternalrequest)
        {
            ec.requestfloor(currentfloor,id);
        }else ec.requestelevator(currentfloor,id,reqdir);
    }
    // getters and setters
};
// done elevatrorequest

// strategy for scheduling elevators
class SchedulingStrategy{
    public:
    virtual int getNextStop()=0;
};

class FCFSStrategy:public SchedulingStrategy{
    public:
    int getNextStop(Elevator* e){
        queue<ElevatorRequest> req=e->getRequests();
        int currfloor=e.getFloor();
        if(req.size()==0) return currfloor;
        int tofloor=q.front().getfloor(); 
        q.pop();
        if(tofloor==currfloor) return currfloor;
        else if(tofloor>currfloor){
            e->setDirection(Direction::UP);
            return tofloor;
        }else{
            e->setDirection(Direction::DOWN);
            return tofloor;
        }
    }
};
class EfficientStrategy:public SchedulingStrategy{
    public:
    int getNextStop(Elevator* e){
        queue<ElevatorRequest> req=e->getRequests();
        int currfloor=e.getFloor();
        if(req.size()==0) return currfloor;
        Direction dir;
        int tofloor=req.getfloor();
        if(tofloor==currfloor) return currfloor;
        if(tofloor>currfloor){
            dir=UP;
        }else dir=DOWN;
        int floorans=currfloor;
        for(auto r:req)
        {
            if(dir==UP){// all internal requests and external request having dir as up and within the floor range can be catered
            if(req.getfloor()<= tofloor and req.getfloor()>= currfloor)
            {
                if(req.checkifinternalrequest() or ( !req.checkifinternalrequest() && req.getdirection()==UP))
                {
                    if(floorans>req.getfloor()){
                        floorans=req.getfloor();
                    }
                }
            }
                
            }else{
                if(dir==DOWN){// all internal requests and external request having dir as up and within the floor range can be catered
            if(req.getfloor()<= currfloor and req.getfloor()>= tofloor)
            {
                if(req.checkifinternalrequest() or ( !req.checkifinternalrequest() && req.getdirection()==DOWN))
                {
                    if(floorans<req.getfloor()){
                        floorans=req.getfloor();
                    }
                }
            }
                
            }
        }
        //delet floorans from req
        queue<ElevatorRequest>q1=req;
        while(!req.empty()){
            auto frontele=req.front();
            int curr=frontele.getfloor();
            req.pop();
            if(curr==floorans) continue;
            else q1.push(frontele);
        }
        e.setrequests(q1);
        return floorans;
    }
};



int main()
{
    return 0;
}