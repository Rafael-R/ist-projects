#include <iostream>
#include <vector>
#include <queue>
using namespace std;


class Vertice 
{
private:
    vector<int> adjacencies;
    bool visited = false;

public:
    void addAdjacencie(int vertice)
    {
        adjacencies.push_back(vertice);
    }

    vector<int> getAdjacencies()
    {
        return adjacencies;
    }

    void setVisited()
    {
        visited = true;
    }

    void setUnvisited()
    {
        visited = false;
    }

    bool isVisited()
    {
        return visited;
    }
};

class Graph
{
private:
    vector<Vertice *> vertices;
    int* incomingEdges;

public:
    Graph(int numVertices)
    {
        incomingEdges = new int[numVertices];
        for (int i = 0; i < numVertices; i++)
        {
            vertices.push_back(new Vertice());
            incomingEdges[i] = 0;
        }
    }

    void addEdge(int x, int y)
    {
        vertices[x - 1]->addAdjacencie(y - 1);
        incomingEdges[y - 1]++;
    }

    int calculate()
    {
        int numVertices = vertices.size();

        int interventions = 0, greaterSequence = 0, sequence;
        
        for (int i = 0; i < numVertices; i++)
        {
            queue<int> q;
            int vertice = i;

            if (incomingEdges[vertice] == 0)
            {
                for (int j = 0; j < numVertices; j++)
                {
                    vertices[j]->setUnvisited();
                }

                interventions++;
                sequence = 0;

                vertices[vertice]->setVisited();

                q.push(vertice);

                while (!q.empty())
                {
                    vertice = q.front();
                    q.pop();

                    for (int u : vertices[vertice]->getAdjacencies())
                    {
                        if (!vertices[u]->isVisited())
                        {
                            vertices[u]->setVisited();
                            q.push(u);
                            sequence++;
                        }
                    }
                }

                printf("%d: %d\n", i + 1, sequence);
                
                greaterSequence = (sequence > greaterSequence) ? sequence : greaterSequence;
            }
        }
        
        return interventions * 10 + greaterSequence;
    }
    
    void free()
    {
        for(Vertice* vertice : vertices)
        {
            delete vertice;
        }
        vertices.clear();
        delete[] incomingEdges;
    }
};


int main()
{
   int n, m;

    cin >> n >> m;

    if (n < 2 || m < 0)
    {
        exit(EXIT_FAILURE);
    }

    Graph g(n);

    int x, y;

    for (int i = 0; i < m; i++)
    {
        cin >> x >> y;

        g.addEdge(x, y);
    }
    
    int result = g.calculate();

    cout << result / 10 << ' ' << result % 10 << endl;

    g.free();

    return 0;
}
