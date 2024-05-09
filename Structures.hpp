#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <memory>

using namespace std;

/* ESTRUCTURAS */

/* Estructura que define un punto */
struct Point
{
    double x;
    double y;
};

string PointToString(Point p)
{
    double x = p.x;
    double y = p.y;
    return (to_string(x) + to_string(y));
};

struct Node
{
    /* Estructura para las entradas que pueden haber en un nodo */
    Point p;              // punto
    double cr;            // radio cobertor: máxima distancia entre p y cualquier punto del árbol
    Node *a;              // dirección en disco a la página de su hijo identificado por la entrada de su nodo interno
    vector<Node *> nodes; // nodos internos

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
            for (int i = 0; i < nodes.size(); i++)
            {
                max_child_h = max(max_child_h, nodes[i]->height());
                h = max_child_h + 1;
            }
        }
        return h;
    }

    bool isLeaf()
    {
        if (nodes.empty())
        {
            a = NULL;
            cr = 0.0; // Assign 0.0 instead of NULL
            return true;
        }
        else
            return false;
    }
};

struct query
{             // es una bola Q=(q,r)
    Point q;  // punto
    double r; // radio
};

/* MÉTODOS */

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

/* Método para eliminar la raíz de un árbol */
vector<Node *> delRoot(Node *T)
{
    vector<Node *> c; // hijos de T
    for (int i = 0; i < T->nodes.size(); i++)
        c.push_back(T->nodes[i]);
    T->nodes.clear();
    delete T;
    return c;
}

/* Método de búsqueda */
vector<Point> search(Node *T, query Q)
{
    vector<Point> res; // vector que contendrá los puntos de Q contenidos en el árbol
    if (T->isLeaf())
    { // si el nodo raíz es una hoja
        if (dist(T->p, Q.q) <= Q.r)
            res.push_back(T->p); // se agrega p a la respuesta
    }
    else
    { // si el nodo es interno
        for (int i = 0; i < T->nodes.size(); i++)
        {
            // se verifica para cada entrada si dist(p,q)<=cr+r
            if (dist(T->nodes[i]->p, Q.q) <= T->nodes[i]->cr + Q.r)
            {
                Node* thisNode = T->nodes[i]; // declare the variable "thisNode"
                for (int j=0; j<thisNode->nodes.size(); j++)
                    search(thisNode->nodes[j], Q); // se buscan posibles respuestas en el hijo a
            }
        }
    }
    return res; // retornamos el vector con los resultados
}

void printMtree(Node *T)
{
    if (T = NULL)
        return;
    else
    {
        const Point p = T->p;

        cout << "punto raíz: " << PointToString(p) << "/np radio cobertor: " << T->cr << endl;
        for (int i = 0; i < T->nodes.size(); i++)
        {
            cout << "Hijos " << i << endl;
            printMtree(T->nodes[i]);
        }
    }
}