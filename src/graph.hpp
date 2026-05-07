// graph.hpp - Tomás Noreña y Miguel Muñoz
#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <utility>
using namespace std;

struct Grafo {
    int numNodos;
    int numAristas;
    vector<vector<pair<int,int>>> adj;

    Grafo(int n) : numNodos(n), numAristas(0), adj(n) {}

    void agregarArista(int u, int v, int peso) {
        adj[u].push_back({v, peso});
        adj[v].push_back({u, peso});
        numAristas++;
    }
};

struct ResultadoDijkstra {
    long long distancia;
    int nodosExplorados;
    double tiempoMs;
    vector<int> camino;
};

struct ResultadoBFS {
    int saltos;
    int nodosExplorados;
    double tiempoMs;
    vector<int> camino;
};

#endif