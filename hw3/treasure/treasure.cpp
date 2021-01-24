#include <iostream>
#include <list>
#include <stack>
#include <vector>
#include <cassert>

using namespace std;

#define MAXN 500005
#define MAXM 500005

/* -------------------------------------------------------------------------- */
/*                                  Reference                                 */
/* -------------------------------------------------------------------------- */
/*         https://www.geeksforgeeks.org/strongly-connected-components/       */

int map[MAXN];

// Treasures in different cities
long long int cityTreasure[MAXN];

// Component treasure
long long int componentTreasure[MAXN] = {};

// max treasures ending at different nodes
long long int maxTreasure[MAXN];

typedef struct edge {
    // Destination vertice
    int dest;
    long long int treasure;
} Edge;

class Graph {

    // Number of vertices
    int V;

    bool visited[MAXN];

    // An array of adjacency lists
    vector<Edge> adj[MAXN];

    // Fill stack with vertices(in increasing order of finishing times). The top element has the maximum finishing time
    void fillStack(int v, bool visited[], stack<int, vector<int>> &Stack);

    // A recursive function to print DFS starting from v
    void tryDFS(int v, bool visited[], vector<int> &SSC);

    public:
        Graph(int V);

        void addVertex(int v, long long int treasure);

        void addEdge(int v, int w, long long int treasure);

        // Find and print strongly connected components
        void printSCCs(vector<vector<int>> &SCCs);

        // Function that returns reverse(or transpose) of this graph
        Graph getTranspose();

        // Function that returns the component graph of this graph
        Graph getComponent();

        // Function that returns the number of vertices of the graph
        int numOfVertices() { return V; }

        // Utility function that prints all the edges of the graph
        void printEdges();

        // Function that returns the max treaasures achievable (Note: can only be used with a DAG)
        long long int getMaxTreasure();
};

Graph::Graph(int V) {
    this->V = V;
}

// A recursive function to print DFS starting from v
void Graph::tryDFS(int v, bool visited[], vector<int> &SSC) {
    // Mark the current node as visited and print it
    visited[v] = true;
    SSC.push_back(v);

    // Recur for all the vertices adjacent to this vertex
    for(auto i = adj[v].begin(); i != adj[v].end(); ++i)
        if(!visited[i -> dest])
            tryDFS(i -> dest, visited, SSC);
}

Graph Graph::getTranspose() {
    Graph g(V);
    for(int v = 0; v < V; ++v) {
        // Reverse all the edges and append to new graph
        for(auto i = adj[v].begin(); i != adj[v].end(); ++i) {
            Edge e;
            e.dest = v;
            g.adj[i -> dest].push_back(e);
        }
    }
    return g;
}

void Graph::addEdge(int v, int w, long long int treasure) {
    // Add w to v's list
    Edge e;
    e.dest = w;
    e.treasure = treasure;
    adj[v].push_back(e);
}

void Graph::addVertex(int v, long long int treasure) {
    cityTreasure[v] = treasure;
}

void Graph::fillStack(int v, bool visited[], stack<int, vector<int>> &Stack) {
    // Mark current node as visited
    visited[v] = true;

    for(auto i = adj[v].begin(); i != adj[v].end(); ++i) {
        if(!visited[i -> dest])
            fillStack(i -> dest, visited, Stack);
    }

    // All vertices reachable have been processed, push current node into stack
    Stack.push(v);
}

// Find and print strongly connected components
void Graph::printSCCs(vector<vector<int>> &SCCs) {
    stack<int, vector<int>> Stack;

    // Mark all vertices as not visited (For first DFS)
    for(int i = 0; i < V; ++i)
        visited[i] = false;

    // Fill vertices in stack according to their finishing time
    for(int i = 0; i < V; ++i)
        if(visited[i] == false)
            fillStack(i, visited, Stack);

    // Create a transpose graph
    Graph gT = getTranspose();

    // Mark all vertices as not visited (For second DFS)
    for(int i = 0; i < V; ++i)
        visited[i] = false;

    #ifdef DEBUG
    cout << "The following are SCCs" << endl;
    #endif
    // Process all vertices according to their finishing time in decreasing order
    while(!Stack.empty()) {
        // Pop a vertex
        int v = Stack.top();
        Stack.pop();

        // Print strongly connected components of the popped vertex
        if(!visited[v]) {
            vector<int> SCC;
            gT.tryDFS(v, visited, SCC);
            SCCs.push_back(SCC);
        }
    }
    #ifdef DEBUG
    cout << "---------- SCCs ----------" << endl;

    for(auto i = SCCs.begin(); i != SCCs.end(); ++i) {
        for(auto j = i -> begin(); j != i -> end(); ++j) {
            cout << *j + 1 << " ";
        }
        cout << endl;
    }
    cout << "--------------------------" << endl;

    #endif
}

Graph Graph::getComponent() {
    vector<vector<int>> SCCs;
    // Store SCC vertices in 2D vector
    printSCCs(SCCs);

    Graph gC(SCCs.size());

    // Map old vertice numbers to new vertice numbers
    int cnt = 0;
    for(auto i = SCCs.begin(); i != SCCs.end(); ++i) {
        for(auto j = i -> begin(); j != i -> end(); ++j)
            map[*j] = cnt;
        ++cnt;
    }
    assert(cnt == SCCs.size());
    cnt = 0;
    for(auto i = SCCs.begin(); i != SCCs.end(); ++i) {
        for(auto j = i -> begin(); j != i -> end(); ++j) {
            // Add city treasure to component treasure
            componentTreasure[cnt] += cityTreasure[*j];

            // Check all edges of the city
            for(auto k = adj[*j].begin(); k != adj[*j].end(); ++k) {
                // If they are in the same component
                if(map[k -> dest] == cnt) {
                    componentTreasure[cnt] += k -> treasure;
                }
                // If they are not in the same component, set up new edge to new vertex
                else {
                    Edge e;
                    e.dest = map[k -> dest];
                    e.treasure = k -> treasure;
                    gC.adj[cnt].push_back(e);
                }
            }
        }
        ++cnt;
    }
    assert(cnt == SCCs.size());


    return gC;
}

// Utility function that prints all the edges
void Graph::printEdges() {

    cout << "---------- Edges of Graph ----------" << endl;

    for(int v = 0; v < V; ++v) {
        for(auto i = adj[v].begin(); i != adj[v].end(); ++i) {
            cout << v + 1 << " " << i -> dest + 1 << " " << i -> treasure << endl;

        }
    }
    cout << "------------------------------------" << endl;
    return;
}

long long int Graph::getMaxTreasure() {
    stack<int, vector<int>> originStack;
    vector<int> Stack;
    // Mark all vertices as not visited (For first DFS)
    for(int i = 0; i < V; ++i)
        visited[i] = false;

    // Fill vertices in stack according to their finishing time
    for(int i = 0; i < V; ++i)
        if(visited[i] == false)
            fillStack(i, visited, originStack);

    // Copy everything to a vector
    while(!originStack.empty()) {
        Stack.push_back(originStack.top());
        originStack.pop();
    }

    long long int max = 0;


    for(int i = 0; i < V; ++i)
        maxTreasure[i] = componentTreasure[i];

    #ifdef DEBUG
    // Process all vertices according to their finishing time in decreasing order
    for(auto i = Stack.begin(); i != Stack.end(); ++i) {
        cout << *i << " ";
    }
    cout << endl;
    #endif

    for(auto i = Stack.begin(); i != Stack.end(); ++i) {
        // Pop a vertex
        int v = *i;
        for(auto i = adj[v].begin(); i != adj[v].end(); ++i) {
            long long int newTreasure = maxTreasure[v] + i -> treasure + componentTreasure[i -> dest];

            if(maxTreasure[i -> dest] < newTreasure) {
                maxTreasure[i -> dest] = newTreasure;
            }
        }
    }

    for(int i = 0; i < V; ++i)
        if(maxTreasure[i] > max)
            max = maxTreasure[i];


    return max;
}

int main() {

    int N, M;
    cin >> N >> M;
    // initialize graph with N vertices
    Graph G(N);
    // Read in treasures of cities
    for(int i = 0; i < N; ++i) {
        long long int treasure;
        cin >> treasure;
        G.addVertex(i, treasure);
    }
    // Read in treasures of edges
    for(int i = 0; i < M; ++i) {
        int v, dest;
        long long int treasure;
        cin >> v >> dest >> treasure;
        G.addEdge(v - 1, dest - 1, treasure);
    }

    Graph GC = G.getComponent();

    cout << GC.getMaxTreasure() << endl;

    return 0;
}
