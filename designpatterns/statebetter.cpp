#include <bits/stdc++.h>
using namespace std;

class TrafficLight;   // forward declaration

// ---------------- STATE INTERFACE ----------------
class State {
public:
    virtual void handle(TrafficLight* light) = 0;
    virtual string name() = 0;
    virtual ~State() {}
};


// ---------------- CONTEXT ----------------
class TrafficLight {
private:
    State* currentState;

public:
    TrafficLight(State* state) {
        currentState = state;
    }

    void setState(State* state) {
        delete currentState;
        currentState = state;
    }

    void change() {
        currentState->handle(this);
    }

    void show() {
        cout << "Current State: " << currentState->name() << endl;
    }

    ~TrafficLight() {
        delete currentState;
    }
};


// ---------------- CONCRETE STATES ----------------

class RedState : public State {
public:
    void handle(TrafficLight* light) override;
    string name() override {
        return "RED";
    }
};

class ReadyState : public State {
public:
    void handle(TrafficLight* light) override;
    string name() override {
        return "READY";
    }
};

class GreenState : public State {
public:
    void handle(TrafficLight* light) override;
    string name() override {
        return "GREEN";
    }
};

class YellowState : public State {
public:
    void handle(TrafficLight* light) override;
    string name() override {
        return "YELLOW";
    }
};


// ---------------- STATE TRANSITIONS ----------------

void RedState::handle(TrafficLight* light) {
    cout << "Transition: RED -> READY\n";
    light->setState(new ReadyState());
}

void ReadyState::handle(TrafficLight* light) {
    cout << "Transition: READY -> GREEN\n";
    light->setState(new GreenState());
}

void GreenState::handle(TrafficLight* light) {
    cout << "Transition: GREEN -> YELLOW\n";
    light->setState(new YellowState());
}

void YellowState::handle(TrafficLight* light) {
    cout << "Transition: YELLOW -> RED\n";
    light->setState(new RedState());
}


// ---------------- CLIENT ----------------

int main() {

    TrafficLight light(new RedState());

    for(int i = 0; i < 8; i++) {
        light.show();
        light.change();
        cout << endl;
    }

    return 0;
}