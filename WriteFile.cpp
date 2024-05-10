#include "CP-algorithm.cpp"
#include <random>
#include <iostream>
#include <fstream>

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
        // cout << "Punto " << i << ": (" << p.x << ", " << p.y << ")" << endl;
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
    for (int i = 0; i < 100; i++)
    { //(int i=0; i<100; i++)
        query q;
        q.q.x = distr(gen);
        q.q.y = distr(gen);
        q.r = 0.1; // lo que retorna aprox. un 0.12% de los puntos del conjunto
        Q.push_back(q);
        // cout << "Consulta " << i << ": (" << q.q.x << ", " << q.q.y << ") con radio " << q.r << endl;
    }
    return Q;
}

int main()
{
    vector<Point> P = createRandomPoints(4096); // 2^10
    vector<query> Q = createRandomQueries();
    vector<Point> res; // vector que contendrá los puntos de Q contenidos en el árbol
    int accesos = 0;
    // Crear un M-Tree con el algoritmo CP
    Node *MTree = CPalgorithm(P, 4096);

    // Abrir un archivo para escribir
    std::ofstream file;
    file.open("resultados.csv");
    file << "Consulta,Accesos,Puntos Encontrados\n"; // Escribir los encabezados de las columnas

    // Buscar cada consulta en el M-Tree
    for (int i = 0; i < Q.size(); i++)
    {
        cout << "Buscando en la consulta " << i << endl;
        pair<vector<Point>, int> result = search(MTree, Q[i], res, accesos);
        vector<Point> points = result.first;
        int accessCount = result.second;

        cout << "Accesos: " << accessCount << " ";
        cout << "Puntos encontrados: " << points.size() << endl;
        // Escribir los resultados en el archivo
        file << Q[i].q.x << "." << Q[i].q.y << "," << accessCount << "," << points.size() << "\n";
    }

    // Cerrar el archivo
    file.close();

    return 0;
}