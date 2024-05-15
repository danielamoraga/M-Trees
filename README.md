# M-Trees

Este proyecto realiza un experimento para recopilar evidencias empíricas sobre las diferencias de costo al realizar una búsqueda en un M-Tree. El experimento, implementado en [`Main.cpp`](Main.cpp), genera un conjunto de puntos y consultas aleatorias. Luego, construye un M-Tree utilizando el algoritmo Ciaccia-Patella y realiza las consultas en el árbol. Los resultados, que incluyen el número de accesos al árbol y los puntos encontrados para cada consulta, se escriben en un archivo CSV. El objetivo es comparar el rendimiento de los M-Trees construidos con los algoritmos Ciaccia-Patella y Sexton-Swinbank.

## Requisitos

- GCC
- Make

## Estructura del Proyecto

Descripción de los archivos principales y su función.

- `CP-algorithm.cpp`: Este archivo implementa el algoritmo Ciaccia-Patella para realizar clustering de puntos y obtener un M-tree balanceado. Puedes ejecutar este algoritmo con el comando make runCP.
- `SS-algorithm.cpp`: Este archivo implementa el algoritmo SS, que también realiza clustering de puntos y genera un M-tree. Puedes ejecutar este algoritmo con el comando make runSS.
- `Main.cpp`: Este archivo es responsable de escribir los resultados de la experimentación con los algoritmos en un archivo CSV. Puedes ejecutar este archivo con el comando make runWF.

- `Structures.hpp`: Este archivo define las estructuras de datos utilizadas en el proyecto, incluyendo `Point`, `entry`, `Node` y `query`. `Point` representa un punto en el espacio, `entry` es una entrada en un nodo del M-Tree, `Node` representa un nodo en el M-Tree, y `query` representa una consulta de búsqueda en el árbol. También se definen varias funciones auxiliares para operaciones como la distancia euclidiana entre dos puntos y la búsqueda en el árbol.

- `Cluster.hpp`: Este archivo define la clase `ClusterT`, que representa un cluster de puntos. Un cluster tiene un conjunto de puntos, un radio y un medoide. La clase `ClusterT` también proporciona métodos para obtener la cardinalidad del cluster y para unir dos clusters en uno nuevo.

- `utils_CP.hpp` y `utils_SS.hpp`: Estos archivos contienen funciones auxiliares utilizadas por los algoritmos Ciaccia-Patella (`CP-algorithm.cpp`) y Sexton-Swinbank (`SS-algorithm.cpp`) respectivamente. Estas funciones ayudan a implementar los algoritmos de manera más eficiente y legible.

- `Makefile`: Este archivo contiene las instrucciones para compilar y ejecutar los algoritmos y el experimento. Utiliza los comandos make runCP, make runSS, y make runWF para ejecutar cada parte del proyecto.

## Compilación y Ejecución

Para compilar y ejecutar el proyecto, utiliza los siguientes comandos:

### Para ejecutar el algoritmo CP:

```sh
make runCP
```

### Para ejecutar el algoritmo SS:

```sh
make runSS
```

### Para ejecutar el experimento:

```sh
make run
```

## Salida

Los resultados de la ejecución se escribirán en archivos `resultados[n].csv`, siendo [n] la potencia de 2 del tamaño del conjunto de puntos.

## Limpieza

Para limpiar los archivos generados durante la compilación, utiliza el siguiente comando:

```sh
make clean
```
