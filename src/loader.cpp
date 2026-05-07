// loader.cpp - Tomás Noreña y Miguel Muñoz
#include "graph.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
using namespace std;

// Primera pasada: solo para saber cuál es el nodo con ID más grande
int encontrarNodoMaximo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "Error: no se pudo abrir " << nombreArchivo << endl;
        return -1;
    }
    string linea;
    int u, v, maxNodo = 0;
    while (getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') continue;
        stringstream ss(linea);
        if (ss >> u >> v) {
            maxNodo = max(maxNodo, max(u, v));
        }
    }
    return maxNodo;
}

Grafo* cargarGrafo(const string& nombreArchivo) {
    cout << "Pasada 1: buscando nodo maximo..." << endl;
    int maxNodo = encontrarNodoMaximo(nombreArchivo);
    if (maxNodo < 0) return nullptr;
    cout << "Nodo maximo: " << maxNodo << endl;

    Grafo* g = new Grafo(maxNodo + 1);

    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "Error abriendo archivo en segunda pasada." << endl;
        return nullptr;
    }

    // srand(42) antes de cualquier rand() — obligatorio para reproducibilidad
    srand(42);

    string linea;
    int u, v;
    int lineasLeidas = 0;

    cout << "Pasada 2: cargando aristas..." << endl;

    while (getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') continue;
        stringstream ss(linea);
        if (!(ss >> u >> v)) continue;
        if (u == v) continue; // ignorar auto-lazos

        // Siempre llamamos rand() para que el avance sea igual en todos los equipos
        int peso = (rand() % 10) + 1;

        // Solo insertamos si u < v para evitar duplicados
        // (el archivo SNAP ya trae u->v y v->u como dos líneas)
        if (u < v) {
            g->agregarArista(u, v, peso);
        }

        lineasLeidas++;
        if (lineasLeidas % 1000000 == 0) {
            cout << "  Lineas procesadas: " << lineasLeidas << endl;
        }
    }

    cout << "Carga completa. Aristas unicas insertadas: " << g->numAristas << endl;
    return g;
}