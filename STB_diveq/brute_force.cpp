#include <bits/stdc++.h>
 
using namespace std;
 
typedef long long ll;
 
ll divide(ll a, ll b) {
    if (a >= 0) {
        return a / b;
    } else {
        return -((-a + b - 1) / b);
    }
}
 
int main() {
    int n, q;
    cin >> n >> q;
    vector<ll> arr(n);
    for (ll &val : arr) {
        cin >> val;
    }
    for (int i = 0; i < q; i++) {
        int type, l, r;
        cin >> type >> l >> r;
        r++;
        if (type == 1) {
            ll c;
            cin >> c;
            for (int j = l; j < r; j++) {
                arr[j] += c;
            }
        } else if (type == 2) {
            ll d;
            cin >> d;
            for (int j = l; j < r; j++) {
                arr[j] = divide(arr[j], d);
            }
        } else if (type == 3) {
            ll minVal = arr[l];
            for (int j = l + 1; j < r; j++) {
                minVal = min(minVal, arr[j]);
            }
            cout << minVal << '\n';
        } else if (type == 4) {
            ll sum = 0;
            for (int j = l; j < r; j++) {
                sum += arr[j];
            }
            cout << sum << '\n';
        }
    }
}
