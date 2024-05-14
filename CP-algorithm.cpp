
#include <random>
#include <iostream>
#include <algorithm>
#include "utils_CP.hpp"

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
    vector<Node *> Tk;
    for (int j = 0; j < Fk.size(); j++)
        Tk.push_back(CPalgorithm(Fk[j], B));

    // Paso 7: 
    vector<int> indices_to_remove;
    vector<pair<Node*, Point>> new_entries;

    for (int j = 0; j < Tk.size(); j++)
    {
        if (Tk[j]->entries.size() < b)
        {
            vector<entry> Tkj = Tk[j]->entries;
            indices_to_remove.push_back(j);
            for (int i = 0; i < Tkj.size(); i++)
            {
                Node *thisSubTree = Tkj[i].a;
                if (thisSubTree != nullptr) new_entries.push_back(make_pair(thisSubTree, Tkj[i].p));
            }
        }
    }
    for (auto &i : indices_to_remove)
    {
        Tk.erase(Tk.begin() + indices_to_remove[i]);
        F.erase(F.begin() + indices_to_remove[i]);
    }
    for (auto &entry : new_entries)
    {
        Tk.push_back(entry.first);
        F.push_back(entry.second);
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