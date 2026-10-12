#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int find_disjoint_paths(int n, const vector<pair<int, int>>& edges, int s, int t) {
    // Build adjacency list
    vector<vector<int>> adj(n);
    vector<vector<bool>> edge_used(n, vector<bool>(n, false));
    
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }
    
    int paths = 0;
    
    // Try to find up to 3 edge-disjoint paths
    for (int attempt = 0; attempt < 3; attempt++) {
        queue<int> q;
        vector<int> parent(n, -1);
        vector<pair<int, int>> path_edges;
        
        q.push(s);
        parent[s] = s;
        
        // BFS to find path
        while (!q.empty() && parent[t] == -1) {
            int v = q.front();
            q.pop();
            
            for (int u : adj[v]) {
                if (parent[u] == -1 && !edge_used[min(v, u)][max(v, u)]) {
                    parent[u] = v;
                    q.push(u);
                    if (u == t) break;
                }
            }
        }
        
        // If no path found, we're done
        if (parent[t] == -1) break;
        
        // Reconstruct path and mark edges as used
        int current = t;
        while (current != s) {
            int prev = parent[current];
            edge_used[min(prev, current)][max(prev, current)] = true;
            current = prev;
        }
        
        paths++;
    }
    
    return paths;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    if (n == 0) {
        cout << 0 << endl;
        return 0;
    }
    
    vector<pair<int, int>> edges;
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        if (a != b) {
            edges.emplace_back(a, b);
        }
    }
    
    long long total = 0;
    
    for (int a = 0; a < n; a++) {
        for (int b = a + 1; b < n; b++) {
            int flow = find_disjoint_paths(n, edges, a, b);
            total += flow;
        }
    }
    
    cout << total << endl;
    
    return 0;
}