#include "Cluster.hpp"

/* FUNCIONES AUXILIARES PARA FUNCION CLUSTER */

/*
Retorna el par más cercano en el cluster C.
Par más cercano: c1 y c2 tq dist(c1,c2) <= dist(ci,cj) para todo i,j
*/
pair<ClusterT, ClusterT> closestPair(vector<ClusterT> C){}

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
pair<ClusterT, ClusterT> minMaxDivide(ClusterT c){}


/* FUNCIONES AUXILIARES PARA FUNCION OUTPUTHOJA */

/*
Obtiene el medoide primario de un set de puntos.
*/
Point getPrimaryMedoid(vector<Point> C){}