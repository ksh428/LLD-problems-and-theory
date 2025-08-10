#include <bits/stdc++.h>

using namespace std;

class TrafficLight{
    public:
    virtual void getState()=0;
};

class RedLight:public TrafficLight{
    public:
    void getState()
    {
        cout<<"RED"<<endl;
    }
};
class YellowLight:public TrafficLight{
    public:
    void getState()
    {
        cout<<"YELLOW"<<endl;
    }
};
class GreenLight:public TrafficLight{
    public:
    void getState()
    {
        cout<<"GREEN"<<endl;
    }
};

//handler for the traffic states
class TrafficContext{
    public:
    TrafficLight* currstate;
    TrafficContext()
    {
        currstate=new RedLight(); // start w red
    }
    void next(TrafficLight* nextState=new RedLight()){
        delete currstate;
        currstate=nextState;
    }
    void getStateColor()
    {
        currstate->getState();
    }
};

int main() {
    TrafficContext* ctx=new TrafficContext();
    ctx->getStateColor();
    ctx->next(new GreenLight());
    ctx->getStateColor();
    ctx->next(new YellowLight());
    ctx->getStateColor();
    ctx->next();
    ctx->getStateColor();
    ctx->next();
    ctx->getStateColor();
    
    
    return 0;
}