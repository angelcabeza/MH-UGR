#include <iostream>
#include "PAR.h"
#include "timer.h"
#include <cmath>
using namespace std;

int main (int argc , char * argv[]){

    if (argc < 6){
        cerr << "Error en los argumentos" << endl;
        cerr << "Uso: " << argv[0] << " fichero_datos, fichero_restricciones, num_clusters, semilla, distancia_optima_fichero_datos" << endl;
        exit(-1);
    }

    string datos = argv[1];
    string restricciones = argv[2];
    int num_clusters = atoi(argv[3]);
    int semilla = atoi(argv[4]);
    double distancia_optima = atof(argv[5]);

    PAR problema(datos,restricciones,num_clusters,semilla);

    vector<PAR::Cluster> clusters;

    /*cout << "Llamando a greedy.." << endl;

    start_timers();
    clusters = problema.Greedy();
    double tiempo = elapsed_time();

    cout << "Terminado el greedy" << endl;

    for (int i = 0; i < clusters.size(); i++){
        cout << "El cluster " << i << " ha terminado con: " << clusters[i].getPuntos().size() << " elementos" << endl;
    }

    cout << endl << endl;

    problema.calcularAgregado();
    cout << "Agregado = " << problema.getAgregado() << endl;

    cout << "Tasa_inf = " << problema.calcularInfeasibilityCluster() << endl;

    cout << "Desviacion general de la particion: " << problema.getDesviacionGeneralParticion() << endl;
    
    cout << "Error_distancia = " << abs(problema.getDesviacionGeneralParticion() - distancia_optima) << endl;

    cout << "Tiempo = " << tiempo << endl << endl << endl;

    cout << " -------------------------------------------------------------- " << endl;

    cout << " BUSQUEDA LOCAL" << endl;

    clusters.clear();

    PAR problema_BL(datos,restricciones,num_clusters,semilla);

    cout << "Vamos a generar la solucion inicial" << endl;
    vector<PAR::Cluster> sol_ini = problema_BL.generarSolucionAleatoria(num_clusters);

    cout << "LLamando a búsqueda local..." << endl;

    start_timers();
    clusters = problema_BL.BusquedaLocal(sol_ini,100000);
    tiempo = elapsed_time();

    cout << "Búsqueda local terminada" << endl;

    for (int i = 0; i < clusters.size(); i++){
        cout << "El cluster " << i << " ha terminado con: " << clusters[i].getPuntos().size() << " elementos" << endl;
    }

    cout << endl << endl;

    problema_BL.calcularAgregado();
    cout << "Agregado = " << problema_BL.getAgregado() << endl;

    cout << "Tasa_inf = " << problema_BL.calcularInfeasibilityCluster() << endl;

    cout << "Desviación general particion: " << problema_BL.getDesviacionGeneralParticion() << endl;
    
    cout << "Error_distancia = " << abs(problema_BL.getDesviacionGeneralParticion() - distancia_optima) << endl;

    cout << "Tiempo = " << tiempo << endl << endl << endl;
    */

    cout << "Algoritmo Genético" << endl;

    double tiempo;

    clusters.clear();

    PAR problema_AG(datos,restricciones,num_clusters,semilla);
    cout << "LLamando al algoritmo genetico..." << endl;

    start_timers();
    clusters = problema_AG.AlgGenetico(100000,50,0.001,1,operador_cruce::SEGMENTO_FIJO,tipo_generacion::ESTACIONARIO,false);
    tiempo = elapsed_time();

    cout << "Algoritmo Genetico terminado" << endl;

    for (int i = 0; i < clusters.size(); i++){
        cout << "El cluster " << i << " ha terminado con: " << clusters[i].getPuntos().size() << " elementos" << endl;
    }

    cout << endl << endl;

    problema_AG.calcularAgregado();
    cout << "Agregado = " << problema_AG.getAgregado() << endl;

    cout << "Tasa_inf = " << problema_AG.calcularInfeasibilityCluster() << endl;

    cout << "Desviación general particion: " << problema_AG.getDesviacionGeneralParticion() << endl;

    cout << "Error_distancia = " << abs(problema_AG.getDesviacionGeneralParticion() - distancia_optima) << endl;

    cout << "Tiempo = " << tiempo << endl << endl << endl;
    return 0;
}