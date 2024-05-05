#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

/* ESTRUCTURAS */

/* Estructura que define un punto */
struct Point {
    double x;
    double y;
};

/* Estructura para las entradas que pueden haber en un nodo */
struct entry {
    Point p; // punto
    double cr; // radio cobertor: máxima distancia entre p y cualquier punto del árbol
    Node* a; // dirección en disco a la página de su hijo identificado por la entrada de su nodo interno
};

struct Node {
    entry entrada; // entrada del nodo
    vector<Node *> nodes; // nodos internos
    bool isLeaf; // booleano para saber si es un nodo hoja
    int h; // altura

    /*void calc_cr() {
        // calcula cr de todos los puntos en el nodo
        for (int i=1; i<children.size(); i++) {
            // calcular cr para cada entries[i]
            Point point = entries[i].p; // punto actual
            Point pivote = entries[0].p; // punto pivote
            entries[i].cr = dist(point, pivote); // cr actual
            for (int j=1; j<entries.size();j++) {
                if (dist(point, entries[j].p) > dist(point, pivote))
                    entries[i].cr = dist(point, entries[j].p);
            }
        }
    }*/

    int height() {
        if (this == nullptr) h = 0; // árbol vacío
        if(isLeaf) h = 1; // nodo hoja
        else {
            int max_child_h = 0;
            for (int i=0; i<nodes.size(); i++) {
                max_child_h = max(max_child_h, nodes[i]->height());
                h = max_child_h + 1;
            }
        }
        return h;
    }
};

struct query { // es una bola Q=(q,r)
    Point q; // punto
    double r; // radio
};

/* MÉTODOS */

/* Distancia euclidiana entre dos puntos */
double dist(Point p, Point q) {
    return sqrt(pow(p.x - q.x,2) + pow(p.y - q.y, 2));
}

/* Método para eliminar la raíz de un árbol */
vector<Node *> delRoot(Node* T) {
    vector<Node *> c; // hijos de T
    for (int i=0; i<T->nodes.size(); i++)
        c.push_back(T->nodes[i]);
    T->nodes.clear();
    delete T;
    return c;
}

/* Método de búsqueda */
vector<Point> search(Node* T, query Q) {
    vector<Point> res; // vector que contendrá los puntos de Q contenidos en el árbol
    if (T->isLeaf) { // si el nodo raíz es una hoja
        if (dist(T->entrada.p, Q.q) <= Q.r)
                res.push_back(T->entrada.p); // se agrega p a la respuesta
    } else { // si el nodo es interno
        for (int i=0; i<T->nodes.size(); i++) {
            // se verifica para cada entrada si dist(p,q)<=cr+r
            if (dist(T->nodes[i]->entrada.p, Q.q) <= T->nodes[i]->entrada.cr+Q.r)
                search(T->nodes[i]->entrada.a, Q); // se buscan posibles respuestas en el hijo a
        }
    }
    return res; // retornamos el vector con los resultados
}