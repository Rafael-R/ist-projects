#include <iostream>
#include <queue>
#include <vector>
using namespace std;

class Vertice {
   private:
    vector<int> adjacencies;
    bool visited = false;

   public:
    Vertice() {}
    void addAdjacencie(int index) { adjacencies.push_back(index); }
    vector<int> getAdjacencies() { return adjacencies; }
    void setVisited() { visited = true; }
    bool isVisited() { return visited; }
};

class Graph {
   private:
    int m, n, numberVertices;
    vector<Vertice *> vertices;

    bool calculateAux(int s, int t, vector<int> &parent) {
        queue<int> q;
        vector<bool> visited(numberVertices, false);

        q.push(s);
        parent[s] = -1;
        visited[s] = true;

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            for (int next : vertices[current]->getAdjacencies()) {
                if (!visited[next] && !vertices[next]->isVisited()) {
                    q.push(next);
                    parent[next] = current;
                    visited[next] = true;
                }
            }
        }

        return (visited[t]);
    }

   public:
    Graph(int m, int n) {
        this->m = m;
        this->n = n;
        this->numberVertices =
            ((m * n) * 2) +
            2;  // (m * n) * 2 porque cada vertice tem um in e um out

        for (int i = 0; i < numberVertices; i++) {
            vertices.push_back(new Vertice());
            if (i >= (numberVertices / 2) && i != numberVertices - 1)
                vertices[i - ((numberVertices - 2) / 2)]->addAdjacencie(i);
        }

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                int index = (((i - 1) * m) + j) + ((numberVertices - 2) / 2);
                if (i != 1)  // Norte
                    vertices[index]->addAdjacencie(((i - 2) * m) + j);
                if (j != 1)  // Oeste
                    vertices[index]->addAdjacencie(((i - 1) * m) + (j - 1));
                if (j != m)  // Este
                    vertices[index]->addAdjacencie(((i - 1) * m) + (j + 1));
                if (i != n)  // Sul
                    vertices[index]->addAdjacencie((i * m) + j);
            }
        }
    }
    void addStore(int a, int b) {
        int index = ((b - 1) * m) + a;
        vertices[0]->addAdjacencie(index);  // connect source to store
    }
    void addCitizen(int a, int b) {
        int index = (((b - 1) * m) + a) + ((numberVertices - 2) / 2);
        vertices[index]->addAdjacencie(numberVertices -
                                       1);  // connect citizen to sink
    }
    int calculateMaxFlow() {
        int s = 0;
        int t = numberVertices - 1;
        int maxFlow = 0;
        vector<int> parent(numberVertices);

        while (calculateAux(s, t, parent)) {
            for (int u, v = t; v != s; v = parent[v]) {
                u = parent[v];
                vertices[u]->setVisited();
            }
            maxFlow += 1;
        }

        return maxFlow;
    }
    void free() {
        for (Vertice *vertice : vertices) {
            delete vertice;
        }
    }
};

int main() {
    int m, n, s, c;

    cin >> m >> n >> s >> c;

    Graph g(m, n);

    for (int a, b, i = 0; i < s + c; i++) {
        cin >> a >> b;

        if (i < s)
            g.addStore(a, b);
        else
            g.addCitizen(a, b);
    }

    int result = g.calculateMaxFlow();

    cout << result << endl;

    g.free();

    return 0;
}