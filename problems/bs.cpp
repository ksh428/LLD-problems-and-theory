#include <bits/stdc++.h>
using namespace std;

enum class SeatType {
    REGULAR,
    RECLINER
};

enum class SeatStatus {
    AVAILABLE,
    LOCKED,
    BOOKED
};

class Seat {
    int id;
    SeatType type;

public:
    Seat(int id, SeatType type)
        : id(id), type(type) {}

    int getId() const {
        return id;
    }

    int getPrice() const {
        return type == SeatType::RECLINER ? 500 : 350;
    }
};

class Show {
    int id;
    vector<Seat> seats;

public:
    Show(int id, vector<Seat> seats)
        : id(id), seats(seats) {}

    int getId() const {
        return id;
    }

    bool hasSeat(int seatId) const {
        for (auto &s : seats)
            if (s.getId() == seatId)
                return true;

        return false;
    }
};

struct LockInfo {
    int userId;
    chrono::steady_clock::time_point expiry;
};

class LockProvider {

    mutex mtx;

    // showId -> seatId -> lock info
    map<int, map<int, LockInfo>> locks;

    // showId -> booked seats
    map<int, set<int>> booked;

public:

    bool tryLock(int showId,
                 int seatId,
                 int userId,
                 int timeout = 300) {

        lock_guard<mutex> lock(mtx);

        if (booked[showId].count(seatId))
            return false;

        auto now = chrono::steady_clock::now();

        auto &showLocks = locks[showId];

        auto it = showLocks.find(seatId);

        if (it == showLocks.end() ||
            it->second.expiry <= now) {

            showLocks[seatId] = {
                userId,
                now + chrono::seconds(timeout)
            };

            return true;
        }

        return false;
    }

    void unlock(int showId,
                int seatId,
                int userId) {

        lock_guard<mutex> lock(mtx);

        auto it = locks[showId].find(seatId);

        if (it != locks[showId].end() &&
            it->second.userId == userId) {

            locks[showId].erase(it);
        }
    }

    bool confirm(int showId,
                 int seatId,
                 int userId) {

        lock_guard<mutex> lock(mtx);

        auto it = locks[showId].find(seatId);

        if (it == locks[showId].end())
            return false;

        if (it->second.userId != userId)
            return false;

        if (it->second.expiry <= chrono::steady_clock::now()) {
            locks[showId].erase(it);
            return false;
        }

        locks[showId].erase(it);
        booked[showId].insert(seatId);

        return true;
    }
};

class PaymentStrategy {
public:
    virtual bool pay(int amount) = 0;
    virtual ~PaymentStrategy() = default;
};

class UPI : public PaymentStrategy {
public:
    bool pay(int amount) override {
        cout << "Paid using UPI\n";
        return true;
    }
};

class Card : public PaymentStrategy {
public:
    bool pay(int amount) override {
        cout << "Paid using Card\n";
        return true;
    }
};

class Booking {
    int id;
    int userId;
    int showId;
    vector<int> seatIds;

public:
    Booking(int id,
            int userId,
            int showId,
            vector<int> seatIds)
        : id(id),
          userId(userId),
          showId(showId),
          seatIds(seatIds) {}
};

class BookingService {

    LockProvider lockProvider;

    int nextBookingId = 1;

public:

    bool book(int userId,
              Show &show,
              vector<Seat> seats,
              PaymentStrategy &payment) {
        vector<int> locked;
        int amount = 0;
        for (auto &seat : seats) {

            if (!show.hasSeat(seat.getId()) ||
                !lockProvider.tryLock(
                    show.getId(),
                    seat.getId(),
                    userId)) {

                // rollback
                for (int id : locked)
                    lockProvider.unlock(
                        show.getId(),
                        id,
                        userId);

                return false;
            }

            locked.push_back(seat.getId());
            amount += seat.getPrice();
        }
        if (!payment.pay(amount)) {

            for (int id : locked)
                lockProvider.unlock(
                    show.getId(),
                    id,
                    userId);

            return false;
        }
        for (int id : locked) {

            if (!lockProvider.confirm(
                    show.getId(),
                    id,
                    userId)) {

                return false;
            }
        }
        Booking booking(
            nextBookingId++,
            userId,
            show.getId(),
            locked
        );
        cout << "Booking successful\n";
        return true;
    }
};