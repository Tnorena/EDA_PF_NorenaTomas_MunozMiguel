//hecho con ayuda de Gemini, Por Tomás Noreña y Miguel muñoz

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <cstdlib>
#include <limits>

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

    string linea;
    int origen, destino;
    int cantidadDeCalles = 0;
    int nodoMaximo = 1090920; // Pensilvania tiene aprox. este número de nodos

    // aquí preparamos el tamaño del mapa para que no falle la memoria
    mapaDeConexiones.resize(nodoMaximo + 1);

    cout << "Cargando el mapa de Pensilvania..." << endl;

    while (getline(archivoDelMapa, linea)) {
        if (linea.empty() || linea[0] == '#') continue;
        stringstream lectorDeLinea(linea);

        if (lectorDeLinea >> origen >> destino) {
            int nodoMax = (origen > destino) ? origen : destino;
            int distancia = (rand() % 10) + 1; // Genera distancia entre 1 y 10
            if (nodoMax >= (int)mapaDeConexiones.size()) {
                mapaDeConexiones.resize(nodoMax + 1);
            }

            mapaDeConexiones[origen].push_back({destino, distancia});
            mapaDeConexiones[destino].push_back({origen, distancia});
            cantidadDeCalles++;
        }
    }

    cout << "Se han cargado con exito " << cantidadDeCalles << " conexiones en el mapa." << endl;
    
    archivoDelMapa.close();

    int nodoA, nodoB;
    cout << "   Calculadora de Ruta Minima (Dijkstra)" << endl;
    cout << "Ingrese origen: "; 
    cin >> nodoA;
    cout << "Ingrese destino: "; 
    cin >> nodoB;

    cout << "\nCalculando ruta mas corta..." << endl;
    
    // Esta función es la que te dará los KILÓMETROS exactos
    encontrarRutaMasCorta(nodoA, nodoB);

    return 0;
    }