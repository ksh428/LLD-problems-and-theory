#include <bits/stdc++.h>
using namespace std;

class Idog{
    public:
    virtual void getname()=0;
    virtual void bark()=0;
    virtual ~Idog()=default;
};
class Germanshephard:public Idog{
    string name;
    public:
    Germanshephard(string s):name(s) {}
    void getname()
    {
        cout<<name<<endl;
    }
    void bark()
    {
        cout<<"Woof Woof"<<endl;
    }
};
class Doberman:public Idog{
    string name;
    public:
    Doberman(string s):name(s) {}
    void getname()
    {
        cout<<name<<endl;
    }
    void bark()
    {
        cout<<"Woof Woof"<<endl;
    }
};
class Dogfactory{
    public:
    static Idog* getdog(string s,string name)
    {
        if(s=="Germanshephard")
        {
            return new Germanshephard(name);
        }
        else if(s=="Doberman")
        {
            return new Doberman(name);
        }
        else
        {
            return nullptr;
        }
    }
};
int main()
{
    Idog* dober=Dogfactory::getdog("Doberman","Max");
    dober->getname();
    dober->bark();
}