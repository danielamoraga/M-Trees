#include "Structures.hpp"

Node *newNode(vector<Point> P)
{
    /* Crea un nodo vacío con las entradas P y devuelve un puntero a él */
    cout << "creo newNode antes del for" << endl;
    Node *T = new Node();
    cout << "se crea el nodo" << endl;
    cout << "tamaño de P: " << P.size() << endl;

    for (int i = 0; i < P.size(); i++)
    {
        cout << "antes de agregar nodo: " << i << endl;
        Node *thisNode = T->nodes[i];
        cout << "recupero el nodo" << endl;
        thisNode->p = P[i];
        cout << ". Se agregó nodo: " << i << endl;
    }
    cout << "después del for de newNode" << endl;
    return T;
}

/*  Paso 2:
    Elige k puntos aleatorios de P que inserta en un conjunto F*/
void k_samples(vector<Point> P, int k, int n, int B, vector<Point> F)
{
    F.clear(); // vacía F para generarlo de nuevo
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distr(0, n);
    int K = min(B, (int)ceil(n / B));
    for (int i = 0; i < K; i++)
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
// Modifica Fk haciendo que sea un conjunto de k conjuntos de puntos
// donde  Fk[k] contiene todos los p que tienen a F[k] como más cercano
void closest(vector<Point> P, vector<Point> F, vector<vector<Point>> Fk)
{
    for (int i = 0; i < P.size(); i++)
    {
        double distance = dist(P[i], F[0]);
        Point closest = F[0];
        int indexClosest = 0;
        for (int k = 0; k < F.size(); k++)
        {
            if (dist(P[i], F[k]) < distance)
            {
                distance = dist(P[i], F[k]);
                closest = F[k];
                indexClosest = k;
            }
        }
        Fk[indexClosest].push_back(P[i]);
    }
}
// asignación de los puntos
vector<vector<Point>> assignation(vector<Point> P, vector<Point> F)
{
    vector<vector<Point>> Fk; // conjunto de los conjuntos de pares (p_i, f_k)
    closest(P, F, Fk);
    return Fk;
}

/*  Paso 4:
    Si un Fk[j] es tq Fk[j].size < b
    * Quita F[j] de F
    * Por cada p en Fk[j] busca su sample F[l] más cercano de F y lo añadimos a Fk[l] */
void redistribution(vector<vector<Point>> Fk, vector<Point> F, double b, int k)
{
    for (int j = 0; j < k; j++)
    {
        int size = Fk[j].size();
        if (size < b)
        {                           // si algún Fk tiene tamaño menor a b, quitamos F[j] de F
            F.erase(F.begin() + j); // quitamos el elemento F[j] del conjunto F
            closest(Fk[j], F, Fk);  // buscamos el sample más cercano de F añadiendolo a su Fk correspondiente
        }
    }
}

/*  Paso 5:
    Si el tamaño de F es 1 volvemos al paso 2
    -> Haremos una función recursiva con los pasos del 2 al 5 */
vector<vector<Point>> steps_2_to_5(vector<Point> P, double b, int B, vector<Point> F)
{
    int n = P.size();
    int k = ceil(n / B);
    int K = min(B, k);
    k_samples(P, K, n, B, F);                     // step 2
    vector<vector<Point>> Fk = assignation(P, F); // step 3
    redistribution(Fk, F, b, k);                  // step 4
    if (F.size() == 1)
    { // step 5
        steps_2_to_5(P, b, B, F);
    }
    else
    {
        return Fk;
    }
}

/*  Paso 9: Balanceamiento */
// calcula altura mínima
int minHeight(vector<Node *> Tk)
{
    int h_min = Tk[0]->height();
    for (int k = 1; k < Tk.size(); k++)
    {
        int h_min = min(h_min, Tk[k]->height());
    }
}
// balancea el árbol y modifica F
vector<Node *> balancing(vector<Node *> Tk, vector<Point> F)
{
    int h = minHeight(Tk); // altura mínima de de los árboles Tj
    vector<Node *> M;      // conjunto inicialmente vacío
    for (int i = 0; i < Tk.size(); i++)
    {                             // por cada Tj,
        if (Tk[i]->height() == h) // si su altura es igual a h
            M.push_back(Tk[i]);   // se añade a M
        else
        {                           // sino
            F.erase(F.begin() + i); // se borra el punto pertinente en F
            // se hace una búsqueda exhaustiva en Tj de todos sus subárboles de altura igual a h y se insertan en M
            // y se insertan los puntos raíz de T1',...,Tp',pf1',...,pfp' en F
            for (int j = 0; j < Tk[i]->nodes.size(); j++)
            {
                balancing(Tk[i]->nodes, F);
            }
        }
    }
    return M;
}

/*Paso 10*/
void searchPoint(Node *T, Point p, Node *M)
{
    for (int i = 0; i < T->nodes.size(); i++)
    {
        Node *thisNode = T->nodes[i];
        if (thisNode->p == p)
            T->nodes[i]->nodes.push_back(M);
        else
            continue;
    }
}

/*  Paso 11:
    Calcula el radio cobertor de cada punto en un árbol */
void setCR(Node *T)
{
    double max_distance = 0.0;
    for (int i = 0; i < T->nodes.size(); i++)
    {
        double distance = dist(T->p, T->nodes[i]->p);
        if (distance > max_distance)
        {
            max_distance = distance;
        }
    }
    T->cr = max_distance;
    for (int i = 0; i < T->nodes.size(); i++)
    {
        setCR(T->nodes[i]);
    }
}
