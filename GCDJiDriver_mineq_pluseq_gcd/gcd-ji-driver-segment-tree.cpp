#include <bits/stdc++.h>
 
using namespace std;
 
typedef long long ll;
 
ll gcd(ll a, ll b) {
    a = abs(a);
    b = abs(b);
    while (a) {
        b %= a;
        swap(a, b);
    }
    return b;
}
 
struct JiDriverSegmentTree {
    static const int T = (1 << 20);
    static const ll INF = 1e15 + 7;
 
    struct Node {
        ll min;
        int minCnt;
        ll secondMin;
        ll max;
        int maxCnt;
        ll secondMax;
        long long sum;
        ll diffGcd;
        ll pushSum;
        ll pushEq;
 
        Node():
            min(-INF),
            minCnt(1),
            secondMin(INF),
            max(INF),
            maxCnt(1),
            secondMax(-INF),
            sum(INF),
            diffGcd(0),
            pushSum(0),
            pushEq(-1) {}
    } tree[T];
 
    void doPushEq(int v, int l, int r, ll val) {
        tree[v].min = tree[v].max = tree[v].pushEq = val;
        tree[v].minCnt = tree[v].maxCnt = r - l;
        tree[v].secondMax = -INF;
        tree[v].secondMin = INF;
        tree[v].sum = val * (r - l);
        tree[v].diffGcd = 0;
        tree[v].pushSum = 0;
    }
 
    void doPushMinEq(int v, int l, int r, ll val) {
        if (tree[v].min >= val) {
            doPushEq(v, l, r, val);
        } else if (tree[v].max > val) {
            if (tree[v].secondMin == tree[v].max) {
                tree[v].secondMin = val;
            }
            tree[v].sum -= 1LL * (tree[v].max - val) * tree[v].maxCnt;
            tree[v].max = val;
        }
    }
 
    void doPushMaxEq(int v, int l, int r, ll val) {
        if (tree[v].max <= val) {
            doPushEq(v, l, r, val);
        } else if (tree[v].min < val) {
            if (tree[v].secondMax == tree[v].min) {
                tree[v].secondMax = val;
            }
            tree[v].sum += 1LL * (val - tree[v].min) * tree[v].minCnt;
            tree[v].min = val;
        }
    }
 
    void doPushSum(int v, int l, int r, ll val) {
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
            tree[v].sum += 1LL * (r - l) * val;
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
 
        tree[v].diffGcd = gcd(tree[2 * v].diffGcd, tree[2 * v + 1].diffGcd);
 
        ll anyLeft = tree[2 * v].secondMax;
        ll anyRight = tree[2 * v + 1].secondMax;
        if (anyLeft != -INF && anyLeft != tree[2 * v].min && anyRight != -INF && anyRight != tree[2 * v + 1].min) {
            tree[v].diffGcd = gcd(tree[v].diffGcd, anyLeft - anyRight);
        }
 
        ll any = -1;
        if (anyLeft != -INF && anyLeft != tree[2 * v].min) {
            any = anyLeft;
        } else if (anyRight != -INF && anyRight != tree[2 * v + 1].min) {
            any = anyRight;
        }
        for (ll val : {tree[2 * v].min, tree[2 * v].max, tree[2 * v + 1].min, tree[2 * v + 1].max}) {
            if (val != tree[v].min && val != tree[v].max) {
                if (any != -1) {
                    tree[v].diffGcd = gcd(tree[v].diffGcd, val - any);
                } else {
                    any = val;
                }
            }
        }
    }
 
    void build(int v, int l, int r, const vector<ll>& inputArray) {
        tree[v].pushSum = 0;
        tree[v].pushEq = -1;
        if (l + 1 == r) {
            tree[v].max = inputArray[l];
            tree[v].secondMax = -INF;
            tree[v].maxCnt = 1;
 
            tree[v].min = inputArray[l];
            tree[v].secondMin = INF;
            tree[v].minCnt = 1;
 
            tree[v].diffGcd = 0;
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
        pushToChildren(v, l, r);
        if (ql <= l && r <= qr && tree[v].secondMax < val) {
            doPushMinEq(v, l, r, val);
            return;
        }
        int mid = (r + l) / 2;
        updateMinEq(2 * v, l, mid, ql, qr, val);
        updateMinEq(2 * v + 1, mid, r, ql, qr, val);
        updateFromChildren(v);
    }
 
    void updateMaxEq(int v, int l, int r, int ql, int qr, ll val) {
        if (qr <= l || r <= ql || tree[v].min >= val) {
            return;
        }
        pushToChildren(v, l, r);
        if (ql <= l && r <= qr && tree[v].secondMin > val) {
            doPushMaxEq(v, l, r, val);
            return;
        }
        int mid = (r + l) / 2;
        updateMaxEq(2 * v, l, mid, ql, qr, val);
        updateMaxEq(2 * v + 1, mid, r, ql, qr, val);
        updateFromChildren(v);
    }
 
    void updateEq(int v, int l, int r, int ql, int qr, ll val) {
        if (qr <= l || r <= ql) {
            return;
        }
        pushToChildren(v, l, r);
        if (ql <= l && r <= qr) {
            doPushEq(v, l, r, val);
            return;
        }
        int mid = (r + l) / 2;
        updateEq(2 * v, l, mid, ql, qr, val);
        updateEq(2 * v + 1, mid, r, ql, qr, val);
        updateFromChildren(v);
    }
 
    void updatePlusEq(int v, int l, int r, int ql, int qr, ll val) {
        if (qr <= l || r <= ql) {
            return;
        }
        pushToChildren(v, l, r);
        if (ql <= l && r <= qr) {
            doPushSum(v, l, r, val);
            return;
        }
        int mid = (r + l) / 2;
        updatePlusEq(2 * v, l, mid, ql, qr, val);
        updatePlusEq(2 * v + 1, mid, r, ql, qr, val);
        updateFromChildren(v);
    }
 
    long long findSum(int v, int l, int r, int ql, int qr) {
        if (qr <= l || r <= ql) {
            return 0;
        }
        pushToChildren(v, l, r);
        if (ql <= l && r <= qr) {
            return tree[v].sum;
        }
        int mid = (r + l) / 2;
        return findSum(2 * v, l, mid, ql, qr) + findSum(2 * v + 1, mid, r, ql, qr);
    }
 
    long long findMin(int v, int l, int r, int ql, int qr) {
        if (qr <= l || r <= ql) {
            return INF;
        }
        pushToChildren(v, l, r);
        if (ql <= l && r <= qr) {
            return tree[v].min;
        }
        int mid = (r + l) / 2;
        return min(findMin(2 * v, l, mid, ql, qr), findMin(2 * v + 1, mid, r, ql, qr));
    }
 
    long long findMax(int v, int l, int r, int ql, int qr) {
        if (qr <= l || r <= ql) {
            return -INF;
        }
        pushToChildren(v, l, r);
        if (ql <= l && r <= qr) {
            return tree[v].max;
        }
        int mid = (r + l) / 2;
        return max(findMax(2 * v, l, mid, ql, qr), findMax(2 * v + 1, mid, r, ql, qr));
    }
 
    long long findGcd(int v, int l, int r, int ql, int qr) {
        if (qr <= l || r <= ql) {
            return 0;
        }
        pushToChildren(v, l, r);
        if (ql <= l && r <= qr) {
            ll ans = tree[v].diffGcd;
            if (tree[v].secondMax != -INF) {
                ans = gcd(ans, tree[v].secondMax - tree[v].max);
            }
            if (tree[v].secondMin != INF) {
                ans = gcd(ans, tree[v].secondMin - tree[v].min);
            }
            ans = gcd(ans, tree[v].max);
            return ans;
        }
        int mid = (r + l) / 2;
        return gcd(findGcd(2 * v, l, mid, ql, qr), findGcd(2 * v + 1, mid, r, ql, qr));
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
            segTree.updateMaxEq(1, 0, n, ql, qr, k);
        } else if (type == 3) {
            ll k;
            cin >> k;
            segTree.updateEq(1, 0, n, ql, qr, k);
        } else if (type == 4) {
            ll k;
            cin >> k;
            segTree.updatePlusEq(1, 0, n, ql, qr, k);
        } else if (type == 5) {
            cout << segTree.findSum(1, 0, n, ql, qr) << '\n';
        } else if (type == 6) {
            cout << segTree.findMin(1, 0, n, ql, qr) << '\n';
        } else if (type == 7) {
            cout << segTree.findMax(1, 0, n, ql, qr) << '\n';
        } else {
            cout << segTree.findGcd(1, 0, n, ql, qr) << '\n';
        }
    }
    return 0;
}
