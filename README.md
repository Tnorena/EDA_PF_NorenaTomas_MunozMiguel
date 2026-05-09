# EDA_PF_NoreñaTomas_MuñozMiguel
# Por: Miguel muñoz y Tomás Noreña

Practica final - EDA 2026 - 01
Universidad EAFIT

Este proyecto usamos la red vial de pennsylvania usando algoritmos sobre grafos implementados en C++.
El dataset tiene 1 millon de nodos aproximadamente y 3 millones de aristas y fue obtenido de snap

El programa esta dividido en tres modulos:
- Modulo A: es el que carga el grafo y calculo estadisticas estructurales
- Modulo B: calcula caminos minimos con dijkstra y bfs para 10 consultas fijas
- Modulo C: construye un subgrafo, calcula su mst y verifica si es dag

# requisitos para ejecutar

- g++ con soporte para c++17
- El archivo roadNet-PA.txt debe estar en la carpeta data/

# instrucciones para ejecutar

El archivo roadNet-PA.txt es demasiado pesado para github.

Para que el programa pueda funcionar tienes que descargar 
el dataset de Pennsylvania y ponerlo enla carpeta de data/ con el mismo nombre
(https://snap.stanford.edu/data/roadNet-PA.html)
En caso de que si funcione, ignorar mensaje


# estructura del repositorio

EDA_PF_NorenaTomas_MunozMiguel

    data
        roadNet-PA.txt
    report
        informe.pdf
    results
        analisis_estructural.txt
        analisis_subgrafo.txt
        camino_Q01.txt
        camino_Q06.txt
        consultas_p2p.csv
        subgrafo_caminos.txt
        tiempos.csv
    src
        bfs.cpp
        dijkstra.cpp
        graph.cpp
        graph.hpp
        loader.cpp
        main.cpp
        modulob.cpp
        proyecto.exe
        structural.cpp
        subgraph.cpp
    README.md

# Compilación

Desde la raíz del repositorio:

    g++ -std=c++17 -O2 -o eda_pf src/main.cpp src/graph.cpp src/loader.cpp src/structural.cpp src/dijkstra.cpp src/bfs.cpp src/modulob.cpp src/subgraph.cpp
    
**Nota si es usuario de Windows con MSYS2:** antes de compilar, asegurarse de tener g++ en el PATH. Si el comando `g++ --version` no funciona, ejecutar primero:

```bash
export PATH=$PATH:/mingw64/bin
```

Luego compilar desde la raíz del repositorio:

```bash
g++ -std=c++17 -O2 -o eda_pf src/main.cpp src/graph.cpp src/loader.cpp src/structural.cpp src/dijkstra.cpp src/bfs.cpp src/modulob.cpp src/subgraph.cpp
```

El flag -O2 es importante para que los tiempos sean realistas sobre el dataset completo.

# Ejecución

Primero crear la carpeta results/ si no existe:


mkdir results


Luego ejecutar cada módulo por separado:

    ./eda_pf A
    ./eda_pf B
    ./eda_pf C


En Windows con MSYS2:

 
    ./eda_pf.exe A
    ./eda_pf.exe B
    ./eda_pf.exe C


# Algo importante sobre el Modulo C

Cuando vayas a correr el modulo C, se va a volver a correr internamente el Módulo B para obtener los caminos de Q01 y Q06 que necesita para construir el subgrafo. Por eso la salida del Módulo B aparece dos veces en pantalla, no es un error.

# que archivos genera

Despues de correr los tres modulos, la carpeta de results/ tiene:

- analisis_estructural.txt: estadisticas del grafo comparadas con los valores de snap
- consultas_p2p.csv: resultados de las 10 consultas con dijkstra y bfs
- camino_Q01.txt: la secuencia de nodos del camino mas corto de Q01
- camino_Q06.txt: la secuencia de nodos del camino mas corto de Q06
- subgrafo_caminos.txt: el subgrafo inducido por Q01 y Q06
- analisis_subgrafo.txt: el mst y el analisis dag del subgrafo
- tiempos.csv: cuanto tardo cada modulo
