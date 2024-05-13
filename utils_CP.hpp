#include "Structures.hpp"

/*  Paso 1:
    Se crea un árbol T, se insertan todos los puntos en este y se retorna el árbol */
Node *newNode(vector<Point> P)
{
    Node *root = new Node();
    for (int i = 0; i < P.size(); i++)
    {
        entry entry(P[i]);
        root->entries.push_back(entry);
    }
    return root;
}

/*  Paso 2:
    Elige k puntos aleatorios de P que inserta en un conjunto F*/
void k_samples(vector<Point> P, int B, vector<Point>&F)
{
    int k = min(B,(int)ceil(P.size()/B));
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distr(0, P.size());
    for (int i = 0; i < k; i++)
    { // creamos k=min(B,n/B) samples de manera aleatoria
        Point pf;
        int random_index = distr(gen);
        pf.x = P[random_index].x;
        pf.y = P[random_index].y;
        F.push_back(pf); // vamos insertando los samples en F
    }
}

/*  Paso 3:
    Asigna a cada punto de P su sample más cercano de F entregando k conjuntos
    (crea Fk) */
// indice de F del punto más cercano a P
int closestIndex(Point p, vector<Point> F)
{
    double distance = dist(p, F[0]);
    int index = 0;
    for (int k = 0; k < F.size(); k++)
    {
        if (dist(p, F[k]) < distance)
        {
            distance = dist(p, F[k]);
            index = k;
        }
    }
    return index;
}
// asignación de los puntos
vector<vector<Point>> assignation(vector<Point> P, vector<Point> F)
{
    vector<vector<Point>> Fk;
    Fk.resize(F.size());
    for (int i = 0; i < P.size(); i++)
    {
        int indexClosest = closestIndex(P[i], F);
        Fk[indexClosest].push_back(P[i]);
    }
    return Fk;
}

/*  Paso 4:
    Si un Fk[j] es tq Fk[j].size < b
    * Quita F[j] de F
    * Por cada p en Fk[j] busca su sample F[l] más cercano de F y lo añadimos a Fk[l] */
void redistribution(vector<vector<Point>> &Fk, vector<Point> &F, double b)
{
    for (int j = 0; j < Fk.size(); j++)
    {
        if (Fk[j].size() < b)
        {                           // si algún Fk tiene tamaño menor a b, quitamos F[j] de F
            F.erase(F.begin() + j); // quitamos el elemento F[j] del conjunto F
            vector<Point> Fkj = Fk[j];
            Fk.erase(Fk.begin() + j);
            for (int i=0; i<Fkj.size(); i++) {
                int l = closestIndex(Fkj[i], F);
                Fk[l].push_back(F[l]);
            }
        }
    }
}

/*  Paso 6:
    Se realiza recursivamente el algoritmo CP en cada Fj, obteniendo el árbol Tj*/
/*vector<Node *> Tk_trees() {
    vector<Node *> Tk;
    for (int j = 0; j < Fk.size(); j++)
        Tk.push_back(CPalgorithm(Fk[j], B));
    return Tk;
}*/

/*  Paso 7:
    Si la raíz del árbol es de un tamaño menor a b, se quita esa raíz, se elimina pfj de F y se trabaja
    con sus subárboles como nuevos Tj , . . . , Tj+p−1, se añaden los puntos pertinentes a F. */
// Método para eliminar la raíz de un árbol, retornando los hijos del arbol como vector de nodos.
vector<entry> delRoot(Node* &T)
{
    vector<entry> c; // entradas de T
    for (int i = 0; i < T->entries.size(); i++)
        c.push_back(T->entries[i]);
    T->entries.clear();
    return c;
}

/*  Paso 9: Balanceamiento */
// calcula altura mínima
int minHeight(vector<Node *> &Tk)
{
    int h_min = Tk[0]->height();
    for (int k = 1; k < Tk.size(); k++)
        h_min = min(h_min, Tk[k]->height());
    return h_min;
}
// balancea el árbol y modifica F
void balancing(Node *Tk, vector<Point> &F, vector<Node *> &M, int h, int i)
{
    if (Tk->height() == h) // si su altura es igual a h
        M.push_back(Tk);   // se añade a M
    else
    {                           // sino
        F.erase(F.begin() + i); // se borra el punto pertinente en F
        // se hace una búsqueda exhaustiva en Tj de todos sus subárboles de altura igual a h y se insertan en M
        // y se insertan los puntos raíz de T1',...,Tp',pf1',...,pfp' en F
        for (int j = 0; j < Tk->entries.size(); j++)
        {
            balancing(Tk->entries[j].a, F, M, h, i);
            if (Tk->height() == h) F.push_back(Tk->entries[j].p);
        }
    }
}

/*Paso 10*/
void searchPoint(Node *T, Point p, Node *M)
{
    for (int i = 0; i < T->entries.size(); i++)
    {
        entry thisEntry = T->entries[i];
        if (thisEntry.p == p)
            T->entries[i].a = M;
        else
            continue;
    }
}