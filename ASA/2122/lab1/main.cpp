#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;


int output1(vector<> graph);
int hist1(vector<> graph);
int hist2(vector<> graph);
int output2(vector<> graph);


int main(int argc, char const *argv[]) 
{
    int n, m;
    vector<vector<int>> graph;

    scanf("%d, %d", &n, &m);

    if (n < 2 || m < 1) { exit(EXIT_FAILURE); }

    graph = vector<vector<int>>(vector<int>, n);

    int u, v;

    for (int i = 0; i < m; i++)
    {
        cin >> u >> v;
        graph[u - 1].push_back(v - 1);
    }

    output1(graph);
    output2(graph);

    return 0;
}


int output1(vector<> graph)
{
    hist1(graph);
    hist2(graph);
}

int hist1(vector<> graph)
{
    puts("Histograma 1");
}

int hist2(vector<> graph)
{
    puts("Histograma 2");
}


int output2(vector<> graph) 
{

}




