#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
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

template <class T, class Compare = less<T>>
class heap {
   private:
    std::vector<shared_ptr<T>> tree;

   protected:
    void push(shared_ptr<T> ptr) {
        tree.push_back(ptr);
        sift_up();
    }

   public:
    heap() = default;
    heap(const std::vector<int>& data) {
        for (const auto& it : data) {
            tree.push_back(make_shared<int>(it));
        }

        for (int i = tree.size() / 2; i >= 0; --i) {
            sift_down(i);
        }
    }

    size_t size() const {
        return tree.size();
    }

    bool empty() const {
        return tree.empty();
    }

    void push(T x) {
        tree.push(make_unique<int>(x));
    }

    T root() const {
        return *tree[0];
    }

    void pop() {
        swap(tree[0], tree[tree.size() - 1]);
        tree.pop_back();
        sift_down();
    }

    vector<T> sort() {
        heap<T> tmp(*this);

        vector<T> res;

        while (!tmp.empty()) {
            res.push_back(tmp.root());
            tmp.pop();
        }

        return res;
    }

   protected:
    void sift_down(size_t begin = 0, size_t end = -1) {
        end = min(tree.size(), max(end, (size_t)0));
        begin = max(min(tree.size() - 1, begin), (size_t)0);

        size_t i = begin;
        while (2 * i + 1 < end) {
            size_t l = 2 * i + 1;
            size_t r = 2 * i + 2;

            size_t j = l;

            if (r < end && Compare{}(*tree[r], *tree[l])) {
                j = r;
            }

            if (Compare{}(*tree[i], *tree[j]) || *tree[i] == *tree[j]) {
                break;
            }

            swap(tree[i], tree[j]);
            i = j;
        }
    }

    void sift_up(size_t begin = 0, size_t end = -1) {
        end = min(tree.size(), max(end, (size_t)0));
        begin = max(min(tree.size() - 1, begin), (size_t)0);

        if (end == 0) {
            return;
        }

        size_t i = end - 1;
        while (i != 0 && Compare{}(*tree[i], *tree[(i - 1) / 2])) {
            swap(tree[i], tree[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }
};

template <class T, class Compare = greater<T>>
class priority_queue : private heap<T, Compare> {
   private:
    vector<shared_ptr<T>> data_;

   public:
    priority_queue() = default;

    void push(T x, size_t idx) {
        data_.resize(idx + 1);
        data_[idx] = make_unique<T>(x);
        heap<T, Compare>::push(data_[idx]);
    }

    void push(T x) {
        data_.push_back(make_unique<T>(x));
        heap<T, Compare>::push(data_.back());
    }

    void pop() {
        heap<T, Compare>::pop();
    }

    T front() const {
        return heap<T, Compare>::root();
    }

    void replace_key(size_t ind, T amo) {
        ind = min(data_.size(), max(ind, (size_t)0));

        *data_[ind] = amo;

        for (size_t i = 0; i < size(); ++i) {
            heap<T, Compare>::sift_down(i);
            heap<T, Compare>::sift_up(0, i);
        }
    }

    using heap<T, Compare>::size;
    using heap<T, Compare>::empty;
};

int main() {
#ifdef _LOCAL
    auto rtime = high_resolution_clock::now();
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    fast();

    priority_queue<int, less<int>> que;

    string cmd;
    size_t cnt = 0;
    while (cin >> cmd) {
        if (cmd == "push") {
            int x;
            cin >> x;
            que.push(x, cnt);
        } else if (cmd == "extract-min") {
            if (que.empty()) {
                cout << '*' << '\n';
            } else {
                cout << que.front() << '\n';
                que.pop();
            }
        } else {
            int x, y;
            cin >> x >> y;
            que.replace_key(x - 1, y);
        }
        ++cnt;
    }

#ifdef _LOCAL
    duration<double> diff = high_resolution_clock::now() - rtime;
    cerr << "Runtime: " << diff.count() << " s.\n";
#endif
    return 0;
}
