#include <bits/stdc++.h>
using namespace std;

template<typename K, typename V>
class LRUCache {
private:
    class Node {
    public:
        K key;
        V val;
        Node* prev;
        Node* next;

        Node(K k, V v) : key(k), val(v), prev(nullptr), next(nullptr) {}
    };

    int capacity;
    unordered_map<K, Node*> mp;
    Node* head;
    Node* tail;

    void addNode(Node* node) {
        Node* temp = head->next;
        head->next = node;
        node->prev = head;
        node->next = temp;
        temp->prev = node;
    }

    void deleteNode(Node* node) {
        Node* p = node->prev;
        Node* n = node->next;
        p->next = n;
        n->prev = p;
    }

public:
    LRUCache(int cap) {
        capacity = cap;
        head = new Node(K(), V()); // dummy
        tail = new Node(K(), V()); // dummy
        head->next = tail;
        tail->prev = head;
    }

    V get(K key) {
        if (mp.find(key) == mp.end()) {
            throw runtime_error("Key not found");
        }

        Node* node = mp[key];
        V value = node->val;

        deleteNode(node);
        addNode(node);

        return value;
    }

    void put(K key, V value) {
        if (mp.find(key) != mp.end()) {
            Node* node = mp[key];
            node->val = value;

            deleteNode(node);
            addNode(node);
            return;
        }

        if (mp.size() == capacity) {
            Node* lru = tail->prev;
            deleteNode(lru);
            mp.erase(lru->key);
            delete lru;
        }

        Node* node = new Node(key, value);
        addNode(node);
        mp[key] = node;
    }
};

int main() {
    LRUCache<int, string> cache(2);

    cache.put(1, "one");
    cache.put(2, "two");

    cout << cache.get(1) << endl; // "one"

    cache.put(3, "three"); // evicts key 2

    // cache.get(2); // throws error

    return 0;
}
