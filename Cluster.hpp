#include "Structures.hpp"

class ClusterT {
  private:
    vector<Point> points;
    double radius;
    Point medoid;

  public:
    ClusterT(){
      this->points = {};
      this->radius = 0;
    }

    ClusterT(vector<Point> points){
      this->setPoints(points);
      this->computeMedoid();
      this->computeRadius();
    }

    vector<Point> getPoints() const {
        return points;
    }

    Point getMedoid() const {
        return medoid;
    }

    double getRadius() const {
        return radius;
    }

    /*
    Retorna el vector de puntos de este cluster.
    */
    vector<Point> getPoints(){
      return this->points;
    }

    /*
    Setter para vector de puntos.
    */
    void setPoints(vector<Point> v){
      this->points = v;
    }

    /*
    Retorna el radio cobertor de este cluster.
    */
    double getRadius(){
      return this->radius;
    }

    /*
    Setter para el radio cobertor.
    */
    void setRadius(double r){
      this->radius = r;
    }

    /*
    Retorna el medoide de este cluster.
    */
    Point getMedoid(){
      return this->medoid;
    }

    /*
    Setter para el medoide.
    */
    void setMedoid(Point p){
      this->medoid = p;
    }


    /*
    Retorna la cantidad de elementos (cardinalidad) de este cluster.
    */
    int getCardinality(){
      return this->points.size();
    }

    /*
    Añade un punto al vector de puntos de este cluster.
    */
    void addPoint(Point p){
      this->points.push_back(p);
    }

    Point getPointByIndex(int i){
      return (this->points)[i];
    }

    /*
    Calcula el medoide primario del set de puntos de este cluster.
    */
    void computeMedoid() {
      if (this->points.empty()) {
        this->medoid = Point(); // Si no hay puntos, el medoide es un punto vacío
        return;
      }

      if (this->points.size() == 1) {
        this->medoid = this->points[0];
        return;
      }

      double minMaxDistance = std::numeric_limits<double>::infinity(); // Inicializar con el máximo valor posible
      Point minMedoid; // Inicializar el medoide mínimo

      for (const Point& p : points) {
        double maxDistance = 0.0; // Inicializar la máxima distancia a este punto

        for (const Point& q : points) {
            maxDistance = std::max(maxDistance, dist(p, q)); // Calcular la distancia máxima a este punto
        }

        if (maxDistance < minMaxDistance) {
            minMaxDistance = maxDistance;
            minMedoid = p;
        }
      }

      // Asignar el medoide mínimo encontrado
      medoid = minMedoid;
  }


    /*
    Calcula el radio de cobertura de este cluster.
    */
    void computeRadius() {
      if (this->points.empty()) {
        this->radius = 0.0;
        return;
      }

      this->radius = 0.0;

      for (const Point& p : this->points) {
        double distance = dist(medoid, p); // Calcular la distancia de cada punto al medoide
        if (distance > radius) {
          radius = distance; // Actualizar el radio si encontramos una distancia mayor
        }
      }
    }

    /*
    Retorna la unión entre el cluster actual y c.
    */
    ClusterT cUnion(ClusterT c){
      ClusterT unionCluster;

      // Agregar todos los puntos del cluster actual al nuevo cluster
      for (const Point& p : this->points) {
        unionCluster.addPoint(p);
      }

      // Agregar todos los puntos del cluster c al nuevo cluster
      for (const Point& p : c.getPoints()) {
        unionCluster.addPoint(p);
      }

      // Actualizar el medoide y el radio del nuevo cluster
      unionCluster.computeMedoid();
      unionCluster.computeRadius();

      // Retornar el cluster que representa la unión
      return unionCluster;
    }

    /*
    Dado un vector de clusters, retorna el vecino más cercano a este cluster.

    Dado un cluster c, su vecino más cercano es otro cluster c′ tal que no hay otro cluster que
    su distancia a c sea menor a la distancia entre c y c′ (se puede tener múltiples vecinos más
    cercanos).
    */
    ClusterT closestNeighbour(vector<ClusterT> C){
      ClusterT closestCluster = C[0];
      double minDistance = numeric_limits<double>::max(); // Inicializamos con el máximo valor posible

      // Iterar sobre todos los clusters en el vector
      for (const ClusterT& cluster : C) {
        if (&cluster == &(*this)) continue; // Evitar comparar con el mismo cluster

        double distance = dist(this->medoid, cluster.medoid); // Calcular distancia entre medoides
        if (distance < minDistance) {
          minDistance = distance;
          closestCluster = cluster;
        }
      }

      return closestCluster;
    }

    bool operator==(const ClusterT& other) {
        // Implementación del operador de igualdad
        return this->points == other.getPoints() &&
           this->medoid == other.getMedoid() &&
           this->radius == other.getRadius();
    }

};