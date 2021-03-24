#ifndef CLUSTER_H_INCLUDED
#define CLUSTER_H_INCLUDED

#include <iostream>
#include <vector>
using namespace std;


class Cluster{
    private:
        vector<double> centroide;
        vector<vector<double>> puntos_incluidos;
        double distancia_media_intracluster;

        double distanciaEuclidea(vector<double> punto_a);

    public:

        Cluster();
        vector<double> getCentroide();
        vector<vector<double>> getPuntos();
        void calcularCentroide();
        void calcularDistanciaMediaIntracluster();
};

#endif