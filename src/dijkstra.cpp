//Dijkstra con heap minimo y reconstruccion de camino con prev[]

#include "graph.hpp"
#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <chrono>
#include <algorithm> 
using namespace std;

const long long INF = numeric_limits<long long>::max();

struct ResultadoDijkstra {
    long long distancia;       // distancia minima encontrada
    int nodosExplorados;       // cuantos nodos saco del heap
    double tiempoMs;           // tiempo en milisegundos
    vector<int> camino;        // secuencia de nodos (solo si se pidio)
};

//Reconstruye el camino desde prev[]
vector<int> reconstruirCamino(const vector<int>& prev, int inicio, int destino) {
    vector<int> camino;
    int actual = destino;

    //si prev[destino] es -1 y destino != inicio, no hay camino
    if (prev[destino] == -1 && destino != inicio) {
        return camino; // vacio = sin camino
    }

    while (actual != -1) {
        camino.push_back(actual);
        actual = prev[actual];
    }

    //el camino queda al reves, lo volteamos
    reverse(camino.begin(), camino.end());
    return camino;
}

ResultadoDijkstra dijkstra(const Grafo* g, int inicio, int destino, bool guardarCamino) {
    ResultadoDijkstra resultado;
    resultado.distancia = INF;
    resultado.nodosExplorados = 0;

    int n = g->numNodos;

    //validar que los nodos existan
    if (inicio < 0 || inicio >= n || destino < 0 || destino >= n) {
        cout << "Error: nodo fuera de rango." << endl;
        resultado.tiempoMs = 0;
        return resultado;
    }

    vector<long long> dist(n, INF);
    vector<int> prev(n, -1);  //para reconstruir el camino

    dist[inicio] = 0;

    //heap minimo: pares (distancia, nodo)
    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<pair<long long,int>>> heap;
    heap.push({0, inicio});

    auto tiempoInicio = chrono::high_resolution_clock::now();

    while (!heap.empty()) {
        long long d = heap.top().first;
        int actual = heap.top().second;
        heap.pop();

        resultado.nodosExplorados++;

        //si ya encontramos una distancia mejor, ignoramos esta entrada
        if (d > dist[actual]) continue;

        //si llegamos al destino podemos parar
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

    if (guardarCamino && dist[destino] != INF) {
        resultado.camino = reconstruirCamino(prev, inicio, destino);
    }

    return resultado;
}