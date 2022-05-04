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
        cout << rnd.next(3, 4) << " \n"[i == n - 1];
    }

    for (int i = 0; i < q; i++) {
        int type = (i % 2 == 0 ? 4 : (i % 4 == 1 ? 2 : 1));
        int ql = rnd.next(0, n / 10);
        int qr = rnd.next(n - 1 - n / 10, n - 1);
        if (ql > qr) {
            swap(ql, qr);
        }
        cout << type << ' ' << ql << ' ' << qr;
        if (type == 1) {
            cout << " 2";
        }
        cout << "\n";
    }
}
