#pragma comment(linker, "/stack:1000000000")
#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,avx,avx2")
#include <bits/stdc++.h>
 
using namespace std;
 
typedef long long ll;
 
const int N = 300001;
 
ll arr[N];
 
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }
    int q;
    cin >> q;
    for (int i = 0; i < q; i++) {
        int type;
        cin >> type;
        int ql, qr;
        cin >> ql >> qr;
        if (type == 1) {
            ll val;
            cin >> val;
            for (int j = ql; j <= qr; j++) {
                arr[j] = min(arr[j], val);
            }
        } else if (type == 2) {
            ll val;
            cin >> val;
            for (int j = ql; j <= qr; j++) {
                arr[j] += val;
            }
        } else {
            long long sum = 0;
            for (int j = ql; j <= qr; j++) {
                sum += arr[j];
            }
            cout << sum << '\n';
        }
    }
}
