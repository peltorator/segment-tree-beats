#include <bits/stdc++.h>

/* calculates always non-negative GCD of a and b */
int64_t gcd(int64_t a, int64_t b) {
    a = abs(a);
    b = abs(b);
    while (a) {
        b %= a;
        std::swap(a, b);
    }
    return b;
}

class GCDJiDriverSegmentTree {
    static const int64_t INF = 1e15 + 7;

    struct Node {
        int64_t min;
        int minCnt;
        int64_t secondMin;
        int64_t max;
        int maxCnt;
        int64_t secondMax;
        long long sum;
        int64_t diffGCD;
        int64_t pushSum;
        int64_t pushEq;
    };

    std::vector<Node> tree;
    int n;

    static int leftChild(const int v) {
        return 2 * v;
    }

    static int rightChild(const int v) {
        return 2 * v + 1;
    }

    /* apply = val to node v */
    void doPushEq(const int v, const int l, const int r, const int64_t val) {
        tree[v].min = tree[v].max = tree[v].pushEq = val;
        tree[v].minCnt = tree[v].maxCnt = r - l;
        tree[v].secondMax = -INF;
        tree[v].secondMin = INF;
        tree[v].sum = val * (r - l);
        tree[v].diffGCD = 0;
        tree[v].pushSum = 0;
    }

    /* apply min= val to node v */
    void doPushMinEq(const int v, const int l, const int r, const int64_t val) {
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

    /* apply max= val to node v */
    void doPushMaxEq(const int v, const int l, const int r, const int64_t val) {
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

    /* apply += val to node v */
    void doPushSum(const int v, const int l, const int r, const int64_t val) {
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

    /* push lazy update to children of node v */
    void pushToChildren(const int v, const int l, const int r) {
        if (l + 1 == r) {
            return;
        }
        int mid = (r + l) / 2;
        if (tree[v].pushEq != -1) {
            doPushEq(leftChild(v), l, mid, tree[v].pushEq);
            doPushEq(rightChild(v), mid, r, tree[v].pushEq);
            tree[v].pushEq = -1;
        } else {
            doPushSum(leftChild(v), l, mid, tree[v].pushSum);
            doPushSum(rightChild(v), mid, r, tree[v].pushSum);
            tree[v].pushSum = 0;
            doPushMinEq(leftChild(v), l, mid, tree[v].max);
            doPushMinEq(rightChild(v), mid, r, tree[v].max);
            doPushMaxEq(leftChild(v), l, mid, tree[v].min);
            doPushMaxEq(rightChild(v), mid, r, tree[v].min);
        }
    }

    /* pull info from children and recalculate info for v */
    void updateFromChildren(const int v) {
        tree[v].sum = tree[leftChild(v)].sum + tree[rightChild(v)].sum;

        tree[v].max = std::max(tree[leftChild(v)].max, tree[rightChild(v)].max);
        tree[v].secondMax = std::max(tree[leftChild(v)].secondMax, tree[rightChild(v)].secondMax);
        tree[v].maxCnt = 0;
        if (tree[leftChild(v)].max == tree[v].max) {
            tree[v].maxCnt += tree[leftChild(v)].maxCnt;
        } else {
            tree[v].secondMax = std::max(tree[v].secondMax, tree[leftChild(v)].max);
        }
        if (tree[rightChild(v)].max == tree[v].max) {
            tree[v].maxCnt += tree[rightChild(v)].maxCnt;
        } else {
            tree[v].secondMax = std::max(tree[v].secondMax, tree[rightChild(v)].max);
        }

        tree[v].min = std::min(tree[leftChild(v)].min, tree[rightChild(v)].min);
        tree[v].secondMin = std::min(tree[leftChild(v)].secondMin, tree[rightChild(v)].secondMin);
        tree[v].minCnt = 0;
        if (tree[leftChild(v)].min == tree[v].min) {
            tree[v].minCnt += tree[leftChild(v)].minCnt;
        } else {
            tree[v].secondMin = std::min(tree[v].secondMin, tree[leftChild(v)].min);
        }
        if (tree[rightChild(v)].min == tree[v].min) {
            tree[v].minCnt += tree[rightChild(v)].minCnt;
        } else {
            tree[v].secondMin = std::min(tree[v].secondMin, tree[rightChild(v)].min);
        }

        tree[v].diffGCD = gcd(tree[leftChild(v)].diffGCD, tree[rightChild(v)].diffGCD);

        const int64_t anyLeft = tree[leftChild(v)].secondMax;
        const int64_t anyRight = tree[rightChild(v)].secondMax;
        if (anyLeft != -INF && anyLeft != tree[leftChild(v)].min && anyRight != -INF && anyRight != tree[rightChild(v)].min) {
            tree[v].diffGCD = gcd(tree[v].diffGCD, anyLeft - anyRight);
        }

        int64_t any = -1;
        if (anyLeft != -INF && anyLeft != tree[leftChild(v)].min) {
            any = anyLeft;
        } else if (anyRight != -INF && anyRight != tree[rightChild(v)].min) {
            any = anyRight;
        }
        for (const int64_t val : {tree[leftChild(v)].min, tree[leftChild(v)].max, tree[rightChild(v)].min, tree[rightChild(v)].max}) {
            if (val != tree[v].min && val != tree[v].max) {
                if (any != -1) {
                    tree[v].diffGCD = gcd(tree[v].diffGCD, val - any);
                } else {
                    any = val;
                }
            }
        }
    }

    /* build segment tree according to inputArray */
    void build(const int v, const int l, const int r, const std::vector<int64_t>& inputArray) {
        tree[v].pushSum = 0;
        tree[v].pushEq = -1;
        if (l + 1 == r) {
            tree[v].max = inputArray[l];
            tree[v].secondMax = -INF;
            tree[v].maxCnt = 1;

            tree[v].min = inputArray[l];
            tree[v].secondMin = INF;
            tree[v].minCnt = 1;

            tree[v].diffGCD = 0;
            tree[v].sum = inputArray[l];
        } else {
            const int mid = (r + l) / 2;
            build(leftChild(v), l, mid, inputArray);
            build(rightChild(v), mid, r, inputArray);
            updateFromChildren(v);
        }
    }

    /* min= val on segment [ql, qr) */
    void updateMinEq(const int v, const int l, const int r, const int ql, const int qr, const int64_t val) {
        if (qr <= l || r <= ql || tree[v].max <= val) {
            return;
        }
        pushToChildren(v, l, r);
        if (ql <= l && r <= qr && tree[v].secondMax < val) {
            doPushMinEq(v, l, r, val);
            return;
        }
        const int mid = (r + l) / 2;
        updateMinEq(leftChild(v), l, mid, ql, qr, val);
        updateMinEq(rightChild(v), mid, r, ql, qr, val);
        updateFromChildren(v);
    }

    /* max= val on segment [ql, qr) */
    void updateMaxEq(const int v, const int l, const int r, const int ql, const int qr, const int64_t val) {
        if (qr <= l || r <= ql || tree[v].min >= val) {
            return;
        }
        pushToChildren(v, l, r);
        if (ql <= l && r <= qr && tree[v].secondMin > val) {
            doPushMaxEq(v, l, r, val);
            return;
        }
        const int mid = (r + l) / 2;
        updateMaxEq(leftChild(v), l, mid, ql, qr, val);
        updateMaxEq(rightChild(v), mid, r, ql, qr, val);
        updateFromChildren(v);
    }

    /* = val on segment [ql, qr) */
    void updateEq(const int v, const int l, const int r, const int ql, const int qr, const int64_t val) {
        if (qr <= l || r <= ql) {
            return;
        }
        pushToChildren(v, l, r);
        if (ql <= l && r <= qr) {
            doPushEq(v, l, r, val);
            return;
        }
        const int mid = (r + l) / 2;
        updateEq(leftChild(v), l, mid, ql, qr, val);
        updateEq(rightChild(v), mid, r, ql, qr, val);
        updateFromChildren(v);
    }

    /* += val on segment [ql, qr) */
    void updatePlusEq(const int v, const int l, const int r, const int ql, const int qr, const int64_t val) {
        if (qr <= l || r <= ql) {
            return;
        }
        pushToChildren(v, l, r);
        if (ql <= l && r <= qr) {
            doPushSum(v, l, r, val);
            return;
        }
        const int mid = (r + l) / 2;
        updatePlusEq(leftChild(v), l, mid, ql, qr, val);
        updatePlusEq(rightChild(v), mid, r, ql, qr, val);
        updateFromChildren(v);
    }

    /* find sum on segment [ql, qr) */
    long long findSum(const int v, const int l, const int r, const int ql, const int qr) {
        if (qr <= l || r <= ql) {
            return 0;
        }
        pushToChildren(v, l, r);
        if (ql <= l && r <= qr) {
            return tree[v].sum;
        }
        const int mid = (r + l) / 2;
        return findSum(leftChild(v), l, mid, ql, qr) + findSum(rightChild(v), mid, r, ql, qr);
    }

    /* find min on segment [ql, qr) */
    long long findMin(const int v, const int l, const int r, const int ql, const int qr) {
        if (qr <= l || r <= ql) {
            return INF;
        }
        pushToChildren(v, l, r);
        if (ql <= l && r <= qr) {
            return tree[v].min;
        }
        const int mid = (r + l) / 2;
        return std::min(findMin(leftChild(v), l, mid, ql, qr), findMin(rightChild(v), mid, r, ql, qr));
    }

    /* find max on segment [ql, qr) */
    long long findMax(const int v, const int l, const int r, const int ql, const int qr) {
        if (qr <= l || r <= ql) {
            return -INF;
        }
        pushToChildren(v, l, r);
        if (ql <= l && r <= qr) {
            return tree[v].max;
        }
        const int mid = (r + l) / 2;
        return std::max(findMax(leftChild(v), l, mid, ql, qr), findMax(rightChild(v), mid, r, ql, qr));
    }

    /* find gcd on segment [ql, qr) */
    long long findGCD(const int v, const int l, const int r, const int ql, const int qr) {
        if (qr <= l || r <= ql) {
            return 0;
        }
        pushToChildren(v, l, r);
        if (ql <= l && r <= qr) {
            int64_t ans = tree[v].diffGCD;
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
        return gcd(findGCD(leftChild(v), l, mid, ql, qr), findGCD(rightChild(v), mid, r, ql, qr));
    }

public:
    GCDJiDriverSegmentTree(const std::vector<int64_t>& inputArray) {
        n = inputArray.size();
        tree.resize(4 * n);
        build(1, 0, n, inputArray);
    }

    /* min= val on segment [ql, qr) */
    void updateMinEq(const int ql, const int qr, const int val) {
        updateMinEq(1, 0, n, ql, qr, val);
    }

    /* max= val on segment [ql, qr) */
    void updateMaxEq(const int ql, const int qr, const int val) {
        updateMaxEq(1, 0, n, ql, qr, val);
    }

    /* = val on segment [ql, qr) */
    void updateEq(const int ql, const int qr, const int val) {
        updateEq(1, 0, n, ql, qr, val);
    }

    /* += val on segment [ql, qr) */
    void updatePlusEq(const int ql, const int qr, const int val) {
        updatePlusEq(1, 0, n, ql, qr, val);
    }

    /* find sum on segment [ql, qr) */
    int64_t findSum(const int ql, const int qr) {
        return findSum(1, 0, n, ql, qr);
    }

    /* find min on segment [ql, qr) */
    int64_t findMin(const int ql, const int qr) {
        return findMin(1, 0, n, ql, qr);
    }

    /* find max on segment [ql, qr) */
    int64_t findMax(const int ql, const int qr) {
        return findMax(1, 0, n, ql, qr);
    }

    /* find gcd on segment [ql, qr) */
    int64_t findGCD(const int ql, const int qr) {
        return findGCD(1, 0, n, ql, qr);
    }
};

int main() {
    int n;
    std::cin >> n;
    std::vector<int64_t> inputArray(n);
    for (int64_t &val : inputArray) {
        std::cin >> val;
    }
    GCDJiDriverSegmentTree segTree(inputArray);
    int q;
    std::cin >> q;
    for (int i = 0; i < q; i++) {
        int type, ql, qr;
        std::cin >> type >> ql >> qr;
        ql--;
        if (type == 1) {
            int64_t val;
            std::cin >> val;
            segTree.updateMinEq(ql, qr, val);
        } else if (type == 2) {
            int64_t val;
            std::cin >> val;
            segTree.updateMaxEq(ql, qr, val);
        } else if (type == 3) {
            int64_t toAssign;
            std::cin >> toAssign;
            segTree.updateEq(ql, qr, toAssign);
        } else if (type == 4) {
            int64_t toAdd;
            std::cin >> toAdd;
            segTree.updatePlusEq(ql, qr, toAdd);
        } else if (type == 5) {
            std::cout << segTree.findSum(ql, qr) << '\n';
        } else if (type == 6) {
            std::cout << segTree.findMin(ql, qr) << '\n';
        } else if (type == 7) {
            std::cout << segTree.findMax(ql, qr) << '\n';
        } else {
            std::cout << segTree.findGCD(ql, qr) << '\n';
        }
    }
    return 0;
}
