// Modulo B: ejecuta las 10 consultas P2P con Dijkstra y BFS

#include "graph.hpp"
#include "dijkstra.cpp"
#include "bfs.cpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

struct Consulta {
    string nombre;
    int origen;
    int destino;
};

void ejecutarModuloB(const Grafo* g, const string& archivoCSV) {
    cout << "\n=== MODULO B: Consultas P2P ===" << endl;

    //las 10 consultas fijas del enunciado
    vector<Consulta> consultas = {
        {"Q01",       1,  500000},
        {"Q02",     100, 1000000},
        {"Q03",   50000,  750000},
        {"Q04",  200000,  800000},
        {"Q05",  300000,  100000},
        {"Q06",       1, 1087562},
        {"Q07",  500000,       1},
        {"Q08",  250000,  600000},
        {"Q09",   10000,  900000},
        {"Q10",  400000,  150000}
    };

    //abrir CSV de salida
    ofstream csv(archivoCSV);
    if (!csv.is_open()) {
        cerr << "No se pudo crear: " << archivoCSV << endl;
        return;
    }

    //encabezado del CSV
    csv << "consulta,origen,destino,dist_dijkstra,saltos_bfs,"
        << "nodos_dijkstra,nodos_bfs,t_dijkstra_ms,t_bfs_ms" << endl;

    for (int i = 0; i < (int)consultas.size(); i++) {
        Consulta& c = consultas[i];
        cout << "\nEjecutando " << c.nombre << " (" << c.origen << " -> " << c.destino << ")..." << endl;

        //Q01 y Q06 guardan el camino completo
        bool guardarCamino = (c.nombre == "Q01" || c.nombre == "Q06");

        ResultadoDijkstra resDijk = dijkstra(g, c.origen, c.destino, guardarCamino);
        ResultadoBFS      resBFS  = bfs(g, c.origen, c.destino, false);

        //mostrar en pantalla
        if (resDijk.distancia == numeric_limits<long long>::max()) {
            cout << "  Dijkstra: SIN CAMINO (nodos en componentes distintas)" << endl;
        } else {
            cout << "  Dijkstra: distancia = " << resDijk.distancia
                 << " | nodos explorados = " << resDijk.nodosExplorados
                 << " | tiempo = " << resDijk.tiempoMs << " ms" << endl;
        }

        if (resBFS.saltos == -1) {
            cout << "  BFS:      SIN CAMINO" << endl;
        } else {
            cout << "  BFS:      saltos = " << resBFS.saltos
                 << " | nodos explorados = " << resBFS.nodosExplorados
                 << " | tiempo = " << resBFS.tiempoMs << " ms" << endl;
        }

        //escribir en CSV
        string distDijk = (resDijk.distancia == numeric_limits<long long>::max()) 
                          ? "INF" : to_string(resDijk.distancia);
        string saltosBFS = (resBFS.saltos == -1) 
                          ? "INF" : to_string(resBFS.saltos);

        csv << c.nombre << ","
            << c.origen << ","
            << c.destino << ","
            << distDijk << ","
            << saltosBFS << ","
            << resDijk.nodosExplorados << ","
            << resBFS.nodosExplorados << ","
            << resDijk.tiempoMs << ","
            << resBFS.tiempoMs << endl;

        //guardar camino completo para Q01 y Q06
        if (guardarCamino && !resDijk.camino.empty()) {
            string nombreCamino = "results/camino_" + c.nombre + ".txt";
            ofstream archivoCamino(nombreCamino);
            archivoCamino << "Camino de " << c.origen << " a " << c.destino << endl;
            archivoCamino << "Distancia total: " << resDijk.distancia << endl;
            archivoCamino << "Numero de nodos en el camino: " << resDijk.camino.size() << endl;
            archivoCamino << "Secuencia de nodos:" << endl;
            for (int nodo : resDijk.camino) {
                archivoCamino << nodo << endl;
            }
            archivoCamino.close();
            cout << "  Camino guardado en: " << nombreCamino << endl;
        }
    }

    csv.close();
    cout << "\nResultados guardados en: " << archivoCSV << endl;
}