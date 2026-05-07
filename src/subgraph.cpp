// Modulo C: subgrafo inducido, MST con Kruskal, verificacion DAG

#include "graph.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
using namespace std;

//1. Construir subgrafo inducido

//El subgrafo solo tiene los nodos que aparecen en los caminos Q01 y Q06
//y las aristas entre esos nodos que existian en el grafo original
struct Subgrafo {
    vector<int> nodos;                    //nodos originales incluidos
    vector<tuple<int,int,int>> aristas;   //(u, v, peso) con IDs originales
};

Subgrafo construirSubgrafo(const Grafo* g, const vector<int>& caminoQ01, const vector<int>& caminoQ06) {
    Subgrafo sub;

    //unimos los nodos de ambos caminos en un set para no repetir
    set<int> conjuntoNodos;
    for (int n : caminoQ01) conjuntoNodos.insert(n);
    for (int n : caminoQ06) conjuntoNodos.insert(n);

    sub.nodos = vector<int>(conjuntoNodos.begin(), conjuntoNodos.end());

    cout << "Nodos en el subgrafo: " << sub.nodos.size() << endl;

    //buscamos aristas entre nodos del subgrafo
    //para cada nodo del subgrafo, revisamos sus vecinos
    for (int u : sub.nodos) {
        for (auto& arista : g->adj[u]) {
            int v    = arista.first;
            int peso = arista.second;
            //solo incluimos si v tambien esta en el subgrafo
            //y para no duplicar, solo cuando u < v
            if (conjuntoNodos.count(v) && u < v) {
                sub.aristas.push_back({u, v, peso});
            }
        }
    }

    cout << "Aristas en el subgrafo: " << sub.aristas.size() << endl;
    return sub;
}

//2. MST con Kruskal

//Union-Find para Kruskal
struct UnionFind {
    vector<int> padre;
    vector<int> rango;

    UnionFind(int n) : padre(n), rango(n, 0) {
        for (int i = 0; i < n; i++) padre[i] = i;
    }

    int encontrar(int x) {
        if (padre[x] != x) padre[x] = encontrar(padre[x]); //compresion de camino
        return padre[x];
    }

    bool unir(int x, int y) {
        int px = encontrar(x);
        int py = encontrar(y);
        if (px == py) return false; //ya estan en el mismo componente

        //union por rango
        if (rango[px] < rango[py]) swap(px, py);
        padre[py] = px;
        if (rango[px] == rango[py]) rango[px]++;
        return true;
    }
};

long long calcularMST(const Subgrafo& sub, vector<tuple<int,int,int>>& aristasMST) {
    //necesitamos reindexar los nodos a 0..n-1 para el union-find
    //creamos un mapa de nodo original -> indice local
    map<int,int> indiceLocal;
    for (int i = 0; i < (int)sub.nodos.size(); i++) {
        indiceLocal[sub.nodos[i]] = i;
    }

    //ordenamos aristas por peso (Kruskal)
    vector<tuple<int,int,int>> aristasOrdenadas = sub.aristas;
    sort(aristasOrdenadas.begin(), aristasOrdenadas.end(), [](const auto& a, const auto& b) {
        return get<2>(a) < get<2>(b);
    });

    UnionFind uf(sub.nodos.size());
    long long pesoTotal = 0;

    for (auto& [u, v, peso] : aristasOrdenadas) {
        int iu = indiceLocal[u];
        int iv = indiceLocal[v];
        if (uf.unir(iu, iv)) {
            pesoTotal += peso;
            aristasMST.push_back({u, v, peso});
        }
    }

    return pesoTotal;
}

//3. Verificacion DAG con DFS

//Un grafo no dirigido NUNCA es un DAG (siempre tiene ciclos porque
//cada arista u-v crea un "ciclo" de longitud 2: u->v->u)
//Pero el enunciado pide verificarlo con DFS y clasificacion de aristas
//asi que lo implementamos correctamente

//estados del DFS: 0=no visitado, 1=en pila, 2=terminado
bool dfsDAG(int nodo, int padre, const vector<vector<pair<int,int>>>& adjLocal, 
            vector<int>& estado) {
    estado[nodo] = 1; //en pila

    for (auto& arista : adjLocal[nodo]) {
        int vecino = arista.first;
        if (vecino == padre) continue; //ignoramos la arista de vuelta al padre

        if (estado[vecino] == 1) {
            //encontramos un ciclo (arista de retroceso)
            return false;
        }
        if (estado[vecino] == 0) {
            if (!dfsDAG(vecino, nodo, adjLocal, estado)) {
                return false;
            }
        }
    }

    estado[nodo] = 2; //terminado
    return true;
}

bool esDAG(const Subgrafo& sub) {
    //construimos lista de adyacencia local para el subgrafo
    map<int,int> indiceLocal;
    for (int i = 0; i < (int)sub.nodos.size(); i++) {
        indiceLocal[sub.nodos[i]] = i;
    }

    int n = sub.nodos.size();
    vector<vector<pair<int,int>>> adjLocal(n);

    for (auto& [u, v, peso] : sub.aristas) {
        int iu = indiceLocal[u];
        int iv = indiceLocal[v];
        adjLocal[iu].push_back({iv, peso});
        adjLocal[iv].push_back({iu, peso});
    }

    vector<int> estado(n, 0);
    for (int i = 0; i < n; i++) {
        if (estado[i] == 0) {
            if (!dfsDAG(i, -1, adjLocal, estado)) {
                return false;
            }
        }
    }
    return true;
}

//4. Funcion principal del Modulo C

void ejecutarModuloC(const Grafo* g, const vector<int>& caminoQ01, 
                     const vector<int>& caminoQ06) {
    cout << "\n=== MODULO C: Subgrafo, MST y DAG ===" << endl;

    if (caminoQ01.empty() || caminoQ06.empty()) {
        cout << "Error: los caminos Q01 o Q06 estan vacios." << endl;
        cout << "Ejecuta primero el Modulo B." << endl;
        return;
    }

    //construir subgrafo
    cout << "Construyendo subgrafo inducido..." << endl;
    Subgrafo sub = construirSubgrafo(g, caminoQ01, caminoQ06);

    //MST
    cout << "Calculando MST con Kruskal..." << endl;
    vector<tuple<int,int,int>> aristasMST;
    long long pesoMST = calcularMST(sub, aristasMST);

    //DAG
    cout << "Verificando si el subgrafo es DAG..." << endl;
    bool dag = esDAG(sub);

    //mostrar resultados
    cout << "\n--- Resultados Modulo C ---" << endl;
    cout << "Nodos en el subgrafo:  " << sub.nodos.size() << endl;
    cout << "Aristas en el subgrafo: " << sub.aristas.size() << endl;
    cout << "Peso total del MST:    " << pesoMST << endl;
    cout << "Aristas en el MST:     " << aristasMST.size() << endl;
    cout << "Es DAG:                " << (dag ? "SI" : "NO") << endl;
    cout << "(Un grafo no dirigido con ciclos no es DAG)" << endl;

    //exportar subgrafo
    string archivoSubgrafo = "results/subgrafo_caminos.txt";
    ofstream salSubgrafo(archivoSubgrafo);
    if (salSubgrafo.is_open()) {
        salSubgrafo << "# Subgrafo inducido por caminos Q01 y Q06" << endl;
        salSubgrafo << "# Nodos: " << sub.nodos.size() << endl;
        salSubgrafo << "# Aristas: " << sub.aristas.size() << endl;
        for (auto& [u, v, peso] : sub.aristas) {
            salSubgrafo << u << " " << v << " " << peso << endl;
        }
        salSubgrafo.close();
        cout << "Subgrafo exportado en: " << archivoSubgrafo << endl;
    }

    //exportar analisis
    string archivoAnalisis = "results/analisis_subgrafo.txt";
    ofstream salAnalisis(archivoAnalisis);
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
        cout << "Analisis guardado en: " << archivoAnalisis << endl;
    }
}