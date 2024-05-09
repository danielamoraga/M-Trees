#include "CP-algorithm.cpp"
#include <random>
#include <iostream>

/*Crea 2^n puntos random*/
vector<Point> createRandomPoints(int n)
{
    vector<Point> P;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> distr(0, 1);
    for (int i = 0; i < n; i++)
    { // int i=0; i<pow(2,n); i++
        Point p;
        p.x = distr(gen);
        p.y = distr(gen);
        P.push_back(p);
        cout << "Punto " << i << ": (" << p.x << ", " << p.y << ")" << endl;
    }
    return P;
}

/*Crea 100 consultas*/
vector<query> createRandomQueries()
{
    vector<query> Q;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> distr(0, 1);
    for (int i = 0; i < 2; i++)
    { //(int i=0; i<100; i++)
        query q;
        q.q.x = distr(gen);
        q.q.y = distr(gen);
        q.r = 0.02; // lo que retorna aprox. un 0.12% de los puntos del conjunto
        Q.push_back(q);
        cout << "Consulta " << i << ": (" << q.q.x << ", " << q.q.y << ") con radio " << q.r << endl;
    }
    return Q;
}

int main()
{
    vector<Point> P = createRandomPoints(10); // 2^10
    vector<query> Q = createRandomQueries();
    // Crear un M-Tree con el algoritmo CP
    cout << "antes del arbol" << endl;
    Node *MTree = CPalgorithm(P, 4096);

    // Buscar cada consulta en el M-Tree
    cout << "antes de buscar por cada consulta" << endl;
    for (int i = 0; i < Q.size(); i++)
    {
        cout << "antes de search" << endl;
        const vector<Point> res = search(MTree, Q[i]);
        cout << "después de search" << endl;
        for (int k; k < P.size();)
        {
            cout << "Punto (" << P[i].x << ", " << P[i].y << ") está en la consulta" << endl;
        }
    }
}