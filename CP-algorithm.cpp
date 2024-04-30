#include "Structures.hpp"
#include <random>

/*  Algoritmo Ciaccia-Patella:
    Realiza Clustering de n puntos P={p1,...pn}
    obteniendo un M-tree balanceado T que cumple
    con tener cada nodo dentro de las capacidades
    permitidas.
    Input: Un set de puntos P
*/
Node* CPalgorithm(vector<Point> P, int n, int B){
    
    // 1.
    // se insertan todos los puntos a T y se retorna T
    if (P.size() <= B) { // si el tamaño de P es menor o igual que B
        Node *T = new Node(); // se crea un árbol T
        for (int i=0;i < P.size();++i) { // se insertan todos los puntos a T
            Node *p_i = new Node();
            p_i->p = P[i];
            T->a.push_back(p_i); // esto no sé, porque a no es un vector :c
        }
        return T; // y se retorna T
    }
    
    // 2. 
    vector<Point> F; // conjunto de samples
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distr(0,P.size());
    int K = min(n, n/B);
    for (int i=0; i<K; i++) { // creamos k=min(B,n/B) samples de manera aleatoria
        Point pf;
        pf.x = P[distr(gen)].x;
        pf.y = P[distr(gen)].y;
        F.push_back(pf); // vamos insertando las samples en F
    }

    // 3.
    // se le asigna a cada punto en P su sample más cercano
    // construyendo k conjuntos
    vector<vector<pair<Point, Point>>> Fk; // vector que contiene los vectores de pares (p_i, f_k)
    for (int i=0; i<P.size(); i++) {
        double distance = dist(P[i],F[0]);
        Point closest = F[0];
        pair<Point,Point> closestPair;
        for (int k=0; k<F.size(); k++) {
            if (dist(P[i], F[k]) < distance) {
                distance = dist(P[i],F[k]);
                closest = F[k];
            }
            closestPair = {P[i],closest};
            Fk[k].push_back(closestPair);
        }
    }

    // 4.
}