#include <bits/stdc++.h>
using namespace std;

//abstract product 
class Coffee{
    public:
    virtual string getDesc()=0;
    virtual string getPrice()=0;
};

//concrete products
class Espresso:public Coffee{
    public:
    string getDesc()
    {
        return "Espresso coffee ";
    }
    string getPrice()
    {
        return "20";
    }
};
class Americano:public Coffee{
    public:
    string getDesc()
    {
        return "Americano coffee ";
    }
    string getPrice()
    {
        return "10";
    }
};

// abstract decorator
class CoffeeDecorator:public Coffee{
    protected:
    Coffee* coffee;
    public:
    CoffeeDecorator(Coffee* pcoffee){
        coffee=pcoffee;
    }
    string getDesc()
    {
        return coffee->getDesc();
    }
    string getPrice()
    {
        return coffee->getPrice();
    }
};

// concrete decorators
class MilkDecorator:public CoffeeDecorator{
    public:
    MilkDecorator(Coffee* cof):CoffeeDecorator(cof){
        
    }
    string getDesc()
    {
        return coffee->getDesc()+"+"+"Milk";
    }
    string getPrice()
    {
        return coffee->getPrice()+"+"+"10";
    }
};

class ChocolateDecorator:public CoffeeDecorator{
    public:
    ChocolateDecorator(Coffee* cof):CoffeeDecorator(cof){
    }
    string getDesc()
    {
        return coffee->getDesc()+"+"+"Chocolate";
    }
    string getPrice()
    {
        return coffee->getPrice()+"+"+"20";
    }
};

int main() {
    // americano+ choco + milk
    
    // base coffee
    Coffee* americano=new Americano();
    
    Coffee* chocoamericano=new ChocolateDecorator(americano);
    
    Coffee* milkchocoamericano=new MilkDecorator(chocoamericano);
    cout<<milkchocoamericano->getDesc()<<" "<<milkchocoamericano->getPrice();
    return 0;
}