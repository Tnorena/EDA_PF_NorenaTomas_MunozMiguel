// structural.cpp - Tomás Noreña y Miguel Muñoz
#include "graph.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

void ejecutarModuloA(const Grafo* g, const string& archivoSalida) {
    cout << "\n=== MODULO A: Analisis estructural ===" << endl;

    int nodosReales = 0;
    int nodoMax = 0;
    int gradoMax = 0;

    for (int i = 0; i < g->numNodos; i++) {
        if (!g->adj[i].empty()) {
            nodosReales++;
            if ((int)g->adj[i].size() > gradoMax) {
                gradoMax = g->adj[i].size();
                nodoMax = i;
            }
        }
    }

    long long sumaGrados = 0;
    for (int i = 0; i < g->numNodos; i++) {
        sumaGrados += g->adj[i].size();
    }
    double promedio = (nodosReales > 0) ? (double)sumaGrados / nodosReales : 0;

    cout << "Detectando componentes conexas..." << endl;
    vector<bool> visitado(g->numNodos, false);
    int numComponentes = 0;
    int tamañoPrincipal = 0;

    for (int i = 0; i < g->numNodos; i++) {
        if (visitado[i] || g->adj[i].empty()) continue;

        queue<int> cola;
        cola.push(i);
        visitado[i] = true;
        int tamaño = 0;

        while (!cola.empty()) {
            int actual = cola.front();
            cola.pop();
            tamaño++;
            for (auto& vecino : g->adj[actual]) {
                if (!visitado[vecino.first]) {
                    visitado[vecino.first] = true;
                    cola.push(vecino.first);
                }
            }
        }

        numComponentes++;
        if (tamaño > tamañoPrincipal) tamañoPrincipal = tamaño;
    }

    cout << "Calculando diametro aproximado desde nodo " << nodoMax << "..." << endl;
    vector<int> dist(g->numNodos, -1);
    queue<int> cola;
    cola.push(nodoMax);
    dist[nodoMax] = 0;
    int diametroAprox = 0;
    int nodosAlcanzados = 0;

    while (!cola.empty()) {
        int actual = cola.front();
        cola.pop();
        nodosAlcanzados++;
        for (auto& vecino : g->adj[actual]) {
            if (dist[vecino.first] == -1) {
                dist[vecino.first] = dist[actual] + 1;
                if (dist[vecino.first] > diametroAprox) {
                    diametroAprox = dist[vecino.first];
                }
                cola.push(vecino.first);
            }
        }
    }

    cout << "\n--- Resultados ---" << endl;
    cout << "Nodos totales (con aristas): " << nodosReales << endl;
    cout << "  Valor SNAP esperado:        1,088,092" << endl;
    cout << "Aristas unicas:               " << g->numAristas << endl;
    cout << "  Valor SNAP esperado:         1,541,898" << endl;
    cout << "Nodos en componente principal: " << tamañoPrincipal << endl;
    cout << "  Valor SNAP esperado:          1,087,562" << endl;
    cout << "Numero de componentes conexas: " << numComponentes << endl;
    cout << "Grado promedio: " << promedio << endl;
    cout << "  Valor SNAP esperado: ~2.83" << endl;
    cout << "Nodo de mayor grado: " << nodoMax << " (grado " << gradoMax << ")" << endl;
    cout << "Diametro aproximado (BFS): " << diametroAprox << endl;
    cout << "  Valor SNAP esperado: 782" << endl;
    cout << "Nodos alcanzados por BFS: " << nodosAlcanzados << endl;

    ofstream salida(archivoSalida);
    if (!salida.is_open()) {
        cerr << "No se pudo crear: " << archivoSalida << endl;
        return;
    }

    salida << "=== ANALISIS ESTRUCTURAL - roadNet-PA ===" << endl;
    salida << "Estadistica                        | Valor obtenido   | Valor SNAP" << endl;
    salida << "-----------------------------------|------------------|------------" << endl;
    salida << "Nodos (con aristas)                | " << nodosReales     << "       | 1,088,092" << endl;
    salida << "Aristas unicas                     | " << g->numAristas   << "       | 1,541,898" << endl;
    salida << "Nodos componente principal         | " << tamañoPrincipal << "       | 1,087,562" << endl;
    salida << "Numero de componentes conexas      | " << numComponentes  << "           |" << endl;
    salida << "Grado promedio                     | " << promedio        << "           | ~2.83" << endl;
    salida << "Nodo de mayor grado                | " << nodoMax         << "           |" << endl;
    salida << "Grado del nodo maximo              | " << gradoMax        << "           |" << endl;
    salida << "Diametro aproximado (BFS)          | " << diametroAprox   << "           | 782" << endl;
    salida << "Nodos alcanzados por BFS           | " << nodosAlcanzados << "       |" << endl;

    salida.close();
    cout << "\nResultados guardados en: " << archivoSalida << endl;
}