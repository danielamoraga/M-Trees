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
      double distance = dist(C[i].medoid, C[j].medoid);
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
void clusterAdd(ClusterT c, vector<ClusterT>& C){}

/*
Remueve el cluster c del vector de clusters C
*/
void clusterRemove(ClusterT c, vector<ClusterT>& C){}

/*
Divide un cluster usando MinMax split policy.

MinMax split policy: Se considera todos los posibles pares de
puntos, y alternadamente se van agregando el punto más cercano a alguno de estos centros
(esto garantiza que la división sea balanceada) y se calcula el radio cobertor máximo entre estos
dos grupos resultantes. Esto se prueba para todo par de puntos y se elige el par que tenga el
mínimo radio cobertor máximo.
*/
pair<ClusterT, ClusterT> minMaxDivide(ClusterT c){
  int n = c.points.size();

  pair<ClusterT, ClusterT> dividedClusters;
  double minMaxRadius = numeric_limits<double>::max(); // Inicializamos con el máximo valor posible

  // Iterar sobre todos los pares de puntos y encontrar el par con el mínimo radio cobertor máximo
  for (int i = 0; i < n; ++i) {
      for (int j = i + 1; j < n; ++j) {
        // Creamos dos clusters temporales con estos dos puntos
        ClusterT cluster1;
        cluster1.points.push_back(c.points[i]);
        ClusterT cluster2;
        cluster2.points.push_back(c.points[j]);

        // Agregar alternadamente el punto más cercano a alguno de estos centros
        for (int k = 0; k < n; ++k) {
          if (k != i && k != j) {
            if (dist(c.points[k], c.points[i]) < dist(c.points[k], c.points[j])) {
              cluster1.points.push_back(c.points[k]);
            } else {
              cluster2.points.push_back(c.points[k]);
            }
          }
        }

        // Calcular el radio cobertor máximo entre los dos grupos resultantes
        int maxRadius = max(calculateCoveringRadius(cluster1), calculateCoveringRadius(cluster2));

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




/* FUNCIONES AUXILIARES PARA FUNCION OUTPUTHOJA */

/*
Obtiene el medoide primario de un set de puntos.
*/
Point getPrimaryMedoid(vector<Point> C){}