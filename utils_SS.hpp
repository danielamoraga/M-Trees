#include "Cluster.hpp"

/* FUNCIONES AUXILIARES PARA FUNCION CLUSTER */

/*
Retorna el par más cercano en el cluster C.
Par más cercano: c1 y c2 tq dist(c1,c2) <= dist(ci,cj) para todo i,j
*/
pair<ClusterT, ClusterT> closestPair(vector<ClusterT> C){
  ClusterT closest1, closest2;

  double minDistance = numeric_limits<double>::max();

  for (int i = 0; i < C.size(); ++i) {
    for (int j = i + 1; j < C.size(); ++j) {
      double distance = dist(C[i].getMedoid(), C[j].getMedoid());
      if (distance < minDistance) {
        minDistance = distance;
        closest1 = C[i];
        closest2 = C[j];
      }
    }
  }

  return make_pair(closest1, closest2);
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