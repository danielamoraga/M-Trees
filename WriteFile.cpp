#include "Structures.hpp"
#include <random>

/*Crea 2^n puntos random*/
vector<Point> createRandomPoints(int n) {
    vector<Point> P;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> distr(0,1);
    for (int i=0; i<pow(2,n); i++) {
        Point p;
        p.x = distr(gen);
        p.y = distr(gen);
        P.push_back(p);
    }
    return P;
}

/*Crea 100 consultas*/
vector<query> createRandomQueries() {
    vector<query> Q;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> distr(0,1);
    for (int i=0; i<100; i++) {
        query q;
        q.q.x = distr(gen);
        q.q.y = distr(gen);
        q.r = 0.02; // lo que retorna aprox. un 0.12% de los puntos del conjunto
        Q.push_back(q);
    }
    return Q;
}

int main() {
    vector<Point> P = createRandomPoints(10); // 2^10
    vector<query> Q = createRandomQueries();
}