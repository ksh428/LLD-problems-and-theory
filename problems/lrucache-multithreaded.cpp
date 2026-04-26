#include <bits/stdc++.h>
#include <mutex>

using namespace std;

template<typename K, typename V>
class Node {
public:
    K key;
    V value;
    Node* prev;
    Node* next;

    Node(const K& k, const V& v)
        : key(k), value(v), prev(nullptr), next(nullptr) {}
};

template<typename K, typename V>
class LRUCache {
private:
    using NodePtr = Node<K,V>*;

    int capacity;
    unordered_map<K, NodePtr> cache;

    NodePtr head;
    NodePtr tail;

    mutex mtx; // 🔐 global lock

    // ---------- DLL Helpers ----------
    void addToFront(NodePtr node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

    void removeNode(NodePtr node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

public:
    LRUCache(int cap) : capacity(cap) {
        head = new Node<K,V>(K(), V());
        tail = new Node<K,V>(K(), V());
        head->next = tail;
        tail->prev = head;
    }

    ~LRUCache() {
        NodePtr curr = head;
        while (curr) {
            NodePtr next = curr->next;
            delete curr;
            curr = next;
        }
    }

    // ---------- API ----------

    bool get(const K& key, V& value) {
        lock_guard<mutex> lock(mtx);  // 🔐 critical section

        auto it = cache.find(key);
        if (it == cache.end()) return false;

        NodePtr node = it->second;
        value = node->value;

        removeNode(node);
        addToFront(node);
        return true;
    }

    void put(const K& key, const V& value) {
        lock_guard<mutex> lock(mtx);  // 🔐 critical section

        auto it = cache.find(key);

        if (it != cache.end()) {
            NodePtr node = it->second;
            node->value = value;
            removeNode(node);
            addToFront(node);
            return;
        }

        if (cache.size() == capacity) {
            NodePtr lru = tail->prev;
            removeNode(lru);
            cache.erase(lru->key);
            delete lru;
        }

        NodePtr node = new Node<K,V>(key, value);
        addToFront(node);
        cache[key] = node;
    }
};

int main() {
    LRUCache<int, string> cache(2);

    string value;

    cache.put(1, "one");
    cache.put(2, "two");

    if (cache.get(1, value))
        cout << "Key 1: " << value << endl;   // one
    else
        cout << "Key 1 not found\n";

    cache.put(3, "three"); // evicts key 2

    if (cache.get(2, value))
        cout << "Key 2: " << value << endl;
    else
        cout << "Key 2 not found\n"; // expected

    cache.put(4, "four"); // evicts key 1

    if (cache.get(1, value))
        cout << "Key 1: " << value << endl;
    else
        cout << "Key 1 not found\n"; // expected

    if (cache.get(3, value))
        cout << "Key 3: " << value << endl; // three

    if (cache.get(4, value))
        cout << "Key 4: " << value << endl; // four

    return 0;
}

// SHARDED CACHE:

template<typename K, typename V>
class ShardedLRU {
private:
    int numShards;
    vector<LRUCache<K,V>*> shards;

    int getShard(const K& key) {
        return hash<K>{}(key) % numShards;
    }

public:
    ShardedLRU(int totalCapacity, int nShards) {
        numShards = nShards;
        int perShard = totalCapacity / nShards;

        for (int i = 0; i < numShards; i++) {
            shards.push_back(new LRUCache<K,V>(perShard));
        }
    }

    bool get(const K& key, V& value) {
        int sid = getShard(key);
        return shards[sid]->get(key, value);
    }

    void put(const K& key, const V& value) {
        int sid = getShard(key);
        shards[sid]->put(key, value);
    }
};