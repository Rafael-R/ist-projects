#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;


class Vertice 
{
private:
    vector<int> adjacencies;
    vector<int> costs;
    bool visited;

public:
    vector<int> getAdjacencies() { return adjacencies; }

    int getCost(int vertice) {
        int i = 0;
        for(int v : adjacencies) {
            if(vertice == v) {
                return costs[i];
            }
            i++;
        }
        return 0;
    }

    bool isVisited() { return visited; }

    void addAdjacencie(int adjacencie, int cost) {
        adjacencies.push_back(adjacencie);
        costs.push_back(cost);
    }

    void setVisited() { visited = true; }

    void setUnvisited() { visited = false; }
};


class Graph
{
private:
    int n, s, t;
    vector<Vertice *> vertices;

    bool bfs(int** rGraph, int parent[]) {
        for (Vertice* v : vertices) {
            v->setUnvisited();
        }

        queue<int> q;
        q.push(s);
        vertices[s]->setVisited();
        parent[s] = -1;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v : vertices[u]->getAdjacencies()) {
                if (!vertices[v]->isVisited() && rGraph[u][v] > 0) {
                    parent[v] = u;
                    if (v == t) {
                        return true;
                    }
                    q.push(v);
                    vertices[v]->setVisited();
                }
            }
        }
        
        return false;
    }

public:
    Graph(int numVertices) {
        n = numVertices + 2;
        s = 0;
        t = numVertices + 1;
        for (int i = 0; i < n; i++) {
            vertices.push_back(new Vertice());
        }
    }

    void addEdge(int i, int j, int c) {
        vertices[i]->addAdjacencie(j, c);
        vertices[j]->addAdjacencie(i, c);
    }

    int calculate() {
        int u, v;

        int** rGraph = new int*[n];
        for(int i = 0; i < n; i++)
            rGraph[i] = new int[n];
        
        for(u = 0; u < n; u++) {
            for(v = 0; v < n; v++) {
                rGraph[u][v] = vertices[u]->getCost(v);
            }
        }

        int parent[n];
        int max_flow = 0;

        while(bfs(rGraph, parent)) {
            int path_flow = INT_MAX;

            for(v = t; v != s; v = parent[v]) {
                u = parent[v];
                path_flow = min(path_flow, rGraph[u][v]);
            }

            for(v = t; v != s; v = parent[v]) {
                u = parent[v];
                rGraph[u][v] -= path_flow;
                rGraph[v][u] += path_flow;
            }

            max_flow += path_flow;
        }

        for(int i = 0; i < n; i++)
            delete[] rGraph[i];
        delete[] rGraph;

        return max_flow;
    }

    void free() {
        for (Vertice* vertice : vertices) { delete vertice; }
        vector<Vertice* >().swap(vertices);
    }
};


int main() {
    int n, k;

    cin >> n >> k;

    if (n < 2 || k < 0) { exit(EXIT_FAILURE); }

    Graph g(n);

    int x, y;
    for (int i = 1; i <= n; i++) {
        cin >> x >> y;
        g.addEdge(0, i, x);
        g.addEdge(i, n + 1, y);
    }

    int i, j, c;
    for (; k > 0; k--) {
        cin >> i >> j >> c;
        g.addEdge(i, j, c);
    }
    
    cout << g.calculate() << endl;

    g.free();

    exit(EXIT_SUCCESS);
}
