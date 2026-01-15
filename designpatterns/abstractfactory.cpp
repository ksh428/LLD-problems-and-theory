#include <bits/stdc++.h>

using namespace std;

class Button{
    public:
    virtual void getbutton()=0;
    virtual ~Button()=default; // this is used to cleanup derived class resources when we delete the base class ptr
    //A virtual destructor ensures that when an object is deleted through a base-class pointer, the derived class destructor is called, preventing resource leaks.
};
class Keyboard{
    public:
    virtual void getkeyboard()=0;
    virtual ~Keyboard()=default;
};
class Winbutton: public Button{
    public:
    void getbutton()
    {
        cout<<"windows button"<<endl;
    }
};
class Macbutton: public Button{
    public:
    void getbutton()
    {
        cout<<"mac button"<<endl;
    }
};
class Winkeyboard:public Keyboard{
    public:
    void getkeyboard()
    {
        cout<<"windows keyboard"<<endl;
    }
};

class Mackeyboard:public Keyboard{
    public:
    void getkeyboard()
    {
        cout<<"mac keyboard"<<endl;
    }
};
class ProdFactory{
    public:
    virtual void createbutton()=0;
    virtual void createkeyboard()=0;
    virtual void displaydetails()=0;
    virtual ~ProdFactory()=default;
};
class WinFactory:public ProdFactory{
    Button* btn;
    Keyboard* kb;
    public:
    WinFactory()
    {
        createbutton();
        createkeyboard();
    }
    void createbutton()
    {
        btn=new Winbutton();
    }
    void createkeyboard()
    {
        kb=new Winkeyboard();
    }
    void displaydetails()
    {
        btn->getbutton();
        kb->getkeyboard();
    }
};
class MacFactory:public ProdFactory{
    Button* btn;
    Keyboard* kb;
    public:
    MacFactory()
    {
        createbutton();
        createkeyboard();
    }
    void createbutton()
    {
        btn=new Macbutton();
    }
    void createkeyboard()
    {
        kb=new Mackeyboard();
    }
    void displaydetails()
    {
        btn->getbutton();
        kb->getkeyboard();
    }
};

int main() {
    ProdFactory* macfact=new MacFactory();
    macfact->displaydetails();
    ProdFactory* winfact=new WinFactory();
    winfact->displaydetails();
    delete macfact;
    delete winfact;
}