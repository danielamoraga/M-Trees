#include "Structures.hpp"
#include <random>
#include <iostream>

/*  Algoritmo Ciaccia-Patella:
    Realiza Clustering de n puntos P={p1,...pn}
    obteniendo un M-tree balanceado T que cumple
    con tener cada nodo dentro de las capacidades
    permitidas.
    Input: Un set de puntos P
*/
Node* CPalgorithm(vector<Point> P, int n, int B, int b){
    
    // 1.
    if (P.size() <= B) { // si el tamaño de P es menor o igual que B
        Node *T = new Node(); // se crea un árbol T
        for (int i=0;i < P.size();++i) { // se insertan todos los puntos a T
            T->p.push_back(P[i]);
        }
        return T; // y se retorna T
    }
    
    // 2. 
    vector<Point> F; // conjunto de samples
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distr(0,P.size());
    int K = min(B, n/B);
    for (int i=0; i<K; i++) { // creamos k=min(B,n/B) samples de manera aleatoria
        Point pf;
        pf.x = P[distr(gen)].x;
        pf.y = P[distr(gen)].y;
        F.push_back(pf); // vamos insertando las samples en F
    }

    // 3.
    // se le asigna a cada punto en P su sample más cercano
    // construyendo K conjuntos
    vector<vector<pair<Point, Point>>> Fk; // conjunto de los conjuntos de pares (p_i, f_k)
    for (int i=0; i<P.size(); i++) {
        pair<Point,Point> closestPair = closestPoints(P[i], F, Fk);
    }

    // 4. etapa de redistribución
    for (int j=0; j<K; j++){
        int size = Fk[j].size();
        if(size < b) { // si algún Fk tiene tamaño menor a b, quitamos F[j] de F
            F.erase(F.begin() + j);
            for(int i=0; i<Fk[j].size(); i++) { // por cada p en Fk[j]
                Point p = Fk[j][i].first;
                pair<Point,Point> closestPair = closestPoints(p, F, Fk);
            }
        }
    }
}

pair<Point,Point> closestPoints(Point p, vector<Point> F, vector<vector<pair<Point,Point>>> Fk) {
    double distance = dist(p,F[0]);
    Point closest = F[0];
    pair<Point,Point> closestPair;
    int indexClosest = 0;
    for (int k=0; k<F.size(); k++) {
        if (dist(p, F[k]) < distance) {
            distance = dist(p,F[k]);
            closest = F[k];
            indexClosest = k;
        }
        closestPair = {p,closest}; // {p_i, f_i}
    }
    Fk[indexClosest].push_back(closestPair); //Fk[k] contiene todos los pares {P[i], F[k]}
    return closestPair;
}