#include <vector>
#include <cmath>

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
    vector<entry> entries; // nodo raíz identificado por su entrada
    bool isLeaf;
    vector<Node *> children; // nodos internos

    void calc_cr() {
        // calcula cr de todos los puntos en el nodo
        for (int i=1; i<entries.size(); i++) {
            // calcular cr para cada entries[i]
            Point point = entries[i].p; // punto actual
            Point pivote = entries[0].p; // punto pivote
            entries[i].cr = dist(point, pivote); // cr actual
            for (int j=1; j<entries.size();j++) {
                if (dist(point, entries[j].p) > dist(point, pivote))
                    entries[i].cr = dist(point, entries[j].p);
            }
        }
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

/* Método para crear una hoja */
Node *newLeaf(vector<Point> points) {
    Node *leaf = new Node;
    for (int i=0; i<points.size(); i++) {
        leaf->entries[i].p = points[i];
        leaf->entries[i].cr = NULL;
        leaf->entries[i].a = NULL;
    }
    leaf->isLeaf = true;
    return leaf;
}

/* Método para eliminar la raíz de un árbol */
vector<Node *> delRoot(Node* T) {
    vector<Node *> nodes; // hijos de T
    for (int i=0; i<T->children.size(); i++)
        nodes.push_back(T->children[i]);
    T->children.clear();
    delete T;
    return nodes;
}

/* Método de búsqueda */
vector<Point> search(Node* T, query Q) {
    vector<Point> res; // vector que contendrá los puntos de Q contenidos en el árbol
    if (T->isLeaf) { // si el nodo raíz es una hoja
        for (int i=0; i<T->entries.size(); i++) {
            // se verifica para cada entrada si p cumple dist(p,q)<=r
            if (dist(T->entries[i].p, Q.q) <= Q.r)
                res.push_back(T->entries[i].p); // se agrega p a la respuesta
        }
    } else { // si el nodo es interno
        for (int i=0; i<T->entries.size(); i++) {
            // se verifica para cada entrada si dist(p,q)<=cr+r
            if (dist(T->entries[i].p, Q.q) <= T->entries[i].cr+Q.r)
                search(T->entries[i].a, Q); // se buscan posibles respuestas en el hijo a
        }
    }
    return res; // retornamos el vector con los resultados
}