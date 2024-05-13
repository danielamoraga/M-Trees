
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

    // Paso 1: si n es menor o igual a B
    cout << "CPalgorithm - Paso 1" << endl;
    if (P.size() <= B)
    {
        Node *T = newNode(P); // se crea un árbol T y se insertan todos los puntos en él
        return T; // se retorna T
    }

    vector<Point> F;
    vector<vector<Point>> Fk;

    while (true) {
    cout << "CPalgorithm - Paso 2" << endl;
        k_samples(P, B, F); // Paso 2: conjunto de samples de P
    cout << "CPalgorithm - Paso 3" << endl;
        Fk = assignation(P, F); // Paso 3: crea el conjunto de conjuntos Fk
    cout << "CPalgorithm - Paso 4" << endl;
        redistribution(Fk, F, b); // Paso 4: redistribuye los valores de Fk y modifica F
    cout << "CPalgorithm - Paso 5" << endl;
        if (F.size() == 1) continue; // Paso 5: si |F|=1, vuelve al paso 2
        else break;
    }

    // Paso 6
    cout << "CPalgorithm - Paso 6" << endl;
    vector<Node *> Tk;
    for (int j = 0; j < Fk.size(); j++) {
        // se realiza recursivamente el algoritmo CP en cada Fk[j] obteniendo el árbol Tk
        Tk.push_back(CPalgorithm(Fk[j], B));
    }

    // Paso 7
    cout << "CPalgorithm - Paso 7" << endl;
    for (int j = 0; j < Tk.size(); j++)
    {

        if (Tk[j]->entries.size() < b)
        {   // si la raíz de Tj es de tamaño menor a b
            vector<entry> Tkj = Tk[j]->entries;
            Tk.erase(Tk.begin() + j); // se quita esa raíz
            F.erase(F.begin() + j);                // se elimina pfj de F
            for (int i = 0; i < Tkj.size(); i++)
            {
                if (Tkj[i].a != nullptr)
                {
                    Tk.push_back(Tkj[i].a); // se trabaja con sus subárboles nuevos Tj,...Tj+p-1
                    F.push_back(Tkj[i].p); // se añaden los puntos pertinentes a F
                }
            }
        }
    }

    // Paso 8
    cout << "CPalgorithm - Paso 8" << endl;
    vector<Node *> M; // se define un conjunto de árboles inicialmente vacío
    int h = minHeight(Tk); // altura mínima de de los árboles Tj

    // Paso 9
    cout << "CPalgorithm - Paso 9" << endl;
    for (int i = 0; i < Tk.size(); i++)
        balancing(Tk[i], F, M, h, i); // por cada Tk[i], si su altura es igual a h, se añade a M, sino..

    // Paso 10: se define Tsup como el resultado de la llamada al algoritmo CP aplicado a F
    cout << "CPalgorithm - Paso 10" << endl;
    Node *Tsup = CPalgorithm(F, B);

    // Paso 11: se une cada Tj en M a su hoja en Tsup correspondiente al punto pfj en F, obteniendo un nuevo árbol T
    cout << "CPalgorithm - Paso 11" << endl;
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
    cout << "CPalgorithm - Paso 12" << endl;
    setCR(Tsup);

    // Paso 13: se retorna Tsup
    cout << "CPalgorithm - Paso 13" << endl;
    return Tsup;
}