#include <bits/stdc++.h>
 
using namespace std;
 
typedef long long ll;
 
struct JiDriverSegmentTree {
    static const int T = (1 << 22);
    static const ll INF = 1e15 + 7;
 
    struct Node {
        ll max;
        int maxCnt;
        ll secondMax;
        long long sum;
        ll pushSum;
 
        Node():
            max(INF),
            maxCnt(1),
            secondMax(-INF),
            sum(INF),
            pushSum(0) {}
    } tree[T];
 
    void updateWithVal(int v, ll val) {
        if (tree[v].max > val) {
            tree[v].sum -= 1LL * (tree[v].max - val) * tree[v].maxCnt;
            tree[v].max = val;
        }
    }
 
    void doPushSum(int v, int l, int r, ll val) {
        tree[v].max += val;
        tree[v].secondMax += val;
        tree[v].sum += 1LL * (r - l) * val;
        tree[v].pushSum += val;
    }
 
    void pushToChildren(int v, int l, int r) {
        int mid = (r + l) / 2;
        doPushSum(2 * v, l, mid, tree[v].pushSum);
        doPushSum(2 * v + 1, mid, r, tree[v].pushSum);
        tree[v].pushSum = 0;
        updateWithVal(2 * v, tree[v].max);
        updateWithVal(2 * v + 1, tree[v].max);
    }
 
    void updateFromChildren(int v) {
        tree[v].sum = tree[2 * v].sum + tree[2 * v + 1].sum;
        tree[v].max = max(tree[2 * v].max, tree[2 * v + 1].max);
        tree[v].secondMax = max(tree[2 * v].secondMax, tree[2 * v + 1].secondMax);
        tree[v].maxCnt = 0;
        if (tree[2 * v].max == tree[v].max) {
            tree[v].maxCnt += tree[2 * v].maxCnt;
        } else {
            tree[v].secondMax = max(tree[v].secondMax, tree[2 * v].max);
        }
        if (tree[2 * v + 1].max == tree[v].max) {
            tree[v].maxCnt += tree[2 * v + 1].maxCnt;
        } else {
            tree[v].secondMax = max(tree[v].secondMax, tree[2 * v + 1].max);
        }
    }
 
    void build(int v, int l, int r, const vector<ll>& inputArray) {
        tree[v].pushSum = 0;
        if (l + 1 == r) {
            tree[v].max = inputArray[l];
            tree[v].secondMax = -INF;
            tree[v].maxCnt = 1;
            tree[v].sum = inputArray[l];
        } else {
            int mid = (r + l) / 2;
            build(2 * v, l, mid, inputArray);
            build(2 * v + 1, mid, r, inputArray);
            updateFromChildren(v);
        }
    }
 
    void build(const vector<ll>& inputArray) {
        build(1, 0, inputArray.size(), inputArray);
    }
 
    void updateMinEq(int v, int l, int r, int ql, int qr, ll val) {
        if (qr <= l || r <= ql || tree[v].max <= val) {
            return;
        }
        if (ql <= l && r <= qr && tree[v].secondMax < val) {
            updateWithVal(v, val);
            return;
        }
        pushToChildren(v, l, r);
        int mid = (r + l) / 2;
        updateMinEq(2 * v, l, mid, ql, qr, val);
        updateMinEq(2 * v + 1, mid, r, ql, qr, val);
        updateFromChildren(v);
    }
 
    void updatePlusEq(int v, int l, int r, int ql, int qr, ll val) {
        if (qr <= l || r <= ql) {
            return;
        }
        if (ql <= l && r <= qr) {
            doPushSum(v, l, r, val);
            return;
        }
        pushToChildren(v, l, r);
        int mid = (r + l) / 2;
        updatePlusEq(2 * v, l, mid, ql, qr, val);
        updatePlusEq(2 * v + 1, mid, r, ql, qr, val);
        updateFromChildren(v);
    }
 
    long long find(int v, int l, int r, int ql, int qr) {
        if (qr <= l || r <= ql) {
            return 0;
        }
        if (ql <= l && r <= qr) {
            return tree[v].sum;
        }
        pushToChildren(v, l, r);
        int mid = (r + l) / 2;
        return find(2 * v, l, mid, ql, qr) + find(2 * v + 1, mid, r, ql, qr);
    }
} segTree;
 
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n;
    cin >> n;
    vector<ll> inputArray(n);
    for (ll &val : inputArray) {
        cin >> val;
    }
    segTree.build(inputArray);
    int q;
    cin >> q;
    for (int i = 0; i < q; i++) {
        int type, ql, qr;
        cin >> type >> ql >> qr;
        ql--;
        if (type == 1) {
            ll k;
            cin >> k;
            segTree.updateMinEq(1, 0, n, ql, qr, k);
        } else if (type == 2) {
            ll k;
            cin >> k;
            segTree.updatePlusEq(1, 0, n, ql, qr, k);
        } else {
            cout << segTree.find(1, 0, n, ql, qr) << '\n';
        }
    }
    return 0;
}
