#include<bits/stdc++.h>
using namespace std;

class Car{
    private:
    string name;
    string color;
    public:
    string getName()
    {
        return name;
    }
    string getColor()
    {
        return color;
    }
    void setName(string pname)
    {
        name=pname;
    }
    void setColor(string pcolor)
    {
        color=pcolor;
    }
};

class Carbuilder
{
    public:
    virtual void buildName(string name)=0;
    virtual void buildColor(string color)=0;
    virtual Car getCar()=0;
};

class BMWBuilder:public Carbuilder
{
    private:
    Car car;
    public:
    BMWBuilder()
    {
        car=Car();
    }
    void buildName(string name) 
    {
        car.setName(name);
    }
    void buildColor(string color) 
    {
        car.setColor(color);
    }
    Car getCar() 
    {
        return car;
    }
};

class CarDirector{
    public:
    Car createCar(Carbuilder& cb)
    {
        cb.buildName("BMW CAR");
        cb.buildColor("Red");
        return cb.getCar();
    }
};

int main()
{
    CarDirector dir;
    BMWBuilder bmw;
    Car bmwcar=dir.createCar(bmw);
    cout<<bmwcar.getName()<<" "<<bmwcar.getColor();
    return 0;
}

// other example
#include<bits/stdc++.h>
using namespace std;

class Desktop{
    private:
    string keyboard;
    string name;
    public:
    void setKeyboard(string kb)
    {
        keyboard=kb;
    }
    void setName(string name)
    {
        name=name;
    }
    string getKeyboard()
    {
        return keyboard;
    }
    string getName()
    {
        return name;
    }
};

class DesktopBuilder
{
    public:
    virtual void buildKeyboard(string name)=0;
    virtual void buildName(string name)=0;
    virtual Desktop getDesktop() =0;
};

class HPBuilder:public DesktopBuilder{
    private:
    Desktop desk;
    public:
    HPBuilder()
    {
        desk=Desktop();
    }
    void buildKeyboard(string name) 
    {
        desk.setKeyboard(name);
    }
    void buildName(string name) 
    {
        desk.setName(name);
    }
    Desktop getDesktop() 
    {
        return desk;
    }
};

class DesktopDirector
{
    public:
    Desktop CreateDesktop(DesktopBuilder &db)
    {
        db.buildName("hp desktop");
        db.buildKeyboard("hp keyboard");
        return db.getDesktop();
    }
};

int main(){
    DesktopDirector dir;
    HPBuilder hpb;
    Desktop hpdesk=dir.CreateDesktop(hpb);
    cout<<hpdesk.getName()<<" "<<hpdesk.getKeyboard();
    return 0;
}