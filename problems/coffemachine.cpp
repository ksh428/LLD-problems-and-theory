#include <bits/stdc++.h>
using namespace std;

enum Size {
    M,
    L,
    XL
};

class Coffee {
    Size sz;

    int sizePrice() const {
        switch(sz) {
            case M: return 10;
            case L: return 15;
            case XL: return 20;
        }
        return 0;
    }

public:
    Coffee(Size s): sz(s) {}

    virtual string getDesc() = 0;
    virtual int getBasePrice() = 0;

    virtual ~Coffee() = default;

    virtual int getPrice() {
        return getBasePrice() + sizePrice();
    }
};

class Espresso : public Coffee {
public:
    Espresso(Size s): Coffee(s) {}

    string getDesc() override {
        return "Espresso";
    }

    int getBasePrice() override {
        return 20;
    }
};

class Americano : public Coffee {
public:
    Americano(Size s): Coffee(s) {}

    string getDesc() override {
        return "Americano";
    }

    int getBasePrice() override {
        return 10;
    }
};

class CoffeeFactory {
public:
    static Coffee* createCoffee(
        const string& type,
        Size size)
    {
        if(type == "espresso")
            return new Espresso(size);

        if(type == "americano")
            return new Americano(size);

        return nullptr;
    }
};

class CoffeeDecorator : public Coffee {
protected:
    Coffee* coffee;

public:
    CoffeeDecorator(Coffee* c)
        : Coffee(M), coffee(c) {}

    virtual ~CoffeeDecorator() {
        delete coffee;
    }
};

class MilkDecorator : public CoffeeDecorator {
public:
    MilkDecorator(Coffee* c)
        : CoffeeDecorator(c) {}

    string getDesc() override {
        return coffee->getDesc() + " + Milk";
    }

    int getBasePrice() override {
        return coffee->getPrice() + 10;
    }

    int getPrice() override {
        return getBasePrice();
    }
};

class ChocolateDecorator : public CoffeeDecorator {
public:
    ChocolateDecorator(Coffee* c)
        : CoffeeDecorator(c) {}

    string getDesc() override {
        return coffee->getDesc() + " + Chocolate";
    }

    int getBasePrice() override {
        return coffee->getPrice() + 20;
    }

    int getPrice() override {
        return getBasePrice();
    }
};

class PaymentStrategy {
public:
    virtual void pay(int amount) = 0;
    virtual ~PaymentStrategy() = default;
};

class UpiPayment : public PaymentStrategy {
public:
    void pay(int amount) override {
        cout << "Paid Rs." << amount
             << " using UPI\n";
    }
};

class CashPayment : public PaymentStrategy {
public:
    void pay(int amount) override {
        cout << "Paid Rs." << amount
             << " using Cash\n";
    }
};

class CoffeeMachine {
    Coffee* coffee = nullptr;
    PaymentStrategy* payment = nullptr;

public:

    void selectCoffee(
        const string& type,
        Size size)
    {
        coffee =
            CoffeeFactory::createCoffee(
                type,
                size
            );
    }

    void addMilk() {
        coffee = new MilkDecorator(coffee);
    }

    void addChocolate() {
        coffee = new ChocolateDecorator(coffee);
    }

    void selectPayment(
        PaymentStrategy* p)
    {
        payment = p;
    }

    void checkout()
    {
        cout << "Coffee : "
             << coffee->getDesc()
             << "\n";
        cout << "Price : "
             << coffee->getPrice()
             << "\n";
        payment->pay(
            coffee->getPrice()
        );
    }
    ~CoffeeMachine()
    {
        delete coffee;
    }
};

int main()
{
    CoffeeMachine machine;
    machine.selectCoffee(
        "americano",
        XL
    );
    machine.addMilk();
    machine.addChocolate();

    machine.selectPayment(
        new UpiPayment()
    );
    machine.checkout();
}