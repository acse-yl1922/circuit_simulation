#pragma once

/**
 * @file DirectedGraph.hpp
 * @author acse-sm222
 * @brief Directed graph class
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <iostream>
#include <vector>
#include <fstream>

/**
 * @class DirectedGraph
 * @brief A directed graph.
 * @author acse-sm222
 *
 */
class DirectedGraph
{
public:
    DirectedGraph(){};                                                         /**< Default constructor*/
    DirectedGraph(int n);                                                      /**< Constructor taking the number of vertices*/
    void addEdge(int u, int v);                                                /**< Function to add an edge*/
    int getNumVertices() const;                                                /**< Function to get the number of vertices*/
    int getNumEdges() const;                                                   /**< Function to get the number of edges*/
    std::vector<int> getNeighbors(int u) const;                                /**< Function to get the neighbors of a vertex*/
    friend std::ostream &operator<<(std::ostream &os, const DirectedGraph &G); /**< Overload the << operator*/
    void writeToFile(std::string filename) const;                              /**< Function to write the graph information to a file*/

private:
    std::vector<std::vector<int>> adjList; /**< Adjacency list*/
};

/**
 * @class DepthFirstSearch
 * @brief Depth first search class for directed graph
 * @author acse-sm222
 *
 */
class DepthFirstSearch
{
public:
    DepthFirstSearch(){};                            /**< Default constructor*/
    DepthFirstSearch(const DirectedGraph &G, int s); /**< Constructor taking the graph and the source vertex*/
    void dfs(const DirectedGraph &G, int u);         /**< Function to perform depth first search*/
    bool visited(int v) const;                       /**< Function to check if a vertex is visited*/

private:
    std::vector<bool> marked; /**< Vector to store the visited vertices*/
};