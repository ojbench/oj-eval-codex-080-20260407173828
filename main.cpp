#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<vector<int>> g(n + 1);
    for (int i = 0; i < m; ++i) {
        int x, y; cin >> x >> y;
        if (x < 1 || x > n || y < 1 || y > n || x == y) continue;
        g[x].push_back(y);
        g[y].push_back(x);
    }

    const int LOG = 17 + 1; // since n <= 1e5
    vector<int> depth(n + 1, 0), tin(n + 1, 0), tout(n + 1, 0), parent(n + 1, 0);
    vector<array<int, 19>> up(n + 1);
    int timer = 0;

    function<void(int,int)> dfs = [&](int u, int p) {
        parent[u] = p;
        tin[u] = ++timer;
        up[u][0] = (p ? p : u);
        for (int k = 1; k < 19; ++k) up[u][k] = up[ up[u][k-1] ][k-1];
        for (int v : g[u]) if (v != p) {
            if (!tin[v]) {
                depth[v] = depth[u] + 1;
                dfs(v, u);
            }
        }
        tout[u] = ++timer;
    };

    auto is_ancestor = [&](int u, int v) { // u is ancestor of v
        return tin[u] <= tin[v] && tout[v] <= tout[u];
    };

    function<int(int,int)> lca = [&](int a, int b) {
        if (is_ancestor(a,b)) return a;
        if (is_ancestor(b,a)) return b;
        for (int k = 18; k >= 0; --k) {
            int ua = up[a][k];
            if (!is_ancestor(ua, b)) a = ua;
        }
        return parent[a];
    };

    for (int i = 1; i <= n; ++i) if (!tin[i]) { depth[i]=0; dfs(i, 0); }

    vector<int> cnt(n + 1, 0);

    // Process each edge once; consider only edges to ancestors (back edges)
    for (int u = 1; u <= n; ++u) {
        for (int v : g[u]) if (tin[u] > tin[v]) { // ensure each edge once; v is ancestor or cross
            if (v == parent[u]) continue; // tree edge
            int a = u, b = v;
            int w = lca(a, b);
            // If parity along the cycle is odd: depth[a]+depth[b]-2*depth[w] + 1 is odd
            int cycle_len = (depth[a] + depth[b] - 2 * depth[w]) + 1;
            if (cycle_len % 2 == 1) {
                cnt[a] += 1;
                cnt[b] += 1;
                cnt[w] -= 2;
            }
        }
    }

    function<void(int,int)> dfs2 = [&](int u, int p){
        for (int v : g[u]) if (v != p && parent[v] == u) {
            dfs2(v, u);
            cnt[u] += cnt[v];
        }
    };
    for (int i = 1; i <= n; ++i) if (parent[i] == 0) dfs2(i, 0);

    long long not_invitable = 0;
    for (int i = 1; i <= n; ++i) if (cnt[i] == 0) ++not_invitable;
    cout << not_invitable << '\n';
    return 0;
}
