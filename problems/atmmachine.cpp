#include <bits/stdc++.h>
#include <mutex>
using namespace std;

// ------------------- USER -------------------
class User {
    int id;
public:
    User(int i) : id(i) {}
    int getid() { return id; }
};

// ------------------- CARD INTERFACE -------------------
class ICard {
public:
    virtual string cardtype() = 0;
    virtual User* getuser() = 0;
    virtual string getcardno() = 0;
    virtual ~ICard() = default;
};

// ------------------- DEBIT CARD -------------------
class DebitCard : public ICard {
    User* u;
    string cardno;
public:
    DebitCard(User* u1, string c) : u(u1), cardno(c) {}
    string cardtype() override { return "debit"; }
    User* getuser() override { return u; }
    string getcardno() override { return cardno; }
};

// ------------------- CREDIT CARD -------------------
class CreditCard : public ICard {
    User* u;
    string cardno;
public:
    CreditCard(User* u1, string c) : u(u1), cardno(c) {}
    string cardtype() override { return "credit"; }
    User* getuser() override { return u; }
    string getcardno() override { return cardno; }
};

// ------------------- BANK SERVER (Mock) -------------------
class BankServer {
public:
    static int checkbal(const string& cardno) {
        // Mock: Just return a random balance
        cout << "[BankServer] Checking balance for card: " << cardno << endl;
        return 5000; 
    }

    static int getpin(const string& cardno) {
        // Mock: Return a constant pin for simplicity
        return 1234;
    }
};

// Forward declaration
class ATMmachine;

// ------------------- ATM STATE INTERFACE -------------------
class ATMstate {
public:
    virtual string getstate() = 0;
    virtual ~ATMstate() = default;
};

// ------------------- SPECIFIC STATES -------------------
class NotProcessing : public ATMstate {
public:
    string getstate() override { return "NotProcessing"; }
};

class Processing : public ATMstate {
public:
    string getstate() override { return "Processing"; }
};

class Failed : public ATMstate {
public:
    string getstate() override { return "Failed"; }
};

class Succeeded : public ATMstate {
public:
    string getstate() override { return "Succeeded"; }
};

// ------------------- ATM MACHINE -------------------
class ATMmachine {
    ATMstate* currstate;
    ICard* card;
    bool ispinvalid = false;
    int retrycnt = 0;

public:
    ATMmachine() {
        currstate = new NotProcessing();
        card = nullptr;
    }

    ~ATMmachine() {
        if (currstate) delete currstate;
    }

    ATMstate* getstate() { return currstate; }

    void insertcard(ICard* c) {
        card = c;
        currstate = new Processing();
        cout << "[ATM] Card inserted (" << card->cardtype() << ")\n";
        cout << "Please enter your PIN:\n";
    }

    void enterpin(int no) {
        if (currstate->getstate() != "Processing") {
            cout << "[ATM] Invalid state. Please insert card first.\n";
            return;
        }

        string cardno = card->getcardno();
        int correctpin = BankServer::getpin(cardno);

        if (validatepin(no, correctpin)) {
            cout << "[ATM] PIN validated successfully. Please select an option.\n";
            ispinvalid = true;
        } else {
            retrycnt++;
            if (retrycnt < 3)
                cout << "[ATM] Incorrect PIN. Try again (" << retrycnt << "/3)\n";
            else {
                cout << "[ATM] Too many wrong attempts. Ejecting card.\n";
                eject();
            }
        }
    }

    void selectoption(int op) {
        if (!ispinvalid) {
            cout << "[ATM] Enter a valid PIN first.\n";
            return;
        }

        if (op == 1) {
            int bal = checkbalance();
            cout << "[ATM] Available balance: " << bal << " Rs\n";
        } else if (op == 2) {
            withdrawcash();
        } else {
            cout << "[ATM] Invalid option selected.\n";
        }
    }

    int checkbalance() {
        string cardno = card->getcardno();
        return BankServer::checkbal(cardno);
    }

    void withdrawcash() {
        cout << "[ATM] Withdrawing cash...\n";
        currstate = new Succeeded();
        cout << "[ATM] Transaction successful. Please collect your cash.\n";
    }

    bool validatepin(int n1, int n2) {
        return n1 == n2;
    }

    void eject() {
        cout << "[ATM] Ejecting card...\n";
        currstate = new Failed();
        card = nullptr;
        ispinvalid = false;
        retrycnt = 0;
        currstate = new NotProcessing();
    }
};

// ------------------- MAIN -------------------
int main() {
    User* u1 = new User(101);
    ICard* c1 = new DebitCard(u1, "1234-5678-9012");

    ATMmachine* atm = new ATMmachine();

    atm->insertcard(c1);
    atm->enterpin(123);   // wrong pin
    atm->enterpin(1111);  // wrong pin
    atm->enterpin(1234);  // correct pin
    atm->selectoption(1); // check balance
    atm->selectoption(2); // withdraw
}
