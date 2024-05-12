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
vector<Point> k_samples(vector<Point> P, int k, int B, vector<Point> F)
{
    F.clear(); // vacía F para generarlo de nuevo
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distr(0, P.size());
    for (int i = 0; i < k; i++)
    { // creamos k=min(B,n/B) samples de manera aleatoria
        Point pf;
        int random_index = distr(gen);
        pf.x = P[random_index].x;
        pf.y = P[random_index].y;
        cout << "insertar (" << pf.x << "," << pf.y << ") en F" << endl;
        F.push_back(pf); // vamos insertando los samples en F
    }
    return F;
}

/*  Paso 3:
    Asigna a cada punto de P su sample más cercano de F entregando k conjuntos
    (crea Fk) */
// Modifica Fk haciendo que sea un conjunto de k conjuntos de puntos
// donde  Fk[k] contiene todos los p que tienen a F[k] como más cercano
vector<vector<Point>> closest(vector<Point> P, vector<Point> F, vector<vector<Point>> Fk)
{
    cout << "tamaño de F: " << F.size() << endl;
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
        cout << "el punto más cercano a (" << P[i].x << "," << P[i].y << ") es: (" << closest.x << "," << closest.y << ")." << endl;
        Fk[indexClosest].push_back(P[i]);
        cout << "agregamos (" << P[i].x << "," << P[i].y << ") a Fk." << endl;
    }
    return Fk;
}
// asignación de los puntos
vector<vector<Point>> assignation(vector<Point> P, vector<Point> F)
{
    cout << "F: ";
    for (int i = 0; i < F.size(); i++)
        cout << "(" << F[i].x << "," << F[i].y << ")" << ", ";
    cout << endl;
    vector<vector<Point>> Fk = closest(P, F, Fk); // conjunto de los conjuntos de pares (p_i, f_k)
    cout << "se crea Fk" << endl;
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
    int k = min(B, (int)ceil(n / B));
    cout << "k = " << k << endl;
    vector<Point> G = k_samples(P, k, B, F);      // step 2
    vector<vector<Point>> Fk = assignation(P, G); // step 3
    cout << "se hace la asignación Fk" << endl;
    redistribution(Fk, F, b, k); // step 4
    if (F.size() == 1)
    { // step 5
        return steps_2_to_5(P, b, B, F);
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
    return h_min;
}
// balancea el árbol y modifica F
void balancing(Node *Tk, vector<Point> F, vector<Node *> M, int h, int i)
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
            for (int k = 0; k < Tk->entries.size(); j++)
                balancing(Tk->entries[k].a.get(), F, M, h, i);
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
            T->entries[i].a.reset(M);
        else
            continue;
    }
}

/*  Paso 11:
    Calcula el radio cobertor de cada punto en un árbol */
void setCR(Node *T)
{
    if (T == nullptr)
        ;
    double max_distance = 0.0;
    for (int i = 0; i < T->entries.size(); i++)
    {
        for (int j = T->entries.size(); j = 0; j--)
        {
            double distance = dist(T->entries[i].p, T->entries[j].p);
            if (distance > max_distance)
            {
                max_distance = distance;
            }
        }
        T->entries[i].cr = max_distance;
    }
    for (int i = 0; i < T->entries.size(); i++)
    {
        setCR(T->entries[i].a.get());
    }
}
