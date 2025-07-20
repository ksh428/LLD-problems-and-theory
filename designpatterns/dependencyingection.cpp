// Online C++ compiler to run C++ program online
#include <bits/stdc++.h>

using namespace std;

class NotificationProvider{
    public:
    virtual void sendNotification()=0;
};
class EmailProvider:public NotificationProvider{
    public:
    void sendNotification() override{
        cout<<"email sent";
    }
};
class SMSProvider:public NotificationProvider{
    public:
    void sendNotification() override{
        cout<<"sms sent";
    }
};

class Client{
    NotificationProvider* provider;
    public:
    Client(NotificationProvider* prov){
        provider=prov;
    }
    void sendnotifications()
    {
        provider->sendNotification();
    }
};

int main() {
    NotificationProvider* sms=new SMSProvider();
    Client c= Client(sms);
    c.sendnotifications();
    delete(sms);
    
    return 0;
}