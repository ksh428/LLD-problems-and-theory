#include <iostream>
#include <unordered_map>
using namespace std;

/*
 * LRU Cache Design
 * 
 * Requirements:
 * - get(key): Return value of key if exists, else -1. Mark as recently used.
 * - put(key, value): Put key-value pair. Evict LRU if cache is full.
 * 
 * Approach:
 * - Use doubly-linked list to maintain LRU order (head = most recent, tail = least recent)
 * - Use hash map for O(1) lookup of nodes
 * - All operations (get, put) should be O(1)
 * 
 * Time: O(1) for both get and put
 * Space: O(capacity)
 */

class LRUCache {
private:
    struct Node {
        int key;
        int value;
        Node* prev;
        Node* next;
        
        Node(int k = -1, int v = -1) : key(k), value(v), prev(nullptr), next(nullptr) {}
    };
    
    int capacity;
    unordered_map<int, Node*> cache;  // key -> Node pointer
    Node* head;  // dummy head (most recently used side)
    Node* tail;  // dummy tail (least recently used side)
    
    // Move node to head (mark as most recently used)
    void moveToHead(Node* node) {
        removeNode(node);
        addToHead(node);
    }
    
    // Add node right after head
    void addToHead(Node* node) {
        node->prev = head;
        node->next = head->next;
        head->next->prev = node;
        head->next = node;
    }
    
    // Remove node from its current position
    void removeNode(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    
    // Remove the least recently used node (right before tail)
    void removeLRU() {
        Node* lru = tail->prev;
        removeNode(lru);
        cache.erase(lru->key);
        delete lru;
    }

public:
    LRUCache(int cap) : capacity(cap) {
        // Create dummy head and tail nodes
        head = new Node();
        tail = new Node();
        head->next = tail;
        tail->prev = head;
    }
    
    ~LRUCache() {
        Node* current = head;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }
    
    int get(int key) {
        if (cache.find(key) == cache.end()) {
            return -1;
        }
        
        Node* node = cache[key];
        moveToHead(node);  // Mark as recently used
        return node->value;
    }
    
    void put(int key, int value) {
        // Key already exists: update value and move to head
        if (cache.find(key) != cache.end()) {
            Node* node = cache[key];
            node->value = value;
            moveToHead(node);
            return;
        }
        
        // New key: create new node
        Node* newNode = new Node(key, value);
        cache[key] = newNode;
        addToHead(newNode);
        
        // If exceeds capacity, remove LRU
        if (cache.size() > capacity) {
            removeLRU();
        }
    }
};

/**
 * Example Usage and Test Cases
 */
int main() {
    cout << "=== LRU Cache Test ===" << endl;
    
    LRUCache lruCache(2);
    
    // Test 1: Basic put and get
    lruCache.put(1, 1);
    lruCache.put(2, 2);
    cout << "get(1): " << lruCache.get(1) << endl;  // Output: 1
    
    // Test 2: Put new key, should evict key 2 (least recently used)
    lruCache.put(3, 3);
    cout << "get(2) after eviction: " << lruCache.get(2) << endl;  // Output: -1
    
    // Test 3: Current state: {1, 3}
    lruCache.put(4, 4);
    cout << "get(1) after eviction: " << lruCache.get(1) << endl;  // Output: -1
    
    // Test 4: Current state: {3, 4}
    cout << "get(3): " << lruCache.get(3) << endl;  // Output: 3
    cout << "get(4): " << lruCache.get(4) << endl;  // Output: 4
    
    cout << "\n=== Test Passed ===" << endl;
    
    return 0;
}

/*
 * Interview Discussion Points:
 * 1. Why doubly-linked list? Allows O(1) removal and insertion
 * 2. Why hash map? O(1) access to nodes
 * 3. Why dummy nodes? Simplifies edge cases (no null checks needed)
 * 4. Trade-offs: Extra space for map and linked list for O(1) operations
 * 5. Could use OrderedDict in Python (TreeMap in Java) for cleaner code
 */
