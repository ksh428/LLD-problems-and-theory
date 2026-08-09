#include <bits/stdc++.h>
#include <mutex>

using namespace std;

class File {
    string name;
    string content;

public:
    File(string s, string c = "") : name(s), content(c) {}

    string getname() {
        return name;
    }

    string getcontent() {
        return content;
    }

    void updatecontent(string s) {
        content += s;
    }
};

class Directory {
    string pathname;

    map<string, Directory*> dirs;
    map<string, File*> files;

public:
    Directory(string s) : pathname(s) {}

    map<string, Directory*>& getdirs() {
        return dirs;
    }

    map<string, File*>& getfiles() {
        return files;
    }

    void updatefile(string s, string c) {
        auto it = files.find(s);

        if (it == files.end()) {
            files[s] = new File(s, c);
        } 
        else {
            it->second->updatecontent(c);
        }
    }
};

class Trie {
    Directory* root;

public:

    Trie(string s) {
        root = new Directory(s);
    }

    vector<string> trimpath(string path) {
        stringstream ss(path);
        string t;
        vector<string> temp;

        while (getline(ss, t, '/')) {
            if (t == "")
                continue;

            temp.push_back(t);
        }

        return temp;
    }

    void insert(string path) {

        auto temp = trimpath(path);

        Directory* curr = root;

        for (string p : temp) {

            auto& mp = curr->getdirs();

            if (mp.find(p) == mp.end()) {
                mp[p] = new Directory(p);
            }

            curr = mp[p];
        }
    }

    void addtofile(string path, string contents) {

        auto t = trimpath(path);

        if (t.empty()) {
            return;
        }

        Directory* curr = root;

        for (int i = 0; i < (int)t.size() - 1; i++) {

            auto& mp = curr->getdirs();

            if (mp.find(t[i]) == mp.end()) {
                mp[t[i]] = new Directory(t[i]);
            }
            curr = mp[t[i]];
        }

        if (curr == nullptr) {
            cout << "Invalid path\n";
            return;
        }
        curr->updatefile(t.back(), contents);
    }

    vector<string> dols(string path) {

        auto t = trimpath(path);

        // Root
        if (t.empty()) {

            vector<string> ans;

            for (auto& it : root->getfiles()) {
                ans.push_back(it.first);
            }

            for (auto& it : root->getdirs()) {
                ans.push_back(it.first);
            }

            return ans;
        }

        Directory* curr = root;
        for (string p : t) {

            auto& mp = curr->getdirs();

            if (mp.find(p) == mp.end()) {

                auto& files = curr->getfiles();

                if (files.find(p) != files.end()) {
                    return {p};
                }

                return {};
            }

            curr = mp[p];
        }
        vector<string> ans;

        for (auto& it : curr->getfiles()) {
            ans.push_back(it.first);
        }

        for (auto& it : curr->getdirs()) {
            ans.push_back(it.first);
        }

        return ans;
    }
    string getfilecontents(string path) {

        auto t = trimpath(path);

        if (t.empty()) {
            return "";
        }
        Directory* curr = root;
        for (int i = 0; i < (int)t.size() - 1; i++) {

            auto& mp = curr->getdirs();

            if (mp.find(t[i]) == mp.end()) {
                cout << "Invalid path\n";
                return "";
            }

            curr = mp[t[i]];
        }

        auto& files = curr->getfiles();

        auto it = files.find(t.back());

        if (it == files.end() || it->second == nullptr) {
            cout << "File does not exist\n";
            return "";
        }

        return it->second->getcontent();
    }
};

class Filesystem {

    static Filesystem* getinstance;
    static mutex mtx;

    Trie* t;

    Filesystem(const Filesystem&) = delete;

    Filesystem& operator=(const Filesystem&) = delete;

    Filesystem() {
        t = new Trie("/");
    }

public:

    static Filesystem* createfilesystem() {

        lock_guard<mutex> lock(mtx);

        if (!getinstance) {
            getinstance = new Filesystem();
        }

        return getinstance;
    }

    vector<string> ls(string path) {
        return t->dols(path);
    }

    void mkdir(string path) {
        t->insert(path);
    }

    void addcontentstofile(string path, string content) {
        t->addtofile(path, content);
    }

    string readfile(string path) {
        return t->getfilecontents(path);
    }
};

Filesystem* Filesystem::getinstance = nullptr;
mutex Filesystem::mtx;

int main() {

    Filesystem* fs = Filesystem::createfilesystem();

    // --------------------------------------------------------
    // 1. Create directories
    // --------------------------------------------------------

    fs->mkdir("/a");
    fs->mkdir("/a/bcd");
    fs->mkdir("/a/bcd/ksh");

    // --------------------------------------------------------
    // 2. Create file and add content
    // --------------------------------------------------------

    fs->addcontentstofile(
        "/a/bcd/ksh/file.txt",
        "testing file content"
    );

    // --------------------------------------------------------
    // 3. Read file
    // --------------------------------------------------------

    cout << "File content:\n";

    cout << fs->readfile(
        "/a/bcd/ksh/file.txt"
    ) << "\n\n";


    // --------------------------------------------------------
    // 4. Append more content
    // --------------------------------------------------------

    fs->addcontentstofile(
        "/a/bcd/ksh/file.txt",
        " + more content"
    );

    cout << "After append:\n";

    cout << fs->readfile(
        "/a/bcd/ksh/file.txt"
    ) << "\n\n";


    // --------------------------------------------------------
    // 5. Create another file
    // --------------------------------------------------------

    fs->addcontentstofile(
        "/a/bcd/ksh/test.txt",
        "hello world"
    );


    // --------------------------------------------------------
    // 6. Create another directory
    // --------------------------------------------------------

    fs->mkdir("/a/bcd/xyz");


    // --------------------------------------------------------
    // 7. ls directory
    // --------------------------------------------------------

    cout << "ls /a/bcd/ksh:\n";

    vector<string> ans = fs->ls("/a/bcd/ksh");

    for (string s : ans) {
        cout << s << " ";
    }

    cout << "\n\n";


    // --------------------------------------------------------
    // 8. ls parent directory
    // --------------------------------------------------------

    cout << "ls /a/bcd:\n";

    ans = fs->ls("/a/bcd");

    for (string s : ans) {
        cout << s << " ";
    }

    cout << "\n\n";


    // --------------------------------------------------------
    // 9. ls file
    // --------------------------------------------------------

    cout << "ls /a/bcd/ksh/file.txt:\n";

    ans = fs->ls("/a/bcd/ksh/file.txt");

    for (string s : ans) {
        cout << s << " ";
    }

    cout << "\n";

    return 0;
}