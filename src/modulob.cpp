// modulob.cpp - Tomás Noreña y Miguel Muñoz
#include "graph.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

ResultadoDijkstra dijkstra(const Grafo* g, int inicio, int destino, bool guardarCamino);
ResultadoBFS bfs(const Grafo* g, int inicio, int destino, bool guardarCamino);

void ejecutarModuloB(const Grafo* g, const string& archivoCSV,
                     vector<int>& caminoQ01, vector<int>& caminoQ06) {
    cout << "\n=== MODULO B: Consultas P2P ===" << endl;

    string nombres[10] = {"Q01","Q02","Q03","Q04","Q05","Q06","Q07","Q08","Q09","Q10"};
    int origenes[10]   = {1, 100, 50000, 200000, 300000, 1, 500000, 250000, 10000, 400000};
    int destinos[10]   = {500000, 1000000, 750000, 800000, 100000, 1087562, 1, 600000, 900000, 150000};

    ofstream csv(archivoCSV);
    if (!csv.is_open()) {
        cerr << "No se pudo crear: " << archivoCSV << endl;
        return;
    }

    csv << "consulta,origen,destino,dist_dijkstra,saltos_bfs,"
        << "nodos_dijkstra,nodos_bfs,t_dijkstra_ms,t_bfs_ms" << endl;

    for (int i = 0; i < 10; i++) {
        cout << "\nEjecutando " << nombres[i] << " (" << origenes[i] << " -> " << destinos[i] << ")..." << endl;

        bool guardarCamino = (nombres[i] == "Q01" || nombres[i] == "Q06");

        ResultadoDijkstra resDijk = dijkstra(g, origenes[i], destinos[i], guardarCamino);
        ResultadoBFS      resBFS  = bfs(g, origenes[i], destinos[i], false);

        if (nombres[i] == "Q01") caminoQ01 = resDijk.camino;
        if (nombres[i] == "Q06") caminoQ06 = resDijk.camino;

        if (resDijk.distancia == numeric_limits<long long>::max()) {
            cout << "  Dijkstra: SIN CAMINO" << endl;
        } else {
            cout << "  Dijkstra: distancia = " << resDijk.distancia
                 << " | nodos explorados = " << resDijk.nodosExplorados
                 << " | tiempo = " << resDijk.tiempoMs << " ms" << endl;
        }

        if (resBFS.saltos == -1) {
            cout << "  BFS: SIN CAMINO" << endl;
        } else {
            cout << "  BFS: saltos = " << resBFS.saltos
                 << " | nodos explorados = " << resBFS.nodosExplorados
                 << " | tiempo = " << resBFS.tiempoMs << " ms" << endl;
        }

        string distDijk  = (resDijk.distancia == numeric_limits<long long>::max()) ? "INF" : to_string(resDijk.distancia);
        string saltosBFS = (resBFS.saltos == -1) ? "INF" : to_string(resBFS.saltos);

        csv << nombres[i] << ","
            << origenes[i] << ","
            << destinos[i] << ","
            << distDijk << ","
            << saltosBFS << ","
            << resDijk.nodosExplorados << ","
            << resBFS.nodosExplorados << ","
            << resDijk.tiempoMs << ","
            << resBFS.tiempoMs << endl;

        if (guardarCamino && !resDijk.camino.empty()) {
            string nombreArchivo = "results/camino_" + nombres[i] + ".txt";
            ofstream archivoCamino(nombreArchivo);
            archivoCamino << "Camino de " << origenes[i] << " a " << destinos[i] << endl;
            archivoCamino << "Distancia total: " << resDijk.distancia << endl;
            archivoCamino << "Numero de nodos en el camino: " << resDijk.camino.size() << endl;
            archivoCamino << "Secuencia de nodos:" << endl;
            for (int nodo : resDijk.camino) {
                archivoCamino << nodo << endl;
            }
            archivoCamino.close();
            cout << "  Camino guardado en: " << nombreArchivo << endl;
        }
    }

    csv.close();
    cout << "\nResultados guardados en: " << archivoCSV << endl;
}