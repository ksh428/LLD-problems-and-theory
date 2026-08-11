#include <bits/stdc++.h>
using namespace std;

// --------------------------------------------------
// Resource
// --------------------------------------------------

class Connection {
    int id;

public:
    Connection(int id) : id(id) {}

    void execute(string query) {
        cout << "Connection " << id
             << " executing: " << query
             << " | thread: "
             << this_thread::get_id()
             << endl;

        // Simulate work
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    int getId() const {
        return id;
    }
};


// --------------------------------------------------
// Object Pool
// --------------------------------------------------

class ObjectPool {
private:

    // Objects currently available for use
    queue<shared_ptr<Connection>> available;

    mutex mtx;
    condition_variable cv;

    bool shutdown = false;

public:

    ObjectPool(int size) {

        for (int i = 0; i < size; i++) {
            available.push(
                make_shared<Connection>(i)
            );
        }
    }


    // --------------------------------------------------
    // Acquire an object
    // --------------------------------------------------

    shared_ptr<Connection> acquire() {

        unique_lock<mutex> lock(mtx);

        cv.wait(lock, [&]() {

            return shutdown || !available.empty();

        });

        if (shutdown) {
            return nullptr;
        }

        auto obj = available.front();

        available.pop();

        return obj;
    }


    // --------------------------------------------------
    // Release an object
    // --------------------------------------------------

    void release(shared_ptr<Connection> obj) {

        if (!obj)
            return;

        {
            lock_guard<mutex> lock(mtx);

            if (shutdown)
                return;

            available.push(obj);
        }

        cv.notify_one();
    }


    // --------------------------------------------------
    // Shutdown
    // --------------------------------------------------

    void close() {

        {
            lock_guard<mutex> lock(mtx);

            shutdown = true;
        }

        cv.notify_all();
    }
};


// --------------------------------------------------
// Client -> thread
// --------------------------------------------------

void client(ObjectPool& pool, int clientId) {

    auto connection = pool.acquire();

    if (!connection) {
        cout << "Client "
             << clientId
             << " : pool is shutting down\n";

        return;
    }

    cout << "Client "
         << clientId
         << " acquired connection "
         << connection->getId()
         << endl;


    connection->execute(
        "SELECT * FROM users"
    );


    cout << "Client "
         << clientId
         << " releasing connection "
         << connection->getId()
         << endl;


    pool.release(connection);
}


// --------------------------------------------------
// Main
// --------------------------------------------------

int main() {

    // Pool contains only 2 connections
    ObjectPool pool(2);

    vector<thread> threads;

    // 5 clients competing for 2 connections
    for (int i = 0; i < 5; i++) {

        threads.emplace_back(
            client,
            ref(pool),
            i
        );
    }

    for (auto& t : threads) {
        t.join();
    }

    pool.close();

    return 0;
}