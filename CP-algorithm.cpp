
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

    if (P.size() <= B)
    {
        Node *T = newNode(P);
        return T;
    }

    vector<Point> F; // conjunto de samples

    vector<vector<Point>> Fk = steps_2_to_5(P, b, B, F); // crea el conjunto de conjuntos Fk y modifica F

    // se realiza recursivamente el algoritmo CP en cada Fk[j] obteniendo el árbol Tj
    vector<Node *> Tk;
    for (int j = 0; j < Fk.size(); j++)
    {
        Node *Tj = CPalgorithm(Fk[j], B);
        Tk.push_back(Tj);

        if (Tj->entries.size() < b)
        {                                          // si la raíz de Tj es de tamaño menor a b
            vector<Node *> Tj_nodes = delRoot(Tj); // se quita esa raíz
            F.erase(F.begin() + j);                // se elimina pfj de F
            for (int i = 0; i < Tj_nodes.size(); i++)
            {
                Tk.push_back(Tj_nodes[i]); // se trabaja con sus subárboles nuevos Tj,...Tj+p-1
                for (int k=0; k<Tj[i].entries.size(); k++)
                    F.push_back(Tj[i].entries[k].p);      // se añaden los puntos pertinentes a F (p de la entrada del nodo asumiendo que solo tiene una entrada)
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
                Tsup->entries[i].a.reset(M[j]); // Reinicializa el shared_ptr para que apunte a M[j]
                break;
            }
            else
                continue;
        }
    }

    // se setean los radios cobertores resultantes para cada entrada en este árbol.
    setCR(Tsup);

    // se retorna T
    return Tsup;
}