#ifndef ALGORITMOS_H_INCLUDED
#define ALOGRITMOS_H_INCLUDED


#include <iostream>
#include <vector>
#include <cluster.h>
using namespace std;


int calcularInfeasibility(vector<vector<double>> & datos,const vector<vector<int>> & restricciones, const int indice_elemento_nuevo, Cluster cluster );

double desviacionGeneralParticion(const vector<Cluster> & clusters);

vector<Cluster> Greedy (const int K, vector<vector<double>> datos, vector<vector<int>> restricciones);

int seleccionarCluster(const vector<vector<double>> datos,const vector<vector<int>> & restricciones, const int indice_elemento_nuevo, vector<Cluster> cluster);

double distanciaEuclidea(vector<double> punto_a,vector<double> punto_b);

int devolverPosPunto(vector<vector<double>> datos, vector<double> punto);



#endif