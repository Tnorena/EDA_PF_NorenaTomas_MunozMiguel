// dijkstra.cpp - Tomás Noreña y Miguel Muñoz
#include "graph.hpp"
#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <chrono>
using namespace std;

vector<int> reconstruirCamino(const vector<int>& prev, int inicio, int destino) {
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

ResultadoDijkstra dijkstra(const Grafo* g, int inicio, int destino, bool guardarCamino) {
    ResultadoDijkstra resultado;
    resultado.distancia = numeric_limits<long long>::max();
    resultado.nodosExplorados = 0;
    resultado.tiempoMs = 0;

    int n = g->numNodos;

    if (inicio < 0 || inicio >= n || destino < 0 || destino >= n) {
        cout << "Error: nodo fuera de rango." << endl;
        return resultado;
    }

    vector<long long> dist(n, numeric_limits<long long>::max());
    vector<int> prev(n, -1);

    dist[inicio] = 0;

    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<pair<long long,int>>> heap;
    heap.push({0, inicio});

    auto tiempoInicio = chrono::high_resolution_clock::now();

    while (!heap.empty()) {
        long long d  = heap.top().first;
        int actual   = heap.top().second;
        heap.pop();

        resultado.nodosExplorados++;

        if (d > dist[actual]) continue;
        if (actual == destino) break;

        for (auto& arista : g->adj[actual]) {
            int vecino = arista.first;
            int peso   = arista.second;

            if (dist[actual] + peso < dist[vecino]) {
                dist[vecino] = dist[actual] + peso;
                prev[vecino] = actual;
                heap.push({dist[vecino], vecino});
            }
        }
    }

    auto tiempoFin = chrono::high_resolution_clock::now();
    resultado.tiempoMs = chrono::duration<double, milli>(tiempoFin - tiempoInicio).count();
    resultado.distancia = dist[destino];

    if (guardarCamino && dist[destino] != numeric_limits<long long>::max()) {
        resultado.camino = reconstruirCamino(prev, inicio, destino);
    }

    return resultado;
}