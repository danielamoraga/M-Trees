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
  double r = 0;
  Node C;
  C.nodes = {};

  // 2. Por cada p ∈ Cin: Añadimos (p, null, null) a C.
  for (Point p : Cin){
    // Añadimos (p, null, null) a C
    entry e = {p, NULL, NULL};
    // C.entradas.push_back(entry);

    // Seteamos r = max(r, dist(g, p))
    r = max(r, dist(g, p));
  }

  // 3. Guardamos el puntero a C como a
  Node* a = &C;

  // 4. Retornamos (g, r, a)
  return make_tuple(g, r, a);
}

/*
OutputInterno: Retorna (G, R, A) donde G es el medoide primario del conjunto de puntos
Cin = {g|∃(g, r, a) ∈ Cmra}, R el radio cobertor, y A la dirección del hijo respectivo.

Input: Cmra, un conjunto de tuplas (g, r, a) retornadas por OutputHoja
*/
tuple<Point, int, Node*> OutputInterno(vector<tuple<Point, int, Node*>> Cmra){
  // 1. Sea Cin = {g|∃(g, r, a) ∈ Cmra}. G el medoide primario de Cin. Sea R = 0. Sea C = {} (el que corresponderá a un nodo interno).
  vector<Point> Cin;
  for(auto& tpl : Cmra) {
      Cin.push_back(get<0>(tpl));
  }
  Point G = getPrimaryMedoid(Cin);
  double R = 0;
  Node* C = new Node();

  // 2. Por cada (g, r, a) ∈ Cmra: Añadir (g, r, a) a C. Se setea R = max(R, dist(G, g) + r)
  for(auto& tpl : Cmra) {
    Point g = get<0>(tpl);
    double r = get<1>(tpl);
    Node* a = get<2>(tpl);
    C->nodes.push_back(a); // Agregar el hijo al nodo interno
    R = max(R, dist(G, g) + r);
  }
  // 3. Guardamos el puntero a C como A.
  Node* A = C;

  // 4. Retornamos (G, R, A)
  return make_tuple(G, R, A);

}

/*
AlgoritmoSS: retorna la raíz del M-tree construído.

Input: Cin, un conjunto de puntos
*/
Node* SSAlgorithm(vector<Point> Cin, int B){

  // 1. Si |Cin| ≤ B: Se define (g, r, a) = OutputHoja(Cin) y se retorna a
  if (Cin.size() <= B){
    tuple<Point, int, Node*> g_r_a =  OutputHoja(Cin);
    Node* a = get<2>(g_r_a);
    return a;
  }

  // 2. Sea Cout = Cluster(Cin). Sea C = {}.
  vector<ClusterT> Cout = Cluster(Cin, B);
  vector<tuple<Point, int, Node*>> C;

  // 3. Por cada c ∈ Cout: Se añade OutputHoja(c) a C
  for (ClusterT c: Cout){
    C.push_back(OutputHoja(c.points));
  }

  // 4. Mientras |C| > B:
  while (C.size() > B){
    // 4.1 Sea Cin = {g|(g, r, a) ∈ C}. Sea Cout = Cluster(Cin). Sea Cmra = {}
    vector<Point> Cin;
    for(auto& tpl : C) {
        Cin.push_back(get<0>(tpl));
    }
    vector<ClusterT> Cout = Cluster(Cin, B);
    vector<tuple<Point, int, Node*>> Cmra;

    // 4.2 Por cada c ∈ Cout: Sea s = {(g, r, a)|(g, r, a) ∈ C ∧ g ∈ c}, se añade s a Cmra
    for (ClusterT c: Cout){
      vector<tuple<Point, int, Node*>> s;
      for(auto& tpl : C) {
        Point g = get<0>(tpl);
        if (find(c.points.begin(), c.points.end(), g) != c.points.end()) {
          s.push_back(tpl);
        }
      }
      Cmra.insert(Cmra.end(), s.begin(), s.end());
    }

    // 4.3 Sea C = {}.

    // 4.4 Por cada s ∈ Cmra: Añadir OutputInterno(s) a C
  }

  // 5. Sea (g, r, a) = OutputInterno(C)

  // 6. Se retorna a

}


// notas:
//  m, r, a -> vector de vector de entradas
//  para entry usar shared_ptr, para no tener problemas de memoria (en caso de que hayan problemas de memoria)
//  funcion AlgoritmoSS -> s: vector de entradas