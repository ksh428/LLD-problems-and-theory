#include <bits/stdc++.h>
using namespace std;

// ---------------- Product Hierarchy ----------------

class Product {
    int number;
public:
    Product(int n) : number(n) {}
    virtual int getno() { return number; }
    virtual string type() = 0;
    virtual ~Product() = default;
};

class Grocery : public Product {
public:
    Grocery(int n) : Product(n) {}
    int getno() override { return Product::getno(); }
    string type() override { return "grocery"; }
};

class Clothing : public Product {
public:
    Clothing(int n) : Product(n) {}
    int getno() override { return Product::getno(); }
    string type() override { return "clothing"; }
};

class Electronics : public Product {
public:
    Electronics(int n) : Product(n) {}
    int getno() override { return Product::getno(); }
    string type() override { return "electronics"; }
};

// ---------------- Factory ----------------

class ProductFactory {
public:
    static Product* getproduct(int n, const string& type) {
        if (type == "grocery") {
            return new Grocery(n);
        } else if (type == "electronics") {
            return new Electronics(n);
        } else if (type == "clothing") {
            return new Clothing(n);
        } else {
            throw invalid_argument("Unknown product type");
        }
    }
};

// ---------------- Supplier / Observer ----------------

class SupplyOrg {
    int sid;
public:
    SupplyOrg(int i) : sid(i) {}

    void refill(int warehouseId, int amt) {
        cout << "Supplier ID " << sid 
             << " received refill request from warehouse " << warehouseId 
             << " for " << amt << " units." << endl;
    }
};

// ---------------- Notifier Interface ----------------

class INotifier {
public:
    virtual void notify(int id, int amt) = 0;
    virtual void subscribe(SupplyOrg* o) = 0;
    virtual void unsubscribe(SupplyOrg* o) = 0;
    virtual ~INotifier() = default;
};

// ---------------- SMS Notifier ----------------

class SMSNotifier : public INotifier {
    vector<SupplyOrg*> suppliers;
public:
    void notify(int id, int amt) override {
        for (auto it : suppliers) {
            it->refill(id, amt);
        }
    }

    void subscribe(SupplyOrg* o) override {
        suppliers.push_back(o);
    }

    void unsubscribe(SupplyOrg* o) override {
        vector<SupplyOrg*> temp;
        for (auto it : suppliers) {
            if (it != o)
                temp.push_back(it);
        }
        suppliers = temp;
    }
};

// ---------------- Call Notifier ----------------

class CallNotifier : public INotifier {
    vector<SupplyOrg*> suppliers;
public:
    void notify(int id, int amt) override {
        for (auto it : suppliers) {
            it->refill(id, amt);
        }
    }

    void subscribe(SupplyOrg* o) override {
        suppliers.push_back(o);
    }

    void subscribeMultiple(const vector<SupplyOrg*>& v) {
        for (auto it : v)
            suppliers.push_back(it);
    }

    void unsubscribe(SupplyOrg* o) override {
        vector<SupplyOrg*> temp;
        for (auto it : suppliers) {
            if (it != o)
                temp.push_back(it);
        }
        suppliers = temp;
    }
};

// ---------------- Warehouse ----------------

class Warehouse {
    int id;
    INotifier* notifier;
    int maxcapacity;
    int currcapacity;
    vector<Product*> products;
    vector<SupplyOrg*> suppliers;  // each warehouse has suppliers

public:
    Warehouse(int i, int n1) : id(i), maxcapacity(n1), currcapacity(0) {
        // Create suppliers
        for (int j = 0; j < 3; j++) {  // 3 sample suppliers per warehouse
            suppliers.push_back(new SupplyOrg(j + i * 10));
        }

        // Use SMS notifier by default
        notifier = new SMSNotifier();
        for (auto s : suppliers)
            notifier->subscribe(s);
    }

    int getid() const { return id; }

    void setproducts(const vector<Product*>& prd) {
        products = prd;
        currcapacity = 0;
        for (auto it : products)
            currcapacity += it->getno();
    }

    bool refillrequired() const {
        return currcapacity < 0.75 * maxcapacity;
    }

    int getcapacity() const {
        return currcapacity;
    }

    void refill() {
        if (refillrequired()) {
            int amt = maxcapacity - currcapacity;
            cout << "\nWarehouse " << id << " needs refill of " << amt << " units.\n";
            notifier->notify(id, amt);
            products.push_back(ProductFactory::getproduct(amt / 10, "grocery"));
            currcapacity = maxcapacity;
        }
    }

    void display() const {
        cout << "Warehouse " << id << " -> Capacity: " 
             << currcapacity << "/" << maxcapacity << endl;
    }

    ~Warehouse() {
        for (auto p : products) delete p;
        for (auto s : suppliers) delete s;
        delete notifier;
    }
};

// ---------------- Management System ----------------

class MgmtSystem {
    vector<Warehouse*> warehouses;

public:
    MgmtSystem(const vector<pair<int, int>>& info) {
        for (auto it : info) {
            Warehouse* w = new Warehouse(it.first, it.second);
            int no = it.second / 2;
            vector<Product*> products;
            for (int i = 0; i < no / 3; i++) {
                products.push_back(ProductFactory::getproduct(10, "grocery"));
                products.push_back(ProductFactory::getproduct(5, "electronics"));
                products.push_back(ProductFactory::getproduct(15, "clothing"));
            }
            w->setproducts(products);
            warehouses.push_back(w);
        }
    }

    void addwarehouse(int id, int n) {
        warehouses.push_back(new Warehouse(id, n));
    }

    void deletewarehouse(int id) {
        vector<Warehouse*> remaining;
        for (auto it : warehouses) {
            if (it->getid() != id) {
                remaining.push_back(it);
            } else {
                cout << "Deleting warehouse " << id << endl;
                delete it;
            }
        }
        warehouses = remaining;
    }

    void refillAll() {
        for (auto w : warehouses)
            w->refill();
    }

    void displayAll() const {
        for (auto w : warehouses)
            w->display();
    }

    ~MgmtSystem() {
        for (auto w : warehouses)
            delete w;
    }
};

// ---------------- Main ----------------

int main() {
    vector<pair<int, int>> info = {{1, 100}, {2, 200}};
    MgmtSystem sys(info);

    cout << "\nInitial status:\n";
    sys.displayAll();

    sys.refillAll();

    sys.addwarehouse(3, 150);
    sys.deletewarehouse(1);

    cout << "\nAfter changes:\n";
    sys.displayAll();

    cout << "\nSystem initialized successfully!" << endl;
    return 0;
}
