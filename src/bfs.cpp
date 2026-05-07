// BFS no ponderado: cuenta saltos y nodos explorados

#include "graph.hpp"
#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <chrono>
#include <algorithm>
using namespace std;

struct ResultadoBFS {
    int saltos;           //numero de saltos (distancia no ponderada)
    int nodosExplorados;  //nodos que BFS visito
    double tiempoMs;
    vector<int> camino;   //solo si se pide
};

vector<int> reconstruirCaminoBFS(const vector<int>& prev, int inicio, int destino) {
    vector<int> camino;
    int actual = destino;

    if (prev[destino] == -1 && destino != inicio) {
        return camino;
    }

    while (actual != -1) {
        camino.push_back(actual);
        actual = prev[actual];
    }
    reverse(camino.begin(), camino.end());
    return camino;
}

ResultadoBFS bfs(const Grafo* g, int inicio, int destino, bool guardarCamino) {
    ResultadoBFS resultado;
    resultado.saltos = -1;  // -1 = sin camino
    resultado.nodosExplorados = 0;

    int n = g->numNodos;

    if (inicio < 0 || inicio >= n || destino < 0 || destino >= n) {
        cout << "Error: nodo fuera de rango." << endl;
        resultado.tiempoMs = 0;
        return resultado;
    }

    vector<int> dist(n, -1);
    vector<int> prev(n, -1);

    dist[inicio] = 0;
    queue<int> cola;
    cola.push(inicio);

    auto tiempoInicio = chrono::high_resolution_clock::now();

    while (!cola.empty()) {
        int actual = cola.front();
        cola.pop();
        resultado.nodosExplorados++;

        if (actual == destino) break;

        for (auto& arista : g->adj[actual]) {
            int vecino = arista.first;
            if (dist[vecino] == -1) {
                dist[vecino] = dist[actual] + 1;
                prev[vecino] = actual;
                cola.push(vecino);
            }
        }
    }

    auto tiempoFin = chrono::high_resolution_clock::now();
    resultado.tiempoMs = chrono::duration<double, milli>(tiempoFin - tiempoInicio).count();

    if (dist[destino] != -1) {
        resultado.saltos = dist[destino];
        if (guardarCamino) {
            resultado.camino = reconstruirCaminoBFS(prev, inicio, destino);
        }
    }

    return resultado;
}