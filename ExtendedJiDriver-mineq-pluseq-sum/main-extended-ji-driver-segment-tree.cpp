#include <iostream>
#include <cmath>
#include <vector>

class ExtendedJiDriverSegmentTree {
    static const int64_t INF = 1e15 + 7;

    struct Node {
        int64_t max;
        int maxCnt;
        int64_t secondMax;
        int64_t sum;
        int64_t pushSum;
    };

    std::vector<Node> tree;
    int n;

    static int leftChild(const int v) {
        return 2 * v;
    }

    static int rightChild(const int v) {
        return 2 * v + 1;
    }

    /* apply min= val to node v */
    void updateWithVal(const int v, const int64_t val) {
        if (tree[v].max > val) {
            tree[v].sum -= 1LL * (tree[v].max - val) * tree[v].maxCnt;
            tree[v].max = val;
        }
    }

    /* apply += val to node v */
    void doPushSum(const int v, const int l, const int r, const int64_t val) {
        tree[v].max += val;
        tree[v].secondMax += val;
        tree[v].sum += 1LL * (r - l) * val;
        tree[v].pushSum += val;
    }

    /* push lazy update to children of node v */
    void pushToChildren(const int v, const int l, const int r) {
        const int mid = (r + l) / 2;
        doPushSum(leftChild(v), l, mid, tree[v].pushSum);
        doPushSum(rightChild(v), mid, r, tree[v].pushSum);
        tree[v].pushSum = 0;
        updateWithVal(leftChild(v), tree[v].max);
        updateWithVal(rightChild(v), tree[v].max);
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
    }

    /* build segment tree according to inputArray */
    void build(const int v, const int l, const int r, const std::vector<int64_t>& inputArray) {
        tree[v].pushSum = 0;
        if (l + 1 == r) {
            tree[v].max = inputArray[l];
            tree[v].secondMax = -INF;
            tree[v].maxCnt = 1;
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
        if (ql <= l && r <= qr && tree[v].secondMax < val) {
            updateWithVal(v, val);
            return;
        }
        pushToChildren(v, l, r);
        const int mid = (r + l) / 2;
        updateMinEq(leftChild(v), l, mid, ql, qr, val);
        updateMinEq(rightChild(v), mid, r, ql, qr, val);
        updateFromChildren(v);
    }

    /* += val on segment [ql, qr) */
    void updatePlusEq(const int v, const int l, const int r, const int ql, const int qr, const int64_t val) {
        if (qr <= l || r <= ql) {
            return;
        }
        if (ql <= l && r <= qr) {
            doPushSum(v, l, r, val);
            return;
        }
        pushToChildren(v, l, r);
        const int mid = (r + l) / 2;
        updatePlusEq(leftChild(v), l, mid, ql, qr, val);
        updatePlusEq(rightChild(v), mid, r, ql, qr, val);
        updateFromChildren(v);
    }

    /* find sum on segment [ql, qr) */
    int64_t findSum(const int v, const int l, const int r, const int ql, const int qr) {
        if (qr <= l || r <= ql) {
            return 0;
        }
        if (ql <= l && r <= qr) {
            return tree[v].sum;
        }
        pushToChildren(v, l, r);
        const int mid = (r + l) / 2;
        return findSum(leftChild(v), l, mid, ql, qr) + findSum(rightChild(v), mid, r, ql, qr);
    }

public:
    ExtendedJiDriverSegmentTree(const std::vector<int64_t>& inputArray) {
        n = inputArray.size();
        tree.resize(4 * n);
        build(1, 0, n, inputArray);
    }

    /* min= val on segment [ql, qr) */
    void updateMinEq(const int ql, const int qr, const int val) {
        updateMinEq(1, 0, n, ql, qr, val);
    }

    /* += val on segment [ql, qr) */
    void updatePlusEq(const int ql, const int qr, const int val) {
        updatePlusEq(1, 0, n, ql, qr, val);
    }

    /* find sum on segment [ql, qr) */
    int64_t findSum(const int ql, const int qr) {
        return findSum(1, 0, n, ql, qr);
    }
};

int main() {
    int n;
    std::cin >> n;
    std::vector<int64_t> inputArray(n);
    for (int64_t &val : inputArray) {
        std::cin >> val;
    }
    ExtendedJiDriverSegmentTree segTree(inputArray);
    int q;
    std::cin >> q;
    for (int i = 0; i < q; i++) {
        int type, ql, qr;
        std::cin >> type >> ql >> qr;
        ql--;
        if (type == 1) {
            int val;
            std::cin >> val;
            segTree.updateMinEq(ql, qr, val);
        } else if (type == 2) {
            int toAdd;
            std::cin >> toAdd;
            segTree.updatePlusEq(ql, qr, toAdd);
        } else {
            std::cout << segTree.findSum(ql, qr) << '\n';
        }
    }
    return 0;
}
