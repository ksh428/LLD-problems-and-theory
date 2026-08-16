#include <bits/stdc++.h>
using namespace std;

// MOVIE BOOKING SYSTEM
//
// In 45 mins we don't need:
// Movie, Theatre, Screen
//
// Core classes:
// Seat
// Show
// LockProvider
// PaymentStrategy
// Booking
// BookingService
//
// Locking is per (showId, seatId)

enum SEAT_TYPE {
    REGULAR,
    RECLINER
};

class Seat {
    SEAT_TYPE st;
    int id;

public:

    Seat(SEAT_TYPE s, int i)
        : st(s), id(i) {}

    int getseatid() const {
        return id;
    }
    int getprice() const {
        if (st == RECLINER)
            return 500;

        return 350;
    }
};

class Show {
    static int sid;
    int showid;
    vector<Seat> seats;
public:
    Show(vector<Seat> s)
        : seats(s) {
        showid = ++sid;
    }
    int getshowid() const {
        return showid;
    }
    bool haseat(int seatid) const {
        for (const auto &it : seats) {

            if (it.getseatid() == seatid)
                return true;
        }
        return false;
    }
};

int Show::sid = 0;

struct LockInfo {
    int userId;
    chrono::steady_clock::time_point expiry;
    LockInfo(
        int id,
        chrono::steady_clock::time_point exp
    )
        : userId(id),
          expiry(exp) {}
};

class Lockprovider {
    mutex mtx;
    map<int, map<int, LockInfo>> locks;
    map<int, set<int>> booked;

public:
    bool trylock(
        int showid,
        int userid,
        int seatid,
        int timeinsec = 300
    ) {
        lock_guard<mutex> lock(mtx);
        if (booked[showid].count(seatid))
            return false;
        auto now = chrono::steady_clock::now();
        auto &showlocks = locks[showid];
        auto it = showlocks.find(seatid);
        if (it == showlocks.end() ||
            it->second.expiry <= now) {
            showlocks.emplace(
            seatid,
            LockInfo(
                userid,
                now + chrono::seconds(timeinsec)
            )
            );
            return true;
        }
        return false;
    }
    void unlock(
        int showid,
        int userid,
        int seatid
    ) {
        lock_guard<mutex> lock(mtx);
        auto showit = locks.find(showid);
        if (showit == locks.end())
            return;
        auto seatit = showit->second.find(seatid);

        if (seatit != showit->second.end() &&
            seatit->second.userId == userid) {

            showit->second.erase(seatit);
        }
    }
    bool confirm(
        int showid,
        int userid,
        int seatid
    ) {
        lock_guard<mutex> lock(mtx);
        auto showit = locks.find(showid);
        if (showit == locks.end())
            return false;
        auto seatit = showit->second.find(seatid);
        if (seatit == showit->second.end())
            return false;
        if (seatit->second.userId != userid)
            return false;
        if (seatit->second.expiry <=
            chrono::steady_clock::now()) {
            showit->second.erase(seatit);
            return false;
        }
        showit->second.erase(seatit);
        booked[showid].insert(seatid);
        return true;
    }
};

class Paymentstrategy {
public:
    virtual bool makepayment(int amt) = 0;
    virtual ~Paymentstrategy() = default;
};

class UPI : public Paymentstrategy {
public:
    bool makepayment(int amt) override {
        cout << "Paid Rs. "<< amt << " using UPI\n";
        return true;
    }
};
class Card : public Paymentstrategy {
public:
    bool makepayment(int amt) override {
        cout << "Paid Rs. "<< amt<< " using Card\n";
        return true;
    }
};

class Booking {
    static int uid;
    int id;
    int userid;
    int showid;
    vector<int> seats;
public:
    Booking(
        int userId,
        int showId,
        vector<int> seatIds
    )
        : userid(userId),
          showid(showId),
          seats(seatIds) {
        id = ++uid;
    }
    int getid() const {
        return id;
    }
};

int Booking::uid = 0;
class Bookingservice {
    Lockprovider lp;
    map<int, Booking> bookings; // bookingId -> Booking
public:
    bool createbooking(
        int userid,
        Show& s,
        const vector<Seat>& seats,
        Paymentstrategy& ps
    ) {
        if (seats.empty())
            return false;
        int amt = 0;
        vector<int> locked;
        for (const auto& x : seats) {
            if (!s.haseat(x.getseatid())) {
                cout << "Seat "<< x.getseatid()<< " does not exist\n";
                for (int seatid : locked) {
                    lp.unlock(
                        s.getshowid(),
                        userid,
                        seatid
                    );
                }
                return false;
            }
            if (!lp.trylock(
                    s.getshowid(),
                    userid,
                    x.getseatid()
                )) {
                cout << "Seat " << x.getseatid() << " is not available\n";
                for (int seatid : locked) {
                    lp.unlock(
                        s.getshowid(),
                        userid,
                        seatid
                    );
                }
                return false;
            }
            locked.push_back(x.getseatid());
            amt += x.getprice();
        }
        if (!ps.makepayment(amt)) {
            cout << "Payment failed\n";
            for (int seatid : locked) {
                lp.unlock(
                    s.getshowid(),
                    userid,
                    seatid
                );
            }
            return false;
        }
        for (int seatid : locked) {
            if (!lp.confirm(
                    s.getshowid(),
                    userid,
                    seatid
                )) {
                cout << "Seat confirmation failed\n";
                return false;
            }
        }
        Booking booking(
            userid,
            s.getshowid(),
            locked
        );
        int bookingid = booking.getid();
        bookings.emplace(
            bookingid,
            std::move(booking)
        );
        cout << "Booking successful. "<< "Booking ID = "<< bookingid<< "\n";
        return true;
    }
};

int main() {
    Show show(
        {
            Seat(REGULAR, 1),
            Seat(REGULAR, 2),
            Seat(RECLINER, 3)
        }
    );
    Bookingservice service;
    UPI upi;
    Card card;
    // ========================================================
    // 1. User 1 books seats 1 and 2
    // ========================================================
    cout << "\nUser 1 books seats 1, 2\n";

    bool result = service.createbooking(
        1,
        show,
        {
            Seat(REGULAR, 1),
            Seat(REGULAR, 2)
        },
        upi
    );
    cout << "Result = " << result<< "\n";

    // ========================================================
    // 2. User 2 tries already booked seat 1
    // ========================================================
    cout << "\nUser 2 tries seat 1\n";

    result = service.createbooking(
        2,
        show,
        {
            Seat(REGULAR, 1)
        },
        card
    );
    cout << "Result = " << result<< "\n";
    // ========================================================
    // 3. User 2 books seat 3
    // ========================================================
    cout << "\nUser 2 books seat 3\n";
    result = service.createbooking(
        2,
        show,
        {
            Seat(RECLINER, 3)
        },
        card
    );
    cout << "Result = "
         << result
         << "\n";
    // ========================================================
    // 4. Non-existent seat
    // ========================================================
    cout << "\nUser 3 tries seat 99\n";
    result = service.createbooking(
        3,
        show,
        {
            Seat(REGULAR, 99)
        },
        upi
    );
    cout << "Result = "
         << result
         << "\n";

    // ========================================================
    // 5. Multiple-seat booking failure
    //
    // Both 1 and 2 are already booked.
    // ========================================================
    cout << "\nUser 4 tries seats 2 and 1\n";
    result = service.createbooking(
        4,
        show,
        {
            Seat(REGULAR, 2),
            Seat(REGULAR, 1)
        },
        upi
    );
    cout << "Result = "
         << result
         << "\n";
    return 0;
}