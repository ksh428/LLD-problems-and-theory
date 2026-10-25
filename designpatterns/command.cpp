
#include <bits/stdc++.h>
using namespace std;

/*
Command Pattern:
Source -> Command -> Receiver

Use cases:
1. Undo/Redo operations
2. Decoupling invoker from receiver
3. Queueing or scheduling commands
*/

class Command {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual ~Command() = default;
};

// Receivers
class Light {
public:
    void on() {
        cout << "Light turned on" << endl;
    }
    void off() {
        cout << "Light turned off" << endl;
    }
};

class Fan {
public:
    void on() {
        cout << "Fan turned on" << endl;
    }
    void off() {
        cout << "Fan turned off" << endl;
    }
};

class Lightcommand : public Command {
    Light* light;
public:
    Lightcommand(Light* pl) : light(pl) {}
    void execute() override {
        light->on();
    }
    void undo() override {
        light->off();
    }
};

class Fancommand : public Command {
    Fan* fan;
public:
    Fancommand(Fan* pf) : fan(pf) {}
    void execute() override {
        fan->on();
    }
    void undo() override {
        fan->off();
    }
};

enum COMMANDS {
    FAN,
    LIGHT
};

// Invoker
class Smartcontroller {
    int numbuttons;
    vector<Command*> commands;
    vector<bool> pressed;
    Fan* f;
    Light* l;
public:
    Smartcontroller(int n) : numbuttons(n) {
        commands.resize(n, nullptr);
        pressed.resize(n, false);
        f = new Fan();
        l = new Light();
    }
    void setcommand(int ind, COMMANDS cmd) {
        if (ind < 0 || ind >= numbuttons) {
            cout << "Invalid button index" << endl;
            return;
        }
        // Undo previous command before replacing it
        if (commands[ind]) {
            if (pressed[ind]) {
                commands[ind]->undo();
            }
            delete commands[ind];
            commands[ind] = nullptr;
        }
        switch (cmd) {
            case FAN:
                commands[ind] = new Fancommand(f);
                break;
            case LIGHT:
                commands[ind] = new Lightcommand(l);
                break;
            default:
                cout << "Invalid command" << endl;
                return;
        }
        pressed[ind] = false;
    }
    void pressbtn(int ind) {
        if (ind < 0 || ind >= numbuttons) {
            cout << "Invalid button index" << endl;
            return;
        }
        if (!commands[ind]) {
            cout << "No command assigned" << endl;
            return;
        }
        if (pressed[ind]) {
            commands[ind]->undo();
            pressed[ind] = false;
        } else {
            commands[ind]->execute();
            pressed[ind] = true;
        }
    }
    ~Smartcontroller() {
        for (Command* cmd : commands) {
            delete cmd;
        }
        delete f;
        delete l;
    }
    // Prevent accidental shallow copies
    Smartcontroller(const Smartcontroller&) = delete;
    Smartcontroller& operator=(const Smartcontroller&) = delete;
};

int main() {
    Smartcontroller controller(3);
    controller.setcommand(0, LIGHT);
    controller.setcommand(1, FAN);
    cout << "--- Button 0: Light ---" << endl;
    controller.pressbtn(0); // Light ON
    controller.pressbtn(0); // Light OFF
    cout << "\n--- Button 1: Fan ---" << endl;
    controller.pressbtn(1); // Fan ON
    controller.pressbtn(1); // Fan OFF
    cout << "\n--- Reassign Button 0 ---" << endl;
    controller.setcommand(0, FAN);
    controller.pressbtn(0); // Fan ON
    controller.pressbtn(0); // Fan OFF
    cout << "\n--- Unassigned Button ---" << endl;
    controller.pressbtn(2);
    return 0;
}
