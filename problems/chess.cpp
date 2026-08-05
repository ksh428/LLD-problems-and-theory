#include <bits/stdc++.h>
using namespace std;

// components:
// player ->b/w
//grid (n*n) , cell -> (r,c,occupied/not if yes, pid, else -1)
// piece -> name of the piece, validmoves
// chessgame ->managar for all
// possible dp -> state

class Player{
    int color; // 0-> black, 1->white , -1 empty
    public:
    Player();
    Player(int c=-1):color(c) {}
    int getcol() {return color;}
};
enum PieceName { PAWN,BISHOP,QUEEN,KING,ROOK,KNIGHT};

class Grid;

class Movestrategy{
    public:
    virtual vector<pair<int,int>> getmoves(Grid* g,Player p,int r,int c)=0;
    virtual ~Movestrategy()=default;
};
class Kingstrategy:public Movestrategy{
    public:
    vector<pair<int,int>> getmoves(Grid* g,Player p,int r,int c)
    {

    }
};
class Queenstrategy:public Movestrategy{
    public:
    vector<pair<int,int>> getmoves(Grid* g,Player p,int r,int c)
    {

    }
};
class Knightstrategy:public Movestrategy{
    public:
    vector<pair<int,int>> getmoves(Grid* g,Player p,int r,int c)
    {

    }
};
class Piece{
    protected:
    Movestrategy* ms;
    PieceName name;
    Player p;
    public:
    Piece(PieceName pp,Player pl,Movestrategy* mms):name(pp),p(pl),ms(mms){}
    vector<pair<int,int>> makemove(Grid* g,int r,int c)
    {
        return ms->getmoves(g,p,r,c);
    }
    int getcolor() {return p.getcol();}
    virtual ~Piece()=default;
};
class King:public Piece{
    public:
    King(Player p):Piece(KING,p, new Kingstrategy()) {}
};

class Queen:public Piece{
    public:
    Queen(Player p):Piece(QUEEN,p, new Queenstrategy()) {}
};
class Knight:public Piece{
    public:
    Knight(Player p):Piece(KNIGHT,p, new Knightstrategy()) {}
};

class PieceFactory{
    public:
    static Piece* getpiece(PieceName s,Player p)
    {
        switch(s){
            case KING:
            return new King(p);
            case QUEEN:
            return new Queen(p);
            case KNIGHT:
            return new Knight(p);
        }
        return nullptr;
    }
};

class Cell{
    int r;
    int c;
    Piece* p;
    public:
    Cell(int rr,int cc){
        r=rr;
        c=cc;
    }
    void setpiece(PieceName s,int col)
    {
        p=PieceFactory::getpiece(s,Player(col));
    }
    void movepiece(Piece* pp)
    {
        p=pp;
    }
    int getplayer()
    {
        return p->getcolor();
    }
    Piece* getpiece() {return p;}
};

class Grid{
    int n;
    vector<vector<Cell*>>cells;
    public:
    Grid(int x){
        n=x;
        cells.resize(n,vector<Cell*>(n));
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                cells[i][j]=new Cell(i,j);
            }
        }
        // set the pieces for initial state
    }
    Cell* getcell(int r,int c)
    {
        return cells[r][c];
    }
    void movepiece(int sr,int sc,int tr,int tc)
    {
        cells[tr][tc]->movepiece(cells[sr][sc]->getpiece());
        cells[sr][sc]->movepiece(nullptr);
    }
};

class Chessgame{
    Grid * g;
    Player* currplayer;
    public:
    Chessgame(int n)
    {
        g=new Grid(n);
        currplayer=new Player(0);
    }
    bool makemove(int r,int c)
    {
        Cell* curr=g->getcell(r,c);
        if(curr->getplayer()==-1) return 0;
        //get the piece at this cell
        //make the moves
        cout<<"Enter position to move"<<endl;
        int tr,tc;
        cin>>tr>>tc;
        g->movepiece(r,c,tr,tc);
        return 1;
    }
    void printboard()
    {

    }
    void checkstatus()
    {

    }
};

int main()
{
    return 0;
}