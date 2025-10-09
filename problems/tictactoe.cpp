// running version

#include <bits/stdc++.h>
#include <mutex>
using namespace std;

enum Symbol{
    X,
    O
};

class Player{
    int id;
    Symbol s;
    public:
    Player(int i,Symbol s1)
    {
        id=i;
        s=s1;
    }
    int getid()
    {
        return id;
    }
    Symbol getsymbol()
    {
        return s;
    }
};

class Cell{
    int r;
    int c;
    public:
    Cell(int pr,int pc){
        r=pr;
        c=pc;
    }
    pair<int,int> getrc()
    {
        return {r,c};
    }
};
class Board{
    vector<pair<Cell,Player>>cells;
    public:
    Board()
    {
        cells.clear();
    }
    vector<pair<Cell,Player>> getcells()
    {
        return cells;
    }
    void insert(int r,int c,int p)
    {
        Cell c1(r,c);
        Symbol s;
        if(p==0) s=Symbol::X;
        else s=Symbol::O;
        Player pl(p,s);
        cells.push_back({c1,pl});
    }
};
class IState{
    public:
    virtual string getstate()=0;
};
class Notstarted:public IState{
    string getstate()
    {
        return "not started";
    }
};
class Playing:public IState{
    string getstate()
    {
        return "Playing";
    }
};
class End:public IState{
    string getstate()
    {
        return "end";
    }
};
class Game{
    static Game* gameinstance;
    static mutex mtx;
    IState* currstate;
    Board* b;
    Game(){};
    public:
    string getgamestate()
    {
        return currstate->getstate();
    }
    static Game* creategame()
    {
        mtx.lock();
        if(gameinstance==nullptr)
        {
            gameinstance=new Game();
            gameinstance->b=new Board();
        }
        mtx.unlock();
        gameinstance->currstate=new Notstarted();
        return gameinstance;
    }
    bool canmakemove(int r,int c,int p)
    {
        auto cells=b->getcells();
        int can=1;
        for(auto it:cells)
        {
            auto rc=it.first.getrc();
            if(rc == make_pair(r,c))
            {
                can=0;
                return can;
            }
        }
        return 1;
    }
    bool isend()
    {
        if(b->getcells().size()==9) return 1;
        else return 0;
    }
    bool checkwinner(int p)
    {
        int ar[3][3];
        memset(ar,-1,sizeof(ar));
        auto cells=b->getcells();
        for(auto it:cells)
        {
            int row=it.first.getrc().first;
            int col=it.first.getrc().second;
            // cout<<row<<" "<<col<<" "<<it.second.getid();
            ar[row][col]=it.second.getid();
        }
        //horizontal check
        for(int i=0;i<3;i++)
        {
            int ismatch=1;
            for(int j=0;j<3;j++)
            {
                if(ar[i][j]==p){
                    continue;
                }else{
                    ismatch=0;
                    break;
                }
            }
            if(ismatch)
            {
                return 1;
            }
        }
        //vertical check
        for(int j=0;j<3;j++)
        {
            int ismatch=1;
            for(int i=0;i<3;i++)
            {
                if(ar[i][j]==p)
                {
                    continue;
                }else{
                    ismatch=0;
                    break;
                }
            }
            if(ismatch)
            {
                return 1;
            }
        }
        //diagonal check
        if(ar[0][0]==p and ar[0][0]==ar[1][1]==ar[2][2]) return 1;
        if(ar[0][2]==p and ar[0][2]==ar[1][1] == ar[2][0]) return 1;
        return 0;
    }
    void makemove(int r,int c,int p)
    {
        if(currstate->getstate()=="not started")
        {
            currstate=new Playing();
        }else if(currstate->getstate()=="end")
        {
            cout<<"game has ended , canot make this move"<<endl;
            return;
        }
        
        if(canmakemove(r,c,p))
        {
            b->insert(r,c,p);
            cout<<"made the move "<<endl;
            if(checkwinner(p))
            {
                cout<<"player "<<p<<" has won the game"<<endl;
                currstate=new End();
                cout<<"game ended"<<endl;
                return;
            }
            if(isend())
            {
                currstate=new End();
                cout<<"game ended in draw"<<endl;
            }
        }
    }
};
Game* Game::gameinstance=nullptr;
mutex Game::mtx;

int main() {
    Game* tictac=Game::creategame();
    tictac->makemove(0,0,0);
    tictac->makemove(0,1,1);
    tictac->makemove(1,0,0);
    tictac->makemove(0,2,1);
    tictac->makemove(2,0,0);
    tictac->makemove(2,1,0);
}

// better

#include <bits/stdc++.h>
#include <mutex>
using namespace std;

enum Symbol{
    X,
    O
};

class Player{
    int id;
    Symbol s;
    public:
    Player(int i,Symbol s1):id(i),s(s1){};
    int getid() const
    {
        return id;
    }
    Symbol getsymbol() const
    {
        return s;
    }
};

class Cell{
    int r;
    int c;
    int id;
    public:
    Cell(int pr,int pc,int i){
        r=pr;
        c=pc;
        id=i;
    }
    int getid() {return id;}
    pair<int,int> getrc()
    {
        return {r,c};
    }
};
class Board{
    int n;
    vector<vector<Cell>>cells;
    public:
    Board(int sz=3)
    {
        n=sz;
        for(int i=0;i<n;i++)
        {
            vector<Cell>r;
            for(int j=0;j<n;j++)
            {
                Cell c1(i,j,-1);
                r.push_back(c1);
            }
            cells.push_back(r);
        }
    }
    vector<vector<Cell>> getcells()
    {
        return cells;
    }
    bool isend()
    {
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                if(cells[i][j]==-1) return 0;
            }
        }
        return 1;
    }
    bool checkandinsert(int r,int c,int p)
    {
        if(cells[r][c]==-1)
        {
            cells[r][c]=p;
            return 1;
        }
        return 0;
    }
    bool checkWinner(const int p) const {
        // Rows
        for (int i=0; i<n; ++i) {
            bool win = true;
            for (int j=0; j<n; ++j) {
                int occ = grid[i][j].getid();
                if (occ==-1 || occ!= p) { win = false; break; }
            }
            if (win) return true;
        }
        // Cols
        for (int j=0; j<n; ++j) {
            bool win = true;
            for (int i=0; i<n; ++i) {
                int occ = grid[i][j].getid();
                if (occ==-1 || occ!= p) { win = false; break; }
            }
            if (win) return true;
        }
        // Diagonals
        bool diag1 = true, diag2 = true;
        for (int i=0; i<n; ++i) {
            int occ1 = grid[i][i].getid();
            int occ2 = grid[i][n-1-i].getid();
            if (occ1==-1 || occ1 != p) diag1 = false;
            if (occ2 ==-1 || occ2 != p) diag2 = false;
        }
        return diag1 || diag2;
    }
    void print() const {
        for (int i=0; i<n; ++i) {
            for (int j=0; j<n; ++j) {
                int occ = grid[i][j].getid();
                if (occ==-1) cout << "- ";
                else cout << (occ == 1 ? "X " : "O ");
            }
            cout << "\n";
        }
        cout << endl;
    }
};
class IState{
    public:
    virtual string getstate()=0;
    virtual void makemove(Game* g, int r,int c)=0;
};
class Notstarted:public IState{
    public:
    string getstate()
    {
        return "not started";
    }
    void makemove(Game*g,int r,int c)
    {
        g->startgame();
        cout<<"Game started "<<endl;
        g->makemove();
    }
};
class Playing:public IState{
    public:
    string getstate()
    {
        return "Playing";
    }
    void makemove(Game*g,int r,int c)
    {
        Board *b =g->getboard();
        if(!b->checkandinsert())
        {
            cout<<"invalid move, try again"<<endl;
            return;
        }
        b->print();
        int p=g->getcurrplayerid();
        if(b->checkwinner(p))
        {
            cout<<"player "<<p<<" has won the game"<<endl;
            g->endgame();
            cout<<"game ended"<<endl;
        }
        if(b->isend())
        {
            g->endgame();
            cout<<"game ended in draw"<<endl;
            return;
        }
        g->switchturn();
    }
};
class End:public IState{
    public:
    string getstate()
    {
        return "end";
    }
    void makemove(Game* g,int r,int c)
    {
        cout<"game has ended, cannot make this move"<<endl;
        return;
    }
};
class Game{
    static Game* gameinstance;
    static mutex mtx;
    IState* currstate;
    Board* b;
    int turn;
    Game()
    {
        gameinstance->b=new Board();
        gameinstance->currstate=new Notstarted();
        turn =0;
    }
    public:
    int getcurrplayerid() { return turn; }
    Board* getboard() {return b;}
    string getgamestate() const
    {
        return currstate->getstate();
    }
    static Game* creategame()
    {
        mtx.lock();
        if(gameinstance==nullptr)
        {
            gameinstance=new Game();
        }
        mtx.unlock();
        return gameinstance;
    }
    void makemove(int r,int c)
    {
        currstate->makemove(this,r,c);
    }
    void startgame()
    {
        currstate=new Playing();
    }
    void endgame()
    {
        currstate=new End();
    }
    void switchturns()
    {
        turn=!turn;
    }
};
Game* Game::gameinstance=nullptr;
mutex Game::mtx;

int main() {
    Game* tictac=Game::creategame();
    tictac->makemove(0,0,0);
    tictac->makemove(0,1,1);
    tictac->makemove(1,0,0);
    tictac->makemove(0,2,1);
    tictac->makemove(2,0,0);
    tictac->makemove(2,1,0);
}