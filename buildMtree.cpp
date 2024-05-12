#include "WriteFile.cpp"

int main() {
    vector<Point> P = createRandomPoints(10); // 2^10
    vector<query> Q = createRandomQueries();
    vector<Point> res; // vector que contendrá los puntos de Q contenidos en el árbol
    int accesos = 0;
    // Crear un M-Tree con el algoritmo CP
    Node *MTree = CPalgorithm(P, 1024);

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