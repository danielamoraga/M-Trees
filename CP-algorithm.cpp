
#include <random>
#include <iostream>
#include <algorithm>
#include "utils_CP.hpp"
#include <set>

/*  Algoritmo Ciaccia-Patella:
    Realiza Clustering de n puntos P = {p1, ..., pn}
    obteniendo un M-tree balanceado T que cumple
    con tener cada nodo dentro de las capacidades
    permitidas.
    Input: Un set de puntos P
*/
Node *CPalgorithm(vector<Point> P, int B)
{
    double b = 0.5 * B;

    if (P.size() <= B) // Paso 1: si n es menor o igual a B
    {
        Node *T = newNode(P); // se crea un árbol T y se insertan todos los puntos en él
        return T; // se retorna T
    }

    vector<Point> F;
    vector<vector<Point>> Fk;

    while (true) {
        k_samples(P, B, F); // Paso 2: conjunto de samples de P
        Fk = assignation(P, F); // Paso 3: crea el conjunto de conjuntos Fk
        redistribution(Fk, F, b); // Paso 4: redistribuye los valores de Fk y modifica F
        if (F.size() == 1) continue; // Paso 5: si |F|=1, vuelve al paso 2
        else break;
    }

    // Paso 6: se realiza recursivamente el algoritmo CP en cada Fk[j] obteniendo el árbol Tj
    vector<Node*> Tk;
    for (int j = 0; j < Fk.size(); j++) {
        Node* Tj = CPalgorithm(Fk[j], B);
        Tk.push_back(Tj);
    }

    // Paso 7:
    for (Node* Tj: Tk) {
        // Si la raiz del arbol es menor a b
        if (Tj->entries.size() < b){

            auto it = find(Tk.begin(), Tk.end(), Tj);
            int j = it - Tk.begin();
            Tk.erase(it);

            // se elimina pfj de F
            F.erase(F.begin() + j);

            for (entry e: Tj->entries){
                if (e.a != nullptr) {
                    Node* newTj = e.a;
                    Tk.push_back(newTj);

                    // se añaden los puntos pertinentes a F
                    F.push_back(e.p);
                }
            }
        }
    }

    vector<Node *> M;
    int h = minHeight(Tk); // altura mínima de de los árboles Tj
    for (int i = 0; i < Tk.size(); i++)
        balancing(Tk[i], F, M, h, i);

    // se define Tsup como el resultado de la llamada al algoritmo CP aplicado a F
    Node *Tsup = CPalgorithm(F, B);

    // se une cada Tj en M a su hoja en Tsup correspondiente al punto pfj en F, obteniendo un nuevo árbol T
    for (int j = 0; j < M.size(); j++)
    {
        // Punto pfj en F
        Point pfj = F[j];
        // Buscar la hoja correspondiente en Tsup
        for (int i = 0; i < Tsup->entries.size(); i++)
        {
            if (Tsup->entries[i].p == pfj)
            {
                Tsup->entries[i].a = M[j];
                break;
            }
            else
                continue;
        }
    }

    // Paso 12: se setean los radios cobertores resultantes para cada entrada en este árbol.
    Tsup->setCR();

    // se retorna T
    return Tsup;
}