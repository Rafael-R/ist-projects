#include <algorithm>
#include <iostream>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

void print_vec(std::string name, std::vector<int>& vec) {
	std::cout << name << ": ";
	for (auto e : vec) {
		std::cout << e << " ";
	}
	std::cout << std::endl;
}

class Graph {
   private:
	int n;
	std::vector<std::vector<int>> adjOut;
	std::vector<std::vector<int>> adjIn;

   public:
	Graph(int n) {
		this->n = n;
		for (int i = 0; i < n; i++)	{
			adjOut.push_back(std::vector<int>());
			adjIn.push_back(std::vector<int>());
		}
	}

	void addEdge(int x, int y) {
		adjOut[x - 1].push_back(y - 1);
		adjIn[y - 1].push_back(x - 1);
	}

	void detectCyclesAux(int vertex, std::vector<int>& visited,
						 std::vector<int>& path,
						 std::vector<std::unordered_set<int>>& cycles) {
		auto it = find(path.begin(), path.end(), vertex);
		if (it != path.end()) {
			int index = it - path.begin();
			cycles.emplace_back(path.begin() + index, path.end());
			return;
		}

		if (!visited[vertex]) {
			visited[vertex] = 1;
			path.push_back(vertex);
			for (int adjacency : adjOut[vertex]) {
				detectCyclesAux(adjacency, visited, path, cycles);
			}
			path.pop_back();
		}
	}

	std::vector<std::unordered_set<int>> detectCycles() {
		std::vector<int> visited(n, 0);
		std::vector<int> path;
		std::vector<std::unordered_set<int>> cycles;

		for (int i = 0; i < n; i++) {
			if (!visited[i]) {
				detectCyclesAux(i, visited, path, cycles);
			}
		}

		return cycles;
	}

	void removeCycles() {
		std::vector<std::unordered_set<int>> cycles = detectCycles();
		std::set<int> toDelete;

		for (auto cycle : cycles) {
			adjOut.push_back(std::vector<int>());
			adjIn.push_back(std::vector<int>());
			for (int vertex : cycle) {
				for (size_t i = 0; i < adjOut[vertex].size(); i++) {
					if (cycle.find(adjOut[vertex][i]) == cycle.end()) {
						for (size_t j = 0; j < adjIn[adjOut[vertex][i]].size(); j++) {
							if (adjIn[adjOut[vertex][i]][j] == vertex) {
								adjIn[adjOut[vertex][i]][j] = n;
								break; 
							}
						}
						adjOut[n].push_back(adjOut[vertex][i]);
					}
				}
				for (size_t i = 0; i < adjIn[vertex].size(); i++) {
					if (cycle.find(adjIn[vertex][i]) == cycle.end()) {
						for (size_t j = 0; j < adjOut[adjIn[vertex][i]].size(); j++) {
							if (adjOut[adjIn[vertex][i]][j] == vertex) {
								adjOut[adjIn[vertex][i]][j] = n;
								break; 
							}
						}
						adjIn[n].push_back(adjIn[vertex][i]);
					}
				}
				toDelete.insert(vertex);
			}
			++n;
		}

		std::set<int>::reverse_iterator rit; 
		for (rit = toDelete.rbegin(); rit != toDelete.rend(); rit++) {
			adjOut[*rit].clear();
			adjIn[*rit].clear();
		}
	}

	int longestPathAux(int v, std::vector<int> dp) {
		if (dp[v] != -1) return dp[v];

		int temp = -1;
		for (int adjacency : adjOut[v]) {
			temp = std::max(temp, longestPathAux(adjacency, dp));
		}
		dp[v] = std::max(dp[v], 1 + temp);

		return dp[v];
	}

	int longestPath() {
		std::vector<int> dp = std::vector<int>(n, -1);

		int max = 0;
		for (int i = 0; i < n; i++) {
			int temp = longestPathAux(i, dp);
			max = (temp > max) ? temp : max;
		}
		return max;
	}

	void free() {
		for (int i = 0; i < n; i++) {
			adjOut[i].clear();
			adjIn[i].clear();
		}
		adjOut.clear();
		adjIn.clear();
	}
};

int main() {
	int n, m;
	scanf("%d %d", &n, &m);

	if (n < 2 || m < 0) {
		return 1;
	}

	Graph graph(n);

	int x, y;
	for (; m > 0; m--) {
		scanf("%d %d", &x, &y);
		graph.addEdge(x, y);
	}

	graph.removeCycles();

	std::cout << graph.longestPath() << std::endl;

	graph.free();

	return 0;
}