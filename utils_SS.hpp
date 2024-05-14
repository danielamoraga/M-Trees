#include "Cluster.hpp"
#include <cfloat>
// Función para comparar dos puntos según su coordenada X
int compareX(const void* a, const void* b) {
    Point *p1 = (Point *)a, *p2 = (Point *)b;
    return (p1->x - p2->x);
}

// Función para comparar dos puntos según su coordenada Y
int compareY(const void* a, const void* b) {
    Point *p1 = (Point *)a, *p2 = (Point *)b;
    return (p1->y - p2->y);
}

// Función de utilidad para encontrar la distancia más pequeña entre dos puntos en un conjunto de puntos
double bruteForce(Point P[], int n) {
    double min = DBL_MAX;
    for (int i = 0; i < n; ++i)
        for (int j = i+1; j < n; ++j)
            if (dist(P[i], P[j]) < min)
                min = dist(P[i], P[j]);
    return min;
}

double min(double x, double y) {
    return (x < y) ? x : y;
}

// Función para encontrar la distancia más cercana entre los puntos en la tira de tamaño dado.
// Todos los puntos en la tira[] están ordenados según la coordenada Y.
// Todos los puntos en la tira[] tienen un límite superior en la distancia mínima como d.
// Esta función se ejecuta en O(n) tiempo.
double stripClosest(Point strip[], int size, double d) {
    double min = d; // Inicializar la distancia mínima como d

    // Ordenar la tira[] de acuerdo con la coordenada Y
    qsort(strip, size, sizeof(Point), compareY);

    // Elegir todos los puntos uno por uno y probar los siguientes puntos
    // hasta que la diferencia entre las coordenadas Y sea menor que d.
    // Esto es probado y probado método.
    // Se demuestra que este bucle se ejecuta como máximo 6 veces.
    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size && (strip[j].y - strip[i].y) < min; ++j) {
            if (dist(strip[i], strip[j]) < min) {
                min = dist(strip[i], strip[j]);
            }
        }
    }

    return min;
}


// Función para encontrar la distancia más cercana entre los puntos de un conjunto ordenado por coordenadas X
double closestUtil(Point P[], int n) {
    // Si hay 2 o 3 puntos, usar fuerza bruta
    if (n <= 3)
        return bruteForce(P, n);

    // Encontrar el punto medio
    int mid = n/2;
    Point midPoint = P[mid];

    // Calcular la distancia más pequeña a la izquierda y a la derecha del punto medio
    double dl = closestUtil(P, mid);
    double dr = closestUtil(P + mid, n - mid);

    // Encontrar la distancia más pequeña entre las dos mitades
    double d = min(dl, dr);

    // Construir un conjunto de puntos strip que contenga puntos cercanos (más cercanos que d) a la línea vertical que pasa por el punto medio
    Point strip[n];
    int j = 0;
    for (int i = 0; i < n; i++)
        if (abs(P[i].x - midPoint.x) < d)
            strip[j] = P[i], j++;

    // Encontrar los puntos más cercanos en la tira y devolver la distancia más pequeña entre d y la distancia más pequeña en la tira
    return min(d, stripClosest(strip, j, d));
}




/* FUNCIONES AUXILIARES PARA FUNCION CLUSTER */

/*
Retorna el par más cercano en el vector de clusters C.
Par más cercano: c1 y c2 tq dist(c1,c2) <= dist(ci,cj) para todo i,j
*/
pair<ClusterT, ClusterT> closestPair(vector<ClusterT>& C) {
    // Convertir los clusters a puntos
    vector<Point> points;
    for (ClusterT& cluster : C) {
        Point medoid = cluster.getMedoid();
        points.push_back(medoid);
    }

    // Ordenar los puntos por su coordenada X
    qsort(&points[0], points.size(), sizeof(Point), compareX);

    // Llamar a la función recursiva closestUtil
    double minDistance = closestUtil(&points[0], points.size());

    // Encontrar los clusters correspondientes al par más cercano
    ClusterT c1, c2;
    for (int i = 0; i < points.size() - 1; ++i) {
        for (int j = i + 1; j < points.size(); ++j) {
            if (dist(points[i], points[j]) == minDistance) {
                c1 = C[i];
                c2 = C[j];
                break;
            }
        }
    }

    return make_pair(c1, c2);
}



/*
Añade el cluster c al vector de clusters C
*/
void clusterAdd(ClusterT c, vector<ClusterT>& C){
  C.push_back(c);
}

/*
Remueve el cluster c del vector de clusters C
*/
void clusterRemove(ClusterT c, vector<ClusterT>& C){
  auto it = find(C.begin(), C.end(), c);
  if (it != C.end()) {
    C.erase(it);
  }
}

/*
Divide un cluster usando MinMax split policy.

MinMax split policy: Se considera todos los posibles pares de
puntos, y alternadamente se van agregando el punto más cercano a alguno de estos centros
(esto garantiza que la división sea balanceada) y se calcula el radio cobertor máximo entre estos
dos grupos resultantes. Esto se prueba para todo par de puntos y se elige el par que tenga el
mínimo radio cobertor máximo.
*/
pair<ClusterT, ClusterT> minMaxDivide(ClusterT c){
  int n = c.getCardinality();

  pair<ClusterT, ClusterT> dividedClusters;
  double minMaxRadius = numeric_limits<double>::max(); // Inicializamos con el máximo valor posible

  // Iterar sobre todos los pares de puntos y encontrar el par con el mínimo radio cobertor máximo
  for (int i = 0; i < n; ++i) {
      for (int j = i + 1; j < n; ++j) {

        // Creamos dos clusters temporales con estos dos puntos
        ClusterT cluster1;
        cluster1.addPoint(c.getPointByIndex(i));
        ClusterT cluster2;
        cluster2.addPoint(c.getPointByIndex(j));

        // Agregar alternadamente el punto más cercano a alguno de estos centros
        for (int k = 0; k < n; ++k) {
          if (k != i && k != j) {
            if (dist(c.getPointByIndex(k), c.getPointByIndex(i)) < dist(c.getPointByIndex(k), c.getPointByIndex(j))) {
              cluster1.addPoint(c.getPointByIndex(k));
            } else {
              cluster2.addPoint(c.getPointByIndex(k));
            }
          }
        }

        // Calcular el radio cobertor máximo entre los dos grupos resultantes
        cluster1.computeRadius();
        cluster2.computeRadius();
        double maxRadius = max(cluster1.getRadius(), cluster2.getRadius());

        // Actualizar si encontramos un mínimo
        if (maxRadius < minMaxRadius) {
          minMaxRadius = maxRadius;
          dividedClusters = make_pair(cluster1, cluster2);
        }
      }
  }

  // Retornar el par de clusters divididos
  return dividedClusters;
}

/*
Obtiene el medoide primario de un set de puntos.
*/
Point getPrimaryMedoid(vector<Point> C){
  double minAvgDistance = numeric_limits<double>::max();
  Point minMedoid;

  for (const Point& p : C) {
    double totalDistance = 0.0; // Inicializar la suma de las distancias a este punto

    for (const Point& q : C) {
      totalDistance += dist(p, q); // Calcular la distancia de este punto a todos los demás
    }

    double avgDistance = totalDistance / C.size(); // Calcular la distancia promedio a este punto

    // Actualizar el medoide mínimo si la distancia promedio es menor
    if (avgDistance < minAvgDistance) {
      minAvgDistance = avgDistance;
      minMedoid = p;
    }
  }

  // Retornar el medoide mínimo encontrado
  return minMedoid;
}