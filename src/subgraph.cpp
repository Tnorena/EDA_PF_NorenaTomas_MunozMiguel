// subgraph.cpp - Tomás Noreña y Miguel Muñoz
#include "graph.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

struct Subgrafo {
    vector<int> nodos;
    vector<tuple<int,int,int>> aristas;
};

Subgrafo construirSubgrafo(const Grafo* g, const vector<int>& caminoQ01, const vector<int>& caminoQ06) {
    Subgrafo sub;

    set<int> conjuntoNodos;
    for (int n : caminoQ01) conjuntoNodos.insert(n);
    for (int n : caminoQ06) conjuntoNodos.insert(n);
    sub.nodos = vector<int>(conjuntoNodos.begin(), conjuntoNodos.end());

    cout << "Nodos en el subgrafo: " << sub.nodos.size() << endl;

    vector<bool> enSubgrafo(g->numNodos, false);
    for (int n : sub.nodos) enSubgrafo[n] = true;

    for (int u : sub.nodos) {
        for (auto& arista : g->adj[u]) {
            int v    = arista.first;
            int peso = arista.second;
            if (enSubgrafo[v] && u < v) {
                sub.aristas.push_back({u, v, peso});
            }
        }
    }

    cout << "Aristas en el subgrafo: " << sub.aristas.size() << endl;
    return sub;
}

// Union-Find para Kruskal
int padreUF[2000];

int encontrar(int x) {
    if (padreUF[x] != x) padreUF[x] = encontrar(padreUF[x]);
    return padreUF[x];
}

bool unir(int x, int y) {
    int px = encontrar(x);
    int py = encontrar(y);
    if (px == py) return false;
    padreUF[px] = py;
    return true;
}

long long calcularMST(const Subgrafo& sub, vector<tuple<int,int,int>>& aristasMST) {
    int n = sub.nodos.size();
    int maxNodo = *max_element(sub.nodos.begin(), sub.nodos.end());
    vector<int> indice(maxNodo + 1, -1);
    for (int i = 0; i < n; i++) indice[sub.nodos[i]] = i;

    for (int i = 0; i < n; i++) padreUF[i] = i;

    vector<tuple<int,int,int>> aristasOrdenadas = sub.aristas;
    sort(aristasOrdenadas.begin(), aristasOrdenadas.end(), [](const auto& a, const auto& b) {
        return get<2>(a) < get<2>(b);
    });

    long long pesoTotal = 0;
    for (auto& [u, v, peso] : aristasOrdenadas) {
        int iu = indice[u];
        int iv = indice[v];
        if (unir(iu, iv)) {
            pesoTotal += peso;
            aristasMST.push_back({u, v, peso});
        }
    }

    return pesoTotal;
}

bool dfsDAG(int nodo, int padreNodo, const vector<vector<int>>& adjLocal, vector<int>& estado) {
    estado[nodo] = 1;
    for (int vecino : adjLocal[nodo]) {
        if (vecino == padreNodo) continue;
        if (estado[vecino] == 1) return false;
        if (estado[vecino] == 0) {
            if (!dfsDAG(vecino, nodo, adjLocal, estado)) return false;
        }
    }
    estado[nodo] = 2;
    return true;
}

bool esDAG(const Subgrafo& sub) {
    int maxNodo = *max_element(sub.nodos.begin(), sub.nodos.end());
    vector<vector<int>> adjLocal(maxNodo + 1);
    for (auto& [u, v, peso] : sub.aristas) {
        adjLocal[u].push_back(v);
        adjLocal[v].push_back(u);
    }

    vector<int> estado(maxNodo + 1, 0);
    for (int nodo : sub.nodos) {
        if (estado[nodo] == 0) {
            if (!dfsDAG(nodo, -1, adjLocal, estado)) return false;
        }
    }
    return true;
}

void ejecutarModuloC(const Grafo* g, const vector<int>& caminoQ01, const vector<int>& caminoQ06) {
    cout << "\n=== MODULO C: Subgrafo, MST y DAG ===" << endl;

    if (caminoQ01.empty() || caminoQ06.empty()) {
        cout << "Error: los caminos Q01 o Q06 estan vacios." << endl;
        return;
    }

    cout << "Construyendo subgrafo inducido..." << endl;
    Subgrafo sub = construirSubgrafo(g, caminoQ01, caminoQ06);

    cout << "Calculando MST con Kruskal..." << endl;
    vector<tuple<int,int,int>> aristasMST;
    long long pesoMST = calcularMST(sub, aristasMST);

    cout << "Verificando si el subgrafo es DAG..." << endl;
    bool dag = esDAG(sub);

    cout << "\n--- Resultados Modulo C ---" << endl;
    cout << "Nodos en el subgrafo:   " << sub.nodos.size() << endl;
    cout << "Aristas en el subgrafo: " << sub.aristas.size() << endl;
    cout << "Peso total del MST:     " << pesoMST << endl;
    cout << "Aristas en el MST:      " << aristasMST.size() << endl;
    cout << "Es DAG:                 " << (dag ? "SI" : "NO") << endl;

    ofstream salSubgrafo("results/subgrafo_caminos.txt");
    if (salSubgrafo.is_open()) {
        salSubgrafo << "# Subgrafo inducido por caminos Q01 y Q06" << endl;
        salSubgrafo << "# Nodos: " << sub.nodos.size() << endl;
        salSubgrafo << "# Aristas: " << sub.aristas.size() << endl;
        for (auto& [u, v, peso] : sub.aristas) {
            salSubgrafo << u << " " << v << " " << peso << endl;
        }
        salSubgrafo.close();
        cout << "Subgrafo exportado en: results/subgrafo_caminos.txt" << endl;
    }

    ofstream salAnalisis("results/analisis_subgrafo.txt");
    if (salAnalisis.is_open()) {
        salAnalisis << "=== ANALISIS DEL SUBGRAFO ===" << endl;
        salAnalisis << "Nodos en el subgrafo: " << sub.nodos.size() << endl;
        salAnalisis << "Aristas en el subgrafo: " << sub.aristas.size() << endl;
        salAnalisis << "Peso total del MST: " << pesoMST << endl;
        salAnalisis << "Aristas en el MST: " << aristasMST.size() << endl;
        salAnalisis << "Es DAG: " << (dag ? "SI" : "NO") << endl;
        salAnalisis << endl;
        salAnalisis << "Aristas del MST:" << endl;
        for (auto& [u, v, peso] : aristasMST) {
            salAnalisis << u << " -- " << v << " (peso: " << peso << ")" << endl;
        }
        salAnalisis.close();
        cout << "Analisis guardado en: results/analisis_subgrafo.txt" << endl;
    }
}