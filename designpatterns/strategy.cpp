// Online C++ compiler to run C++ program online
#include <bits/stdc++.h>

using namespace std;

class DriveStrategy{
    public:
    virtual void drive()=0;
};

class NormalDrive:public DriveStrategy{
    public:
    void drive() override{
        cout<<"normal drive";
    }
};

class SportDrive:public DriveStrategy{
    public:
    void drive() override{
        cout<<"sport drive";
    }
};

class Vehicle{
    DriveStrategy* ds;
    public:
    // dependency ingection
    Vehicle(DriveStrategy* par){
        ds=par;
    }
    virtual void Drive()
    {
        ds->drive();
    }
};

class SportVehicle :public Vehicle{
    public:
    // void Drive() override{
    //     cout<<"sports drive";
    // }
    SportVehicle():Vehicle(new SportDrive()){};
};
class PassengerVehicle :public Vehicle{
    public:
    // void Drive() override{
    //     cout<<"normal drive";
    // }
    PassengerVehicle():Vehicle(new NormalDrive()){};
};

// duplication of drive methid between offroadvehicle and sportsvehicle

class OffroadVehicle:public Vehicle{
    public:
    // void Drive() override{
    //     cout<<"sports drive";
    // }
    OffroadVehicle():Vehicle(new SportDrive()){};
};

int main() {
    
    Vehicle* veh=new OffroadVehicle();
    veh->Drive();
    return 0;
}


//SIMPLER AND ENOUGH

#include <bits/stdc++.h>

using namespace std;

class Paymentstrategy{
    public:
    virtual void pay()=0;
    virtual ~Paymentstrategy()=default;
};
class CardPayment:public Paymentstrategy{
    public:
    void pay()
    {
        cout<<"making card payments "<<endl;
    }
};
class UpiPayment:public Paymentstrategy{
    public:
    void pay()
    {
        cout<<"making upi payments "<<endl;
    }
};
class Baseapp{
    Paymentstrategy* ps;
    public:
    Baseapp(Paymentstrategy* p):ps(p) {}
    void makepayment()
    {
        ps->pay();
    }
    void setstrategy(Paymentstrategy* p){
        ps=p;
    }
};

int main() {
    Baseapp* train=new Baseapp(new UpiPayment());
    train->makepayment();
    train->setstrategy(new CardPayment());
    train->makepayment();
    delete train;
}