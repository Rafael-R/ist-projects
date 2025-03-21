#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;


vector<int> problem1(vector<int> list);
int problem2(vector<int> l1, vector<int> l2);


int main() {
    int n;

    cin >> n;
    vector<vector<int>> problems(n);

    string line;
    int number;
    for (int i = -1; i < n; i++) {
        getline(cin, line);
        istringstream iss(line);
        while (iss >> number) {
            problems[i].push_back(number);
        }
    }

    if (n == 1) {
        vector<int> result = problem1(problems[0]);
        cout << result[0] << ' ' << result[1] << endl;
    } else {
        cout << problem2(problems[0], problems[1]) << endl;
    }

    problems.clear();
    problems.shrink_to_fit();

    exit(EXIT_SUCCESS);
}


vector<int> problem1(vector<int> list) {
    int n = list.size();
    vector<int> len(n, 1), count(n, 1), result;

    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (list[i] > list[j]) {
                if (len[i] < len[j] + 1) {
                    len[i] = len[j] + 1;
                    count[i] = count[j];
                } else if (len[i] == len[j] + 1) {
                    count[i] += count[j];
                }
            }
        }
    }

    result.push_back(*max_element(len.begin(), len.end()));

    int aux = 0;
    for (int i = 0; i < n; i++) {
        if (len[i] == result[0]) {
            aux += count[i];
        }
    }

    result.push_back(aux);

    return result;
}

int problem2(vector<int> l1, vector<int> l2) {
    int n = l2.size();
    vector<int> aux(n, 0);

    int current;
    for (int i : l1) {
        current = 0;
        for (int j = 0; j < n; j++) {
            if (i == l2[j]) {
                if (current + 1 > aux[j]) {
                    aux[j] = current + 1;
                }
            }
            if (i > l2[j]) {
                if (aux[j] > current) {
                    current = aux[j];
                }
            }
        }
    }

    return *max_element(aux.begin(), aux.end());
}


