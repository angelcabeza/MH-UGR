#ifndef PAR_H_INCLUDED
#define PAR_H_INCLUDED

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <list>
using namespace std;

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
        double distanciaEuclidea(vector<double> & p1, vector<double> & p2) const;
        int buscarClusterContieneElemento(int elemento);
        void calcularAgregado();
        double getAgregado();
        vector<Cluster> generarSolucionAleatoria(int num_clusters);
        vector<Cluster> BusquedaLocal(vector<Cluster> & sol_inicial,const int MAX_ITER);

        //friend ostream & operator << (ostream & os, const PAR & p);
        

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