//hecho con ayuda de Gemini, Por Tomás Noreña y Miguel muñoz

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// aqui estamos definiendo el mapa como una lista de adyacencia (un vector de vectores)
// Lo llamamos 'mapaDeConexiones'
vector<vector<int>> mapaDeConexiones;

int main() {
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

        stringstream ss(linea);
        if (ss >> origen >> destino) {
            // Guardamos la conexión: del origen al destino
            mapaDeConexiones[origen].push_back(destino);
            cantidadDeCalles++;
        }
    }

    cout << "Se han cargado con exito" << cantidadDeCalles << " conexiones en el mapa." << endl;
    
    archivoDelMapa.close();
    return 0;
}