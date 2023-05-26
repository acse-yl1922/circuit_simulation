/**
 * @file DirectedGraph.cpp
 * @author your name (you@domain.com)
 * @brief Directed graph implementation
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "DirectedGraph.hpp"

/**
 * @brief Construct a new Directed Graph:: Directed Graph object
 *
 * @param n Number of vertices
 */
DirectedGraph::DirectedGraph(int n)
{
    adjList.resize(n);
}

/**
 * @brief Add an edge to the graph
 *
 * @param u Source vertex
 * @param v Destination vertex
 */
void DirectedGraph::addEdge(int u, int v)
{
    adjList[u].push_back(v);
}

/**
 * @brief Get the number of vertices in the graph
 *
 * @return int Number of vertices
 */
int DirectedGraph::getNumVertices() const
{
    return adjList.size();
}

/**
 * @brief Get the number of edges in the graph
 *
 * @return int Number of edges
 */
int DirectedGraph::getNumEdges() const
{
    int numEdges = 0;
    for (int u = 0; u < adjList.size(); u++)
    {
        numEdges += adjList[u].size();
    }
    return numEdges;
}

/**
 * @brief Get the neighbors of a vertex
 *
 * @param u Vertex
 * @return std::vector<int> Neighbors of a vertex
 */
std::vector<int> DirectedGraph::getNeighbors(int u) const
{
    return adjList[u];
}

/**
 * @brief Overload the << operator
 *
 * @param os Output stream
 * @param G Directed graph
 * @return std::ostream& Output stream
 */
std::ostream &operator<<(std::ostream &os, const DirectedGraph &G)
{
    for (int u = 0; u < G.getNumVertices(); u++)
    {
        for (int v : G.getNeighbors(u))
        {
            os << u << " " << v << "\n";
        }
    }
    return os;
}

/**
 * @brief Write the graph information to a file
 *
 * @param filename Filename
 */
void DirectedGraph::writeToFile(std::string filename) const
{
    std::ofstream file;
    file.open(filename);
    file << *this;
    file.close();
}

/**
 * @brief Construct a new Depth First Search:: Depth First Search object
 *
 * @param G Directed graph
 * @param s Source vertex
 */
DepthFirstSearch::DepthFirstSearch(const DirectedGraph &G, int s)
{
    marked.resize(G.getNumVertices());
    dfs(G, s);
}

/**
 * @brief Perform depth first search
 *
 * @param G Directed graph
 * @param u Vertex
 */
void DepthFirstSearch::dfs(const DirectedGraph &G, int u)
{
    marked[u] = true;
    for (int v : G.getNeighbors(u))
    {
        if (!marked[v])
        {
            dfs(G, v);
        }
    }
}

/**
 * @brief Check if a vertex is visited
 *
 * @param v Vertex
 * @return true If the vertex is visited
 * @return false If the vertex is not visited
 */
bool DepthFirstSearch::visited(int v) const
{
    return marked[v];
}