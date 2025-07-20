
#include <bits/stdc++.h>
using namespace std;

// chain of responsibility for auth

class AuthHandler{
    public:
    virtual void HandleRequest(string request)=0;
    virtual void setNextHandler(AuthHandler* handler)=0;
};
class EmailPasswordHandler:public AuthHandler{
    AuthHandler* nexthandler;
    public:
     void setNextHandler(AuthHandler* handler) override
    {
        nexthandler=handler;
    }
    void HandleRequest(string request){
        if(request=="email and password")
        {
            cout<<"handled using email password handler"<<endl;
        }else if(nexthandler!=nullptr)
        {
            nexthandler->HandleRequest(request);
        }else{
            cout<<"invalid request"<<endl;
        }
    }
   
};
class OAuthHandler:public AuthHandler{
    AuthHandler* nexthandler;
    public:
    void HandleRequest(string request){
        if(request=="oauth")
        {
            cout<<"handled using oauth handler"<<endl;
        }else if(nexthandler!=nullptr)
        {
            nexthandler->HandleRequest(request);
        }else{
            cout<<"invalid request"<<endl;
        }
    }
    void setNextHandler(AuthHandler* handler)
    {
        nexthandler=handler;
    }
};

int main() {
    AuthHandler* emailhandler=new EmailPasswordHandler();
    AuthHandler* oauthhandler=new OAuthHandler();
    emailhandler->setNextHandler(oauthhandler);
    emailhandler->HandleRequest("oauth");
    emailhandler->HandleRequest("email and password");
    emailhandler->HandleRequest("dkf");
    return 0;
}