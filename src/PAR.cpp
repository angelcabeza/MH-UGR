#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <math.h>
#include "PAR.h"
#include "random.h"


using namespace std;

PAR::PAR(string fichero_datos,string fichero_restricc,int num_clusters,int semilla){
    leerDatos(fichero_datos);
    leerRestricciones(fichero_restricc);

    int num_restricciones = 0;

    for (auto it = restricciones.begin(); it != restricciones.end(); ++it){
        lista_restricciones.push_back(make_tuple( (*it).first.first, (*it).first.second , (*it).second ));

        if ( (*it).first.first < (*it).first.second ){

            if ((*it).second == -1 || (*it).second == 1)
                num_restricciones++;

            lista_restricciones_diag_sup.push_back(make_tuple((*it).first.first, (*it).first.second , (*it).second ));
        }
    }

    datos.erase(datos.end());

    this->num_clusters = num_clusters;

    for (int i = 0; i < num_clusters; i++){
        clusters.push_back(Cluster(*this));
    }

    double distancia;
    double mayor_distancia = -1;

    for (int i = 0; i < datos.size(); i++){
        for (int j = i; j < datos.size(); j++){
            distancia = distanciaEuclidea(datos[i],datos[j]);

            if (distancia > mayor_distancia){
                mayor_distancia = distancia;
            }
        }
    }

    LAMBDA = mayor_distancia / (restricciones.size()/2.0);

    Set_random(semilla);
}

void PAR::leerDatos(const string path){

    ifstream fichero (path);
    vector<double> fila;
    string dato;
    string linea;
    string dato_linea;
    size_t pos_coma;
    double valor;

    if (fichero.is_open()){

        while(!fichero.eof()){
            
            // Cojo una línea del fichero
            getline(fichero,linea);

            while (!linea.empty()){
                pos_coma = linea.find(',');

                if (pos_coma != linea.npos){
                    dato = linea.substr(0,pos_coma);
                    valor = stod(dato);
                    fila.push_back(valor);
                    linea.erase(0,pos_coma+1);
                }
                else{
                    valor = stod(linea);
                    linea = "";
                    fila.push_back(valor);
                }
            }

            datos.push_back(fila);
            fila.clear();
        }
    }
    else{
        cout << "Error al abrir el fichero" << endl;
    }
}

void PAR::leerRestricciones(const string path){
    std::ifstream entrada(path);

	if (entrada.is_open()){

		std::string linea;

		std::string valor;


		int num_linea = 0;
		int num_elemento = 0;

		int v_res;


		std::istringstream dato_linea;

		getline(entrada, linea);

		while( !entrada.eof() ){
			dato_linea.str(linea);

			//restricciones.resize(num_linea+1);

			num_elemento = 0;

			while( !dato_linea.eof() /*&& num_elemento < num_linea */){

				getline(dato_linea, valor, ',');

				v_res = atoi(valor.c_str());

				//restricciones[num_linea].push_back(atoi(valor.c_str()));

				if (v_res != 0 && num_linea != num_elemento)
					restricciones.insert( std::make_pair( std::make_pair(num_linea, num_elemento), v_res ));

				num_elemento++;

			}

			num_linea++;


			dato_linea.clear();
			dato_linea.str("");

			getline(entrada, linea);

		}

    }
    else{
        cout << "Error al abrir el fichero" << endl;
    }
}

int PAR::calcularInfeasibility(const int indice_elemento_nuevo, int indice_cluster_evaluando ){
    int infeasibility = 0;

    // Si dentro del cluster que estamos evaluando hay una restriccion
    // CannotLink aumentamos la infeasibility
    for (auto it = clusters[indice_cluster_evaluando].getPuntos().begin(); it != clusters[indice_cluster_evaluando].getPuntos().end(); ++it){

        auto pos = restricciones.find(make_pair(indice_elemento_nuevo,(*it)));

        if ( pos != restricciones.end() && (*pos).second == -1){
            infeasibility ++;
        }
    }

    // Para todos los demás cluster vemos cuántas ML incumpliriamos
    for (int i = 0; i < clusters.size(); i++){
        if ( i != indice_cluster_evaluando){
            
            for (auto it = clusters[i].getPuntos().begin(); it != clusters[i].getPuntos().end(); ++it){

                auto pos = restricciones.find(make_pair(indice_elemento_nuevo,(*it)));

                if (pos != restricciones.end() && (*pos).second == 1){
                    infeasibility++;
                }
            }
        }
    }
    
    return infeasibility;
}

void PAR::desviacionGeneralParticion(){

    double sum = 0;

    for (int i = 0; i < clusters.size(); i++){
        clusters[i].calcularDistanciaMediaIntracluster();
        sum += clusters[i].getDistanciaMediaIntracluster();
    }; 

    desviacion_general = sum/clusters.size();
}

double PAR::getDesviacionGeneralParticion(){
    return desviacion_general;
}

vector<PAR::Cluster> PAR::Greedy (){

    // inicialización de indices aleatorios
    std::vector<int> indices;

    for (unsigned i = 0; i < datos.size(); i++){
        indices.push_back(i);
    }

    std::random_shuffle(indices.begin(), indices.end());


    double minimo = std::numeric_limits<double>::infinity();

    double maximo = std::numeric_limits<double>::infinity();
    maximo = -maximo;


    for (unsigned i = 0; i < datos.size(); i++){
        for (unsigned j = 0; j < datos[i].size(); j++){
            if (datos[i][j] < minimo){
                minimo = datos[i][j];
            }
            if (datos[i][j] > maximo){
                maximo = datos[i][j];
            }
        }
    }

    // inicialización de centroides aleatorios
    for (unsigned i = 0; i < clusters.size(); i++){
        std::vector<double> n_centroide;
        for (unsigned j = 0; j < clusters[i].getCentroide().size(); j++){
            n_centroide.push_back(Randfloat(minimo, maximo));
        }
        clusters[i].clearPuntos();
        clusters[i].setCentroide(n_centroide);
    }



    bool hay_cambios = false;
    std::vector<bool> cambios(clusters.size(), false);
    std::vector<bool> cambios_anterior = cambios;
    bool sol_vacia = false;

    int num_cluster;

    std::vector<Cluster> n_sol = clusters;
    int contador = 0;

    do {
    hay_cambios = false;
    cambios = std::vector<bool>(clusters.size(), false);

    for (auto it = indices.begin(); it != indices.end(); ++it){

        num_cluster = seleccionarCluster( (*it) );

        clusters[num_cluster].aniadirElemento( (*it) );

    }

    for (unsigned i = 0; i < clusters.size(); i++){

        cambios[i] = n_sol[i].getPuntos() != clusters[i].getPuntos();

        if (clusters[i].getPuntos().size() == 0){
            double max_dist = numeric_limits<double>::min();
            double dist;
            int cluster_mas_elementos = i;

            for(int j = 0; j < clusters.size(); j++){
                if (i != j){
                    if (clusters[j].getPuntos().size() > clusters[cluster_mas_elementos].getPuntos().size()){
                        cluster_mas_elementos = j;
                    }
                }
            }

            int indice_elemento_sacar;
            for (auto k = clusters[cluster_mas_elementos].getPuntos().begin(); k != clusters[cluster_mas_elementos].getPuntos().end(); ++k){

                dist = distanciaEuclidea(clusters[cluster_mas_elementos].getCentroide(),datos[(*k)]);

                if (dist > max_dist){
                    max_dist = dist;
                    indice_elemento_sacar = (*k);
                }
            }

            clusters[i].aniadirElemento(indice_elemento_sacar);
            clusters[i].calcularCentroide();
            clusters[cluster_mas_elementos].eliminarElemento(indice_elemento_sacar);
            clusters[cluster_mas_elementos].calcularCentroide();

        }

        if (cambios[i]){
            clusters[i].calcularCentroide();

            n_sol[i] = clusters[i];
        }


        clusters[i].clearPuntos();

    }

    for (unsigned i = 0; i < cambios.size(); i++){
        hay_cambios = hay_cambios || cambios[i];
    }

    hay_cambios = cambios != cambios_anterior;

    // ESTO LO HAGO EXCLUSIVAMENTE PORQUE EN EL DATASET BUPA 10% RESTRICCIONES TENGO PROBLEMAS
    if (sol_vacia){
        hay_cambios = true;
    }

    sol_vacia = false;

    cambios_anterior = cambios;
    contador++;

    } while(hay_cambios);


    clusters = n_sol;

    desviacionGeneralParticion();

    return clusters;
}

int PAR::devolverPosPunto(vector<double> punto){
    bool encontrado = false;
    int i;
    int pos = -1;

    for(i = 0; i < datos.size() && !encontrado; i++){
        encontrado = true;
        for (int j = 0; j < datos[i].size() && encontrado; j++){
            if( (datos[i][j] - punto[j]) > 0.01 ){
                encontrado = false;
            }
        }
    }

    if (encontrado){
        pos = i;
    }

    return pos;
}

int PAR::seleccionarCluster(const int elemento){
    double d;
	double menor_distancia = std::numeric_limits<double>::infinity();
	int cluster_menor_distancia = -1;
	int menor_restricciones = restricciones.size();


	std::vector< std::pair<int, int>> aumento_infactibilidad;


	for (unsigned i = 0; i < clusters.size(); i++){

		aumento_infactibilidad.push_back( std::make_pair(calcularInfeasibility(elemento, i), i ) );

	}

	std::sort(aumento_infactibilidad.begin(), aumento_infactibilidad.end());

	menor_restricciones = aumento_infactibilidad.begin()->first;

	for (auto it = aumento_infactibilidad.begin(); it->first == menor_restricciones && it != aumento_infactibilidad.end(); ++it){

		d = distanciaEuclidea(clusters[(*it).second].getCentroide(), datos[elemento]);

		if (d < menor_distancia){
			menor_distancia = d;
			cluster_menor_distancia = (*it).second;
		}

	}

	return cluster_menor_distancia;
}

double PAR::distanciaEuclidea(vector<double> & p1, vector<double> & p2) const {
    double distancia = 0;

    for (int i = 0; i < p1.size(); i++){
        distancia = pow(abs(p1[i] - p2[i]),2);
    }

    return distancia;
}

int PAR::calcularInfeasibilityCluster(){

    int infeasibility = 0;
    for(auto it = lista_restricciones_diag_sup.begin(); it != lista_restricciones_diag_sup.end(); ++it){

        if ( get<0>((*it)) <= get<1>((*it)) ){

            int cluster1 = buscarClusterContieneElemento(get<0>((*it)));
            int cluster2 = buscarClusterContieneElemento(get<1>((*it)));

            if (get<2>((*it)) == 1 && cluster1 != cluster2 ){
                infeasibility++;
            }

            if (get<2>((*it)) == -1 && cluster1 == cluster2 ){
                infeasibility++;
            }
        }
    }

    return infeasibility;
}

int PAR::buscarClusterContieneElemento(int elemento){

    int cluster = -1;
    bool encontrado = false;

    for (int i = 0; i < clusters.size() && !encontrado; i++ ){

        auto it = clusters[i].getPuntos().find(elemento);

        if (it != clusters[i].getPuntos().end()){
            encontrado = true;
            cluster = i;
        }
    }

    return cluster;
}

void PAR::calcularAgregado(){
    agregadoSol = desviacion_general + calcularInfeasibilityCluster() * LAMBDA;
}

double PAR::getAgregado(){
    return agregadoSol;
}

vector<PAR::Cluster> PAR::generarSolucionAleatoria(int num_clusters){
    vector<PAR::Cluster> sol;

    for (int i = 0; i < num_clusters; i++){
        sol.push_back(PAR::Cluster((*this)));
        sol[i].clearPuntos();
    }

    vector<int> ind_alea;

    for (int i = 0; i < datos.size(); i++){
        ind_alea.push_back(i);
    }

    random_shuffle(ind_alea.begin(),ind_alea.end());

    auto it = ind_alea.begin();

    // metemos 1 elemento en cada cluster para que ninguno se quede vacio
    for (int i = 0; i < num_clusters; i++){
        sol[i].aniadirElemento((*it));
        ++it;
    }

    // Vamos añadiendo elementos a clusters elegido de forma aleatoria
    int indice;
    while(it != ind_alea.end()){
        indice = Randint(0,sol.size()-1);
        sol[indice].aniadirElemento((*it));
        it++;
    }


    // Inicializamos centroide y la distancia media intracluster
    for(int i = 0; i < sol.size(); i++){
        sol[i].calcularDistanciaMediaIntracluster();
    }

    return sol;
}

vector<PAR::Cluster> PAR::BusquedaLocal(vector<PAR::Cluster> & sol_ini,const int MAX_ITER){

    clusters = sol_ini;
    desviacionGeneralParticion();

    bool hay_mejor = true;
    int contador = 0;

    //Inicializacion de indices que luego mezclaremos para aleatorizarlos
    vector<int> indices;
    vector<int> indices_clusters;

    for (int i = 0; i < datos.size(); i++){
        indices.push_back(i);
    }

    for(int i = 0; i< clusters.size(); i++){
        indices_clusters.push_back(i);
    }

    int infac = calcularInfeasibilityCluster();
    int nueva_infac = infac;

    double funcion_objetivo = desviacion_general + (calcularInfeasibilityCluster() * LAMBDA);
    double nueva_func_objetivo = 0.0;

    while(hay_mejor && contador < MAX_ITER){
        hay_mejor = false;

        random_shuffle(indices.begin(),indices.end());
        random_shuffle(indices_clusters.begin(),indices_clusters.end());

        for (auto it = indices.begin(); it != indices.end() && !hay_mejor; ++it){

            for (auto jt = indices_clusters.begin(); jt != indices_clusters.end() && !hay_mejor; ++jt){
                
                int clust_anterior = buscarClusterContieneElemento( (*it) );

                if (clust_anterior != (*jt) && clusters[clust_anterior].getPuntos().size() > 1){

                    clusters[clust_anterior].eliminarElemento( (*it) );

                    // Le quitamos la infeasibility que producia el elemento anterior
                    nueva_infac -= calcularInfeasibility( (*it) , clust_anterior);

                    // Le sumamos la que produce el nuevo elemento
                    nueva_infac += calcularInfeasibility( (*it), (*jt));

                    // Añadimos el elemento al cluster anterior y calculamos la desviacion general
                    clusters[(*jt)].aniadirElemento( (*it) );
                    desviacionGeneralParticion();

                    nueva_func_objetivo = desviacion_general + nueva_infac * LAMBDA;

                    contador++;

                    if (nueva_func_objetivo < funcion_objetivo){
                        funcion_objetivo = nueva_func_objetivo;
                        infac = nueva_infac;
                        hay_mejor = true;
                    }
                    else{
                        clusters[(*jt)].eliminarElemento((*it));
                        clusters[clust_anterior].aniadirElemento((*it));
                        nueva_infac = infac;
                    }


                }
            }
        }
    }

    desviacionGeneralParticion();
    calcularAgregado();

    return clusters;
}


// CLASE CLUSTER

PAR::Cluster::Cluster(PAR & P){
    this->p = &P;
    centroide = vector<double>(p->datos[0].size(),0);
}

vector<double> & PAR::Cluster::getCentroide(){
    return centroide;
}

set<int> & PAR::Cluster::getPuntos(){
    return puntos_incluidos;
}

set<int> PAR::Cluster::getPuntos() const{
    return puntos_incluidos;
}

void PAR::Cluster::calcularCentroide(){

    // Lo inicializamos a 0
    for (int i = 0; i < centroide.size(); i++){
        centroide[i] = 0.0;
    }

    // Sumamos todos los puntos que tiene el cluster
    for (auto it = puntos_incluidos.begin(); it != puntos_incluidos.end(); ++it){
        for (int j = 0; j < p->datos[(*it)].size(); ++j){
            centroide[j] += p->datos[(*it)][j];
        }
    }
    // Hacemos la media de los puntos que tiene el cluster
    for (int i = 0; i < centroide.size(); i++){
        centroide[i] = centroide[i] / centroide.size();
    }
}

void PAR::Cluster::calcularDistanciaMediaIntracluster(){
    double sumatoria = 0;

    calcularCentroide();

    for (auto i = puntos_incluidos.begin(); i != puntos_incluidos.end(); ++i){
        sumatoria += p->distanciaEuclidea(p->datos[(*i)],centroide);
    }

    distancia_media_intracluster = sumatoria/puntos_incluidos.size();
}

double PAR::Cluster::getDistanciaMediaIntracluster(){
    return distancia_media_intracluster;
}

void PAR::Cluster::aniadirElemento(int punto){
    puntos_incluidos.insert(punto);
}

void PAR::Cluster::setCentroide(vector<double> n_centroide){
    centroide = n_centroide;
}

void PAR::Cluster::clearPuntos(){
    puntos_incluidos.clear();
}

PAR::Cluster & PAR::Cluster::operator=(const PAR::Cluster & otro){
    this->p = otro.p;
    this->centroide = otro.centroide;
    this->puntos_incluidos = otro.puntos_incluidos;
    this->distancia_media_intracluster = otro.distancia_media_intracluster;

    return (*this);
}

void PAR::Cluster::eliminarElemento(int elemento){
    puntos_incluidos.erase(elemento);
}