#include <iostream>
#include <vector>
#include <queue>
#include <stack>
using namespace std;


#include <iostream>
#include <vector>
#include <queue>
#include <stack>
using namespace std;


class Vertice 
{
private:
    vector<int> adjacencies;
    bool visited = false;
    int parent;
    int distance = -1;

public:
    void addAdjacencie(int vertice)
    {
        adjacencies.push_back(vertice);
    }

    vector<int> getAdjacencies() { return adjacencies; }

    void setVisited() { visited = true; }

    void setUnvisited() { visited = false; }

    bool isVisited() { return visited; }

    int getParent() { return parent; }

    void setParent(int parent) { this->parent = parent; }

    int getDistance() { return distance; }

    void setDistance(int distance) { this->distance = distance; }
};

class Graph
{
private:
    vector<Vertice *> vertices;
    int* incomingEdges;

public:
    Graph(int numVertices) {
        incomingEdges = new int[numVertices];
        for (int i = 0; i < numVertices; i++) {
            vertices.push_back(new Vertice());
            incomingEdges[i] = 0;
        }
    }

    void addEdge(int x, int y) {
        vertices[x - 1]->addAdjacencie(y - 1);
        incomingEdges[y - 1]++;
    }

    int* calculate() {}

    void free() {
        for (Vertice* vertice : vertices) { delete vertice; }
        vector<Vertice* >().swap(vertices);
        delete[] incomingEdges;
    }
};


int main() {
    int n, m;

    cin >> n >> m;

    if (n < 2 || m < 0) { exit(EXIT_FAILURE); }

    Graph g(n);

    int x, y;
    for (; m > 0; m--) {
        cin >> x >> y;
        g.addEdge(x, y);
    }
    
    int* result = g.calculate();

    cout << result[0] << ' ' << result[1] << endl;

    delete[] result;
    g.free();

    exit(EXIT_SUCCESS);
}
