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

    vector<int> disc(n + 1, 0), low(n + 1, 0), parent(n + 1, -1);
    int timer = 0;
    vector<pair<int,int>> estack;
    estack.reserve(max(1, m));
    vector<char> inOdd(n + 1, 0);

    function<void(int)> dfs = [&](int u) {
        disc[u] = low[u] = ++timer;
        for (int v : g[u]) {
            if (!disc[v]) {
                parent[v] = u;
                estack.emplace_back(u, v);
                dfs(v);
                low[u] = min(low[u], low[v]);
                if (low[v] >= disc[u]) {
                    // Extract biconnected component edges
                    vector<pair<int,int>> edges;
                    while (!estack.empty()) {
                        auto e = estack.back(); estack.pop_back();
                        edges.push_back(e);
                        if ((e.first == u && e.second == v) || (e.first == v && e.second == u)) break;
                    }
                    if (!edges.empty()) {
                        vector<int> nodes;
                        nodes.reserve(edges.size() * 2);
                        for (auto &e : edges) { nodes.push_back(e.first); nodes.push_back(e.second); }
                        sort(nodes.begin(), nodes.end());
                        nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());
                        int k = (int)nodes.size();
                        vector<vector<int>> lg(k);
                        auto idx = [&](int x) { return int(lower_bound(nodes.begin(), nodes.end(), x) - nodes.begin()); };
                        for (auto &e : edges) {
                            int a = idx(e.first), b = idx(e.second);
                            if (a == b) continue;
                            lg[a].push_back(b);
                            lg[b].push_back(a);
                        }
                        vector<int> color(k, -1);
                        bool non_bip = false;
                        deque<int> dq;
                        for (int s = 0; s < k && !non_bip; ++s) if (color[s] == -1) {
                            color[s] = 0; dq.push_back(s);
                            while (!dq.empty() && !non_bip) {
                                int x = dq.front(); dq.pop_front();
                                for (int y : lg[x]) {
                                    if (color[y] == -1) { color[y] = color[x] ^ 1; dq.push_back(y); }
                                    else if (color[y] == color[x]) { non_bip = true; break; }
                                }
                            }
                        }
                        if (non_bip) {
                            for (int vtx : nodes) inOdd[vtx] = 1;
                        }
                    }
                }
            } else if (v != parent[u] && disc[v] < disc[u]) {
                estack.emplace_back(u, v);
                low[u] = min(low[u], disc[v]);
            }
        }
    };

    for (int i = 1; i <= n; ++i) if (!disc[i]) dfs(i);

    long long not_invitable = 0;
    for (int i = 1; i <= n; ++i) if (!inOdd[i]) ++not_invitable;
    cout << not_invitable << '\n';
    return 0;
}
