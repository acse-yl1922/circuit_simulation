#include "DirectedGraph.hpp"

int testDigraph()
{

    DirectedGraph G(5);
    G.addEdge(0, 1);
    G.addEdge(0, 2);
    G.addEdge(1, 2);
    G.addEdge(2, 0);
    G.addEdge(2, 3);
    G.addEdge(3, 3);

    int v = G.getNumVertices();
    int e = G.getNumEdges();
    std::vector<int> neighbors = G.getNeighbors(2);

    if (v != 5)
        return 1;
    if (e != 6)
        return 1;
    if (neighbors.size() != 2)
        return 1;
    if (neighbors[0] != 0)
        return 1;
    if (neighbors[1] != 3)
        return 1;

    return 0;
}

int testDFS()
{
    DirectedGraph G(5);
    G.addEdge(0, 1);
    G.addEdge(0, 2);
    G.addEdge(1, 2);
    G.addEdge(2, 0);
    G.addEdge(2, 3);
    G.addEdge(3, 3);

    DepthFirstSearch dfs(G, 2);
    dfs.dfs(G, 2);
    bool visited = dfs.visited(3);
    if (!visited)
        return 1;
    visited = dfs.visited(4);
    if (visited)
        return 1;
    visited = dfs.visited(2);
    if (!visited)
        return 1;
    return 0;
}

int main()
{
    int STATUS = 0;
    STATUS += testDigraph();
    STATUS += testDFS();
    return STATUS;
}