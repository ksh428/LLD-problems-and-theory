#include<bits/stdc++.h>
using namespace std;

#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

class Desktop{
    string keyboard;
    string mouse;
    public:
    void setkeyboard(string kb){
        keyboard=kb;
    }
    void setmouse(string m)
    {
        mouse=m;
    }
    string getkeyboard() {return keyboard;}
    string getmouse() {return mouse;}
};
class DesktopBuilder{
    public:
    virtual void buildkeyboard()=0;
    virtual void buildmouse()=0;
    virtual Desktop* getDesktop()=0;
};
class HPBuilder: public DesktopBuilder{
    Desktop* desktop=nullptr;
    public:
    HPBuilder()
    {
        desktop=new Desktop();
    }
    void buildkeyboard()
    {
        desktop->setkeyboard("hp kb");
    }
    void buildmouse()
    {
        desktop->setmouse("hp mouse");
    }
    Desktop* getDesktop()
    {
        return desktop;
    }
};
class DellBuilder: public DesktopBuilder{
    Desktop* desktop=nullptr;
    public:
    DellBuilder()
    {
        desktop=new Desktop();
    }
    void buildkeyboard()
    {
        desktop->setkeyboard("Dell kb");
    }
    void buildmouse()
    {
        desktop->setmouse("Dell mouse");
    }
    Desktop* getDesktop()
    {
        return desktop;
    }
};
class DesktopDirector{
    public:
    static Desktop* getDesktop(DesktopBuilder* builder)
    {
        builder->buildkeyboard();
        builder->buildmouse();
        return builder->getDesktop();
    }
};

int main()
{
    DesktopBuilder* b=new HPBuilder();
    Desktop* hpdesk=DesktopDirector::getDesktop(b);
    std::cout<<hpdesk->getkeyboard()<<" "<<hpdesk->getmouse();   
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