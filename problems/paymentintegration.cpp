#include<bits/stdc++.h>
using namespace std;
// USE OF ADAPTER DP

class Paymentprocessor{
    public:
    virtual void makepayment(int amt)=0;
    virtual ~Paymentprocessor()=default;
};

// 3rd party SDK's
class Stripesdk{
    public:
    void pay(int x)
    {
        cout<<"paying using stripe "<<x<<endl;
    }
};

class Paypalsdk{
    public:
    void paypal(int x)
    {
        cout<<"paying using paypal "<<x<<endl;
    }
};

class Stripeadapter:public Paymentprocessor{
    Stripesdk* sdk;
    public:
    Stripeadapter(Stripesdk* s):sdk(s) {}
    void makepayment(int x)
    {
        sdk->pay(x);
    }
};

class Paypaladapter:public Paymentprocessor{
    Paypalsdk* sdk;
    public:
    Paypaladapter(Paypalsdk* s):sdk(s) {}
    void makepayment(int x)
    {
        sdk->paypal(x);
    }
};

class Orderservice{
    Paymentprocessor* ps;
    public:
    Orderservice(Paymentprocessor* p):ps(p) {}
    void setsdk(Paymentprocessor* p)
    {
        ps=p;
    }
    void makepayment(int x)
    {
        ps->makepayment(x);
    }
};


int main()
{
    Stripesdk stripeSdk;
    Paypalsdk paypalSdk;

    Stripeadapter stripe(&stripeSdk);
    Paypaladapter paypal(&paypalSdk);

    Orderservice order(&stripe);

    order.makepayment(1000);

    order.setsdk(&paypal);

    order.makepayment(2000);
}