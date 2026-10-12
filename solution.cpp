#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class MaxFlow {
private:
    struct Edge {
        int to, cap, rev;
        Edge(int t, int c, int r) : to(t), cap(c), rev(r) {}
    };
    
    vector<vector<Edge>> graph;
    vector<int> level, iter;
    int n;
    
    bool bfs(int s, int t) {
        level.assign(n, -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (const Edge& e : graph[v]) {
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] >= 0;
    }
    
    int dfs(int v, int t, int f) {
        if (v == t) return f;
        for (int& i = iter[v]; i < graph[v].size(); i++) {
            Edge& e = graph[v][i];
            if (e.cap > 0 && level[v] < level[e.to]) {
                int d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    graph[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }
    
public:
    MaxFlow(int size) : n(size) {
        graph.resize(n);
        level.resize(n);
        iter.resize(n);
    }
    
    void clear() {
        graph.clear();
        graph.resize(n);
    }
    
    void add_edge(int from, int to, int cap) {
        graph[from].emplace_back(to, cap, graph[to].size());
        graph[to].emplace_back(from, 0, graph[from].size() - 1);
    }
    
    int max_flow(int s, int t) {
        int flow = 0;
        while (bfs(s, t)) {
            iter.assign(n, 0);
            while (true) {
                int f = dfs(s, t, 1);
                if (f == 0) break;
                flow += f;
                if (flow >= 3) return flow; // Early termination since max degree is 3
            }
        }
        return flow;
    }
};

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
    MaxFlow maxflow(n);
    
    for (int a = 0; a < n; a++) {
        for (int b = a + 1; b < n; b++) {
            maxflow.clear();
            for (const auto& e : edges) {
                maxflow.add_edge(e.first, e.second, 1);
                maxflow.add_edge(e.second, e.first, 1);
            }
            int flow = maxflow.max_flow(a, b);
            total += flow;
        }
    }
    
    cout << total << endl;
    
    return 0;
}