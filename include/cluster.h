#ifndef CLUSTER_H_INCLUDED
#define CLUSTER_H_INCLUDED

#include <iostream>
#include <vector>
#include <set>
#include <algoritmos.h>
using namespace std;


class Cluster{
    private:
        vector<double> centroide;
        set<int> puntos_incluidos;
        double distancia_media_intracluster;


    public:

        Cluster();
        vector<double> getCentroide();
        vector<vector<double>> getPuntos();
        void calcularCentroide();
        void calcularDistanciaMediaIntracluster();
        double getDistanciaMediaIntracluster();
};

#endif