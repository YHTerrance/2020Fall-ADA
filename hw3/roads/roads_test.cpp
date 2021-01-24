#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

/*  --------------------------------------------------------------------------  */
/*                                  Reference                                   */
/*  --------------------------------------------------------------------------  */
// https://www.geeksforgeeks.org/kruskals-minimum-spanning-tree-using-stl-in-c/
// https://www.geeksforgeeks.org/c-program-for-dijkstras-shortest-path-algorithm-greedy-algo-7/

#define MAXN 300005
#define MAXM 300005

using namespace std;

typedef struct edge {
    int src;
    int dest;
    long long int weight;
}Edge;

// To represent disjoint sets
class DisjointSets {
    int parent[MAXN], rank[MAXN];
    int n;

    public:
    // Constructor
    DisjointSets(int n) {
        this->n = n;

        // initially, ranks of every set is 0, and their parent is themselves
        for(int i = 0; i < n; ++i) {
            rank[i] = 0;
            parent[i] = i;
        }
    }

    // Find the parent of node u + path compression
    int findParent(int u) {
        if(u != parent[u])
            parent[u] = findParent(parent[u]);
        return parent[u];
    }

    // Union by rank
    void merge(int x, int y) {
        if(x == y)
            return;

        if(rank[x] > rank[y])
            parent[y] = x;
        else
            parent[x] = y;

        if(rank[x] == rank[y])
            ++rank[y];
    }
};

class Graph {
    int V, E;
    vector<Edge> edges;
    long long int **adj;

    public:

        Graph(int V, int E);
        void addEdge(Edge e);
        void readEdges();
        void printEdges();
        void constructGraph();
        long long int findShortestPath(int src, int dest);
        // Utility function for find shortest path
        int minDistance(long long int dist[], bool processed[]);
};

// Constructor
Graph::Graph(int V, int E) {
    this->V = V;
    this->E = E;

    adj = new long long int*[V];
    for(int i = 0; i < V; ++i)
        adj[i] = new long long int[V];


    // Initialize array to -1
    for(int i = 0; i < V; ++i)
        for(int j = 0; j < V; ++j)
            adj[i][j] = -1;

}

// Function that adds edge to graph
void Graph::addEdge(Edge e) {
    #ifdef DEBUG
    cout << "Add edge: " << e.src + 1 << " " << e.dest + 1 << " " << e.weight << endl;
    #endif

    adj[e.dest][e.src] = adj[e.src][e.dest] = e.weight;
}

// Function that reads in the edges and stores them in a vector
void Graph::readEdges() {
    for(int i = 0; i < E; ++i) {
        Edge e;
        cin >> e.src >> e.dest >> e.weight;
        --e.src; --e.dest;
        edges.push_back(e);
    }
}

// Function that prints the edges
void Graph::printEdges() {

    cout << "Print graph edges: " << endl;
    for(int i = 0; i < V; ++i) {
        for(int j = i + 1; j < V; ++j) {
            if(adj[i][j] != -1)
                cout << i + 1 << " " << j + 1 << " " << adj[i][j] << endl;
        }
    }
}

// Sorted increasingly
bool edgeCompare(Edge a, Edge b) {
    return a.weight < b.weight;
}

void Graph::constructGraph() {
    // Sort edges in increasing order
    sort(edges.begin(), edges.end(), edgeCompare);

    DisjointSets ds(V);

    for(auto i = edges.begin(); i != edges.end();) {
        // Vector that stores all the edges selected for this specific weight
        vector<Edge> selected;
        long long int weight = i -> weight;

        while(i != edges.end() && weight == i -> weight) {
            int set_u = ds.findParent(i -> src);
            int set_v = ds.findParent(i -> dest);

            // They do not form a cycle
            if(set_u != set_v)
                selected.push_back(*i);
            ++i;
        }

        // Merge all selected vertices
        for(auto j = selected.begin(); j != selected.end(); ++j) {
            addEdge(*j);
            ds.merge(ds.findParent(j -> src), ds.findParent(j -> dest));
        }
    }
}

int Graph::minDistance(long long int dist[], bool processed[]) {
    long long int min_value = LLONG_MAX;
    int min;

    for(int i = 0; i < V; ++i) {
        if(processed[i] == false && dist[i] < min_value) {
            min_value = dist[i];
            min = i;
        }
    }
    return min;
}

long long int Graph::findShortestPath(int src, int dest) {

    // Stores the distance of different vertices from our src
    long long int dist[V];

    // Will be true if vertex i is selected in the shortest path
    bool processed[V];

    // Initialization
    for(int i = 0; i < V; ++i) {
        dist[i] = LLONG_MAX;
        processed[i] = false;
    }

    // Distance of source vertex is always 0
    dist[src] = 0;

    for(int i = 0; i < V - 1; ++i) {

        // Vertex u is the minimum distance vertex from the set of vertices not processed
        int u = minDistance(dist, processed);
        processed[u] = true;

        // Update values around u
        for(int v = 0; v < V; ++v) {
            if(!processed[v] && adj[u][v] != -1 && dist[u] != LLONG_MAX && dist[u] + adj[u][v] < dist[v]) {
                dist[v] = dist[u] + adj[u][v];
            }
        }
    }

    #ifdef DEBUG
    for(int i = 0; i < V; ++i)
        cout << dist[i] << " ";
    cout << endl;
    #endif

    return dist[dest];
}

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Read in number of vertices and edges
    int V, E, start, end;
    cin >> V >> E >> start >> end;


    Graph G(V, E);

    G.readEdges();

    G.constructGraph();

    #ifdef DEBUG
    G.printEdges();
    #endif

    cout << G.findShortestPath(start - 1, end - 1) << endl;

    return 0;
}
