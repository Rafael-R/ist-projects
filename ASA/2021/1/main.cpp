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

    void aux(int vertice, int distance) {
        while (vertice != -1) {
            if (distance <= vertices[vertice]->getDistance()) {
                break;
            } else {
                vertices[vertice]->setDistance(distance++);
                vertice = vertices[vertice]->getParent();
            }
        } 
    }

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

    int* calculate() {
        int numVertices = vertices.size();
        vertices.push_back(new Vertice());
        vertices[numVertices]->setParent(-1);

        int interventions = 0;
        for (int i = 0; i < numVertices; i++) {
            if (incomingEdges[i] == 0) {
                vertices[numVertices]->addAdjacencie(i);
                interventions++;
            }
        }
    
        stack<int> s;
        vector<int> adjacencies;

        int vertice = numVertices;

        s.push(vertice);

        while (!s.empty()) {
            vertice = s.top();
            s.pop();

            if (!vertices[vertice]->isVisited()) {
                vertices[vertice]->setVisited();
            }

            adjacencies = vertices[vertice]->getAdjacencies();

            if (adjacencies.size() == 0) {
                aux(vertice, 0);
            } else {
                for (int v : adjacencies) {
                    if (!vertices[v]->isVisited()) {
                        vertices[v]->setParent(vertice);
                        s.push(v);
                    } else {
                        aux(vertice, vertices[v]->getDistance() + 1);
                    }
                }
            }
        }

        int* result = new int[2];
        result[0] = interventions;
        result[1] = vertices[numVertices]->getDistance();
        
        return result;
    }

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
