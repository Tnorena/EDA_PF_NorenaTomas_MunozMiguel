// Modulo A: estadisticas del grafo, componentes conexas, diametro aproximado

#include "graph.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>
using namespace std;

//1. Estadísticas básicas

//Encuentra el nodo con más conexiones (mayor grado)
int nodoDeMayorGrado(const Grafo* g) {
    int nodoMax = 0;
    int gradoMax = 0;
    for (int i = 0; i < g->numNodos; i++) {
        int grado = (int)g->adj[i].size();
        if (grado > gradoMax) {
            gradoMax = grado;
            nodoMax = i;
        }
    }
    return nodoMax;
}

//Calcula el grado promedio de todos los nodos
double gradoPromedio(const Grafo* g) {
    long long sumaGrados = 0;
    int nodosConAristas = 0;
    for (int i = 0; i < g->numNodos; i++) {
        if (!g->adj[i].empty()) {
            sumaGrados += g->adj[i].size();
            nodosConAristas++;
        }
    }
    //grado promedio = suma de grados / nodos que tienen al menos una arista
    if (nodosConAristas == 0) return 0.0;
    return (double)sumaGrados / nodosConAristas;
}

//2. BFS para estimar diámetro

//Hace BFS desde 'inicio' y devuelve la distancia máxima encontrada
//También cuenta cuántos nodos alcanzó (tamaño de componente)
int bfsDesdeNodo(const Grafo* g, int inicio, int& nodosAlcanzados) {
    int n = g->numNodos;
    vector<int> dist(n, -1);  // -1 = no visitado
    queue<int> cola;

    dist[inicio] = 0;
    cola.push(inicio);
    nodosAlcanzados = 0;
    int distanciaMax = 0;

    while (!cola.empty()) {
        int actual = cola.front();
        cola.pop();
        nodosAlcanzados++;

        for (auto& vecino : g->adj[actual]) {
            int v = vecino.first;
            if (dist[v] == -1) {
                dist[v] = dist[actual] + 1;
                distanciaMax = max(distanciaMax, dist[v]);
                cola.push(v);
            }
        }
    }

    return distanciaMax;
}

//3. Componentes conexas 

//Cuenta componentes conexas usando BFS con marcado global
//También detecta cuál es la componente más grande
void detectarComponentes(const Grafo* g, int& numComponentes, int& tamañoComponentePrincipal) {
    int n = g->numNodos;
    vector<bool> visitado(n, false);
    numComponentes = 0;
    tamañoComponentePrincipal = 0;

    for (int i = 0; i < n; i++) {
        //Solo empezamos BFS desde nodos que tienen aristas y no fueron visitados
        if (visitado[i] || g->adj[i].empty()) continue;

        //BFS desde nodo i
        queue<int> cola;
        cola.push(i);
        visitado[i] = true;
        int tamañoActual = 0;

        while (!cola.empty()) {
            int actual = cola.front();
            cola.pop();
            tamañoActual++;

            for (auto& vecino : g->adj[actual]) {
                int v = vecino.first;
                if (!visitado[v]) {
                    visitado[v] = true;
                    cola.push(v);
                }
            }
        }

        numComponentes++;
        if (tamañoActual > tamañoComponentePrincipal) {
            tamañoComponentePrincipal = tamañoActual;
        }
    }
}

//4. Función principal del Módulo A 

void ejecutarModuloA(const Grafo* g, const string& archivoSalida) {
    cout << "\n=== MODULO A: Analisis estructural ===" << endl;

    //Nodos con al menos una arista (nodos reales del dataset)
    int nodosReales = 0;
    for (int i = 0; i < g->numNodos; i++) {
        if (!g->adj[i].empty()) nodosReales++;
    }

    //Grado promedio
    double promedio = gradoPromedio(g);

    //Nodo de mayor grado
    int nodoMax = nodoDeMayorGrado(g);
    int gradoMax = (int)g->adj[nodoMax].size();

    //Componentes conexas
    cout << "Detectando componentes conexas (puede tardar un momento)..." << endl;
    int numComp, tamañoPrincipal;
    detectarComponentes(g, numComp, tamañoPrincipal);

    //Diámetro aproximado con BFS desde nodo de mayor grado
    cout << "Calculando diametro aproximado con BFS desde nodo " << nodoMax << "..." << endl;
    int nodosAlcanzados = 0;
    int diametroAprox = bfsDesdeNodo(g, nodoMax, nodosAlcanzados);

    //Mostrar en pantalla
    cout << "\n     Resultados    " << endl;
    cout << "Nodos totales (con aristas): " << nodosReales << endl;
    cout << "  Valor SNAP esperado:        1,088,092" << endl;
    cout << "Aristas unicas:              " << g->numAristas << endl;
    cout << "  Valor SNAP esperado:        1,541,898" << endl;
    cout << "Nodos en componente principal: " << tamañoPrincipal << endl;
    cout << "  Valor SNAP esperado:          1,087,562" << endl;
    cout << "Numero de componentes conexas: " << numComp << endl;
    cout << "Grado promedio: " << promedio << endl;
    cout << "  Valor SNAP esperado: ~2.83" << endl;
    cout << "Nodo de mayor grado: " << nodoMax << " (grado " << gradoMax << ")" << endl;
    cout << "Diametro aproximado (BFS desde nodo mayor grado): " << diametroAprox << endl;
    cout << "  Valor SNAP esperado: 782" << endl;
    cout << "Nodos alcanzados por BFS: " << nodosAlcanzados << endl;

    //Guardar en archivo
    ofstream salida(archivoSalida);
    if (!salida.is_open()) {
        cerr << "No se pudo crear el archivo de salida: " << archivoSalida << endl;
        return;
    }

    salida << "     ANALISIS ESTRUCTURAL - roadNet-P        " << endl;
    salida << "Estadistica                        | Valor obtenido   | Valor SNAP" << endl;
    salida << "-----------------------------------|------------------|------------" << endl;
    salida << "Nodos (con aristas)                | " << nodosReales      << "       | 1,088,092" << endl;
    salida << "Aristas unicas                     | " << g->numAristas    << "       | 1,541,898" << endl;
    salida << "Nodos componente principal         | " << tamañoPrincipal  << "       | 1,087,562" << endl;
    salida << "Numero de componentes conexas      | " << numComp          << "           |" << endl;
    salida << "Grado promedio                     | " << promedio         << "           | ~2.83" << endl;
    salida << "Nodo de mayor grado                | " << nodoMax          << "           |" << endl;
    salida << "Grado del nodo maximo              | " << gradoMax         << "           |" << endl;
    salida << "Diametro aproximado (BFS)          | " << diametroAprox    << "           | 782" << endl;
    salida << "Nodos alcanzados por BFS           | " << nodosAlcanzados  << "       |" << endl;

    salida.close();
    cout << "\nResultados guardados en: " << archivoSalida << endl;
}