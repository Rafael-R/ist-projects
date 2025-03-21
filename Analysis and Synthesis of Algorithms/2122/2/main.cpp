#include <iostream>
#include <vector>
using namespace std;


void printVector(vector<int> v) {
    for (auto i : v) {
        cout << i << ' ';
    } cout << endl;
}


enum colors {WHITE, GREY, BLACK};

class Graph
{
private:
    int numVertices;
    int** vertices;

    void checkCycles(vector<int> &count) {
        vector<colors> color(numVertices, WHITE);
        for (int i = 0; i < numVertices; i++) {
            if (color[i] == WHITE) {
                dfs_visit(count, 0, color, i); 
            }
        }
    }

    void dfs(vector<int> &count, int s) {
        vector<colors> color(numVertices, WHITE);
        dfs_visit(count, 1, color, s);
    }

    void dfs_visit(vector<int> &count, int flag, vector<colors> &color, int v) {
        color[v] = GREY;
        for (int i = 0; i < vertices[v][2]; i++) {
            int current = vertices[v][i];
            if (color[current] == WHITE) {
                dfs_visit(count, flag, color, current);
            } else if (color[current] == GREY) {
                vector<colors>().swap(color);
                this->clean();
                cout << "0" << endl;
                exit(EXIT_SUCCESS);
            }
        }
        color[v] = BLACK;
        if (flag) { count[v]++; }
    }
public: 
    Graph(int numVertices) {
        this->numVertices = numVertices;
        vertices = new int*[numVertices];
        for (int i = 0; i < numVertices; i++) {
            vertices[i] = new int[3];
            for (int j = 0; j < 3; j++) {
                vertices[i][j] = 0;
            }
        }
    }

    void addEdge(int x, int y) {
        int index = vertices[y - 1][2];
        if (index < 2) {
            vertices[y - 1][index] = x - 1;
            vertices[y - 1][2]++;
            return;
        } else {
            this->clean();
            cout << "0" << endl;
            exit(EXIT_SUCCESS);
        }
    }

    vector<int> solve(int v1, int v2) {
        vector<int> count(numVertices, 0);

        checkCycles(count);
        dfs(count, v1 - 1);
        dfs(count, v2 - 1);

        for (int i = 0; i < numVertices; i++) {
            if (count[i] == 2) {
                dfs(count, i);
            }
        }

        vector<int> ancestors;
        for (int i = 0; i < numVertices; i++) {
            if (count[i] == 3) {
                ancestors.push_back(i);
            }
        }

        return ancestors;
    }

    void clean() {
        for(int i = 0; i < numVertices; ++i) { delete[] vertices[i]; }
        delete[] vertices;
    }
};


int main() {
    int v1, v2, n, m;

    cin >> v1 >> v2;
    cin >> n >> m;

    if (n < 1 || m < 0) { exit(EXIT_FAILURE); }

    Graph g(n);

    int x, y;
    for (; m > 0; m--) {
        cin >> x >> y;
        g.addEdge(x, y);
    }

    auto result = g.solve(v1, v2);

    if (result.size() == 0) {
        cout << "-" << endl;
    } else {
        for (auto i : result) {
            cout << i + 1 << ' ';
        } cout << endl;
    }
    
    vector<int>().swap(result);
    g.clean();

    exit(EXIT_SUCCESS);
}