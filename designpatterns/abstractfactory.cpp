#include <iostream>

using namespace std;

class Vehicle
{
    public:
    virtual void drive()=0;
};

class Honda : public Vehicle
{
    public:
    void drive() override {
        cout << "Driving a Honda civic!" << endl;
    }
};

class Toyota : public Vehicle
{
    public:
    void drive() override {
        cout << "Driving a Toyota!" << endl;
    }
};

class VehicleFactory
{
    public:
    virtual Vehicle* getVehicle(const string &name) = 0;
};

class ToyotaFactory : public VehicleFactory
{
    public:
     Vehicle* getVehicle(const string &name){
        return new Toyota();
    }
};

class HondaFactory : public VehicleFactory
{
    public:
     Vehicle* getVehicle(const string &name){
        return new Honda();
    }
};

int main()
{
    VehicleFactory *factory = new HondaFactory();
    Vehicle *vehicle = factory->getVehicle("Honda");
    vehicle->drive();

    // create toyota
    delete vehicle; // Clean up the Honda vehicle
    factory = new ToyotaFactory();
    vehicle = factory->getVehicle("Toyota");
    vehicle->drive();
    
    return 0;
}