#include <iostream>
#include <vector>
#include <cmath>

class SqrtSegmentTree {
    static const int64_t INF = 2e18 + 7;

    struct Node {
        int64_t min;
        int64_t max;
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

    /* apply += val to node v */
    void doPushSum(const int v, const int l, const int r, const int64_t val) {
        tree[v].max += val;
        tree[v].min += val;
        tree[v].sum += (r - l) * val;
        tree[v].pushSum += val;
    }

    /* push lazy update to children of node v */
    void pushToChildren(const int v, const int l, const int r) {
        if (l + 1 == r) {
            return;
        }
        const int mid = (r + l) / 2;
        doPushSum(leftChild(v), l, mid, tree[v].pushSum);
        doPushSum(rightChild(v), mid, r, tree[v].pushSum);
        tree[v].pushSum = 0;
    }

    /* pull info from children and recalculate info for v */
    void updateFromChildren(const int v) {
        tree[v].sum = tree[leftChild(v)].sum + tree[rightChild(v)].sum;
        tree[v].max = std::max(tree[leftChild(v)].max, tree[rightChild(v)].max);
        tree[v].min = std::min(tree[leftChild(v)].min, tree[rightChild(v)].min);
    }

    /* build segment tree according to inputArray */
    void build(const int v, const int l, const int r, const std::vector<int>& inputArray) {
        tree[v].pushSum = 0;
        if (l + 1 == r) {
            tree[v].max = inputArray[l];
            tree[v].min = inputArray[l];
            tree[v].sum = inputArray[l];
        } else {
            const int mid = (r + l) / 2;
            build(leftChild(v), l, mid, inputArray);
            build(rightChild(v), mid, r, inputArray);
            updateFromChildren(v);
        }
    }

    /* += val on segment [ql, qr) */
    void updatePlusEq(const int v, const int l, const int r, const int ql, const int qr, const int val) {
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

    /* sqrt= on segment [ql, qr) */
    void updateSqrtEq(const int v, const int l, const int r, const int ql, const int qr) {
        if (qr <= l || r <= ql) {
            return;
        }
        if (ql <= l && r <= qr && tree[v].max - tree[v].min <= 1) {
            const int64_t maxDiff = static_cast<int64_t>(sqrt(tree[v].max)) - tree[v].max;
            const int64_t minDiff = static_cast<int64_t>(sqrt(tree[v].min)) - tree[v].min;
            if (maxDiff == minDiff) {
                doPushSum(v, l, r, maxDiff);
                return;
            }
        }
        pushToChildren(v, l, r);
        const int mid = (r + l) / 2;
        updateSqrtEq(leftChild(v), l, mid, ql, qr);
        updateSqrtEq(rightChild(v), mid, r, ql, qr);
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

    /* find minimum on segment [ql, qr) */
    int64_t findMin(const int v, const int l, const int r, const int ql, const int qr) {
        if (qr <= l || r <= ql) {
            return INF;
        }
        if (ql <= l && r <= qr) {
            return tree[v].min;
        }
        pushToChildren(v, l, r);
        const int mid = (r + l) / 2;
        return std::min(findMin(leftChild(v), l, mid, ql, qr), findMin(rightChild(v), mid, r, ql, qr));
    }

public:
    /* construct segment tree from inputArray */
    SqrtSegmentTree(const std::vector<int>& inputArray) {
        n = inputArray.size();
        tree.resize(4 * n);
        build(1, 0, n, inputArray);
    }

    /* += val on segment [ql, qr) */
    void updatePlusEq(const int ql, const int qr, const int val) {
        updatePlusEq(1, 0, n, ql, qr, val);
    }

    /* sqrt= on segment [ql, qr) */
    void updateSqrtEq(const int ql, const int qr) {
        updateSqrtEq(1, 0, n, ql, qr);
    }

    /* find sum on segment [ql, qr) */
    int64_t findSum(const int ql, const int qr) {
        return findSum(1, 0, n, ql, qr);
    }

    /* find minimum on segment [ql, qr) */
    int64_t findMin(const int ql, const int qr) {
        return findMin(1, 0, n, ql, qr);
    }
};

int main() {
    int n, q;
    std::cin >> n >> q;
    std::vector<int> inputArray(n);
    for (int &val : inputArray) {
        std::cin >> val;
    }
    SqrtSegmentTree segTree(inputArray);
    for (int i = 0; i < q; i++) {
        int type, ql, qr;
        std::cin >> type >> ql >> qr;
        qr++;
        if (type == 1) {
            int toAdd;
            std::cin >> toAdd;
            segTree.updatePlusEq(ql, qr, toAdd);
        } else if (type == 2) {
            segTree.updateSqrtEq(ql, qr);
        } else if (type == 3) {
            std::cout << segTree.findMin(ql, qr) << '\n';
        } else if (type == 4) {
            std::cout << segTree.findSum(ql, qr) << '\n';
        }

    }
    return 0;
}
