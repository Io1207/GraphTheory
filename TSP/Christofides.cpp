//Not sure this version works- the person who last spent 3hrs straight trying to understand the error
#include<iostream>
#include<vector>
#include<cmath>
#include<map>
#include<algorithm>
#include<cstdlib>
#include<iterator>
#include<cassert>
#include<chrono> 

using namespace std;

struct Edge {
    int u, v;
    double weight;
};

double get_length(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

map<int, map<int, double>> build_graph(const std::vector<pair<int, int>>& data) {
    map<int, map<int, double>> graph;
    for (int this_point = 0; this_point < data.size(); this_point++) {
        for (int another_point = 0; another_point < data.size(); another_point++) {
            if (this_point != another_point) {
                graph[this_point][another_point] = get_length(data[this_point].first, data[this_point].second,
                                                              data[another_point].first, data[another_point].second);
            }
        }
    }
    return graph;
}

class UnionFind {
public:
    UnionFind() {}

    int find(int u) {
        if (parent.find(u) == parent.end()) {
            parent[u] = u;
            size[u] = 1;
            return u;
        }
        vector<int> path = {u};
        int root = parent[u];
        while (root != path.back()) {
            path.push_back(root);
            root = parent[root];
        }
        for (int ancestor : path) {
            parent[ancestor] = root;
        }
        return root;
    }

    void union_sets(int u, int v) {
        int root_u = find(u);
        int root_v = find(v);
        if (root_u != root_v) {
            if (size[root_u] >= size[root_v]) {
                parent[root_v] = root_u;
                size[root_u] += size[root_v];
            } else {
                parent[root_u] = root_v;
                size[root_v] += size[root_u];
            }
        }
    }

private:
    map<int, int> parent;
    map<int, int> size;
};

std::vector<Edge> minimum_spanning_tree(const map<int, map<int, double>>& G) {
    vector<Edge> tree;
    UnionFind uf;
    vector<Edge> edges;

    for (auto& u : G) {
        for (auto& v : u.second) {
            edges.push_back({u.first, v.first, v.second});
        }
    }

    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.weight < b.weight;
    });

    for (const Edge& edge : edges) {
        if (uf.find(edge.u) != uf.find(edge.v)) {
            tree.push_back(edge);
            uf.union_sets(edge.u, edge.v);
        }
    }

    return tree;
}

vector<int> find_odd_vertices(const vector<Edge>& MST) {
    map<int, int> degree_count;
    vector<int> odd_vertices;

    for (const Edge& edge : MST) {
        degree_count[edge.u]++;
        degree_count[edge.v]++;
    }

    for (const auto& v : degree_count) {
        if (v.second % 2 == 1) {
            odd_vertices.push_back(v.first);
        }
    }

    return odd_vertices;
}

// Perform Minimum Weight Matching on odd degree vertices
void minimum_weight_matching(vector<Edge>& MST, const map<int, map<int, double>>& G, vector<int>& odd_vertices) {
    random_shuffle(odd_vertices.begin(), odd_vertices.end());

    while (!odd_vertices.empty()) {
        int v = odd_vertices.back();
        odd_vertices.pop_back();
        double min_length = numeric_limits<double>::infinity();
        int closest = -1;

        for (int u : odd_vertices) {
            if (G.at(v).at(u) < min_length) {
                min_length = G.at(v).at(u);
                closest = u;
            }
        }

        MST.push_back({v, closest, min_length});
        odd_vertices.erase(remove(odd_vertices.begin(), odd_vertices.end(), closest), odd_vertices.end());
    }
}

void remove_edge_from_matchedMST(vector<Edge>& MatchedMST, int v1, int v2) {
    for (auto it = MatchedMST.begin(); it != MatchedMST.end(); ++it) {
        if ((it->u == v1 && it->v == v2) || (it->u == v2 && it->v == v1)) {
            MatchedMST.erase(it);
            break;
        }
    }
}

vector<int> find_eulerian_tour(vector<Edge>& MatchedMST) {
    map<int, vector<int>> neighbors;

    for (const Edge& edge : MatchedMST) {
        neighbors[edge.u].push_back(edge.v);
        neighbors[edge.v].push_back(edge.u);
    }

    vector<int> tour = {neighbors.begin()->first};

    while (!MatchedMST.empty()) {
        int v = tour.back();

        while (!neighbors[v].empty()) {
            int w = neighbors[v].back();
            remove_edge_from_matchedMST(MatchedMST, v, w);
            neighbors[v].pop_back();
            neighbors[w].erase(remove(neighbors[w].begin(), neighbors[w].end(), v), neighbors[w].end());
            tour.push_back(w);
            v = w;
        }
    }

    return tour;
}

// TSP SSolver Christofides
pair<double, vector<int>> tsp(const vector<pair<int, int>>& data) {
    auto G = build_graph(data);
    cout << "Graph built!" << endl;
    auto MST = minimum_spanning_tree(G);

    // Looking for odd degreed vertex
    auto odd_vertices = find_odd_vertices(MST);
    // cout << "Odd vertices found!" << endl;
    minimum_weight_matching(MST, G, odd_vertices);
    cout << "Minimum weight matching completed!" << endl;

    // finding eulerian path
    auto eulerian_tour = find_eulerian_tour(MST);
    cout << "Eulerian tour found!" << endl;

    // making the TSP path
    vector<bool> visited(data.size(), false);
    vector<int> path = {eulerian_tour[0]};
    visited[eulerian_tour[0]] = true;
    double length = 0;

    int current = eulerian_tour[0];
    for (int v : eulerian_tour) {
        if (!visited[v]) {
            path.push_back(v);
            visited[v] = true;
            length += G.at(current).at(v);
            current = v;
        }
    }

    //starting point
    length += G.at(current).at(eulerian_tour[0]);
    path.push_back(eulerian_tour[0]);

    return {length, path};
}

int main() {
    vector<pair<int, int>> data = [
        {1380, 939}, {2848, 96}, {3510, 1671}, {457, 334}, {3888, 666},
        {984, 965}, {2721, 1482}, {1286, 525}, {2716, 1432}, {738, 1325},
        // more points can be added
    ];

    auto [length, path] = tsp(data);
    cout << "Path length: " << length << endl;
    cout << "TSP path: ";
    for (int p : path) {
        cout << p << " ";
    }
    cout << endl;

    return 0;
}
