#include <bits/stdc++.h>

using namespace std;

long long division(long long a, long long b) {
    if (a >= 0) {
        return a / b;
    } else {
        return -((-a + b - 1) / b);
    }
}

int main() {
    int n, q;
    cin >> n >> q;
    vector<long long> inputArray(n);
    for (long long &val : inputArray) {
        cin >> val;
    }
    for (int i = 0; i < q; i++) {
        int type, ql, qr;
        cin >> type >> ql >> qr;
        qr++;
        if (type == 1) {
            long long toAdd;
            cin >> toAdd;
            for (int j = ql; j < qr; j++) {
                inputArray[j] += toAdd;
            }
        } else if (type == 2) {
            long long divisor;
            cin >> divisor;
            for (int j = ql; j < qr; j++) {
                inputArray[j] = division(inputArray[j], divisor);
            }
        } else if (type == 3) {
            long long minVal = inputArray[ql];
            for (int j = ql + 1; j < qr; j++) {
                minVal = min(minVal, inputArray[j]);
            }
            cout << minVal << '\n';
        } else if (type == 4) {
            long long sum = 0;
            for (int j = ql; j < qr; j++) {
                sum += inputArray[j];
            }
            cout << sum << '\n';
        }
    }
}
