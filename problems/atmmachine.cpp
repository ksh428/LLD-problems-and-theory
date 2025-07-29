#include <bits/stdc++.h>

using namespace std;

//state design pattern
class ATMState{ // ex: READY,WITHDRAWING,EJECTING etc
    public:
    virtual void InsertCard()=0;
    virtual void EjectCard()=0;
    virtual string GetStateName()=0;
    virtual void Cancel()=0;
    virtual void enterState()=0;
    virtual void RequestCash()=0;
    virtual void enterpin(string pin)=0;
};

// database's

class DatabaseRepository{
    private:
    // data.emplace_back(std::initializer_list<std::string>{"001", "1234", "init"});

    public:
    string fetchATMDetails(string atmid); // returns the state of the atm
    void updateBalance();
    void updatedetails(string id,string state);
};

class ATM{
    string id;
    ATMState* state;
    DatabaseRepository* repo;
    public:
    ATM(string atmid,DatabaseRepository* db)
    {
        id=atmid;
        repo=db;
        string currstate=repo->fetchATMDetails(id);
        //initialize the states
    }
    void setState(ATMState* st)
    {
        state=st;
        state->enterState();
        repo->updatedetails(id,state->GetStateName());
    }
    bool pincorrect(string pin){
        // logic to check whether the pin is correct or not
    }
    void insertcard(){
        state->InsertCard();
    }
    void eject(){
        state->EjectCard();
    }
    void cancel(){
        state->Cancel();
    }
    void requestcash(){
        state->RequestCash();
    }
};
// factory for state
class StateFactory{
    public:
    static ATMState* getState(ATM* a,string state)
    {
        if(state=="ready")
        {
            return new ReadyState(a);
        }else if(state=="reading")
        {
            return new ReadingState(a);
        }else if(state == "cancel")
        {
            return new CancelState(a);
        }
    }
};

class ReadyState:public ATMState{
    ATM* atm;
    public:
    ReadyState(ATM* a):atm(a) {}
    string GetStateName()
    {
        return "ready";
    }
    void InsertCard(){
        cout<<"inserted card, moving to reading"<<endl;
        atm->setState(StateFactory::getState(a,"reading"));
    }
    void EjectCard(){
        cout<<"eject not supported in ready state"<<endl;
    }
    void Cancel(){
        cout<<"eject not supported in ready state"<<endl;
    }
    void enterpin(string pin)
    {
        cout<<"not supported"<<endl;
    }
    // list the supported and not support
};

class ReadingState:public ATMState{
    ATM* atm;
    public:
    ReadingState(ATM* a):atm(a) {}
    string GetStateName()
    {
        return "reading";
    }
    void InsertCard(){
        cout<<"not suported"<<endl;
    }
    void EjectCard(){
        cout<<"ejected,moved to ready state"<<endl;
        atm->setState(StateFactory::getState(a,"ready"));
    }
    void Cancel(){
        cout<<"cancelled,moved to cancel state"<<endl;
        atm->setState(StateFactory::getState(a,"cancel"));
    }
    void enterpin(string pin)
    {
        if(atm->pincorrect(pin))
        {
            atm->setState(new CashWithdrawl());
        }else{
            atm->setState(new ReadyState());
        }
    }
    // list the supported and not support
};
class CashWithdrawl:public ATMState{
    ATM* atm;
    public:
    CashWithdrawl(ATM* a):atm(a) {}
    string GetStateName()
    {
        return "reading";
    }
    void InsertCard(){
        cout<<"not suported"<<endl;
    }
    void EjectCard(){
        cout<<"ejected,moved to ready state"<<endl;
        atm->setState(StateFactory::getState(a,"ready"));
    }
    void Cancel(){
        cout<<"cancelled,moved to cancel state"<<endl;
        atm->setState(StateFactory::getState(a,"cancel"));
    }
    void enterpin(string pin)
    {
        if(atm->pincorrect(pin))
        {
            atm->setState(new CashWithdrawl());
        }else{
            atm->setState(new ReadyState());
        }
    }
    // list the supported and not support
};

int main()
{
    
    return 0;
}