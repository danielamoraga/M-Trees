#include "Structures.hpp"

#include <random>
#include <iostream>
#include <algorithm>

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
        Node *T = newNode(P); // el árbol contiene a todos los puntos
        return T;
    }

    vector<Point> F; // conjunto de samples

    vector<vector<Point>> Fk = steps_2_to_5(P, b, B, F); // crea el conjunto de conjuntos Fk y modifica F

    // se realiza recursivamente el algoritmo CP en cada Fk[j] obteniendo el árbol Tj
    vector<Node*> Tk;
    for (int j=0; j<Fk.size(); j++) {
        Node* Tj = CPalgorithm(Fk[j], B);
        Tk.push_back(Tj);

        if (Tj->nodes.size() < b) { // si la raíz de Tj es de tamaño menor a b
            vector<Node *> Tj_nodes = delRoot(Tj); // se quita esa raíz
            F.erase(F.begin() + j); // se elimina pfj de F
            for (int i=0; i<Tj_nodes.size(); i++) {
                Tk.push_back(Tj_nodes[i]); // se trabaja con sus subárboles nuevos Tj,...Tj+p-1
                F.push_back(Tj[i].entrada.p); // se añaden los puntos pertinentes a F (p de la entrada del nodo asumiendo que solo tiene una entrada)
            }
        }
    }

    balancing(Tk, F);

    // se define Tsup como el resultado de la llamada al algoritmo CP aplicado a F
    Node* Tsup = CPalgorithm(F, B);

    // se une cada Tj en T' a su hoja en Tsup correspondiente al punto pfj en F, obteniendo un nuevo árbol T

    // se setean los radios cobertores resultantes para cada entrada en este árbol.

    // se retorna T
    // return T;
}

/* FUNCIONES PARA EL ALGORITMO */

Node* newNode(vector<Point> P) {
    /* Crea un nodo vacío con las entradas P y devuelve un puntero a él */
    Node* T = new Node;
    for(int i=0; i<P.size(); i++)
        T->nodes[i]->entrada.p = P[i];
    return T;
}

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
// Modifica Fk haciendo que sea un conjunto de k conjuntos de puntos
// donde  Fk[k] contiene todos los p que tienen a F[k] como más cercano
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
// asignación de los puntos
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

/*  Paso 9: Balanceamiento */
// calcula altura mínima
int minHeight(vector<Node*> Tk) {
    int h_min = Tk[0]->height();
    for (int k=1; k<Tk.size(); k++) {
        int h_min = min(h_min, Tk[k]->height());
    }
}
// balancea el árbol y modifica F
void balancing(vector<Node*> Tk, vector<Point> F){
    // h_min: altura mínima de de los árboles Tj
    int h_min = minHeight(Tk);

    // M: conjunto inicialmente vacío
    vector<Node*> M;

    // por cada Tj, si su altura es igual a h, se añade a M
    for (int i=0; i<Tk.size(); i++) {
        if (Tk[i]->height() == h_min)  
            M.push_back(Tk[i]);
        else { // sino, 
            F.erase(F.begin() + i); // se borra el punto pertinente en F
            // se hace una búsqueda exhaustiva en Tj de todos sus subárboles de altura igual a h y se insertan en M
            // y se insertan los puntos raíz de T1',...,Tp',pf1',...,pfp' en F
            for (int j=0; j<Tk[i]->nodes.size(); j++) {
                balancing(Tk[i]->nodes, F);
            }
        }
    }
}