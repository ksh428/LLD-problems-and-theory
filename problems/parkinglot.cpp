#include <bits/stdc++.h>
#include <mutex>
using namespace std;

// =========================== ENGINE TYPES ===========================
class Enginetype {
public:
    virtual string getengtype() = 0;
    virtual ~Enginetype() = default;
};

class EV : public Enginetype {
public:
    string getengtype() override { return "ev"; }
};

class Nonev : public Enginetype {
public:
    string getengtype() override { return "non-ev"; }
};

// =========================== VEHICLE TYPES ===========================
class IVehicle {
public:
    virtual string Vehicletype() = 0;
    virtual Enginetype* getengtype() = 0;
    virtual string getvehicleno() = 0;
    virtual ~IVehicle() = default;
};

class Twowheeler : public IVehicle {
    Enginetype* eng;
    string no;
public:
    Twowheeler(Enginetype* e, string n) : eng(e), no(n) {}
    string getvehicleno() override { return no; }
    Enginetype* getengtype() override { return eng; }
    string Vehicletype() override { return "2 wheeler"; }
};

class Fourwheeler : public IVehicle {
    Enginetype* eng;
    string no;
public:
    Fourwheeler(Enginetype* e, string n) : eng(e), no(n) {}
    string getvehicleno() override { return no; }
    Enginetype* getengtype() override { return eng; }
    string Vehicletype() override { return "4 wheeler"; }
};

// =========================== ENUMS ===========================
enum Spot {
    TWO_EV,
    TWO_NONEV,
    FOUR_EV,
    FOUR_NONEV
};

// =========================== VEHICLE FACTORY ===========================
class VehicleFactory {
public:
    static IVehicle* getvehicle(const string& type, const string& no, const string& engtype, Spot& s) {
        if (type == "2 wheeler") {
            if (engtype == "ev") {
                s = TWO_EV;
                return new Twowheeler(new EV(), no);
            } else {
                s = TWO_NONEV;
                return new Twowheeler(new Nonev(), no);
            }
        } else {
            if (engtype == "ev") {
                s = FOUR_EV;
                return new Fourwheeler(new EV(), no);
            } else {
                s = FOUR_NONEV;
                return new Fourwheeler(new Nonev(), no);
            }
        }
    }
};

// =========================== PAYMENT STRATEGY ===========================
class Paystrategy {
public:
    virtual string getstrategy() = 0;
    virtual void pay() = 0;
    virtual ~Paystrategy() = default;
};

class Upi : public Paystrategy {
public:
    string getstrategy() override { return "upi"; }
    void pay() override { cout << "Paid using UPI" << endl; }
};

class Cash : public Paystrategy {
public:
    string getstrategy() override { return "cash"; }
    void pay() override { cout << "Paid using Cash" << endl; }
};

// =========================== TICKET ===========================
class Ticket {
    IVehicle* v;
    Spot s;
    Paystrategy* ps;
public:
    Ticket(const string& type, const string& no, const string& engtype, const string& p) {
        Spot s1 = FOUR_NONEV;
        v = VehicleFactory::getvehicle(type, no, engtype, s1);
        s = s1;

        if (p == "cash")
            ps = new Cash();
        else
            ps = new Upi();
    }

    IVehicle* getvehiclefromticket() { return v; }
    Spot getspot() { return s; }
    Paystrategy* getps() { return ps; }

    ~Ticket() {
        delete v->getengtype(); // free engine
        delete v;               // free vehicle
        delete ps;              // free payment strategy
    }
};

// =========================== PARKING LOT ===========================
class ParkingLot {
    vector<Ticket*> tickets;
    map<string, Ticket*> nototicket;

public:
    void enter(const string& type, const string& no, const string& engtype, const string& p) {
        if (nototicket.find(no) != nototicket.end()) {
            cout << "Vehicle " << no << " already entered." << endl;
            return;
        }

        Ticket* t = new Ticket(type, no, engtype, p);
        nototicket[no] = t;
        tickets.push_back(t);
        cout << "Ticket issued for vehicle no: " << no << endl;
    }

    void park(const string& no) {
        if (nototicket.find(no) != nototicket.end()) {
            cout << "Parking vehicle no: " << no << endl;
        } else {
            cout << "Please purchase a ticket before parking" << endl;
        }
    }

    void exitparking(const string& no) {
        auto it = nototicket.find(no);
        if (it != nototicket.end()) {
            cout << "Exiting parking for vehicle no: " << no << endl;
            Ticket* t = it->second;

            // Simulate payment
            t->getps()->pay();

            // Remove from vector
            tickets.erase(remove(tickets.begin(), tickets.end(), t), tickets.end());

            delete t;
            nototicket.erase(it);
        } else {
            cout << "No such vehicle found" << endl;
        }
    }

    ~ParkingLot() {
        for (auto& t : tickets) delete t;
        tickets.clear();
        nototicket.clear();
    }
};

// =========================== MAIN ===========================
int main() {
    ParkingLot lot;
    lot.enter("2 wheeler", "WB1234", "ev", "upi");
    lot.park("WB1234");
    lot.exitparking("WB1234");

    lot.enter("4 wheeler", "DL9999", "non-ev", "cash");
    lot.park("DL9999");
    lot.exitparking("DL9999");

    return 0;
}


// ADDITIONAL MODIFICATIONS:
// implement builder dp for ticket class


class Ticket {
    IVehicle* vehicle;
    SpotType spot;
    PayStrategy* payment;
    time_t entryTime;
    time_t exitTime;
    double fee;
    string gate;
    bool paid;

    // private constructor
    Ticket(IVehicle* v, SpotType s, PayStrategy* p,
           time_t eIn, time_t eOut, double f, string g, bool pd)
        : vehicle(v), spot(s), payment(p),
          entryTime(eIn), exitTime(eOut), fee(f), gate(g), paid(pd) {}

public:
    // ---------- Getters ----------
    IVehicle* getVehicle() { return vehicle; }
    SpotType getSpot() { return spot; }
    PayStrategy* getPayment() { return payment; }
    double getFee() { return fee; }
    bool isPaid() { return paid; }

    // ---------- Builder Class ----------
    class Builder {
        IVehicle* vehicle = nullptr;
        SpotType spot = TWO_NON_EV;
        PayStrategy* payment = nullptr;
        time_t entryTime = time(nullptr);
        time_t exitTime = 0;
        double fee = 0.0;
        string gate = "Default";
        bool paid = false;

    public:
        Builder& setVehicle(IVehicle* v) { vehicle = v; return *this; }
        Builder& setSpot(SpotType s) { spot = s; return *this; }
        Builder& setPayment(PayStrategy* p) { payment = p; return *this; }
        Builder& setEntry(time_t e) { entryTime = e; return *this; }
        Builder& setExit(time_t e) { exitTime = e; return *this; }
        Builder& setFee(double f) { fee = f; return *this; }
        Builder& setGate(string g) { gate = g; return *this; }
        Builder& setPaid(bool p) { paid = p; return *this; }

        Ticket* build() {
            if (!vehicle || !payment) {
                throw runtime_error("Missing mandatory fields for Ticket");
            }
            return new Ticket(vehicle, spot, payment, entryTime, exitTime, fee, gate, paid);
        }
    };
};

// USING IT IN Parking lot

Ticket* t = Ticket::Builder()
                .setVehicle(v)
                .setSpot(s)
                .setPayment(p)
                .setEntry(time(nullptr))
                .setGate("Gate A")
                .build();