#include <bits/stdc++.h>
using namespace std;

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
}