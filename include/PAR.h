#ifndef PAR_H_INCLUDED
#define PAR_H_INCLUDED

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <list>
using namespace std;


enum class operador_cruce {SEGMENTO_FIJO, UNIFORME};
enum class tipo_generacion {GENERACIONAL, ESTACIONARIO};
enum class tipo_memetico {M_110,M_101,M_101mej};

class PAR{
    public:

        class Cluster{
            private:
                vector<double> centroide;
                set<int> puntos_incluidos;
                double distancia_media_intracluster;
                PAR * p;

            public:

                Cluster(PAR & p);
                vector<double> & getCentroide();
                set<int> & getPuntos();
                set<int> getPuntos() const;
                void calcularCentroide();
                void calcularDistanciaMediaIntracluster();
                double getDistanciaMediaIntracluster();
                void aniadirElemento(int elemento);
                void setCentroide(vector<double> n_centroide);
                void clearPuntos();
                void eliminarElemento(int elemento);
                Cluster & operator = (const Cluster & otro);
                //friend ostream & operator << (ostream & os, const Cluster & c);
        };

        PAR(string fichero_datos,string fichero_restricc,int num_clusters, int semilla);
        int calcularInfeasibility(const int indice_elemento_nuevo,int indice_cluster_evaluando );
        int calcularInfeasibilityCluster();
        void desviacionGeneralParticion();
        double getDesviacionGeneralParticion();
        vector<Cluster> Greedy ();
        int devolverPosPunto(vector<double> punto);
        int seleccionarCluster(const int indice_elemento_nuevo);
        double distanciaEuclidea(const vector<double> & p1, const vector<double> & p2) const;
        int buscarClusterContieneElemento(int elemento);
        void calcularAgregado();
        double getAgregado();
        vector<Cluster> generarSolucionAleatoria(int num_clusters);
        vector<Cluster> BusquedaLocal(vector<Cluster> & sol_inicial,const int MAX_ITER);
        vector<Cluster> AlgGenetico(const int EV_MAX,const int TAM_POB,const double PROB_MUT, const double PROB_CRUCE, operador_cruce tipo_cruce, tipo_generacion tipo_gen ,bool elitismo);
        vector<Cluster> AlgMemetico(const int EV_MAX, const int TAM_POB,const double PROB_MUT, const double PROB_CRUCE,bool elitismo,tipo_memetico memtico);
        vector<int> clustersToSolucion( vector<Cluster> clustersSol);
        vector<Cluster> solucionToClusters(const vector<int> & sol);
        vector<pair<vector<int>,double>> operadorSeleccion(const vector<pair<vector<int>, double>> & poblacion, const int tam);
        int operadorCruceUniforme (vector<pair<vector<int>, double>> & poblacion, const double PROB_CRUCE);
        void repararCruce(vector<int> & reparado);
        int operadorCruceSegmentoFijo(vector<pair<vector<int>,double>> & poblacion, const double PROB_CRUCE);
        int operadorMutacion(vector<pair<vector<int>, double>> & poblacion, const double PROB_MUT,tipo_generacion tipo_gen);
        vector<vector<int>> generarPoblacionInicial(const int TAM_POB);
        int busquedaLocalSuave(pair<vector<int>,double> & solucion, const int num_fallos_permitidos);
        

    private:
        int num_clusters;
        double agregadoSol;
        double LAMBDA;
        double mayor_distancia;
        vector<Cluster> clusters;
        vector<vector<double>> datos;
        map<pair<int,int>,int> restricciones;
        list<tuple<int,int,int>> lista_restricciones;
        list<tuple<int,int,int>> lista_restricciones_diag_sup;
        double desviacion_general;

        void leerDatos(const string path);
        void leerRestricciones (const string path);

};


#endif