#include <bits/stdc++.h>
#include <thread>
#include <chrono>

using namespace std;

class Trafficontext;

// ---------------- STATE INTERFACE ----------------
class Trafficstate {
public:
    virtual string getstate() = 0;
    virtual void handle(Trafficontext* ctx) = 0;
    virtual ~Trafficstate() = default;
};

// ---------------- CONTEXT ----------------
class Trafficontext {
    Trafficstate* currstate;

public:
    Trafficontext();
    ~Trafficontext();

    void setState(Trafficstate* state);
    void request();
    void getcurrstate();
};

// ---------------- STATES ----------------
class Greenlight : public Trafficstate {
public:
    string getstate() override { return "green"; }

    void handle(Trafficontext* ctx) override;
};

class Yellowlight : public Trafficstate {
public:
    string getstate() override { return "yellow"; }

    void handle(Trafficontext* ctx) override;
};

class Redlight : public Trafficstate {
public:
    string getstate() override { return "red"; }

    void handle(Trafficontext* ctx) override;
};

// ---------------- STATE LOGIC ----------------
void Greenlight::handle(Trafficontext* ctx) {
    this_thread::sleep_for(chrono::seconds(3));
    cout << "Green → Yellow\n";
    ctx->setState(new Yellowlight());
}

void Yellowlight::handle(Trafficontext* ctx) {
    this_thread::sleep_for(chrono::seconds(2));
    cout << "Yellow → Red\n";
    ctx->setState(new Redlight());
}

void Redlight::handle(Trafficontext* ctx) {
    this_thread::sleep_for(chrono::seconds(3));
    cout << "Red → Green\n";
    ctx->setState(new Greenlight());
}

// ---------------- CONTEXT IMPLEMENTATION ----------------
Trafficontext::Trafficontext() {
    currstate = new Redlight();
}

Trafficontext::~Trafficontext() {
    delete currstate;
}

void Trafficontext::setState(Trafficstate* state) {
    delete currstate;
    currstate = state;
}

void Trafficontext::request() {
    currstate->handle(this);
}

void Trafficontext::getcurrstate() {
    cout << "Current State: " << currstate->getstate() << endl;
}

// ---------------- MAIN ----------------
int main() {
    Trafficontext ctx;

    for (int i = 0; i < 6; i++) {
        ctx.getcurrstate();
        ctx.request();
    }
}
