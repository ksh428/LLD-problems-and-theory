#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

class Button{
    public:
    virtual void getButton()=0;
};
class KeyBoard{
    public:
    virtual void getKeyboard()=0;
};
class WinButton:public Button
{
    public:
    void getButton()
    {
        cout<<"windows button";
    }
};
class WinKeyboard:public KeyBoard
{
    public:
    void getKeyboard()
    {
        cout<<"windows Keyboard";
    }
};
class MacButton:public Button
{
    public:
    void getButton()
    {
        cout<<"Mac button";
    }
};
class MacKeyboard:public KeyBoard
{
    public:
    void getKeyboard()
    {
        cout<<"Mac Keyboard";
    }
};
class UIFactory{
    public:
    virtual Button* getUIButton()=0;
    virtual KeyBoard* getUIKeyboard()=0;
};
class WinFactory:public UIFactory{
    public:
    Button* getUIButton()
    {
        return new WinButton();
    }
    KeyBoard* getUIKeyboard()
    {
        return new WinKeyboard();
    }
};
class MacFactory:public UIFactory{
    public:
    Button* getUIButton()
    {
        return new MacButton();
    }
    KeyBoard* getUIKeyboard()
    {
        return new MacKeyboard();
    }
};

int main()
{
    UIFactory* winfact=new WinFactory();
    winfact->getUIButton()->getButton();
    winfact->getUIKeyboard()->getKeyboard();
    
}