#include "utils_SS.hpp"


// Medoide: punto de un cluster que se elige como candidato a "centro" de una bola. Se caracteriza porque es el punto que genera el menor radio para un cluster.
// Medoide primario: un cluster tiene varios medoides. El primario se escoge según distintas estrategias.

/*
Cluster: retorna vector de clusters de tamaño entre b y B.
input: vector de puntos de tamaño mínimo b
*/
vector<ClusterT> Cluster(vector<Point> Cin, int B){

  // c = cluster
  // C = vector de clusters

  // 1. Se define Cout = {} y C = {}
  vector<ClusterT> Cout = {};
  vector<ClusterT> C = {};

  // 2. Por cada punto p ∈ Cin se añade {p} a C.
  for (Point p : Cin){
    ClusterT singleton;
    singleton.points = {p};
    singleton.medoid = p;
    clusterAdd(singleton, C);
  }

  // 3. Mientras |C| > 1:
  while (C.size() > 1){
    // 3.1 Sea c1, c2 los pares más cercanos de clusters en C tal que |c1| ≥ |c2|.
    pair<ClusterT, ClusterT> closest_pair = closestPair(C);
    ClusterT c1 = closest_pair.first;
    ClusterT c2 = closest_pair.second;

    // 3.2 Si |c1 ∪ c2| ≤ B, se remueve c1 y c2 de C y se añade c1 ∪ c2 a C.
    ClusterT c1_u_c2 = c1.cUnion(c2);
    if(c1_u_c2.getCardinality() <= B) {

      clusterRemove(c1, C);
      clusterRemove(c2, C);

      clusterAdd(c1_u_c2, C);
    }

    // 3.2 Si no, se remueve c1 de C y se añade c1 a Cout.
    else{
      clusterRemove(c1, C);
      clusterAdd(c1, Cout);
    }
  }

  // 4. Sea c el último elemento de C
  ClusterT c = C[0];

  // 5. Si |Cout| > 0:
  ClusterT c_p; // c′
  if (Cout.size() > 0){
    // 5.1 definimos c′ como el vecino más cercano a c en Cout. Removemos c′ de Cout
    c_p = c.closestNeighbour(Cout);
    clusterRemove(c_p, Cout);
  }

  // 5.2 Si no, se define c′ = {}.
  else{
    c_p = {};
  }

  // 6. Si |c ∪ c′| ≤ B:
  ClusterT c_u_cp = c.cUnion(c_p);
  if (c_u_cp.getCardinality() <= B){
    // 6.1 Añadimos c ∪ c′ a Cout.
    clusterAdd(c_u_cp, Cout);
  }

  // 6.2 Si no, dividimos c ∪ c′ en c1 y c2 usando MinMax split policy. Se añaden c1 y c2 a Cout.
  else{
    pair<ClusterT, ClusterT> c1_c2 = minMaxDivide(c_u_cp);
    ClusterT c1 = c1_c2.first;
    ClusterT c2 = c1_c2.second;
    clusterAdd(c1, Cout);
    clusterAdd(c2, Cout);
  }

  // 7. Se retorna Cout
  return Cout;
}

/*
OutputHoja: Retorna tupla (g, r, a) donde g es el medoide primario de Cin, r es radio cobertor y a la direccion del hijo respectivo.
Input: Cin
*/
tuple<Point, int, Node*> OutputHoja(vector<Point> Cin){

  // 1. Sea g el medoide primario de Cin. Sea r = 0. Sea C = {} (el que corresponderá al nodo hoja).
  Point g = getPrimaryMedoid(Cin);
  int r = 0;
  Node C;
  C.nodes = {};

  // 2. Por cada p ∈ Cin: Añadimos (p, null, null) a C. Seteamos r = max(r, dist(g, p))
  for (Point p : Cin){
    // Añadimos (p, null, null) a C
    // Seteamos r = max(r, dist(g, p))
  }

  // 3. Guardamos el puntero a C como a
  Node* a;

}

// notas:
//  m, r, a -> vector de vector de entradas
//  para entry usar shared_ptr, para no tener problemas de memoria (en caso de que hayan problemas de memoria)
//  funcion AlgoritmoSS -> s: vector de entradas