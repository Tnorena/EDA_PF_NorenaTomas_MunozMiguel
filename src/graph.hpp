// Estructura principal del grafo como lista de adyacencia

#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <utility>  // para pair

using namespace std;

// Cada nodo guarda una lista de (vecino, peso)
// Usamos lista de adyacencia porque la matriz requeriría ~4.7 TB
// para 1,088,092 nodos (1088092^2 * 4 bytes)
struct Grafo {
    int numNodos;
    int numAristas;
    vector<vector<pair<int,int>>> adj; // adj[u] = { {v, peso}, ... }

    // Constructor: inicializa con n nodos y sin aristas
    Grafo(int n) : numNodos(n), numAristas(0), adj(n) {}

    // Agrega arista no dirigida con peso dado
    void agregarArista(int u, int v, int peso) {
        adj[u].push_back({v, peso});
        adj[v].push_back({u, peso});
        numAristas++;
    }
};

#endif