#include<bits/stdc++.h>

using namespace std;

template<typename K,typename V>
struct Node
{
    K key;
    V val;
    Node* next;
    Node* prev;
    Node(const K& k,const V& v){
        key=k;
        val=v;
        next=nullptr;
        prev=nullptr;
    }
    void setval(const V& v) {val=v;}
};

template<typename K,typename V>
class Shard{
    int cap;
    map<K,Node<K,V>*>mp;
    mutex mtx;
    Node<K,V>* head;
    Node<K,V>* tail;

    void inserttohead(Node<K,V>* node)
    {
        Node<K,V>* nh=head->next;
        head->next=node;
        node->prev=head;
        nh->prev=node;
        node->next=nh;
    }
    void deletenode(Node<K,V>* node)
    {
        Node<K,V>* p=node->prev;
        Node<K,V>* n=node->next;
        p->next=n;
        n->prev=p;
    }
    void movetofront(Node<K,V>* node)
    {
        deletenode(node);
        inserttohead(node);
    }
    public:
    Shard(int c)
    {
        cap=c;
        head=new Node<K,V>(K{},V{});
        tail =new Node<K,V>(K{},V{});
        head->next=tail;
        tail->prev=head;
    }
    ~Shard()
    {
        auto curr=head;
        while(curr)
        {
            auto nxt=curr->next;
            delete curr;
            curr=nxt;
        }
    }
    bool get(const K& key,V& val)
    {
        lock_guard<mutex>lock(mtx);
        if(mp.find(key)==mp.end()) return 0;
        auto node=mp[key];
        movetofront(node);
        val=node->val;
        return 1;
    }
    void put(const K& key,const V& val){
        lock_guard<mutex> lock(mtx);
        if(mp.find(key)==mp.end())
        {
            if(mp.size()==cap)
            {
                Node<K,V>* pt=tail->prev;
                deletenode(pt);
                mp.erase(pt->key);
                delete pt;
            }
            Node<K,V>* nn=new Node<K,V>(key,val);
            inserttohead(nn);
            mp[key]=nn;
        }else{
            auto node=mp[key];
            node->setval(val);
            movetofront(node);
        }
    }
    bool remove(const K& key)
    {
        lock_guard<mutex> lock(mtx);
        if(mp.find(key)==mp.end())
        {
            return 0;
        }
        auto node=mp[key];
        deletenode(node);
        mp.erase(key);
        delete node;
        return 1;
    }
    size_t getsize()
    {
        lock_guard<mutex> lock(mtx);
        return mp.size();
    }
};

template<typename K,typename V>
class LRUCache{
    size_t numshards;
    vector<Shard<K,V>*>shards;
    size_t getshardidx(const K& key)
    {
        size_t hashval=hash<K>{}(key);
        return hashval%numshards;
    }
    Shard<K,V>* getshard(const K& key){
    return shards[getshardidx(key)];
   }
    public:
    LRUCache(int totcap,int nums){
        numshards=nums;
        int cap=totcap/nums;
        int rem=totcap%nums;
        for(int i=0;i<nums;i++)
        {
            if(i==nums-1)
            shards.push_back(new Shard<K,V>(cap+rem));
            else shards.push_back(new Shard<K,V>(cap));
        }
        
    }
    bool get(const K& key,V& val)
    {
        auto shard=getshard(key);
        return shard->get(key,val);
    }
    void put(const K& key, const V& val)
    {
         auto shard=getshard(key);
         shard->put(key,val);
    }
    bool remove(const K& key)
    {
        auto shard=getshard(key);
        return shard->remove(key);
    }
    int size()
    {
        int ans=0;
        for(auto it:shards)
        {
            ans+=it->getsize();
        }
        return ans;
    }
};

int main()
{
    LRUCache<int,string> cache(6,3);
    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three");
    cache.put(4, "four");
    string value;

    if (cache.get(2, value)) {
        cout << "Value = " << value << endl;
    }
    cache.remove(3);
    cout << "Cache size = "
         << cache.size()
         << endl;
}