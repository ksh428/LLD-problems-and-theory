
#include <bits/stdc++.h>

using namespace std;

enum Symbol {
    X,
    O
};

class Player {
    int id;
    Symbol s;
public:
    Player(int i, Symbol s1) : id(i), s(s1) {}
    int getid() const { return id; }
    Symbol getsymbol() const { return s; }
};

class Cell {
    int r;
    int c;
    int id; // stores player id, -1 if empty
public:
    Cell(int pr = 0, int pc = 0, int i = -1) : r(pr), c(pc), id(i) {}
    int getid() const { return id; }
    void setid(int i) { id = i; }
    pair<int, int> getrc() const { return {r, c}; }
};

class Board {
    int n;
    vector<vector<Cell>> cells;
    vector<vector<int>>rows,cols;
    vector<int>d1,d2;
public:
    Board(int sz = 3) {
        n = sz;
        cells.resize(n, vector<Cell>(n));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cells[i][j] = Cell(i, j, -1);
            }
        }
        rows.resize(n,vector<int>(2,0));
        cols.resize(n,vector<int>(2,0));
        d1.resize(2,0);
        d2.resize(2,0);
    }

    vector<vector<Cell>> getcells() const { return cells; }

    bool isend() const {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (cells[i][j].getid() == -1)
                    return false;
            }
        }
        return true;
    }

    bool checkandinsert(int r, int c, int p) {
        if (r < 0 || c < 0 || r >= n || c >= n) return false;
        if (cells[r][c].getid() == -1) {
            rows[r][p]++;
            cols[c][p]++;
            if(r==c) d1[p]++;
            if(r+c==n-1) d2[p]++;
            cells[r][c].setid(p);
            return true;
        }
        return false;
    }

    bool checkWinner(int p,int r,int c) const {
        if(rows[r][p]==n or cols[c][p]==n or d1[p]==n or d2[p]==n) return 1;
        return 0;
    }

    void print() const {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                int occ = cells[i][j].getid();
                if (occ == -1) cout << "- ";
                else cout << (occ == 0 ? "X " : "O ");
            }
            cout << "\n";
        }
        cout << endl;
    }
};

// Forward declaration for State classes
class Game;

class IState {
public:
    virtual string getstate() = 0;
    virtual void makemove(Game* g, int r, int c) = 0;
    virtual ~IState() = default;
};

class Notstarted : public IState {
public:
    string getstate() override { return "Not Started"; }

    void makemove(Game* g, int r, int c) override;
};

class Playing : public IState {
public:
    string getstate() override { return "Playing"; }
    void makemove(Game* g, int r, int c) override;
};

class End : public IState {
public:
    string getstate() override { return "End"; }
    void makemove(Game* g, int r, int c) override {
        cout << "Game has ended, cannot make a move\n";
    }
};

class Game {
    IState* currstate;
    Board* b;
    int turn;

public:
    Game(int n) {
        b = new Board(n);
        currstate = new Notstarted();
        turn = 0;
    }

    int getcurrplayerid() const { return turn; }
    Board* getboard() const { return b; }
    string getgamestate() const { return currstate->getstate(); }

    void makemove(int r, int c) { currstate->makemove(this, r, c); }

    void startgame() { currstate = new Playing(); }
    void endgame() { 
        delete currstate;
        currstate = new End(); }
    void switchturn() { turn = !turn; }

    void setstate(IState* st) { 
        delete currstate;
        currstate = st; }
};

// Implement Notstarted::makemove now that Game exists
void Notstarted::makemove(Game* g, int r, int c) {
    g->startgame();
    cout << "Game started\n";
    g->makemove(r, c); // Delegate to Playing state
}

void Playing::makemove(Game* g, int r, int c) {
    Board* b = g->getboard();
    int p = g->getcurrplayerid();

    if (!b->checkandinsert(r, c, p)) {
        cout << "Invalid move, try again\n";
        return;
    }

    b->print();

    if (b->checkWinner(p,r,c)) {
        cout << "Player " << (p == 0 ? "X" : "O") << " has won the game!\n";
        g->endgame();
        return;
    }

    if (b->isend()) {
        cout << "Game ended in a draw.\n";
        g->endgame();
        return;
    }

    g->switchturn();
}

// Driver
int main() {
    Game* tictac = new Game(3);

    tictac->makemove(0, 0);
    tictac->makemove(0,1);
    tictac->makemove(0, 2);
    tictac->makemove(1, 0);
    tictac->makemove(1, 1); 
    tictac->makemove(1, 2);
    tictac->makemove(2, 0);
    tictac->makemove(2, 1);
    tictac->makemove(2, 2);
    return 0;
}