#include "CP-algorithm.cpp"
#include <random>
#include <iostream>
#include <fstream>
#include <chrono>

/*Crea 2^n puntos random*/
vector<Point> createRandomPoints(int n)
{
    vector<Point> P;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> distr(0, 1);
    for (int i = 0; i < pow(2, n); i++)
    {
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
    {
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
    for (int exp = 10; exp <= 25; exp++)
    {
        vector<Point> P = createRandomPoints(exp); // 2^10
        vector<query> Q = createRandomQueries();
        vector<Point> resCP; // vector que contendrá los puntos de Q contenidos en el árbol
        vector<Point> resSS; // vector que contendrá los puntos de Q contenidos en el árbol
        int accesos = 0;
        // Crear un M-Tree con el algoritmo CP
        Node *MTree = CPalgorithm(P, 4096);

        // Abrir un archivo para escribir
        std::ofstream file;
        file.open("resultados" + to_string(exp) + ".csv");
        file << "Arbol,Exponente,Consulta,Accesos,Tiempo\n"; // Escribir los encabezados de las columnas

        // Buscar cada consulta en el M-Tree creado por CP
        cout << "Buscando en el árbol creado por CP" << endl;
        for (int i = 0; i < Q.size(); i++)
        {
            cout << "Buscando en la consulta " << i << endl;

            // Inicio del tiempo
            auto start = std::chrono::high_resolution_clock::now();

            // Buscar la consulta en el M-Tree
            pair<vector<Point>, int> result = search(MTree, Q[i], resCP, accesos);

            // Fin del tiempo
            auto finish = std::chrono::high_resolution_clock::now();

            // Calcular el tiempo transcurrido
            std::chrono::duration<double> duration = finish - start;

            // Mostrar los resultados
            int accessCount = result.second;

            cout << "Accesos: " << accessCount << " ";
            cout << "Tiempo transcurrido: " << duration.count() << "s" << endl;

            // Escribir los resultados en el archivo
            file << "CP" << "," << exp << "," << i << "," << accessCount << "," << duration.count() << "\n";
        }

        // Buscar cada consulta en el M-Tree creado por SS
        cout << "Buscando en el árbol creado por SS" << endl;
        for (int i = 0; i < Q.size(); i++)
        {
            cout << "Buscando en la consulta " << i << endl;

            // Inicio del tiempo
            auto start = std::chrono::high_resolution_clock::now();

            // Buscar la consulta en el M-Tree
            pair<vector<Point>, int> result = search(MTree, Q[i], resCP, accesos);

            // Fin del tiempo
            auto finish = std::chrono::high_resolution_clock::now();

            // Calcular el tiempo transcurrido
            std::chrono::duration<double> duration = finish - start;

            // Mostrar los resultados
            vector<Point> points = result.first;
            int accessCount = result.second;

            cout << "Accesos: " << accessCount << " ";
            cout << "Tiempo transcurrido: " << duration.count() << "s" << endl;

            // Escribir los resultados en el archivo
            file << "SS" << "," << exp << "," << i << "," << accessCount << "," << duration.count() << "\n";
        }

        // Cerrar el archivo
        file.close();
    }

    return 0;
}