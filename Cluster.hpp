#include "Structures.hpp"

class ClusterT {
  public:
    vector<Point> points;
    int radius;
    Point medoid;

    /*
    Retorna la cantidad de elementos (cardinalidad) de este cluster.
    */
    int getCardinality(){
      return points.size();
    }

    /*
    Retorna la unión entre el cluster actual y c.
    */
    ClusterT cUnion(ClusterT c){
      ClusterT unionCluster;

      // Agregar todos los puntos del cluster actual al nuevo cluster
      for (const Point& p : this->points) {
        unionCluster.points.push_back(p);
      }

      // Agregar todos los puntos del cluster c al nuevo cluster
      for (const Point& p : c.points) {
          unionCluster.points.push_back(p);
      }

      // Actualizar el medoide y el radio del nuevo cluster
      unionCluster.medoid = calcularMedoide(unionCluster.points);
      unionCluster.radius = calcularRadioCobertura(unionCluster.points, unionCluster.medoid);

      // Retornar el cluster que representa la unión
      return unionCluster;
    }

    /*
    Dado un vector de clusters, retorna el vecino más cercano a este cluster.

    Dado un cluster c, su vecino más cercano es otro cluster c′ tal que no hay otro cluster que
    su distancia a c sea menor a la distancia entre c y c′ (se puede tener múltiples vecinos más
    cercanos).
    */
    ClusterT closestNeighbour(vector<ClusterT> C){
      ClusterT closestCluster = C[0];
      double minDistance = numeric_limits<double>::max(); // Inicializamos con el máximo valor posible

      // Iterar sobre todos los clusters en el vector
      for (const ClusterT& cluster : C) {
        if (&cluster == &(*this)) continue; // Evitar comparar con el mismo cluster

        double distance = dist(this->medoid, cluster.medoid); // Calcular distancia entre medoides
        if (distance < minDistance) {
          minDistance = distance;
          closestCluster = cluster;
        }
      }

      return closestCluster;
    }


};