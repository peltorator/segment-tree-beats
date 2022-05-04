#include "testlib.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(0);
    cin.tie(0);
    registerGen(argc, argv, 1);

    const int MX = 1000000000;
    int n = atoi(argv[2]);
    int q = atoi(argv[3]);
    if (n == 0) {
        n = rnd.next(1, 300000);
    }
    bool mxtest = false;
    bool shuffled = false;
    if (q == 0) {
        q = n;
        mxtest = true;
    } else if (q == -1) {
        q = n;
        mxtest = true;
        shuffled = true;
    }

    cout << n << endl;
    for (int i = 0; i < n; i++) {
        bool small = true;
        if (mxtest) {
            if (i % 2 == 1) {
                small = false;
            }
        } else {
            if (rnd.next(1, 2) == 1) {
                small = false;
            }
        }
        int val = 0;
        if (small) {
            val = rnd.next(1, 100000);
        } else {
            if (mxtest) {
                val = rnd.next((int)(MX * 0.99), MX);
            } else {
                val = rnd.next(1000000, MX);
            }
        }
        cout << val << " \n"[i == n - 1];
    }

    vector<int> vals;
    for (int i = 0; i < q / 2; i++) {
        vals.push_back(rnd.next(1, MX));
    }
    if (!shuffled) {
        sort(vals.rbegin(), vals.rend());
    }

    cout << q << endl;
    for (int i = 0; i < q; i++) {
        int type = 2 - (i % 2);
        int ql = 1;
        int qr = n;
        if (!mxtest) {
            ql = rnd.next(1, n);
            qr = rnd.next(1, n);
            if (ql > qr) {
                swap(ql, qr);
            }
        }
        cout << type << ' ' << ql << ' ' << qr;
        if (type == 1) {
            cout << ' ' << vals[i / 2];
        }
        cout << "\n";
    }
}
