#pragma once

#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

using namespace std;

/* ESTRUCTURAS */

/* Estructura que define un punto */
struct Point
{
    double x;
    double y;
};

/* Operador booleano == para la estructura Point */
bool operator==(const Point &p1, const Point &p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

/* Distancia euclidiana entre dos puntos */
double dist(Point p, Point q)
{
    return sqrt(pow(p.x - q.x, 2) + pow(p.y - q.y, 2));
}

struct Node;

/* Estructura para las entradas que pueden haber en un nodo */
struct entry
{
    Point p;            // punto
    double cr;          // radio cobertor: máxima distancia entre p y cualquier punto del árbol
    Node* a; // dirección en disco a la página de su hijo identificado por la entrada de su nodo interno
    entry(Point point){p=point; cr = 0.0; a = nullptr;}
    entry(Point init_p, double init_r, Node* init_a){p = init_p; cr = init_r; a = init_a;}
};

struct Node
{
    vector<entry> entries; // entradas que componen el nodo
    Node(){};

    int height()
    {
        int h;
        if (this == nullptr)
            return 0; // árbol vacío
        if (isLeaf())
            return 1; // nodo hoja
        else
        {
            int max_child_h = 0;
            for (int i = 0; i < entries.size(); i++)
            {
                if (entries[i].a != nullptr)
                    max_child_h = max(max_child_h, entries[i].a->height());
            }
            return max_child_h + 1;
        }
    }

    bool isLeaf()
    {
        bool isLeaf = false;
        for (entry& e : entries)
        {
            if (e.a == nullptr && e.cr == 0.0)
            {
                isLeaf = true;
            }
        }
        return isLeaf;
    }

    void setCR()
    {
        for (entry& e : entries)
        {
            double max_distance = 0.0;
            for (entry& other_entry : entries)
            {
                if (&e != &other_entry)
                {
                    double d = dist(e.p, other_entry.p);
                    max_distance = max(max_distance, d);
                }
            }
            e.cr = max_distance;
        }
    }
};

struct query
{             // es una bola Q=(q,r)
    Point q;  // punto
    double r; // radio
};

/* MÉTODOS */

/* Método de búsqueda */
pair<vector<Point>, int> search(Node *T, query Q, vector<Point> res, int accesos)
{
    accesos++;
    if (T->isLeaf())
    { // si el nodo raíz es una hoja
        // cout << "si el arbol es una hoja: " << endl;
        for (int i = 0; i < T->entries.size(); i++)
            if (dist(T->entries[i].p, Q.q) <= Q.r)
            {
                res.push_back(T->entries[i].p); // se agrega p a la respuesta
                // cout << "se agrega " << T.entries[i].p.x << "," << T.entries[i].p.y << " a la respuesta" << endl;
            }
    }
    else
    { // si el nodo es interno
        // cout << "si el nodo es interno" << endl;
        for (int i = 0; i < T->entries.size(); i++)
        {
            // cout << "buscando en la entrada " << i << " si la distancia cumple" << endl;
            //  se verifica para cada entrada si dist(p,q)<=cr+r
            //  cout << "verificando si " << dist(T->nodes[i]->p, Q.q) << " es menor o igual a " << T->nodes[i]->cr + Q.r << endl;
            if (dist(T->entries[i].p, Q.q) <= T->entries[i].cr + Q.r)
            {
                // cout << "busca en los hijos" << endl;
                pair<vector<Point>, int> nodeResults = search(T->entries[i].a, Q, res, accesos); // declare the variable "thisNode"
                res.assign(nodeResults.first.begin(), nodeResults.first.end());                        // se buscan posibles respuestas en el hijo a
                accesos = nodeResults.second;
            }
        }
    }
    return make_pair(res, accesos);
}

string PointToString(Point p)
{
    double x = p.x;
    double y = p.y;
    return (to_string(x) + to_string(y));
};