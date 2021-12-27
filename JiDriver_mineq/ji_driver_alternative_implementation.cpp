#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
 
const int MAXN = 5000007;
const ll INF = 1e9 + 7;
 
struct Node
{
    ll max, nummax, nextmax, push, sum;
    Node():
        max(INF),
        nummax(0),
        nextmax(0),
        push(0),
        sum(0) {}
};
 
Node tree[MAXN];
int arr[MAXN];
  
void upd(int v)
{
    tree[v].sum = tree[2 * v].sum + tree[2 * v + 1].sum;
    tree[v].max = max(tree[2 * v].max, tree[2 * v + 1].max);
    if (tree[2 * v].max == tree[2 * v + 1].max)
    {
        tree[v].nummax = tree[2 * v].nummax + tree[2 * v + 1].nummax;
        tree[v].nextmax = max(tree[2 * v].nextmax, tree[2 * v + 1].nextmax);
    }
    else if (tree[2 * v].max > tree[2 * v + 1].max)
    {
        tree[v].nextmax = max(tree[2 * v].nextmax, tree[2 * v + 1].max);
        tree[v].nummax = tree[2 * v].nummax;
    }
    else
    {
        tree[v].nextmax = max(tree[2 * v + 1].nextmax, tree[2 * v].max);
        tree[v].nummax = tree[2 * v + 1].nummax;
    }
 
}
 
void init(int v, int l, int r)
{
    if (r < l)
        return;
    if (l == r) {
        tree[v].max = arr[l];
        tree[v].nextmax = 0;
        tree[v].sum = arr[l];
        tree[v].nummax = 1;
        tree[v].push = 0;
    } else {
        int mid = (r + l) / 2;
        init(2 * v, l, mid);
        init(2 * v + 1, mid + 1, r);
        upd(v);
    }
}
 
 
void push1(int v, int l, int r);
 
void take(int v, int l, int r, int k)
{  
    if (r < l || tree[v].max <= k)
        return;
    if (tree[v].nextmax < k)
    {
        tree[v].sum += (ll)(k - tree[v].max) * tree[v].nummax;
        tree[v].max = k;
        tree[v].push = k;
        return;
    }
}
 
void push1(int v, int l, int r)
{
    if (r < l || tree[v].push == 0)
        return;
    int mid = (r + l) / 2;
    take(2 * v, l, mid, tree[v].push);
    take(2 * v + 1, mid + 1, r, tree[v].push);
    tree[v].push = 0;
}
 
void tu(int v, int l, int r, int ql, int qr, int k)
{
    push1(v, l, r);
    if (r < l || qr < l || r < ql || k >= tree[v].max)
        return;
    if (ql <= l && r <= qr && k > tree[v].nextmax)
    {
        take(v, l, r, k);
        return;
    }
    int mid = (r + l) / 2;
    tu(2 * v, l, mid, ql, qr, k);
    tu(2 * v + 1, mid + 1, r, ql, qr, k);
    upd(v);
}
 
ll tf(int v, int l, int r, int ql, int qr)
{
    push1(v, l, r);
    if (r < l || ql > r || l > qr || INF * (ll)(r - l + 1) == tree[v].sum)
        return 0;
    if (ql <= l && r <= qr && tree[v].max != INF)
    {
        return tree[v].sum;
    }
    if (r == l)
        return 0;
    int mid = (r + l) / 2;
    ll ans = tf(2 * v, l, mid, ql, qr) + tf(2 * v + 1, mid + 1, r, ql, qr);
    return ans;
}
 
int main()
{
    ios::sync_with_stdio(0); cin.tie(0);
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }
    int q;
    cin >> q;
    init(1, 1, n);
    for (int i = 0; i < q; i++)
    {
        int x, l, r;
        cin >> x >> l >> r;
        if (x == 1)
        {
            int k;
            cin >> k;
            tu(1, 1, n, l, r, k);
        }
        else
            cout << tf(1, 1, n, l, r) << "\n";
    }
    return 0;
}
