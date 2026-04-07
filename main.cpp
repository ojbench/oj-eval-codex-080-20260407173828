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

    vector<int> tin(n + 1, 0), depth(n + 1, 0), parent(n + 1, 0);
    int timer = 0;

    function<void(int,int)> dfs = [&](int u, int p) {
        parent[u] = p;
        tin[u] = ++timer;
        for (int v : g[u]) {
            if (v == p) continue;
            if (!tin[v]) {
                depth[v] = depth[u] + 1;
                dfs(v, u);
            }
        }
    };
    for (int i = 1; i <= n; ++i) if (!tin[i]) dfs(i, 0);

    vector<long long> add(n + 1, 0);
    // For each back-edge (u -> ancestor v), if the cycle length is odd, mark path u..v
    for (int u = 1; u <= n; ++u) {
        for (int v : g[u]) {
            if (v == parent[u]) continue; // tree edge
            if (tin[v] < tin[u]) { // v is ancestor (back edge)
                int len = (depth[u] - depth[v]) + 1;
                if (len % 2 == 1) {
                    add[u] += 1;
                    if (parent[v] != 0) add[parent[v]] -= 1;
                }
            }
        }
    }

    function<void(int)> dfs2 = [&](int u){
        for (int v : g[u]) if (parent[v] == u) {
            dfs2(v);
            add[u] += add[v];
        }
    };
    for (int i = 1; i <= n; ++i) if (parent[i] == 0) dfs2(i);

    long long not_invitable = 0;
    for (int i = 1; i <= n; ++i) if (add[i] == 0) ++not_invitable;
    cout << not_invitable << '\n';
    return 0;
}
