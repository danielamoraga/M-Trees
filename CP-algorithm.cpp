#include "Structures.hpp"

#include <random>
#include <iostream>

/*  Algoritmo Ciaccia-Patella:
    Realiza Clustering de n puntos P = {p1, ..., pn}
    obteniendo un M-tree balanceado T que cumple
    con tener cada nodo dentro de las capacidades
    permitidas.
    Input: Un set de puntos P
*/
Node* CPalgorithm(vector<Point> P, int B){
    double b = 0.5 * B;

    if (P.size() <= B) { // si el tamaño de P es menor o igual que B
        Node *T = newLeaf(P); // el árbol es una hoja que contiene a todos los puntos
        return T;
    }
    vector<Point> F; // conjunto de samples
    vector<vector<Point>> Fk = steps_2_to_5(P, b, B, F);

    // se realiza recursivamente el algoritmo CP en cada Fk[j] obteniendo el árbol Tj
    // -> genera k árboles Tj ?
    for (int j=0; j<Fk.size(); j++) {
        vector<Node*> Tk;
        Node* Tj = CPalgorithm(Fk[j], B);
        Tk.push_back(Tj);
    }

    // si la raíz de T es de tamaño menor a b
    // -> se quita esa raíz
    // se elimina pfj de F
    // se trabaja con sus subárboles como nuevos Tj,...,Tj+p-1
    
}

/*  Modifica Fk haciendo que sea un conjunto de k conjuntos de puntos
    donde  Fk[k] contiene todos los p que tienen a F[k] como más cercano*/
void closest(vector<Point> P, vector<Point> F, vector<vector<Point>> Fk) {
    for (int i=0; i<P.size(); i++) {
        double distance = dist(P[i], F[0]);
        Point closest = F[0];
        int indexClosest = 0;
        for (int k=0; k<F.size(); k++) {
            if (dist(P[i], F[k]) < distance) {
                distance = dist(P[i], F[k]);
                closest = F[k];
                indexClosest = k;
            }
        }
        Fk[indexClosest].push_back(P[i]);
    }
}

/* PASOS DEL ALGORITMO */

/*  Paso 2:
    Elige k puntos aleatorios de P que inserta en un conjunto F*/
void k_samples(vector<Point> P, int k, int n, int B, vector<Point> F) {
    F.clear(); // vacía F para generarlo de nuevo
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distr(0,n);
    int K = min(B, n/B);
    for (int i=0; i<K; i++) { // creamos k=min(B,n/B) samples de manera aleatoria
        Point pf;
        int random_index = distr(gen);
        pf.x = P[random_index].x;
        pf.y = P[random_index].y;
        F.push_back(pf); // vamos insertando los samples en F
    }
}

/*  Paso 3:
    Asigna a cada punto de P su sample más cercano de F entregando k conjuntos
    (crea Fk) */
vector<vector<Point>> assignation(vector<Point> P, vector<Point> F) {
    vector<vector<Point>> Fk; // conjunto de los conjuntos de pares (p_i, f_k)
    closest(P, F, Fk);
    return Fk;
}

/*  Paso 4:
    Si un Fk[j] es tq Fk[j].size < b
    * Quita F[j] de F
    * Por cada p en Fk[j] busca su sample F[l] más cercano de F y lo añadimos a Fk[l] */
void redistribution(vector<vector<Point>> Fk, vector<Point> F, double b, int k) {
    for (int j=0; j<k; j++){
        int size = Fk[j].size();
        if(size < b) { // si algún Fk tiene tamaño menor a b, quitamos F[j] de F
            F.erase(F.begin() + j); // quitamos el elemento F[j] del conjunto F
            closest(Fk[j], F, Fk); // buscamos el sample más cercano de F añadiendolo a su Fk correspondiente
        }
    }
}

/*  Paso 5:
    Si el tamaño de F es 1 volvemos al paso 2
    -> Haremos una función recursiva con los pasos del 2 al 5 */
vector<vector<Point>> steps_2_to_5(vector<Point> P, double b, int B, vector<Point> F) {
    int n = P.size();
    int k = min(B, n/B);
    k_samples(P, k, n, B, F); // step 2
    vector<vector<Point>> Fk = assignation(P, F); // step 3
    redistribution(Fk, F, b, k); // step 4
    if (F.size()==1) { // step 5
        steps_2_to_5(P, b, B, F);
    } else {
        return Fk;
    }
}