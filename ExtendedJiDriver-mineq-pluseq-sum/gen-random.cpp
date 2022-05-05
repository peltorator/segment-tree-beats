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
    if (q == 0) {
        q = n;
    }

    cout << n << endl;
    for (int i = 0; i < n; i++) {
        cout << rnd.next(-MX, MX) << " \n"[i == n - 1];
    }

    cout << q << endl;
    for (int i = 0; i < q; i++) {
        int type = rnd.next(1, 3);
        int ql = rnd.next(1, n);
        int qr = rnd.next(1, n);
        if (ql > qr) {
            swap(ql, qr);
        }
        cout << type << ' ' << ql << ' ' << qr;
        if (type == 1) {
            cout << ' ' << rnd.next(-MX, MX);
        } else if (type == 2) {
            int ten7 = 10000000;
            cout << ' ' << rnd.next(-ten7, ten7);
        }
        cout << "\n";
    }
}
