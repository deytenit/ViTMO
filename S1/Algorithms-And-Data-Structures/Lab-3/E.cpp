#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

#define fast()                        \
    ios_base::sync_with_stdio(false); \
    cin.tie(0)

#define ff first
#define ss second
#define pb push_back
#define mp make_pair
#define sqr(a) ((a) * (a))
#define vout(a) cout << #a << ": " << (a) << '\n'

using ll = long long;
using ld = long double;

const int INF = 0x7fffffff;
const ll MOD = 1e9 + 7;
const ld EPS = 1e-8;
const ld PI = 3.14159265358979323;
const int MAXN = 1e5 + 1000;

template <typename T>
struct list {
   private:
    struct node {
        T value;
        node* next;
        node* prev;

        node(T value) : value(value), next(nullptr), prev(nullptr) {}
        node(T value, node* next, node* prev) : value(value), next(next), prev(prev) {}
    };

    node* head;
    node* tail;
    size_t length;

    node* get_node(size_t pos) {
        if (length == 0) {
            return nullptr;
        }

        if (pos < 0 || pos >= length) {
            pos = length - 1;
        }

        if (length - pos - 1 > pos) {
            node* result = tail;

            for (size_t i = 0; i < pos; i++) {
                result = result->next;
            }

            return result;
        } else {
            node* result = head;

            for (size_t i = 0; i < length - pos - 1; i++) {
                result = result->prev;
            }

            return result;
        }
    }

   public:
    list() : head(nullptr), tail(nullptr), length(0) {}

    ~list() {
        clear();
    }

    size_t size() {
        return length;
    }

    void clear() {
        while (length > 0) {
            erase();
        }
    }

    T get() {
        return get_node(length - 1)->value;
    }

    T get(size_t pos) {
        return get_node(pos)->value;
    }

    void add(T val) {
        node* son = new node(val, nullptr, head);

        if (head) {
            head->next = son;
        }

        head = son;

        if (!tail) {
            tail = head;
        }

        length++;
    }

    void insert(T val, size_t pos) {
        if (pos == length || length == 0) {
            return add(val);
        }

        node* son = get_node(pos);

        node* father = new node(val, son, son->prev);

        if (!son->prev) {
            tail = father;
        } else {
            son->prev->next = father;
        }

        son->prev = father;

        length++;
    }

    void erase() {
        erase(length - 1);
    }

    void erase(size_t pos) {
        node* current = get_node(pos);

        if (!current) {
            return;
        }

        if (!current->next && !current->prev) {
            head = nullptr;
            tail = nullptr;
            delete current;
            length--;
            return;
        }

        if (current->next) {
            current->next->prev = current->prev;
        } else {
            head = current->prev;
            head->next = nullptr;
        }

        if (current->prev) {
            current->prev->next = current->next;
        } else {
            tail = current->next;
            tail->prev = nullptr;
        }

        delete current;
        length--;
    }
};

list<pair<int, int>> ls;

int main() {
#ifdef _LOCAL
    auto rtime = high_resolution_clock::now();
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    fast();

    int n;
    cin >> n;

    for (int i = 0; i < n; i++) {
        string cmd;
        cin >> cmd;
        if (cmd == "+") {
            int val;
            cin >> val;
            ls.add({val, ls.size() > 0 ? max(ls.get().second, val) : val});
        } else if (cmd == "-") {
            ls.erase();
        } else {
            cout << ls.get().second << '\n';
        }
    }

#ifdef _LOCAL
    duration<double> diff = high_resolution_clock::now() - rtime;
    cerr << "Runtime: " << diff.count() << " s.\n";
#endif
    return 0;
}