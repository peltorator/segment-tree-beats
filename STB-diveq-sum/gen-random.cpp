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
        n = rnd.next(1, 200000);
    }
    if (q == 0) {
        q = n;
    }

    cout << n << ' ' << q << endl;
    for (int i = 0; i < n; i++) {
        cout << rnd.next(-MX, MX) << " \n"[i == n - 1];
    }

    for (int i = 0; i < q; i++) {
        int type = rnd.next(1, 4);
        int ql = rnd.next(0, n - 1);
        int qr = rnd.next(0, n - 1);
        if (ql > qr) {
            swap(ql, qr);
        }
        cout << type << ' ' << ql << ' ' << qr;
        if (type == 1) {
            cout << ' ' << rnd.next(-10000000, 10000000);
        } else if (type == 2) {
            cout << ' ' << rnd.next(1, MX);
        }
        cout << "\n";
    }
}
