#include <iostream>
#include <vector>
using namespace std;


enum colors {WHITE, GREY, BLACK};

void dfs_visit(int graph[][3], vector<int> count, int flag, vector<colors> color, int v) {
    color[v] = GREY;
    for (int i = 0; i < graph[v][2]; i++) {
        int current = graph[v][i];
        if (color[current] == WHITE) {
            dfs_visit(graph, count, flag, color, current);
        } else if (color[current] == GREY) {
            cout << "0" << endl;
            exit(EXIT_SUCCESS);
        }
    }
    color[v] = BLACK;
    if (flag) { count[v]++; }
}

void dfs(int graph[][3], int n, vector<int> count, int s) {
    vector<colors> color(n, WHITE);
    dfs_visit(graph, count, 1, color, s);
}

void checkCycles(int graph[][3], int n, vector<int> count) {
    vector<colors> color(n, WHITE);
    for (int i = 0; i < n; i++) {
        if (color[i] == WHITE) { 
            dfs_visit(graph, count, 0, color, i); 
        }
    }
}

vector<int> solve(int graph[][3], int n, int v1, int v2) {
    vector<int> count(n, 0);

    checkCycles(graph, n, count);

    dfs(graph, n, count, v1 - 1);
    dfs(graph, n, count, v2 - 1);

    for (int i = 0; i < n; i++) {
        if (count[i] == 3) {
            dfs(graph, n, count, i);
        }
    }

    vector<int> ancestors;
    for (int i = 0; i < n; i++) {
        if (count[i] == 4) {
            ancestors.push_back(i);
        }
    }

    return ancestors;
}


int main() {
    int v1, v2, n, m;

    cin >> v1 >> v2;
    cin >> n >> m;

    if (n < 1 || m < 0) { exit(EXIT_FAILURE); }

    int graph[n][3] = {0};

    int x, y, index;
    for (; m > 0; m--) {
        cin >> x >> y;
        index = graph[y - 1][2];
        if (index < 2) {
            graph[y - 1][index] = x - 1;
            graph[y - 1][2]++;
        } else {
            cout << "0" << endl;
            exit(EXIT_SUCCESS);
        }
    }

    auto result = solve(graph, n, v1, v2);

    if (result.size() == 0) {
        cout << "-" << endl;
    } else {
        for (auto i : result) {
            cout << i + 1 << ' ';
        } cout << endl;
    }
    
    vector<int>().swap(result);

    exit(EXIT_SUCCESS);
}