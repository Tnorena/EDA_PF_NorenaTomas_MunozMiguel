#include <iostream>
#include <string>
#include "graph.hpp"
#include "loader.cpp"
#include "structural.cpp"
#include "modulob.cpp"
using namespace std;

// aqui estamos definiendo el mapa como una lista de adyacencia (un vector de vectores)
vector<vector<pair<int, int>>> mapaDeConexiones;
// Esta función reemplaza a estanConectados para darnos la distancia real
void encontrarRutaMasCorta(int inicio, int destino) {
    if (inicio < 0 || inicio >= (int)mapaDeConexiones.size() || destino < 0 || destino >= (int)mapaDeConexiones.size()) {
        cout << "Error: Uno de los nodos no existe en el mapa." << endl;
        return;
    }

    const long long INF = numeric_limits<long long>::max();
    vector<long long> distancias(mapaDeConexiones.size(), INF);
    distancias[inicio] = 0;

    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> cola;
    cola.push({0, inicio});

    while (!cola.empty()) {
        long long d = cola.top().first;
        int actual = cola.top().second;
        cola.pop();

        if (d > distancias[actual]) continue;
        if (actual == destino) break;

        for (auto& calle : mapaDeConexiones[actual]) {
            int vecino = calle.first;
            int peso = calle.second;

            if (distancias[actual] + peso < distancias[vecino]) {
                distancias[vecino] = distancias[actual] + peso;
                cola.push({distancias[vecino], vecino});
            }
        }
    }

    if (distancias[destino] == INF) {
        cout << "Resultado: No existe ninguna ruta entre " << inicio << " y " << destino << endl;
    } else {
        cout << "Resultado: ¡Ruta encontrada!" << endl;
        cout << "La distancia mas corta es de: " << distancias[destino] << " km." << endl;
    }
}

int main() {
    srand(42);
    // usamos 'archivoDelMapa' para el lector
    ifstream archivoDelMapa("datos/roadNet-PA.txt"); 
    
    if (!archivoDelMapa.is_open()) {
        cout << "Error: No se encontro el archivo en la carpeta 'datos'" << endl;
        return 1;
    }

    if (modulo == "A") {
        ejecutarModuloA(g, "results/analisis_estructural.txt");
    } else if (modulo == "B") {
        ejecutarModuloB(g, "results/consultas_p2p.csv");
    } else if (modulo == "C") {
        cout << "Modulo C: proximamente..." << endl;
    } else {
        cout << "Modulo no reconocido: " << modulo << endl;
    }

    delete g;
    return 0;
}
