#include <iostream>

using namespace std;

class Animal
{
    public:
    virtual void speak()=0;
};

class Dog: public Animal{
    public:
    void speak(){
        cout << "Woof!" << endl;
    }
};
class Elephant: public Animal{
    public:
    void speak(){
        cout << "Trumpet!" << endl;
    }
};

class AnimalFactory{
    public:
    static Animal* getAnimal(const string &name){
        if(name == "Dog"){
            return new Dog();
        } else if(name == "Elephant"){
            return new Elephant();
        } else {
            return nullptr; // Return nullptr for unknown animal types
        }
    }
};


int main()
{
    Animal * mydog=AnimalFactory::getAnimal("Dog");
    mydog->speak(); // Outputs: Woof!
    return 0;
}