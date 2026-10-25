#include<bits/stdc++.h>

using namespace std;

class Document{
    string name;
    string text;
    string path;
    public:
    Document(string n,string t,string p):name(n),text(t),path(p) {}
    void inserttext(int pos,string s)
    {
        text.insert(pos,s);
    }
    string deletetext(int pos,int len)
    {
        string deleted=text.substr(pos,len);
        text.erase(pos,len);
        return deleted;
    }
    string getname() {return name;}
    string gettext() {return text;}
    string getpath() {return path;}
    void updatename(string s){
        name=s;
    }
    void updatepath(string s)
    {
        path=s;
    }
};

class Command{
    public:
    virtual void execute()=0;
    virtual void undo()=0;
    virtual ~Command()=default;
};
class Insert:public Command{
    Document* doc;
    int pos;
    string s;
    public:
    Insert(Document* dd,int p,string ps):doc(dd),pos(p),s(ps) {}
    void execute()
    {
        doc->inserttext(pos,s);
    }
    void undo()
    {
        doc->deletetext(pos,s.size());
    }
};
class Delete:public Command{
    Document* doc;
    int pos;
    int len;
    string deletedtext="";
    public:
    Delete(Document* d,int p,int l):doc(d),pos(p),len(l) {}
    void execute()
    {
        deletedtext=doc->deletetext(pos,len);
    }
    void undo()
    {
        doc->inserttext(pos,deletedtext);
    }
};
class Commandmanager{
    stack<Command*> undostack;
    stack<Command*> redostack;
    public:
    void executecommand(Command* c){
        c->execute();
        undostack.push(c);
        while(!redostack.empty())
        {
            delete redostack.top();
            redostack.pop();
        }
    }
    void undo()
    {
        if(undostack.empty())
        {
            cout<<"Nothing to undo"<<endl;
            return;
        }
        auto c=undostack.top();
        undostack.pop();
        redostack.push(c);
        c->undo();
    }
    void redo()
    {
        if(redostack.empty())
        {
            cout<<"Nothing to redo"<<endl;
            return ;
        }
        auto c=redostack.top();
        redostack.pop();
        undostack.push(c);
        c->execute();
    }
    ~Commandmanager()
    {
        while(!redostack.empty())
        {
            delete redostack.top();
            redostack.pop();
        }
        while(!undostack.empty())
        {
            delete undostack.top();
            undostack.pop();
        }
    }
};
class IOops{
    map<string,Document*>docs;
    public:
    void savedoc(Document* doc)
    {
        if(docs.find(doc->getname())!=docs.end())
        {
            cout<<"doc already saved"<<endl;
            return;
        }
        docs[doc->getname()]=doc;
    }
    void deletedoc(string name)
    {
        if(docs.find(name)==docs.end())
        {
            cout<<"doc does not exist"<<endl;
            return;
        }
        auto it=docs[name];
        delete it;
        docs.erase(name);
    }
};

class TextEditor{
    Document* doc;
    Commandmanager* mgr;
    IOops* io;
    public:
    TextEditor()
    {
        doc=nullptr;
        mgr=new Commandmanager();
        io=new IOops();
    }
    void createdoc(string name,string text,string path)
    {
        doc=new Document(name,text,path);
        io->savedoc(doc);
    }
    void insert(int pos,string text)
    {
        Command* c=new Insert(doc,pos,text);
        mgr->executecommand(c);
    }
    void erase(int pos,int len){
        Command* c=new Delete(doc,pos,len);
        mgr->executecommand(c);
    }
    void undo()
    {
        mgr->undo();
    }
    void redo()
    {
        mgr->redo();
    }
    void print()
    {
        cout<<doc->getname()<<endl;
        cout<<doc->gettext()<<endl;
    }
    void deletedoc(string name)
    {
        io->deletedoc(name);
        delete doc;
    }
    ~TextEditor()
    {
        delete io;
        delete mgr;
        delete doc;
    }
};

int main()
{
    TextEditor* editor=new TextEditor();
    editor->createdoc("test1","dummy","d/data/test");
    editor->print();
    editor->insert(0,"hello");
    editor->print();
    editor->undo();
    editor->print();
}