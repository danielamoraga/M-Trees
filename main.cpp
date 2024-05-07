#include <cstdlib>
#include <vector>
#include "Structures.hpp"

int main()
{
    std::vector<Point> P; // Vector de Points
    std::vector<query> Q; // Vector de queries
    int accesos = 0;      // Número de accesos a disco

    for (int n = 10; n <= 25; n++)
    {
        int num_points = 1 << n; // 2^n Points

        // Crea 2^n Points para vector P
        for (int i = 0; i < num_points; i++)
        {
            Point p;
            p.x = (double)rand() / RAND_MAX;
            p.y = (double)rand() / RAND_MAX;
            P.push_back(p);
        }

        // Crea 100 queries para vector Q
        for (int i = 0; i < 100; i++)
        {
            query q;
            q.q.x = (double)rand() / RAND_MAX;
            q.q.y = (double)rand() / RAND_MAX;
            q.r = 0.02;
            Q.push_back(q);
        }
    }
}