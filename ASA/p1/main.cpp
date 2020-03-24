#include <iostream>
#include <vector>
using namespace std;

class Vertice
{
private:
    int classification;
    vector<Vertice *> adjacencies;
    bool visited = false;

public:
    Vertice(int classification)
    {
        this->classification = classification;
    }
    void setClassification(int classification)
    {
        this->classification = classification;
    }
    void addAdjacencies(Vertice *vertice)
    {
        adjacencies.push_back(vertice);
    }
    void setVisited()
    {
        this->visited = true;
    }
    int getClassification()
    {
        return classification;
    }
    vector<Vertice *> getAdjacencies()
    {
        return adjacencies;
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
    void swap(vector<int> &vector, int x, int y)
    {
        int temp = vector[x];
        vector[x] = vector[y];
        vector[y] = temp;
    }
    int partition(vector<int> &v1, vector<int> &v2, int left, int right)
    {
        int pivot = v1[right];
        int leftPointer = left - 1;
        int rightPointer = right;
        for (;;)
        {
            while (v1[++leftPointer] > pivot)
                ;
            while (rightPointer > 0 && v1[--rightPointer] < pivot)
                ;
            if (leftPointer >= rightPointer)
            {
                break;
            }
            else
            {
                swap(v1, leftPointer, rightPointer);
                swap(v2, leftPointer, rightPointer);
            }
        }
        swap(v1, leftPointer, right);
        swap(v2, leftPointer, right);

        return leftPointer;
    }
    void quicksort(vector<int> &v1, vector<int> &v2, int left, int right)
    {
        if (left < right)
        {
            int pivotIndex = partition(v1, v2, left, right);
            quicksort(v1, v2, left, pivotIndex - 1);
            quicksort(v1, v2, pivotIndex + 1, right);
        }
    }
    void calculateAux(Vertice *vertice)
    {
        vertice->setVisited();
        for (Vertice *adjacent : vertice->getAdjacencies())
        {
            if (adjacent->getClassification() < vertice->getClassification())
            {
                adjacent->setClassification(vertice->getClassification());
            }
            if (!adjacent->isVisited())
            {
                calculateAux(adjacent);
            }
        }
    }

public:
    int getClassification(int n)
    {
        return vertices[n - 1]->getClassification();
    }
    vector<Vertice *> getAdjacencies(int n)
    {
        return vertices[n - 1]->getAdjacencies();
    }
    void addVertice(int classification)
    {
        vertices.push_back(new Vertice(classification));
    }
    void addEdge(int x, int y)
    {
        vertices[x - 1]->addAdjacencies(vertices[y - 1]);
    }
    void calculateClassifications()
    {
        vector<int> sortedVertices;
        vector<int> classifications;

        int v = vertices.size();
        for (int i = 0; i < v; i++)
        {
            sortedVertices.push_back(i);
            classifications.push_back(vertices[i]->getClassification());
        }
        quicksort(classifications, sortedVertices, 0, v - 1);

        for (int n : sortedVertices)
        {
            if (!vertices[n]->isVisited())
            {
                calculateAux(vertices[n]);
            }
        }

        vector<int>().swap(classifications);
        vector<int>().swap(sortedVertices);
    }
    void free()
    {
        for (Vertice *vertice : vertices)
        {
            delete vertice;
        }
        vector<Vertice *>().swap(vertices);
    }
};

int main()
{
    int n, m;

    scanf("%d, %d", &n, &m);

    if (n < 2 || m < 1)
    {
        exit(EXIT_FAILURE);
    }

    Graph g;

    int classification;
    for (int i = 1; i <= n; i++)
    {
        cin >> classification;
        g.addVertice(classification);
    }

    int u, v;
    for (int i = 1; i <= m; i++)
    {
        cin >> u;
        cin >> v;
        g.addEdge(v, u);
    }

    g.calculateClassifications();

    for (int i = 1; i <= n; i++)
    {
        cout << g.getClassification(i) << endl;
    }

    g.free();

    exit(EXIT_SUCCESS);
}