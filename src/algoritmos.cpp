#include <algoritmos.h



int calcularInfeasibility(const vector<vector<int>> & restricciones, const int indice_elemento_nuevo, vector<int> cluster ){

    int infeasibility = 0;

    for (int i = 0; i < cluster.size(); i++){
        if (restricciones[indice_elemento_nuevo][cluster[i]] != 0 )
            infeasibility += 1;
    }
    
    return infeasibility;
}