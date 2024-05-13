#include "utils_SS.hpp"


// Medoide: punto de un cluster que se elige como candidato a "centro" de una bola. Se caracteriza porque es el punto que genera el menor radio para un cluster.
// Medoide primario: un cluster tiene varios medoides. El primario se escoge según distintas estrategias.

/*
Cluster: retorna vector de clusters de tamaño entre b y B.
input: vector de puntos de tamaño mínimo b
*/
vector<ClusterT> Cluster(vector<Point> Cin, int B){

  // 1. Se define Cout = {} y C = {}
  vector<ClusterT> Cout = {};
  vector<ClusterT> C = {};

  // 2. Por cada punto p ∈ Cin se añade {p} a C.
  for (Point p : Cin){
    ClusterT singleton({p});
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
entry OutputHoja(vector<Point> Cin){
  // 1. Sea g el medoide primario de Cin. Sea r = 0. Sea C = {} (el que corresponderá al nodo hoja).
  Point g = getPrimaryMedoid(Cin);
  double r = 0;
  Node* C = new Node();

  // 2. Por cada p ∈ Cin: Añadimos (p, null, null) a C.
  for (Point p : Cin){
    // Añadimos (p, null, null) a C
    entry e(p);
    C->entries.push_back(e);

    // Seteamos r = max(r, dist(g, p))
    r = max(r, dist(g, p));
  }

  // 3. Guardamos el puntero a C como a
  Node* a = C;

  // 4. Retornamos (g, r, a)
  entry ret(g,r,a);
  return ret;
}

/*
OutputInterno: Retorna (G, R, A) donde G es el medoide primario del conjunto de puntos
Cin = {g|∃(g, r, a) ∈ Cmra}, R el radio cobertor, y A la dirección del hijo respectivo.

Input: Cmra, un conjunto de tuplas (g, r, a) retornadas por OutputHoja
*/
entry OutputInterno(vector<entry> Cmra){
  // 1. Sea Cin = {g|∃(g, r, a) ∈ Cmra}. G el medoide primario de Cin. Sea R = 0. Sea C = {} (el que corresponderá a un nodo interno).
  vector<Point> Cin;
  for(entry ent : Cmra) {
    Cin.push_back(ent.p);
  }
  Point G = getPrimaryMedoid(Cin);
  double R = 0;
  Node *C = new Node();

  // 2. Por cada (g, r, a) ∈ Cmra: Añadir (g, r, a) a C. Se setea R = max(R, dist(G, g) + r)
  for(entry ent : Cmra) {
    Point g = ent.p;
    double r = ent.cr;
    Node* a = ent.a;
    entry e(g, r, a);
    R = max(R, dist(G, g) + r);
  }

  // 3. Guardamos el puntero a C como A.
  Node* A = C;

  // 4. Retornamos (G, R, A)
  entry ret(G, R, A);
  return ret;

}

/*
AlgoritmoSS: retorna la raíz del M-tree construído.

Input: Cin, un conjunto de puntos
*/
Node* SSAlgorithm(vector<Point> Cin, int B){
  cout << "size Cin " << Cin.size() << endl;


  // 1. Si |Cin| ≤ B: Se define (g, r, a) = OutputHoja(Cin) y se retorna a
  if (Cin.size() <= B){
    entry g_r_a =  OutputHoja(Cin);
    Node* a = g_r_a.a;
    return a;
  }

  // 2. Sea Cout = Cluster(Cin). Sea C = {}.
  cout << "SSA - Paso 2" << endl;
  vector<ClusterT> Cout = Cluster(Cin, B);
  vector<entry> C;
  cout << "size Cout " << Cout.size() << endl;

  // 3. Por cada c ∈ Cout: Se añade OutputHoja(c) a C
  cout << "SSA - Paso 3" << endl;
  for (ClusterT c: Cout){
    C.push_back(OutputHoja(c.getPoints()));
  }
  cout << "size C " << C.size() << endl;

  // 4. Mientras |C| > B:
  cout << "SSA - Paso 4" << endl;
  while (C.size() > B){
    // 4.1 Sea Cin = {g|(g, r, a) ∈ C}. Sea Cout = Cluster(Cin). Sea Cmra = {}
    for(entry ent : C) {
      Cin.push_back(ent.p);
    }
    vector<ClusterT> Cout = Cluster(Cin, B);
    vector<vector<entry>> Cmra;

    // 4.2 Por cada c ∈ Cout: Sea s = {(g, r, a)|(g, r, a) ∈ C ∧ g ∈ c}, se añade s a Cmra
    for (ClusterT c: Cout){
      vector<entry> s;
      for(entry ent : C) {
        Point g = ent.p;
        vector<Point> c_points = c.getPoints();
        if (find(c_points.begin(), c_points.end(), g) != c_points.end()) {
          s.push_back(ent.p);
        }
      }
      Cmra.push_back(s);
    }

    // 4.3 Sea C = {}.
    C.clear();

    // 4.4 Por cada s ∈ Cmra: Añadir OutputInterno(s) a C
    for(vector<entry> s : Cmra) {
      C.push_back(OutputInterno(s));
    }
  }

  // 5. Sea (g, r, a) = OutputInterno(C)
  cout << "SSA - Paso 5" << endl;
  entry g_r_a = OutputInterno(C);

  // 6. Se retorna a
  cout << "SSA - Paso 6" << endl;
  return g_r_a.a;
}


// notas:
//  m, r, a -> vector de vector de entradas
//  para entry usar shared_ptr, para no tener problemas de memoria (en caso de que hayan problemas de memoria)
//  funcion AlgoritmoSS -> s: vector de entradas