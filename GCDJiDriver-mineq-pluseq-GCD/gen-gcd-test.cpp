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
 
    cout << n << endl;
    for (int i = 0; i < n; i++) {
        cout << rnd.next(1, MX) << " \n"[i == n - 1];
    }
 
    cout << q << endl;
    for (int i = 0; i < q; i++) {
        int type = 8;
        int ql = rnd.next(1, n - 100);
        int qr = 0;
        if (i % 2 == 0) {
            qr = ql + rnd.next(1, 10);
        } else {
            qr = rnd.next(1, n);
        }
        if (qr < ql) {
            swap(ql, qr);
        }
        cout << type << ' ' << ql << ' ' << qr;
        cout << "\n";
    }
}
