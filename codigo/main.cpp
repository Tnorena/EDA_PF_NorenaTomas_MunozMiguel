//hecho con ayuda de Gemini, Por Tomás Noreña y Miguel muñoz

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <cstdlib>

using namespace std;

// aqui estamos definiendo el mapa como una lista de adyacencia (un vector de vectores)
// Lo llamamos 'mapaDeConexiones'

vector<vector<pair<int, int>>> mapaDeConexiones;
bool estanConectados(int inicio, int fin) {
    if (inicio == fin) return true;
    if (inicio < 0 || inicio >= mapaDeConexiones.size()) return false;
    vector<bool> visitado(mapaDeConexiones.size(), false);
    queue<int> cola;
    visitado[inicio] = true;
    cola.push(inicio);
    while (!cola.empty()) {
        int actual = cola.front(); cola.pop();
        for (auto& con : mapaDeConexiones[actual]) {
            if (con.first == fin) return true;
            if (!visitado[con.first]) {
                visitado[con.first] = true;
                cola.push(con.first);
            }
        }
    }
    return false;
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
    // esto es para verificar que no solo cargo los datos, sino que puedo leerlos
    int nodoUsuario;
    cout << "\n¿Que nodo quieres revisar? (Escribe el numero): ";
    cin >> nodoUsuario;

    if (nodoUsuario >= 0 && nodoUsuario < mapaDeConexiones.size()) {
        if (mapaDeConexiones[nodoUsuario].empty()) {
            cout << "Ese nodo existe pero no tiene calles conectadas." << endl;
        } else {
            cout << "El nodo " << nodoUsuario << " tiene conexiones con: ";
            for (auto& con : mapaDeConexiones[nodoUsuario]) {
                cout << con.first << "(" << con.second << "km) ";
            }
            cout << endl;
        }
    } else {
        cout << "Ese ID de nodo no esta en el mapa de Pensilvania." << endl;
    }
    int nodoA, nodoB;
    cout << "\n--- Verificador de Conectividad ---" << endl;
    cout << "Ingrese origen: "; cin >> nodoA;
    cout << "Ingrese destino: "; cin >> nodoB;

    if (estanConectados(nodoA, nodoB)) {
        cout << "RESULTADO: SI existe un camino entre " << nodoA << " y " << nodoB << endl;
    } else {
        cout << "RESULTADO: NO estan conectados." << endl;
    }
    return 0;
}