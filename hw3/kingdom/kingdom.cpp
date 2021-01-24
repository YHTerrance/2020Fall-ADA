#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <cstdlib>
#include <cstring>

/* -------------------------------------------------------------------------- */
/*                                  Reference                                 */
/* -------------------------------------------------------------------------- */
/*         https://www.geeksforgeeks.org/longest-path-undirected-tree/        */

#define MAXN 500005

using namespace std;

typedef struct edge {
    int dest;
    long long int weight;
} Edge;

// Records which vertices have been visited
bool visited[MAXN];
// Records the distance of different vertices from the specifiied source
long long int dist[MAXN];

class Graph {
    // Number of vertices
    int V;
    // Adjacency list
    vector<Edge> *adj;
    // Longest path in the tree
    vector<int> longestPath;
    vector<int> tmp;

    // Stores the weights of different paths
    priority_queue<long long int, vector<long long int>> PQ;

    public:
        // Constructor
        Graph(int V);
        // Function to add an edge to the graph
        void addEdge(int v, int w, long long int weight);
        // Function that extracts paths according to their weights from the graph and returns its total weight
        void extractPaths();
        // Find path
        long long int findLongestPath(int src);
        // Find farthest city from specified source
        pair<int, long long int> farthestCity(int src);
        // Utility function that prints all the edges of the graph
        void printEdges();

};

Graph::Graph(int V) {
    this->V = V;
    adj = new vector<Edge>[V];
}

// Undirected graph add edge
void Graph::addEdge(int v, int w, long long int weight) {
    Edge e_v, e_w;
    e_v.dest = v;
    e_w.dest = w;
    e_v.weight = e_w.weight = weight;
    adj[w].push_back(e_v);
    adj[v].push_back(e_w);
}

void Graph::printEdges() {
    cout << "---------- Edges of Graph ----------" << endl;

    for(int v = 0; v < V; ++v) {
        for(auto i = adj[v].begin(); i != adj[v].end(); ++i) {
            cout << v + 1 << " " << i -> dest + 1 << " " << i -> weight << endl;

        }
    }
    cout << "------------------------------------" << endl;
    return;
}

// Returns the farthest city from specified source
pair<int, long long int> Graph::farthestCity(int src) {
    // Initialize all distances to -1
    memset(dist, -1, sizeof(long long int) * V);

    stack<int, vector<int>> q;
    q.push(src);

    // distance from source equals 0
    dist[src] = 0;

    while(!q.empty()) {
        int t = q.top();
        q.pop();

        // loop for all adjacent vertices of vertex t
        for(auto i = adj[t].begin(); i != adj[t].end(); ++i) {
            int v = i -> dest;

            // Push it into stack if it is not visited yet
            if(dist[v] == -1) {
                q.push(v);
                // Update distance value
                dist[v] = dist[t] + i -> weight;
            }
        }
    }

    long long int maxDis = -1;
    int farthestCity;

    for(int i = 0; i < V; ++i) {
        if(dist[i] > maxDis) {
            maxDis = dist[i];
            farthestCity = i;
        }
    }
    return make_pair(farthestCity, maxDis);
}

void Graph::extractPaths() {

    pair<int, long long int> startCity;

    // Get two ends of the longest path
    startCity = farthestCity(0);

    #ifdef DEBUG
    cout << "start city: " << startCity.first + 1 << endl;
    #endif

    for(int i = 0; i < V; ++i)
        visited[i] = false;

    long long int totalWeight;

    cout << 0 << " ";

    // Find longest path
    totalWeight = findLongestPath(startCity.first);

    cout << totalWeight << " ";

    for(int i = 3; i <= V; ++i) {
        if(!PQ.empty()) {
            totalWeight += PQ.top();
            PQ.pop();
        }
        cout << totalWeight << " ";
    }
}

long long int Graph::findLongestPath(int src) {

    visited[src] = true;

    long long int weight, maxWeight = 0;

    for(auto i = adj[src].begin(); i != adj[src].end(); ++i) {
        if(!visited[i -> dest]) {

            weight = findLongestPath(i -> dest) + i -> weight;

            if(weight > maxWeight) {
                if(maxWeight != 0)
                    PQ.push(maxWeight);
                maxWeight = weight;
            }
            else
                PQ.push(weight);
        }
    }
    return maxWeight;
}

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Number of cities
    int n;
    cin >> n;

    Graph G(n);

    // Read in all edges
    int u, v;
    long long int weight;
    for(int i = 0; i < n - 1; ++i) {
        cin >> u >> v >> weight;
        G.addEdge(u - 1, v - 1, weight);
    }

    #ifdef DEBUG
    G.printEdges();
    #endif

    G.extractPaths();

    return 0;
}


