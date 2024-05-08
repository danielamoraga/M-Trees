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
    ClusterT cUnion(ClusterT c){}

    /*
    Dado un vector de clusters, retorna el vecino más cercano a este cluster.

    Dado un cluster c, su vecino más cercano es otro cluster c′ tal que no hay otro cluster que
    su distancia a c sea menor a la distancia entre c y c′ (se puede tener múltiples vecinos más
    cercanos).
    */
    ClusterT closestNeighbour(vector<ClusterT> C){}


};