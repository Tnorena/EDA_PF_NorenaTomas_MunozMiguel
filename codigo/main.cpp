#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// Definimos el mapa como una lista de adyacencia (un vector de vectores)
// Lo llamamos 'mapaDeConexiones' como pediste
vector<vector<int>> mapaDeConexiones;

int main() {
    // Usamos el nombre 'archivoDelMapa' para el lector
    ifstream archivoDelMapa("datos/roadNet-PA.txt"); 
    
    if (!archivoDelMapa.is_open()) {
        cout << "Error: No encontré el archivo en la carpeta 'datos'" << endl;
        return 1;
    }

    string linea;
    int origen, destino;
    int cantidadDeCalles = 0;
    int nodoMaximo = 1090920; // Pensilvania tiene aprox. este número de nodos

    // Preparamos el tamaño del mapa para que no falle la memoria
    mapaDeConexiones.resize(nodoMaximo + 1);

    cout << "Cargando el mapa de Pensilvania..." << endl;

    while (getline(archivoDelMapa, linea)) {
        if (linea.empty() || linea[0] == '#') continue;

        stringstream ss(linea);
        if (ss >> origen >> destino) {
            // Guardamos la conexión: del origen al destino
            mapaDeConexiones[origen].push_back(destino);
            cantidadDeCalles++;
        }
    }

    cout << "¡Éxito! Se han cargado " << cantidadDeCalles << " conexiones en el mapa." << endl;
    
    archivoDelMapa.close();
    return 0;
}