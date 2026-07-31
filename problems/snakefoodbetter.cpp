#include<bits/stdc++.h>
using namespace std;

class Ifood{
    public:
    virtual string getfoodtype()=0;
    virtual int getscore()=0;
    virtual pair<int,int> getfoodpos()=0;
    virtual ~Ifood()=default;
};
class Fruit:public Ifood{
    int score;
    int x,y;
    public:
    Fruit(int sc=5,int r=0,int c=0):score(sc),x(r),y(c) {}
    
    string getfoodtype()
    {
        return "fruit";
    }
    pair<int,int>getfoodpos() {return {x,y};}
    int getscore() {return score;}
};
class Thorn:public Ifood{
    int score;
    int x,y;
    public:
    Thorn(int sc=-1,int r=0,int c=0):score(sc),x(r),y(c) {}
    pair<int,int>getfoodpos() {return {x,y};}
    string getfoodtype()
    {
        return "thorn";
    }
    int getscore() {return score;}
};
class Foodfactory{
    public:
    static Ifood* createfood(int type,int x,int y)
    {
        if(type==1)
        {
            return new Fruit(5,x,y);
        }else return new Thorn(-1,x,y);
    }
};
class Grid{
    int m;
    int n;
    vector<vector<int>>cells;
    public:
    Grid(int x,int y):m(x),n(y) {
        cells.resize(m,vector<int>(n,0));
    }
    pair<int,int> getdim() {return {m,n};}
};

// dir map: enum
enum DIR{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Snake{
    deque<pair<int,int>>dq;
    map<pair<int,int>,int>snakepos;
    DIR d;
    public:
    Snake(int m,int n)
    {
        dq.push_back({m/2,n/2});
        snakepos[{m/2,n/2}]=1;
        d=RIGHT;
    }
    bool canmoved(DIR d1)
    {
        if(d==RIGHT and d1==LEFT) return 0;
        if(d==LEFT and d1==RIGHT) return 0;
        if(d==UP and d1==DOWN) return 0;
        if(d==DOWN and d1==UP) return 0;
        return 1;
    }
    void setdir(DIR d1)
    {
        d=d1;
    }
    pair<int,int>getnewhead(DIR d1){
        auto ch=dq.front();
        if(d1==UP) {
            return {ch.first-1,ch.second};
        }else if(d1==DOWN)
        {
            return {ch.first+1,ch.second};
        }else if(d1==LEFT)
        {
            return {ch.first,ch.second-1};
        }else return {ch.first,ch.second+1};
    }
    int move(pair<int,int>nh,map<pair<int,int>,Ifood*>&foodmap)
    {
        int x=nh.first;
        int y=nh.second;
        if(snakepos.find({x,y})!=snakepos.end())
        {
            return INT_MIN; // 0-> self collision
        }
        int ate=0;
        int score=0;
        if(foodmap.find({nh})!=foodmap.end())
        {
            ate=1;
            score=foodmap[nh]->getscore();
            delete foodmap[nh];
            foodmap.erase(nh);
        }
        dq.push_front({x,y});
        snakepos[{x,y}]=1;
        if(ate==0)
        {
            auto bk=dq.back();
            snakepos.erase(bk);
            dq.pop_back();
        }
        return score;
    }
};

class Game;

class Gamestate{
    public:
    virtual void getstate()=0;
    virtual void makemove(Game* g,DIR d)=0;
    virtual ~Gamestate()=default;
};
class Notready:public Gamestate{
    public:
    void getstate()
    {
        cout<<"Not ready yet"<<endl;
    }
    void makemove(Game* g,DIR d);
};
class Playing:public Gamestate{
    public:
    void getstate()
    {
        cout<<"Playing"<<endl;
    }
    void makemove(Game* g,DIR d);
};
class End:public Gamestate{
    public:
    void getstate()
    {
        cout<<"Game Ended"<<endl;
    }
    void makemove(Game* g,DIR d);
};

class Game{
    Grid* g;
    map<pair<int,int>,Ifood*>foodmap;
    Gamestate* currstate;
    Snake* snake;
    int score;
    public:
    void populatefoods(){
        foodmap[{0,0}]=Foodfactory::createfood(1,0,0);
        foodmap[{2,0}]=Foodfactory::createfood(1,2,0);
        foodmap[{0,2}]=Foodfactory::createfood(1,0,2);
        foodmap[{3,3}]=Foodfactory::createfood(2,3,3);
        
    }
    map<pair<int,int>,Ifood*>& getfoodmap()
    {
        return foodmap;
    }
    Game(int m,int n)
    {
        g=new Grid(m,n);
        populatefoods();
        snake=new Snake(m,n);
        currstate=new Notready();
        score=0;
    }
    ~Game()
    {
        delete currstate;
        delete snake;
        delete g;
    }
    Snake* getsnake(){
        return snake;
    }
    pair<int,int>getdim()
    {
        return g->getdim();
    }
    
    int getcurrscore()
    {
        return score;
    }
    void cleargame()
    {
        delete snake;
    }
    void move(DIR d)
    {
        currstate->makemove(this,d);
    }
    void setstate(Gamestate* s)
    {
        delete currstate;
        currstate=s;
    }
    void updatescore(int x)
    {
        score+=x;
    }
};
void Notready::makemove(Game* g, DIR d)
{
    cout<<"Staarting game"<<endl;
    g->setstate(new Playing());
    g->move(d);
}
void Playing::makemove(Game* g, DIR d)
{
    Snake* s=g->getsnake();
    int m=g->getdim().first;
    int n=g->getdim().second;
    auto foodmap=g->getfoodmap();
    if(!s->canmoved(d))
    {
        cout<<"invalid dir, cannot move"<<endl;
        return;
    }
    auto nh=s->getnewhead(d);
    if(nh.first<0 or nh.second<0 or nh.second>=n or nh.first>=m)
    {
        cout<<"Self collision, game ended with score"<<g->getcurrscore()<<endl;
        g->setstate(new End());
        return;
    }
    int val=s->move(nh,foodmap);
    s->setdir(d);
    if(val==INT_MIN)
    {
        cout<<"Self collision, game ended with score"<<g->getcurrscore()<<endl;
        g->setstate(new End());
        return;
    }else{
        g->updatescore(val);
        cout<<"Curr score "<<g->getcurrscore()<<endl;
        return;
    }

}
void End::makemove(Game* g, DIR d)
{
    cout<<"game ENDED"<<endl;
    g->cleargame();
    g->setstate(new Notready());
}


int main()
{
    Game game(5, 5);

    game.move(RIGHT);   // starts game
    game.move(RIGHT);
    game.move(UP);
    game.move(LEFT);
    game.move(DOWN);
    game.move(LEFT);
    game.move(UP);
    game.move(UP);
    game.move(RIGHT);
    game.move(DOWN);

    return 0;
}