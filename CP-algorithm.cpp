
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
        cout << "while... " << endl;
        k_samples(P, B, F); // Paso 2: conjunto de samples de P
        cout << "se crea conjunto de samples de tamaño " << F.size() << endl;
        Fk = assignation(P, F); // Paso 3: crea el conjunto de conjuntos Fk
        cout << "se crea conjunto de conjuntos de samples de tamaño " << Fk.size() << endl;
        redistribution(Fk, F, b); // Paso 4: redistribuye los valores de Fk y modifica F
        cout << "redistribuimos los conjuntos obteniendo los tamaños" << endl;
        cout << "F: " << F.size() << endl;
        cout << "Fk: " << Fk.size() << endl;
        if (F.size() == 1) continue; // Paso 5: si |F|=1, vuelve al paso 2
        else break;
    }

    cout << "Fuera del while, tamaño de Fk: " << Fk.size() << endl;

    // Paso 6: se realiza recursivamente el algoritmo CP en cada Fk[j] obteniendo el árbol Tj
    vector<Node *> Tk;
    cout << "creamos un conjunto de " << Fk.size() << " árboles" << endl;
    for (int j = 0; j < Fk.size(); j++)
    {
        cout << "Tamaño de Fk[j]: " << Fk[j].size() << endl;
        Node *Tj = CPalgorithm(Fk[j], B);
        cout << "tj entries antes: " <<Tj->entries.size() << endl;
        cout << "se crea un árbol con cada Fk[j]" << endl;
        Tk.push_back(Tj);
        cout << "ponemos el árbol en el vector anterior" << endl;

        cout << "tj entries " << Tj->entries.size() << endl;

        cout << "b: " << b << endl;
    }

    // Paso 7: 
    for (int j = 0; j < Tk.size(); j++)
    {
        if (Tk[j]->entries.size() < b)
        {
            cout << "if?" << endl;                  // si la raíz de Tj es de tamaño menor a b
            vector<entry> Tj_entries = delRoot(Tk[j]); // se quita esa raíz
            F.erase(F.begin() + j);                // se elimina pfj de F
            for (int i = 0; i < Tj_entries.size(); i++)
            {
                Tk.push_back(Tj_entries[i].a); // se trabaja con sus subárboles nuevos Tj,...Tj+p-1
                cout << "Tj entries " << Tk[j]->entries.size() << endl;
                F.push_back(Tj_entries[i].p); // se añaden los puntos pertinentes a F
            }
        }
    }

    vector<Node *> M;
    cout << "Tk size " <<Tk.size() << endl;
    int h = minHeight(Tk); // altura mínima de de los árboles Tj
    cout << "pasa minheight, h: " << h << endl;
    for (int i = 0; i < Tk.size(); i++)
        balancing(Tk[i], F, M, h, i);

    cout << "M: " << M.size() << endl;

    // se define Tsup como el resultado de la llamada al algoritmo CP aplicado a F
    Node *Tsup = CPalgorithm(F, B);

    // se une cada Tj en M a su hoja en Tsup correspondiente al punto pfj en F, obteniendo un nuevo árbol T
    for (int j = 0; j < M.size(); j++)
    {
        // Punto pfj en F
        Point pfj = F[j];
        // Buscar la hoja correspondiente en Tsup
        cout <<"tsup entries size: " <<Tsup->entries.size() << endl;
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