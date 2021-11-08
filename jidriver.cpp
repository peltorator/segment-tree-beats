#include <bits/stdc++.h>

using namespace std;

struct JiDriverSegmentTree {
    static const int T = (1 << 20);
    static const int INF = 1e9 + 7;

    struct Node {
        int max; // We can not store pushMax because we can just think of max as pushMax
        int maxCnt;
        int secondMax;
        long long sum;
    } tree[T];

    int n;

    void doPushMinEq(int v, int val) {
        if (tree[v].max > val) {
            tree[v].sum -= 1LL * (tree[v].max - val) * tree[v].maxCnt;
            tree[v].max = val;
        }
    }

    void pushToChildren(int v) {
        doPushMinEq(2 * v, tree[v].max);
        doPushMinEq(2 * v + 1, tree[v].max);
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

    void build(int v, int l, int r, const vector<int>& inputArray) {
        if (l + 1 == r) {
            tree[v].max = tree[v].sum = inputArray[l];
            tree[v].maxCnt = 1;
            tree[v].secondMax = -INF;
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

    void updateMineq(int v, int l, int r, int ql, int qr, int val) {
        if (qr <= l || r <= ql || tree[v].max <= val) {
            return;
        }
        if (ql <= l && r <= qr && tree[v].secondMax < val) {
            doPushMinEq(v, val);
            return;
        }
        pushToChildren(v);
        int mid = (r + l) / 2;
        updateMineq(2 * v, l, mid, ql, qr, val);
        updateMineq(2 * v + 1, mid, r, ql, qr, val);
        updateFromChildren(v);
    }

    void updateMineq(int ql, int qr, int val) {
        updateMineq(1, 0, n, ql, qr, val);
    }

    long long findSum(int v, int l, int r, int ql, int qr) {
        if (qr <= l || r <= ql) {
            return 0;
        }
        if (ql <= l && r <= qr) {
            return tree[v].sum;
        }
        pushToChildren(v);
        int mid = (r + l) / 2;
        return findSum(2 * v, l, mid, ql, qr) + findSum(2 * v + 1, mid, r, ql, qr);
    }

    long long findSum(int ql, int qr) {
        return findSum(1, 0, n, ql, qr);
    }
} segTree;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n;
    cin >> n;
    vector<int> inputArray(n);
    for (int &val : inputArray) {
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
            int k;
            cin >> k;
            segTree.updateMineq(ql, qr, k);
        } else {
            cout << segTree.findSum(ql, qr) << '\n';
        }
    }
    return 0;
}
