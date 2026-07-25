#include <bits/stdc++.h>
using namespace std;

class Drivestrategy{
    public:
    virtual void printstrategy()=0;
    virtual ~Drivestrategy()=default;
};
class Sportsdrive:public Drivestrategy{
    public:
    void printstrategy()
    {
        cout<<"sports drive"<<endl;
    }
};
class Normaldrive:public Drivestrategy{
    public:
    void printstrategy()
    {
        cout<<"normal drive"<<endl;
    }
};
class Vehicle{
    Drivestrategy* ds;
    public:
    Vehicle(Drivestrategy* dss=new Normaldrive())
    {
        ds=dss;
    }
    virtual void drive()
    {
        ds->printstrategy();
    }
    virtual void setmode(Drivestrategy* dss)
    {
        delete ds;
        ds=dss;
    }
    virtual ~Vehicle()=default;
};
class Normalcar: public Vehicle{
    public:
    Normalcar(Drivestrategy* ds):Vehicle(ds){}
    void drive()
    {
        Vehicle::drive();
    }
    void setmode(Drivestrategy* ds)
    {
        Vehicle::setmode(ds);
    }
};

int main()
{
    Vehicle* normalcar=new Vehicle(new Normaldrive());
    normalcar->drive();
    normalcar->setmode(new Sportsdrive());
    normalcar->drive();

}