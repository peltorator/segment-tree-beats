#include <bits/stdc++.h>

using namespace std;

struct JiDriverSegmentTree {
    static const int T = (1 << 20);
    static const long long INF = 1e15 + 7;

    struct Node {
        long long min;
        int minCnt;
        long long secondMin;

        long long max;
        int maxCnt;
        long long secondMax;

        long long sum;

        long long pushSum;
        long long pushEq;
        // If we have pushEq, no other pushes should be made. They're all combined together in pushEq.
        // Otherwise we first apply pushSum and then min= and max= pushes (in any order btw).
    } tree[T];

    int n;

    void doPushEq(int v, int l, int r, long long val) {
        tree[v].min = tree[v].max = tree[v].pushEq = val;
        tree[v].minCnt = tree[v].maxCnt = r - l;
        tree[v].secondMax = -INF;
        tree[v].secondMin = INF;

        tree[v].sum = val * (r - l);
        tree[v].pushSum = 0;
    }

    void doPushMinEq(int v, int l, int r, long long val) {
        if (tree[v].min >= val) {
            doPushEq(v, l, r, val);
        } else if (tree[v].max > val) {
            if (tree[v].secondMin == tree[v].max) {
                tree[v].secondMin = val;
            }
            tree[v].sum -= (tree[v].max - val) * tree[v].maxCnt;
            tree[v].max = val;
        }
    }

    void doPushMaxEq(int v, int l, int r, long long val) {
        if (tree[v].max <= val) {
            doPushEq(v, l, r, val);
        } else if (tree[v].min < val) {
            if (tree[v].secondMax == tree[v].min) {
                tree[v].secondMax = val;
            }
            tree[v].sum += (val - tree[v].min) * tree[v].minCnt;
            tree[v].min = val;
        }
    }

    void doPushSum(int v, int l, int r, long long val) {
        if (tree[v].min == tree[v].max) {
            doPushEq(v, l, r, tree[v].min + val);
        } else {
            tree[v].max += val;
            if (tree[v].secondMax != -INF) {
                tree[v].secondMax += val;
            }

            tree[v].min += val;
            if (tree[v].secondMin != INF) {
                tree[v].secondMin += val;
            }

            tree[v].sum += (r - l) * val;
            tree[v].pushSum += val;
        }
    }

    void pushToChildren(int v, int l, int r) {
        if (l + 1 == r) {
            return;
        }
        int mid = (r + l) / 2;
        if (tree[v].pushEq != -1) {
            doPushEq(2 * v, l, mid, tree[v].pushEq);
            doPushEq(2 * v + 1, mid, r, tree[v].pushEq);
            tree[v].pushEq = -1;
        } else {
            doPushSum(2 * v, l, mid, tree[v].pushSum);
            doPushSum(2 * v + 1, mid, r, tree[v].pushSum);
            tree[v].pushSum = 0;

            doPushMinEq(2 * v, l, mid, tree[v].max);
            doPushMinEq(2 * v + 1, mid, r, tree[v].max);

            doPushMaxEq(2 * v, l, mid, tree[v].min);
            doPushMaxEq(2 * v + 1, mid, r, tree[v].min);
        }
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

        tree[v].min = min(tree[2 * v].min, tree[2 * v + 1].min);
        tree[v].secondMin = min(tree[2 * v].secondMin, tree[2 * v + 1].secondMin);
        tree[v].minCnt = 0;
        if (tree[2 * v].min == tree[v].min) {
            tree[v].minCnt += tree[2 * v].minCnt;
        } else {
            tree[v].secondMin = min(tree[v].secondMin, tree[2 * v].min);
        }
        if (tree[2 * v + 1].min == tree[v].min) {
            tree[v].minCnt += tree[2 * v + 1].minCnt;
        } else {
            tree[v].secondMin = min(tree[v].secondMin, tree[2 * v + 1].min);
        }
    }

    void build(int v, int l, int r, const vector<int>& inputArray) {
        tree[v].pushSum = 0;
        tree[v].pushEq = -1;
        if (l + 1 == r) {
            tree[v].max = inputArray[l];
            tree[v].secondMax = -INF;
            tree[v].maxCnt = 1;

            tree[v].min = inputArray[l];
            tree[v].secondMin = INF;
            tree[v].minCnt = 1;

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

    void updateMinEq(int v, int l, int r, int ql, int qr, int val) {
        if (qr <= l || r <= ql || tree[v].max <= val) {
            return;
        }
        if (ql <= l && r <= qr && tree[v].secondMax < val) {
            doPushMinEq(v, l, r, val);
            return;
        }
        pushToChildren(v, l, r);
        int mid = (r + l) / 2;
        updateMinEq(2 * v, l, mid, ql, qr, val);
        updateMinEq(2 * v + 1, mid, r, ql, qr, val);
        updateFromChildren(v);
    }

    void updateMinEq(int ql, int qr, int val) {
        updateMinEq(1, 0, n, ql, qr, val);
    }

    void updateMaxEq(int v, int l, int r, int ql, int qr, int val) {
        if (qr <= l || r <= ql || tree[v].min >= val) {
            return;
        }
        if (ql <= l && r <= qr && tree[v].secondMin > val) {
            doPushMaxEq(v, l, r, val);
            return;
        }
        pushToChildren(v, l, r);
        int mid = (r + l) / 2;
        updateMaxEq(2 * v, l, mid, ql, qr, val);
        updateMaxEq(2 * v + 1, mid, r, ql, qr, val);
        updateFromChildren(v);
    }

    void updateMaxEq(int ql, int qr, int val) {
        updateMaxEq(1, 0, n, ql, qr, val);
    }

    void updateEq(int v, int l, int r, int ql, int qr, int val) {
        if (qr <= l || r <= ql) {
            return;
        }
        if (ql <= l && r <= qr) {
            doPushEq(v, l, r, val);
            return;
        }
        pushToChildren(v, l, r);
        int mid = (r + l) / 2;
        updateEq(2 * v, l, mid, ql, qr, val);
        updateEq(2 * v + 1, mid, r, ql, qr, val);
        updateFromChildren(v);
    }

    void updateEq(int ql, int qr, int val) {
        updateEq(1, 0, n, ql, qr, val);
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

    long long findMax(int v, int l, int r, int ql, int qr) {
        if (qr <= l || r <= ql) {
            return -INF;
        }
        if (ql <= l && r <= qr) {
            return tree[v].max;
        }
        pushToChildren(v, l, r);
        int mid = (r + l) / 2;
        return max(findMax(2 * v, l, mid, ql, qr), findMax(2 * v + 1, mid, r, ql, qr));
    }

    long long findMax(int ql, int qr) {
        return findMax(1, 0, n, ql, qr);
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
            segTree.updateMinEq(ql, qr, k);
        } else if (type == 2) {
            int k;
            cin >> k;
            segTree.updatePlusEq(ql, qr, k);
        } else {
            cout << segTree.findSum(ql, qr) << '\n';
        }
    }
    return 0;
}
