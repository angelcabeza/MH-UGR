#include <algoritmos.h>
#include <math.h>
#include <random.h>
#include <algorithm>
using namespace std;



int calcularInfeasibility(const vector<vector<double>> datos,const vector<vector<int>> & restricciones, const int indice_elemento_nuevo, vector<Cluster> cluster,int indice_cluster_evaluando ){

    int infeasibility = 0;

    // Si dentro del cluster que estamos evaluando hay una restriccion
    // CannotLink aumentamos la infeasibility
    for (int i = 0; i < cluster[indice_cluster_evaluando].getPuntos().size(); i++){

        int pos = devolverPosPunto(datos,cluster[indice_cluster_evaluando].getPuntos()[i]);

        if (pos != -1 && restricciones[indice_elemento_nuevo][pos] == -1 )
            infeasibility += 1;
    }

    // Si fuera del cluster que estamos evaluando hay restricciones
    // MustLink aumentamos la infeasibility
    for (int i = 0; i < cluster.size(); i++){
        if (i != indice_cluster_evaluando){
            for(int j = 0; j < cluster[i].getPuntos().size(); j++){

                auto pos = devolverPosPunto(datos,cluster[i].getPuntos()[j]);

                if ( pos != -1 && restricciones[indice_elemento_nuevo][pos] == 1 )
                    infeasibility += 1;
            }
        }
    }
    
    return infeasibility;
}


double desviacionGeneralParticion(vector<Cluster> clusters){

    double sum = 0;

    for (int i = 0; i < clusters.size(); i++){
        sum += clusters[i].getDistanciaMediaIntracluster();
    }

    return sum/clusters.size();
}

vector<Cluster> Greedy (const int K, vector<vector<double>> datos, vector<vector<int>> restricciones){
    vector<Cluster> clusters;
    clusters.resize(K);

    // Vamos a inicializar los centroides de los clusters de manera aleatoria
    // Para ello tenemos que acotar el dominio a un minimo y un máximo
    double maximo = -999999;
    double minimo = 999999;

    for(int i = 0; i < datos.size(); i++){
        for (int j = 0; j < datos[i].size(); j++){
            if (datos[i][j] > maximo){
                maximo = datos[i][j];
            }

            if (datos[i][j] < minimo){
                minimo = datos[i][j];
            }
        }
    }

    // Una vez hemos encontrado el dominio del problema generamos los centroides aleatorios
    for (int i = 0; i < clusters.size(); i++){
        for (int j = 0; datos[0].size(); j++){
            clusters[i].getCentroide().push_back(Randfloat(minimo,maximo));
        }
    }

    // Vamos a generar indices aleatorios
    vector<int> indices;

    for (int i = 0; i < datos.size(); i++){
        indices.push_back(i);
    }

    random_shuffle(indices.begin(),indices.end());

    bool hay_cambios = true;

    while(hay_cambios){
        hay_cambios = false;

        int indice_cluster_seleccionado;
        int i = 0;
        // Iteramos sobre el vector de indices mezclado
        for( auto it = indices.begin(); it < indices.end(); ++it){
            
            int pos_indice_actual = devolverPosPunto(datos,datos[(*it)]);

            indice_cluster_seleccionado = seleccionarCluster(datos,restricciones,pos_indice_actual,clusters);

            if (indice_cluster_seleccionado != -1){
                clusters[indice_cluster_seleccionado].getPuntos().push_back(datos[pos_indice_actual]);
                hay_cambios = true;
            }
        }
    }

    return clusters;

}


int devolverPosPunto(vector<vector<double>> datos, vector<double> punto){

    bool encontrado = false;
    int i;
    int pos = -1;

    for(i = 0; i < datos.size() && !encontrado; i++){
        encontrado = true;
        for (int j = 0; j < datos[i].size() && encontrado; j++){
            if(datos[i][j] != punto[j]){
                encontrado = false;
            }
        }
    }

    if (encontrado){
        int pos = i;
    }

    return pos;
}

int seleccionarCluster(const vector<vector<double>> datos,const vector<vector<int>> & restricciones, const int indice_elemento_nuevo, vector<Cluster> cluster){
    vector<pair<int,int>> infeasibility_cluster;

    for(int i = 0; i < cluster.size(); i++){
        infeasibility_cluster.push_back(make_pair(calcularInfeasibility(datos,restricciones,indice_elemento_nuevo,cluster,i),i));
    }

    // Ordenamos el vector por su infeasibility
    sort(infeasibility_cluster.begin(), infeasibility_cluster.end());

    int menor_restricciones = infeasibility_cluster.begin()->first;
    double distancia;
    double menor_distancia = 999999;
    int cluster_elegido = -1;

    for (auto it = infeasibility_cluster.begin(); (*it).first == menor_restricciones && it != infeasibility_cluster.end(); ++it){
        distancia = distanciaEuclidea(cluster[(*it).second].getCentroide(),datos[indice_elemento_nuevo]);

        if (distancia < menor_distancia){
            menor_distancia = distancia;
            cluster_elegido = (*it).second;
        }
    }

    return cluster_elegido;
}

double distanciaEuclidea(vector<double> p1, vector<double> p2){

    double distancia = 0;

    for (int i = 0; i < p1.size(); i++){
        distancia = pow(abs(p1[i] - p2[i]),2);
    }

    return sqrt(distancia);
}