#include <cluster.h>
#include <math>

Cluster::Cluster(){}

vector<double> Cluster::getCentroide(){
    return centroide;
}

vector<vector<double>> Cluster::getPuntos(){
    return puntos_incluidos;
}

void Cluster::calcularCentroide(){

    // Creamos una variable centroide donde haremos la sumatoria  
    vector<double> centroide_nuevo;
    centroide_nuevo.resize(centroide.size());


    // Lo inicializamos a 0
    for (int i = 0; i < centroide_nuevo.size(); i++){
        centroide_nuevo[i] = 0.0;
    }


    // Sumamos todos los puntos que tiene el cluster
    for (int i = 0; i < puntos_incluidos.size(); i++){
        for (int j = 0; i < puntos_incluidos[i].size(); j++){
            centroide_nuevo[j] += puntos_incluidos[i][j];
        }
    }

    // Hacemos la media de los puntos que tiene el cluster
    for (int i = 0; i < centroide.size(); i++){
        centroide[i] = centroide_nuevo[i] / centroide.size();
    }
}


double Cluster::distanciaEuclidea(vector<double> a){

    double distancia = 0;

    for (int i = 0; i < centroide.size(); i++){
        distancia = pow(abs(a[i] - centroide[i]),2)
    }

    return sqrt(distancia);
}

void Cluster::calcularDistanciaMediaIntracluster(){
    double sumatoria = 0;

    for (int i = 0; i < puntos_incluidos.size(); i++){
        sumatoria += distanciaEuclidea(puntos_incluidos[i]);
    }

    distancia_media_intracluster = sumatoria/puntos_includios.size();
}