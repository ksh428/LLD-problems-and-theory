#include <bits/stdc++.h>
using namespace std;

/// PREFER THIS VARIANT OF BUILDER DP OVER THE OTHER ONE IN INTERVIEWS

class Computer{
    string cpu;
    int ram;
    int rom;
    string storage;
    string gpu;
    Computer(string c,int r,int ro,string s,string g){
        cpu=c;
        ram=r;
        rom=ro;
        storage=s;
        gpu=g;
    }
    public:
    void getcomputer()
    {
        cout<<"computer details "<<endl;
        cout<<cpu<<" "<<ram<<" "<<rom<<" "<<storage<<" "<<gpu<<endl;
    }
    class Builder{
        string cpu="intel";
        int ram=8;
        int rom=512;
        string gpu="nvidia";
        string storage="default";
        public:
        Builder* setcpu(string c){
            cpu=c;
            return this;
        }
        Builder* setram(int r)
        {
            ram=r;
            return this;
        }
        Builder* setrom(int r)
        {
            rom=r;
            return this;
        }
        Builder* setgpu(string g)
        {
            gpu=g;
            return this;
        }
        Builder* setstorage(string s)
        {
            storage=s;
            return this;
        }
        Computer* build()
        {
            return new Computer(cpu,ram,rom,storage,gpu);
        }
    };
};

int main() {
    Computer* gamingpc=Computer::Builder()
    .setgpu("intel1")
    ->setcpu("amd")
    ->build();
    gamingpc->getcomputer();
    delete gamingpc;
}


/// GENERIC TEMPLATE
class Product {
    int a;
    int b;
    string c;

    Product(int a, int b, string c) : a(a), b(b), c(c) {}

public:
    class Builder {
        int a = 0;
        int b = 0;
        string c = "default";

    public:
        Builder& setA(int x) { a = x; return *this; }
        Builder& setB(int y) { b = y; return *this; }
        Builder& setC(string s) { c = move(s); return *this; }

        Product build() {   // value return preferred if cheap
            return Product(a, b, c);
        }
    };
};
