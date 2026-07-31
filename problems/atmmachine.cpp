
//BETTER STATE DESIGN PATTERN

#include <bits/stdc++.h>
using namespace std;

// ================= USER =================
class User {
    int id;
public:
    User(int i) : id(i) {}
    int getid() { return id; }
};

// ================= CARD =================
class Icard {
public:
    virtual string getcardtype() = 0;
    virtual string getcardno() = 0;
    virtual User* getuser() = 0;
    virtual ~Icard() = default;
};

class Debitcard : public Icard {
    string cardno;
    User* user;
public:
    Debitcard(string c, User* u) : cardno(c), user(u) {}
    string getcardtype() override { return "debit"; }
    string getcardno() override { return cardno; }
    User* getuser() override { return user; }
};

// ================= BANK SERVER =================
class Bankserver {
public:
    static int getbalance(const string&) { return 5000; }
    static int getpin(const string&) { return 1234; }
};

// ================= ATM INVENTORY =================
class ATMinventory {
    int cash;
public:
    ATMinventory(int c) : cash(c) {}

    bool withdrawcash(int amt) {
        if (amt > cash) {
            cout << "ATM has insufficient cash\n";
            return false;
        }
        cash -= amt;
        return true;
    }
};

// Forward declaration (ONLY this is fine)
class ATMmachine;

// ================= ATM STATE =================
class ATMstate {
public:
    virtual void nextstate(ATMmachine*) = 0;
    virtual void getstate() = 0;
    virtual ~ATMstate() = default;
};

// ================= CONCRETE STATES =================
class Notprocessed : public ATMstate {
public:
    void getstate() override {
        cout << "STATE: NotProcessed\n";
    }
    void nextstate(ATMmachine* m) override;
};

class Cardinserted : public ATMstate {
public:
    void getstate() override {
        cout << "STATE: CardInserted\n";
    }
    void nextstate(ATMmachine* m) override;
};

class Authenticated : public ATMstate {
public:
    void getstate() override {
        cout << "STATE: Authenticated\n";
    }
    void nextstate(ATMmachine* m) override;
};

// ================= ATM MACHINE =================
class ATMmachine {
    ATMstate* state;
    Icard* card;
    ATMinventory* inventory;
    int retrycnt;

public:
    ATMmachine(int cash)
        : state(nullptr), card(nullptr),
          inventory(new ATMinventory(cash)), retrycnt(0) {
        setstate(new Notprocessed());
    }

    ~ATMmachine() {
        delete state;
        delete inventory;
        delete card;
    }

    void setstate(ATMstate* s) {
        if (state) delete state;
        state = s;
    }

    void setnext() {
        state->nextstate(this);
    }

    void insertcard(Icard* c) { card = c; }
    Icard* getcard() { return card; }
    ATMinventory* getinventory() { return inventory; }

    bool withdraw(int amt) {
        return Bankserver::getbalance(card->getcardno()) >= amt;
    }
    void getcurrentstate()
    {
        state->getstate();
    }

    void increaseretrycnt() { retrycnt++; }
    int getretrycnt() { return retrycnt; }

    void eject() {
        cout << "Card ejected\n";
        retrycnt = 0;
        delete card;
        card = nullptr;
        setstate(new Notprocessed());
    }
};

// ================= STATE LOGIC =================
void Notprocessed::nextstate(ATMmachine* m) {
    cout << "Insert card\n";
    m->insertcard(new Debitcard("123456", new User(1)));
    m->setstate(new Cardinserted());
}

void Cardinserted::nextstate(ATMmachine* m) {
    cout << "Enter PIN: ";
    int pin;
    cin >> pin;

    if (m->getretrycnt() == 3) {
        cout << "Too many attempts\n";
        m->eject();
        return;
    }

    if (pin == Bankserver::getpin(m->getcard()->getcardno())) {
        cout << "Authentication successful\n";
        m->setstate(new Authenticated());
    } else {
        cout << "Wrong PIN\n";
        m->increaseretrycnt();
    }
}

void Authenticated::nextstate(ATMmachine* m) {
    cout << "1. Balance\n2. Withdraw\nChoice: ";
    int op;
    cin >> op;

    if (op == 1) {
        cout << "Balance: "
             << Bankserver::getbalance(m->getcard()->getcardno()) << endl;
             return;
    }
    else if (op == 2) {
        cout << "Amount: ";
        int amt;
        cin >> amt;

        if (!m->getinventory()->withdrawcash(amt) ||
            !m->withdraw(amt)) {
            cout << "Transaction failed\n";
            m->eject();
            return;
        }
        cout << "Cash withdrawn\n";
        m->setstate(new Notprocessed());
    }
    else {
        cout << "Invalid option\n";
        m->eject();
    }
}

// ================= MAIN =================
int main() {
    ATMmachine atm(10000);
    atm.getcurrentstate();
    atm.setnext();
    atm.getcurrentstate();
    atm.setnext();
    atm.getcurrentstate();
    atm.setnext();
    atm.getcurrentstate();
    atm.setnext();
    return 0;
}

