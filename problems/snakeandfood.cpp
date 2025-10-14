#include <bits/stdc++.h>
#include <mutex>
using namespace std;

// ---------------- Food ----------------
class Food {
    int r;
    int c;
public:
    Food(int pr, int pc) : r(pr), c(pc) {}
    int getfoodr() { return r; }
    int getfoodc() { return c; }
};

// ---------------- Board ----------------
class Board {
    int nrows;
    int ncols;
    vector<vector<int>> grid;
public:
    Board(int n, int m) {
        nrows = n;
        ncols = m;
        grid.resize(nrows, vector<int>(ncols, 0));
    }
    int getrows() { return nrows; }
    int getcols() { return ncols; }
};

// ---------------- Snake ----------------
class Snake {
    deque<pair<int, int>> dq;
    map<pair<int, int>, int> mp;
    int dir;
    Board* b;
public:
    Snake(int m, int n, Board* b1) {
        dir = 3; // right
        b = b1;
        int startx = m / 2;
        int starty = n / 2;
        mp[{startx, starty}] = 1;
        dq.push_back({startx, starty});
    }

    int getcurrscore() {
        return (int)dq.size() - 1;
    }

    deque<pair<int, int>> getsnakepositions() { return dq; }

    bool canmoved(int d) {
        if ((d == 0 && dir == 1) || (d == 1 && dir == 0) ||
            (d == 2 && dir == 3) || (d == 3 && dir == 2))
            return false;
        return true;
    }

    pair<int, int> getnewpos(pair<int, int> a, int d) {
        if (d == 0) return {a.first - 1, a.second}; // up
        if (d == 1) return {a.first + 1, a.second}; // down
        if (d == 2) return {a.first, a.second - 1}; // left
        return {a.first, a.second + 1};             // right
    }

    int move(int d, vector<Food>& foods, int& fpos) {
        if (!canmoved(d)) return -1;

        dir = d; // update direction

        auto shead = dq.front();
        auto newhead = getnewpos(shead, d);

        int nr = b->getrows();
        int nc = b->getcols();

        // wall collision
        if (newhead.first < 0 || newhead.first >= nr ||
            newhead.second < 0 || newhead.second >= nc)
            return 0;
        cout<<"not wall collision "<<endl;
        auto stail = dq.back();

        // self-collision
        if (mp.find(newhead) != mp.end() &&
            !(newhead.first == stail.first && newhead.second == stail.second)) {
            return 0;
        }
        cout<<"not self collision "<<endl;

        bool ate = false;

        if (fpos < (int)foods.size()) {
            int foodx = foods[fpos].getfoodr();
            int foody = foods[fpos].getfoodc();

            if (foodx == newhead.first && foody == newhead.second) {
                ate = true;
            }
        }

        dq.push_front(newhead);
        mp[newhead] = 1;

        if (!ate) {
            mp.erase(stail);
            dq.pop_back();
        } else {
            fpos++;
        }

        return 1;
    }
};

// ---------------- Game State ----------------
class IGamestate {
public:
    virtual string getstate() = 0;
    virtual ~IGamestate() = default;
};

class Notstarted : public IGamestate {
public:
    string getstate() override { return "not started"; }
};
class Started : public IGamestate {
public:
    string getstate() override { return "started"; }
};
class Finished : public IGamestate {
public:
    string getstate() override { return "finished"; }
};

// ---------------- Game ----------------
class Game {
    static Game* game;
    static mutex mtx;
    vector<Food> foods;
    int foodpos = 0;
    Board* b;
    Snake* s;
    IGamestate* state;

    Game(int m, int n) {
        b = new Board(m, n);
        s = new Snake(m, n, b);
        foods.push_back({5, 5});
        foods.push_back({3, 4});
        foods.push_back({5, 5});
        state = new Notstarted();
    }

public:
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    static Game* getgame(int m, int n) {
        lock_guard<mutex> lock(mtx);
        if (game == nullptr) {
            game = new Game(m, n);
        }
        return game;
    }

    // ✅ Print the board with snake and food
    void printboard() {
        int rows = b->getrows();
        int cols = b->getcols();
        vector<vector<char>> grid(rows, vector<char>(cols, '0'));

        // mark snake
        auto snakepos = s->getsnakepositions();
        for (auto [r, c] : snakepos) {
            grid[r][c] = '$';
        }

        // mark current food
        if (foodpos < (int)foods.size()) {
            int fx = foods[foodpos].getfoodr();
            int fy = foods[foodpos].getfoodc();
            grid[fx][fy] = '@';
        }

        cout << "\n";
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << grid[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "--------------------\n";
    }

    int move(int dir) {
        int res = s->move(dir, foods, foodpos);
        if (res == 0) {
            cout << "Game ended with score " << s->getcurrscore() << endl;
            printboard();
            return 0;
        } else if (res == -1) {
            cout << "Can't make this move (reverse direction)" << endl;
            return 1;
        } else {
            cout << "Move made, current score " << s->getcurrscore() << endl;
            printboard();
            return 2;
        }
    }
};

// static members
Game* Game::game = nullptr;
mutex Game::mtx;

// ---------------- Main ----------------
int main() {
    Game* g = Game::getgame(6, 6);

    g->printboard();
    g->move(3); // right
    g->move(3); // right
    g->move(1); // down
    g->move(1); // down
    g->move(2); // left
}

// ALL FOODS AT ONCE

#include <bits/stdc++.h>
#include <mutex>
using namespace std;

// ---------------- Food ----------------
class Food {
    int r;
    int c;
public:
    Food(int pr, int pc) : r(pr), c(pc) {}
    int getfoodr() const { return r; }
    int getfoodc() const { return c; }
};

// ---------------- Board ----------------
class Board {
    int nrows;
    int ncols;
public:
    Board(int n, int m) : nrows(n), ncols(m) {}
    int getrows() const { return nrows; }
    int getcols() const { return ncols; }
};

// ---------------- Snake ----------------
class Snake {
    deque<pair<int, int>> dq;
    map<pair<int, int>, int> mp; // snake cells
    int dir;
    Board* b;
public:
    Snake(int m, int n, Board* b1) {
        dir = 3; // start moving right
        b = b1;
        int startx = m / 2;
        int starty = n / 2;
        dq.push_back({startx, starty});
        mp[{startx, starty}] = 1;
    }

    int getcurrscore() const {
        return (int)dq.size() - 1;
    }

    const deque<pair<int,int>>& getsnakepositions() const { return dq; }

    bool canmoved(int d) const {
        if ((d == 0 && dir == 1) || (d == 1 && dir == 0) ||
            (d == 2 && dir == 3) || (d == 3 && dir == 2))
            return false;
        return true;
    }

    pair<int,int> getnewpos(pair<int,int> a, int d) const {
        if (d == 0) return {a.first - 1, a.second}; // up
        if (d == 1) return {a.first + 1, a.second}; // down
        if (d == 2) return {a.first, a.second - 1}; // left
        return {a.first, a.second + 1};             // right
    }

    // returns -1 if invalid, 0 if dead, else current score
    int move(int d, vector<Food>& foods, set<pair<int,int>>& foodset) {
        if (!canmoved(d)) return -1;
        dir = d;

        auto shead = dq.front();
        auto newhead = getnewpos(shead, d);

        int nr = b->getrows();
        int nc = b->getcols();

        // wall collision
        if (newhead.first < 0 || newhead.first >= nr ||
            newhead.second < 0 || newhead.second >= nc)
            return 0;

        auto stail = dq.back();

        // self-collision
        if (mp.find(newhead) != mp.end() &&
            !(newhead == stail)) {
            return 0;
        }

        bool ate = false;

        // ✅ check if food exists at newhead (O(logN))
        if (foodset.find(newhead) != foodset.end()) {
            ate = true;
            foodset.erase(newhead);

            // remove from vector<Food> too
            foods.erase(remove_if(foods.begin(), foods.end(),
                                  [&](const Food& f) {
                                      return f.getfoodr() == newhead.first &&
                                             f.getfoodc() == newhead.second;
                                  }),
                        foods.end());
        }

        dq.push_front(newhead);
        mp[newhead] = 1;

        if (!ate) {
            mp.erase(stail);
            dq.pop_back();
        }

        return (int)dq.size() - 1;
    }
};

// ---------------- Game State ----------------
class IGamestate {
public:
    virtual string getstate() = 0;
    virtual ~IGamestate() = default;
};
class Notstarted : public IGamestate {
public:
    string getstate() override { return "not started"; }
};
class Started : public IGamestate {
public:
    string getstate() override { return "started"; }
};
class Finished : public IGamestate {
public:
    string getstate() override { return "finished"; }
};

// ---------------- Game ----------------
class Game {
    static Game* game;
    static mutex mtx;
    vector<Food> foods;
    set<pair<int,int>> foodset; // ✅ for fast lookup
    Board* b;
    Snake* s;
    IGamestate* state;

    Game(int m, int n) {
        b = new Board(m, n);
        s = new Snake(m, n, b);
        foods.push_back({5, 5});
        foods.push_back({3, 4});
        foods.push_back({2, 2});

        // fill food set
        for (auto &f : foods) foodset.insert({f.getfoodr(), f.getfoodc()});

        state = new Notstarted();
    }

public:
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    static Game* getgame(int m, int n) {
        lock_guard<mutex> lock(mtx);
        if (!game) game = new Game(m, n);
        return game;
    }

    void printboard() const {
        int rows = b->getrows();
        int cols = b->getcols();
        vector<vector<char>> grid(rows, vector<char>(cols, '.'));

        auto snakepos = s->getsnakepositions();
        bool first = true;
        for (auto [r, c] : snakepos) {
            grid[r][c] = first ? 'H' : '$';
            first = false;
        }

        for (auto &f : foods) {
            int r = f.getfoodr();
            int c = f.getfoodc();
            if (r >= 0 && r < rows && c >= 0 && c < cols)
                grid[r][c] = '@';
        }

        cout << "\n";
        for (auto &row : grid) {
            for (auto ch : row) cout << ch << ' ';
            cout << '\n';
        }
        cout << "--------------------\n";
    }

    int move(int dir) {
        if (foods.empty()) {
            cout << "Game already ended!\n";
            return 0;
        }

        int res = s->move(dir, foods, foodset);
        if (res == 0) {
            cout << "Game ended with score " << s->getcurrscore() << endl;
            printboard();
            return 0;
        } else if (res == -1) {
            cout << "Can't move in opposite direction!\n";
            return 1;
        } else {
            cout << "Move made, score " << s->getcurrscore() << endl;
            printboard();
            return 2;
        }
    }
};

// static members
Game* Game::game = nullptr;
mutex Game::mtx;

// ---------------- Main ----------------
int main() {
    Game* g = Game::getgame(6, 6);

    g->printboard();
    g->move(3); // right
    g->move(3); // right
    g->move(1); // down
    g->move(1); // down
    g->move(2); // left
}
