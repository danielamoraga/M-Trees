#include <vector>
#include <cmath>

using namespace std;

/* Estructura que define un punto */
struct Point {
    double x;
    double y;
};

/* Distancia euclidiana entre dos puntos */
double dist(Point p, Point q) {
    return sqrt((p.x - q.x)*(p.x - q.x) + (p.y - q.y)*(p.y - q.y));
}

/* Estructura que define a un nodo de un MTree */
struct Node {
    Point p; // punto que define al nodo
    double cr; // nadio cobertor: máxima distancia entre p y cualquier punto del árbol
    Node *a; // dirección en disco a la página de su hijo identificado por (p,cr,a) de su nodo interno

    bool isLeaf() {
        if (a == NULL) { // no hay direcciones a nodos hijo
            cr = NULL; // radio cobertor también es nulo
            return true; // es hoja
        }
        else return false; // en caso contrario, no es hoja
    }
};

struct query { // es una bola
    Point q; // punto
    double r; // radio
};

/* Búsqueda recursiva de los puntos de la bola que son parte del MTree */
vector<Point> search(Node* T, query Q) {
    vector<Point> res; // vector de resultados con los puntos que se buscan
    if (T->isLeaf()) { // caso base: estamos buscando en una hoja
        if (dist(T->p, Q.q) <= Q.r) { // si dist(p,q) <= r -> agregamos p a los resultados
            res.push_back(T->p);
        }
    } else { // buscando en un nodo interno
        if (dist(T->p, Q.q) <= T->cr + Q.r) { // si dist(p,q) <= cr+r -> buscamos en el hijo de este nodo
            search(T->a, Q);
        }
    }
    return res;
}