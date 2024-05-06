#include "Structures.hpp"

// Medoide: punto de un cluster que se elige como candidato a "centro" de una bola. Se caracteriza porque es el punto que genera el menor radio para un cluster.
// Medoide primario: un cluster tiene varios medoides. El primario se escoge según distintas estrategias.

/*
Retorna el par más cercano en el cluster C
*/
pair<vector<Point>, vector<Point>> closestPair(vector<Point> C){}

/*
Retorna la unión entre 2 clusters.
*/
vector<Point> clusterUnion(vector<Point> c1, vector<Point> c2){}

/*
Añade los elementos de c en target
*/
void clusterAdd(vector<Point> c, vector<Point>& target){}
/*

Cluster: retorna vector de clusters de tamaño entre b y B.
input: vector de puntos de tamaño mínimo b
*/
vector<Point> Cluster(vector<Point> Cin, int B){

  // 1. Se define Cout = {} y C = {}
  vector<Point> Cout = {};
  vector<Point> C = {};

  // 2. Por cada punto p ∈ Cin se añade {p} a C.
  for (Point p : Cin){
    C.push_back(p);
  }

  // 3. Mientras |C| > 1:
  while (C.size()>1){
    // 3.1 Sea c1, c2 los pares más cercanos de clusters en C tal que |c1| ≥ |c2|.
    // Par más cercano: c1 y c2 tq dist(c1,c2) <= dist(ci,cj) para todo i,j
    pair<vector<Point>, vector<Point>> closest_pair;
    closest_pair = closestPair(C);
    vector<Point> c1 = closest_pair.first;
    vector<Point> c2 = closest_pair.second;

    // 3.2 Si |c1 ∪ c2| ≤ B, se remueve c1 y c2 de C y se añade c1 ∪ c2 a C.
    vector<Point> cluster_union = clusterUnion(c1, c2);
    auto it_c1 = find(C.begin(), C.end(), c1);
    auto it_c2 = find(C.begin(), C.end(), c2);
    if(cluster_union.size()<=B) {
      C.erase(it_c1);
      C.erase(it_c2);

      clusterAdd(cluster_union, C);
    }

    // 3.2 Si no, se remueve c1 de C y se añade c1 a Cout.
    else{
      C.erase(it_c1);
      clusterAdd(c1, Cout);
    }
  }

  // Sea c el último elemento de C
  Point c = C[0];

  // Si |Cout| > 0:
  if (size(Cout) > 0){
    // 5.1 definimos c′ como el vecino más cercano a c en Cout. Removemos c′ de Cout

    // 5.2 Si no, se define c′ = {}.
  }

  // 6. Si |c ∪ c′| ≤ B:

    // 6.1 Añadimos c ∪ c′ a Cout.

    // 6.2 Si no, dividimos c ∪ c′ en c1 y c2 usando MinMax split policy. Se añaden c1 y c2 a Cout.

  // 7. Se retorna Cout
  return Cout;
}