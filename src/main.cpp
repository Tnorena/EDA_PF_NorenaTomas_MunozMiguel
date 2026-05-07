// main.cpp - Tomás Noreña y Miguel Muñoz
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <limits>
#include <chrono>
#include <string>
#include <sstream>
#include <tuple>
#include "graph.hpp"
using namespace std;

Grafo* cargarGrafo(const string& nombreArchivo);
void ejecutarModuloA(const Grafo* g, const string& archivoSalida);
void ejecutarModuloB(const Grafo* g, const string& archivoCSV, vector<int>& caminoQ01, vector<int>& caminoQ06);
void ejecutarModuloC(const Grafo* g, const vector<int>& caminoQ01, const vector<int>& caminoQ06);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Uso: ./eda_pf <modulo>" << endl;
        cout << "  Modulos disponibles: A, B, C" << endl;
        return 1;
    }

    string modulo = argv[1];

    // medir tiempo de carga
    auto inicioCarga = chrono::high_resolution_clock::now();
    cout << "Cargando grafo desde data/roadNet-PA.txt ..." << endl;
    Grafo* g = cargarGrafo("data/roadNet-PA.txt");
    if (g == nullptr) {
        cerr << "Error: no se pudo cargar el grafo." << endl;
        return 1;
    }
    auto finCarga = chrono::high_resolution_clock::now();
    double tiempoCarga = chrono::duration<double, milli>(finCarga - inicioCarga).count();

    // medir tiempo del modulo seleccionado
    double tiempoModulo = 0;
    auto inicioModulo = chrono::high_resolution_clock::now();

    if (modulo == "A") {
        ejecutarModuloA(g, "results/analisis_estructural.txt");

    } else if (modulo == "B") {
        vector<int> caminoQ01, caminoQ06;
        ejecutarModuloB(g, "results/consultas_p2p.csv", caminoQ01, caminoQ06);

    } else if (modulo == "C") {
        cout << "Recalculando caminos Q01 y Q06 para el subgrafo..." << endl;
        vector<int> caminoQ01, caminoQ06;
        ejecutarModuloB(g, "results/consultas_p2p.csv", caminoQ01, caminoQ06);
        ejecutarModuloC(g, caminoQ01, caminoQ06);

    } else {
        cout << "Modulo no reconocido: " << modulo << endl;
        delete g;
        return 1;
    }

    auto finModulo = chrono::high_resolution_clock::now();
    tiempoModulo = chrono::duration<double, milli>(finModulo - inicioModulo).count();

    // guardar tiempos en CSV
    // usamos app para no borrar tiempos de modulos anteriores
    ofstream tiempos("results/tiempos.csv", ios::app);

    // si el archivo esta vacio escribimos el encabezado
    ifstream verificar("results/tiempos.csv");
    verificar.seekg(0, ios::end);
    bool estaVacio = (verificar.tellg() == 0);
    verificar.close();

    if (estaVacio) {
        tiempos << "modulo,t_carga_ms,t_modulo_ms,t_total_ms" << endl;
    }

    tiempos << modulo << ","
            << tiempoCarga << ","
            << tiempoModulo << ","
            << (tiempoCarga + tiempoModulo) << endl;

    tiempos.close();
    cout << "\nTiempos guardados en: results/tiempos.csv" << endl;

    delete g;
    return 0;
}