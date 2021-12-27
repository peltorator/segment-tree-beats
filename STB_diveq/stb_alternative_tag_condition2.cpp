#include <bits/stdc++.h>
 
using namespace std;
 
long long division(long long a, long long b) {
    if (a >= 0) {
        return a / b;
    } else {
        return -((-a + b - 1) / b);
    }
}
 
struct DivideSegmentTree {
    static const int T = (1 << 20);
    static const long long INF = 2e18 + 7;
 
    struct Node {
        long long min;
        long long max;
        long long sum;
        long long pushSum;
        long long pushEq;
    } tree[T];
 
    int n;
 
    void doPushEq(int v, int l, int r, long long val) {
        tree[v].max = tree[v].min = tree[v].pushEq = val;
        tree[v].sum = (r - l) * val;
        tree[v].pushSum = 0;
    }
 
    void doPushSum(int v, int l, int r, long long val) {
        if (tree[v].pushEq != INF) {
            doPushEq(v, l, r, val + tree[v].min);
        } else {
            tree[v].max += val;
            tree[v].min += val;
            tree[v].sum += (r - l) * val;
            tree[v].pushSum += val;
        }
    }
 
    void pushToChildren(int v, int l, int r) {
        if (l + 1 == r) {
            return;
        }
        int mid = (r + l) / 2;
        if (tree[v].pushEq != INF) {
            doPushEq(2 * v, l, mid, tree[v].pushEq);
            doPushEq(2 * v + 1, mid, r, tree[v].pushEq);
            tree[v].pushEq = INF;
        } else {
            doPushSum(2 * v, l, mid, tree[v].pushSum);
            doPushSum(2 * v + 1, mid, r, tree[v].pushSum);
            tree[v].pushSum = 0;
        }
    }
 
    void updateFromChildren(int v) {
        tree[v].sum = tree[2 * v].sum + tree[2 * v + 1].sum;
        tree[v].max = max(tree[2 * v].max, tree[2 * v + 1].max);
        tree[v].min = min(tree[2 * v].min, tree[2 * v + 1].min);
    }
 
    void build(int v, int l, int r, const vector<int>& inputArray) {
        tree[v].pushSum = 0;
        tree[v].pushEq = INF;
        if (l + 1 == r) {
            tree[v].max = inputArray[l];
            tree[v].min = inputArray[l];
            tree[v].sum = inputArray[l];
        } else {
            int mid = (r + l) / 2;
            build(2 * v, l, mid, inputArray);
            build(2 * v + 1, mid, r, inputArray);
            updateFromChildren(v);
        }
    }
 
    void build(const vector<int>& inputArray) {
        n = inputArray.size();
        build(1, 0, n, inputArray);
    }
 
    void updatePlusEq(int v, int l, int r, int ql, int qr, int val) {
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
 
    void updatePlusEq(int ql, int qr, int val) {
        updatePlusEq(1, 0, n, ql, qr, val);
    }
 
    void updateDivideEq(int v, int l, int r, int ql, int qr, int val) {
        if (qr <= l || r <= ql) {
            return;
        }
        pushToChildren(v, l, r);
        if (ql <= l && r <= qr && tree[v].max - tree[v].min <= 1) {
            long long maxDiv = division(tree[v].max, val);
            long long minDiv = division(tree[v].min, val);
            if (maxDiv == minDiv) {
                doPushEq(v, l, r, maxDiv);
                return;
            } else {
                doPushSum(v, l, r, maxDiv - tree[v].max);
                return;
            }
        }
        int mid = (r + l) / 2;
        updateDivideEq(2 * v, l, mid, ql, qr, val);
        updateDivideEq(2 * v + 1, mid, r, ql, qr, val);
        updateFromChildren(v);
    }
 
    void updateDivideEq(int ql, int qr, int val) {
        if (val != 1) {
            updateDivideEq(1, 0, n, ql, qr, val);
        }
    }
 
    long long findSum(int v, int l, int r, int ql, int qr) {
        if (qr <= l || r <= ql) {
            return 0;
        }
        if (ql <= l && r <= qr) {
            return tree[v].sum;
        }
        pushToChildren(v, l, r);
        int mid = (r + l) / 2;
        return findSum(2 * v, l, mid, ql, qr) + findSum(2 * v + 1, mid, r, ql, qr);
    }
 
    long long findSum(int ql, int qr) {
        return findSum(1, 0, n, ql, qr);
    }
 
    long long findMin(int v, int l, int r, int ql, int qr) {
        if (qr <= l || r <= ql) {
            return INF;
        }
        if (ql <= l && r <= qr) {
            return tree[v].min;
        }
        pushToChildren(v, l, r);
        int mid = (r + l) / 2;
        return min(findMin(2 * v, l, mid, ql, qr), findMin(2 * v + 1, mid, r, ql, qr));
    }
 
    long long findMin(int ql, int qr) {
        return findMin(1, 0, n, ql, qr);
    }
} segTree;
 
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n, q;
    cin >> n >> q;
    vector<int> inputArray(n);
    for (int &val : inputArray) {
        cin >> val;
    }
    segTree.build(inputArray);
    for (int i = 0; i < q; i++) {
        int type, ql, qr;
        cin >> type >> ql >> qr;
        qr++;
        if (type == 1) {
            int k;
            cin >> k;
            segTree.updatePlusEq(ql, qr, k);
        } else if (type == 2) {
            int k;
            cin >> k;
            segTree.updateDivideEq(ql, qr, k);
        } else if (type == 3) {
            cout << segTree.findMin(ql, qr) << '\n';
        } else if (type == 4) {
            cout << segTree.findSum(ql, qr) << '\n';
        }
 
    }
    return 0;
}
