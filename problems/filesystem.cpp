#include <bits/stdc++.h>
#include <chrono>
#include <mutex>
using namespace std;

// this is done using trie, the nodes will be either a folder/file but of the same type FileNode here
// then leaf node will be file always

class FileNode{// this can be file or folder
    string name;
    map<string,set<FileNode*>>children;
    public:
    FildeNode(string n)
    {
        name=n;
    }
    void addchild(string name,FileNode* f)
    {
        children[name].insert(f);
    }
    bool haschild(string name)
    {
        return children.find(name)!=children.end();
    }
    bool removechild(string name)
    {
        if(haschild(name))
        {
            children.erase(name);
            return 1;
        }
        return 0;
    }
    virtual bool isfile()=0;
    virtual void display()=0;
    // getters
    string getname() {return name;}
    set<FileNode*> getchildren() { return children[name];}
};
class File:public FileNode{
    string data;
    string extension;
    public:
    File(string name,string d,string e) :FileNode(name),data(d),extension(e) {}
    void setdata(string d)
    {
        data=d;
    }
    string getdata() { return data;}
    bool isfile()
    {
        return 1;
    }
    void display()
    {
        cout<<getname()<<endl;
        cout<<data<<endl;
    }
};
class Folder:public FileNode{
    public:
    Folder(string name):FileNode(name) {}
    bool isfile()
    {
        return 0;
    }
    void display()
    {
        cout<<getname()<<endl;
        for(auto it:getchildren())
        {
            it->display();
        }
    }
};
class FileSystem{
    FileNode* root;
    public:
    FileSystem()
    {
        root=new Folder("/");
    }
    bool createpath(string path)
    {
        stringstream ss(path);
        vector<string>comps;
        string t;
        char del = '/';
        while (getline(ss, t, del))
            comps.push_back(t);
        }
        FileNode* node=root;
        for(auto c:comps)
        {
            if(!node->haschild(c))
            {
                FileNode* fol=new Folder(c);
                node->addchild(fol);
            }
            
        }
    }   
    FileNode* getNode(string path)
    {
        
    }
    bool deletepath(string path)
    {
        
    }
}

int main() {
    
}