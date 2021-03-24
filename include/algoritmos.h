#ifndef ALGORITMOS_H_INCLUDED
#define ALOGRITMOS_H_INCLUDED


#include <iostream>
#include <vector>
#include <cluster.h>
using namespace std;


int calcularInfeasibility(const vector<vector<int>> & restricciones, const int indice_elemento_nuevo, Cluster cluster );

vector<Cluster> Greedy (const int K, vector<vector<double>> datos, vector<vector<int>> restricciones);




#endif