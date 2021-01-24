#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <queue>
#include <cassert>

/*  --------------------------------------------------------------------------  */
/*                                  Reference                                   */
/*  --------------------------------------------------------------------------  */
// https://www.geeksforgeeks.org/kruskals-minimum-spanning-tree-using-stl-in-c/
// https://www.geeksforgeeks.org/c-program-for-dijkstras-shortest-path-algorithm-greedy-algo-7/

#define MAXN 300005
#define MAXM 300005

using namespace std;

typedef pair<long long int, int> pii;

typedef struct edge {
    int src;
    int dest;
    long long int weight;
}Edge;

// To represent disjoint sets
class DisjointSets {
    int n;
    int parent[MAXN], height[MAXN] = {};

    public:
    // Constructor
    DisjointSets(int n) {
        this->n = n;

        // initially, heights of every set is 0, and their parent is themselves
        for(int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }

    // Find the parent of node u + path compression
    int findParent(int u) {
        if(u != parent[u])
            parent[u] = findParent(parent[u]);
        return parent[u];
    }

    // Union by height
    void merge(int x, int y) {
        if(x == y)
            return;

        if(height[x] > height[y])
            parent[y] = x;
        else
            parent[x] = y;

        if(height[x] == height[y])
            ++height[y];
    }
};

class Graph {
    int V, E;
    vector<Edge> edges;
    vector<Edge> adj[MAXN];

    public:

        Graph(int V, int E);
        void addEdge(Edge e);
        void readEdges();
        void printEdges();
        void constructGraph();
        long long int findShortestPath(int src, int dest);
};

// Constructor
Graph::Graph(int V, int E) {
    this->V = V;
    this->E = E;
}

// Function that adds edge to graph
void Graph::addEdge(Edge e) {
    #ifdef DEBUG
    cout << "Add edge: " << e.src + 1 << " " << e.dest + 1 << " " << e.weight << endl;
    #endif

    adj[e.src].push_back(e);
    swap(e.src, e.dest);
    adj[e.src].push_back(e);

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
        for(auto j : adj[i]) {
            cout << j.src + 1 << " " << j.dest + 1 << " " << j.weight << endl;
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
        for(auto j : selected) {
            addEdge(j);
            ds.merge(ds.findParent(j.src), ds.findParent(j.dest));
        }
    }
}

long long int Graph::findShortestPath(int src, int dest) {

    // Stores the distance of different vertices from our src
    long long int dist[V];

    for(int i = 0; i < V; ++i)
        dist[i] = LLONG_MAX;

    priority_queue< pii, vector<pii>, greater<pii> > pq;

    pq.push(make_pair(0, src));
    dist[src] = 0;

    while(!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        #ifdef DEBUG
        cout << "Processing " << u << endl;
        #endif

        for(auto i : adj[u]) {
            int v = i.dest;
            long long int weight = i.weight;

            if(dist[v] > dist[u] + weight) {
                dist[v] = dist[u] + weight;
                pq.push(make_pair(dist[v], v));

                #ifdef DEBUG
                for(int i = 0; i < V; ++i) {
                    if(dist[i] == LLONG_MAX)
                        cout << "inf ";
                    else
                        cout << dist[i] << " ";
                }
                cout << endl;
                #endif
            }
        }
    }
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
