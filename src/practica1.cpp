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

    cout << "AGG-UN" << endl;

    double tiempo;

    clusters.clear();

    PAR problema_AG_UN(datos,restricciones,num_clusters,semilla);
    cout << "LLamando al algoritmo genetico..." << endl;

    start_timers();
    clusters = problema_AG_UN.AlgGenetico(100000,50,0.001,0.7,operador_cruce::UNIFORME,tipo_generacion::GENERACIONAL,true);
    tiempo = elapsed_time();

    cout << "Algoritmo Genetico terminado" << endl;

    for (int i = 0; i < clusters.size(); i++){
        cout << "El cluster " << i << " ha terminado con: " << clusters[i].getPuntos().size() << " elementos" << endl;
    }

    cout << endl << endl;

    problema_AG_UN.calcularAgregado();
    cout << "Agregado = " << problema_AG_UN.getAgregado() << endl;

    cout << "Tasa_inf = " << problema_AG_UN.calcularInfeasibilityCluster() << endl;

    cout << "Desviación general particion: " << problema_AG_UN.getDesviacionGeneralParticion() << endl;

    cout << "Error_distancia = " << abs(problema_AG_UN.getDesviacionGeneralParticion() - distancia_optima) << endl;

    cout << "Tiempo = " << tiempo << endl << endl << endl;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    cout << "AGG-SF" << endl;

    clusters.clear();

    PAR problema_AG_SF(datos,restricciones,num_clusters,semilla);
    cout << "LLamando al algoritmo genetico..." << endl;

    start_timers();
    clusters = problema_AG_SF.AlgGenetico(100000,50,0.001,0.7,operador_cruce::SEGMENTO_FIJO,tipo_generacion::GENERACIONAL,true);
    tiempo = elapsed_time();

    cout << "Algoritmo Genetico terminado" << endl;

    for (int i = 0; i < clusters.size(); i++){
        cout << "El cluster " << i << " ha terminado con: " << clusters[i].getPuntos().size() << " elementos" << endl;
    }

    cout << endl << endl;

    problema_AG_SF.calcularAgregado();
    cout << "Agregado = " << problema_AG_SF.getAgregado() << endl;

    cout << "Tasa_inf = " << problema_AG_SF.calcularInfeasibilityCluster() << endl;

    cout << "Desviación general particion: " << problema_AG_SF.getDesviacionGeneralParticion() << endl;

    cout << "Error_distancia = " << abs(problema_AG_SF.getDesviacionGeneralParticion() - distancia_optima) << endl;

    cout << "Tiempo = " << tiempo << endl << endl << endl;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    cout << "AGE-SF" << endl;

    clusters.clear();

    PAR problema_AGE_SF(datos,restricciones,num_clusters,semilla);
    cout << "LLamando al algoritmo genetico..." << endl;

    start_timers();
    clusters = problema_AGE_SF.AlgGenetico(100000,50,0.001,1,operador_cruce::SEGMENTO_FIJO,tipo_generacion::ESTACIONARIO,true);
    tiempo = elapsed_time();

    cout << "Algoritmo Genetico terminado" << endl;

    for (int i = 0; i < clusters.size(); i++){
        cout << "El cluster " << i << " ha terminado con: " << clusters[i].getPuntos().size() << " elementos" << endl;
    }

    cout << endl << endl;

    problema_AGE_SF.calcularAgregado();
    cout << "Agregado = " << problema_AGE_SF.getAgregado() << endl;

    cout << "Tasa_inf = " << problema_AGE_SF.calcularInfeasibilityCluster() << endl;

    cout << "Desviación general particion: " << problema_AGE_SF.getDesviacionGeneralParticion() << endl;

    cout << "Error_distancia = " << abs(problema_AGE_SF.getDesviacionGeneralParticion() - distancia_optima) << endl;

    cout << "Tiempo = " << tiempo << endl << endl << endl;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    cout << "AGE-UN" << endl;

    clusters.clear();

    PAR problema_AGG_UN(datos,restricciones,num_clusters,semilla);
    cout << "LLamando al algoritmo genetico..." << endl;

    start_timers();
    clusters = problema_AGG_UN.AlgGenetico(100000,50,0.001,1,operador_cruce::UNIFORME,tipo_generacion::ESTACIONARIO,true);
    tiempo = elapsed_time();

    cout << "Algoritmo Genetico terminado" << endl;

    for (int i = 0; i < clusters.size(); i++){
        cout << "El cluster " << i << " ha terminado con: " << clusters[i].getPuntos().size() << " elementos" << endl;
    }

    cout << endl << endl;

    problema_AGG_UN.calcularAgregado();
    cout << "Agregado = " << problema_AGG_UN.getAgregado() << endl;

    cout << "Tasa_inf = " << problema_AGG_UN.calcularInfeasibilityCluster() << endl;

    cout << "Desviación general particion: " << problema_AGG_UN.getDesviacionGeneralParticion() << endl;

    cout << "Error_distancia = " << abs(problema_AGG_UN.getDesviacionGeneralParticion() - distancia_optima) << endl;

    cout << "Tiempo = " << tiempo << endl << endl << endl;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    cout << "AM-(1,1.0)" << endl;

    clusters.clear();

    PAR problema_AM_110(datos,restricciones,num_clusters,semilla);
    cout << "LLamando al algoritmo memetico..." << endl;

    start_timers();
    clusters = problema_AM_110.AlgMemetico(100000,50,0.001,0.7,true,tipo_memetico::M_110);
    tiempo = elapsed_time();

    cout << "Algoritmo Memetico terminado" << endl;

    for (int i = 0; i < clusters.size(); i++){
        cout << "El cluster " << i << " ha terminado con: " << clusters[i].getPuntos().size() << " elementos" << endl;
    }

    cout << endl << endl;

    problema_AM_110.calcularAgregado();
    cout << "Agregado = " << problema_AM_110.getAgregado() << endl;

    cout << "Tasa_inf = " << problema_AM_110.calcularInfeasibilityCluster() << endl;

    cout << "Desviación general particion: " << problema_AM_110.getDesviacionGeneralParticion() << endl;

    cout << "Error_distancia = " << abs(problema_AM_110.getDesviacionGeneralParticion() - distancia_optima) << endl;

    cout << "Tiempo = " << tiempo << endl << endl << endl;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
    cout << "AM-(1,0.1)" << endl;

    clusters.clear();

    PAR problema_AM_101(datos,restricciones,num_clusters,semilla);
    cout << "LLamando al algoritmo memetico..." << endl;

    start_timers();
    clusters = problema_AM_101.AlgMemetico(100000,50,0.001,0.7,true,tipo_memetico::M_101);
    tiempo = elapsed_time();

    cout << "Algoritmo Memetico terminado" << endl;

    for (int i = 0; i < clusters.size(); i++){
        cout << "El cluster " << i << " ha terminado con: " << clusters[i].getPuntos().size() << " elementos" << endl;
    }

    cout << endl << endl;

    problema_AM_101.calcularAgregado();
    cout << "Agregado = " << problema_AM_101.getAgregado() << endl;

    cout << "Tasa_inf = " << problema_AM_101.calcularInfeasibilityCluster() << endl;

    cout << "Desviación general particion: " << problema_AM_101.getDesviacionGeneralParticion() << endl;

    cout << "Error_distancia = " << abs(problema_AM_101.getDesviacionGeneralParticion() - distancia_optima) << endl;

    cout << "Tiempo = " << tiempo << endl << endl << endl;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    cout << "AM-(1,0.1mej)" << endl;

    clusters.clear();

    PAR problema_AM_101_mej(datos,restricciones,num_clusters,semilla);
    cout << "LLamando al algoritmo memetico..." << endl;

    start_timers();
    clusters = problema_AM_101_mej.AlgMemetico(100000,50,0.001,0.7,true,tipo_memetico::M_101mej);
    tiempo = elapsed_time();

    cout << "Algoritmo Memetico terminado" << endl;

    for (int i = 0; i < clusters.size(); i++){
        cout << "El cluster " << i << " ha terminado con: " << clusters[i].getPuntos().size() << " elementos" << endl;
    }

    cout << endl << endl;

    problema_AM_101_mej.calcularAgregado();
    cout << "Agregado = " << problema_AM_101_mej.getAgregado() << endl;

    cout << "Tasa_inf = " << problema_AM_101_mej.calcularInfeasibilityCluster() << endl;

    cout << "Desviación general particion: " << problema_AM_101_mej.getDesviacionGeneralParticion() << endl;

    cout << "Error_distancia = " << abs(problema_AM_101_mej.getDesviacionGeneralParticion() - distancia_optima) << endl;

    cout << "Tiempo = " << tiempo << endl << endl << endl;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    return 0;
}