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

            if (distancia > mayor_distancia && distancia <= 1.0){
                mayor_distancia = distancia * 1.0;
            }
        }
    }


    LAMBDA = (mayor_distancia) / (restricciones.size()/2.0);

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
    ifstream entrada(path);

	if (entrada.is_open()){

		string linea;

		string valor;


		int num_linea = 0;
		int num_elemento = 0;

		int v_res;


		istringstream dato_linea;

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
					restricciones.insert( make_pair( make_pair(num_linea, num_elemento), v_res ));

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
    vector<int> indices;

    for (unsigned i = 0; i < datos.size(); i++){
        indices.push_back(i);
    }

    random_shuffle(indices.begin(), indices.end(), RandPositiveInt);

    double minimo = numeric_limits<double>::infinity();

    double maximo = numeric_limits<double>::infinity();
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
        vector<double> n_centroide;
        for (unsigned j = 0; j < clusters[i].getCentroide().size(); j++){
            n_centroide.push_back(Randfloat(minimo, maximo));
        }
        clusters[i].clearPuntos();
        clusters[i].setCentroide(n_centroide);
    }



    bool hay_cambios = false;
    vector<bool> cambios(clusters.size(), false);
    vector<bool> cambios_anterior = cambios;
    bool sol_vacia = false;

    int num_cluster;

    vector<Cluster> n_sol = clusters;
    int contador = 0;

    do {
    hay_cambios = false;
    cambios = vector<bool>(clusters.size(), false);

    for (auto it = indices.begin(); it != indices.end(); ++it){

        num_cluster = seleccionarCluster( (*it) );

        clusters[num_cluster].aniadirElemento( (*it) );

    }

    int indice_elemento_sacar;

    for (unsigned i = 0; i < clusters.size(); i++){

        cambios[i] = n_sol[i].getPuntos() != clusters[i].getPuntos();

        if (clusters[i].getPuntos().size() == 0){
            double max_dist = -99999999;
            double dist;
            int cluster_mas_elementos = i;

            for(int j = 0; j < clusters.size(); j++){
                if (i != j){
                    if (clusters[j].getPuntos().size() > clusters[cluster_mas_elementos].getPuntos().size()){
                        cluster_mas_elementos = j;
                    }
                }
            }

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

    hay_cambios = cambios != cambios_anterior;

    cambios_anterior = cambios;

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
	double menor_distancia = numeric_limits<double>::infinity();
	int cluster_menor_distancia = -1;
	int menor_restricciones = restricciones.size();


	vector< pair<int, int>> aumento_infactibilidad;


	for (unsigned i = 0; i < clusters.size(); i++){

		aumento_infactibilidad.push_back( make_pair(calcularInfeasibility(elemento, i), i ) );

	}

	sort(aumento_infactibilidad.begin(), aumento_infactibilidad.end());

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

double PAR::distanciaEuclidea(const vector<double> & p1, const vector<double> & p2) const {
    double distancia = 0.0;

    for (int i = 0; i < p1.size(); i++){
        distancia = pow(p1[i] - p2[i],2);
    }

    return sqrt(distancia);
}

int PAR::calcularInfeasibilityCluster(){

    int infeasibility = 0;
    for(auto it = lista_restricciones_diag_sup.begin(); it != lista_restricciones_diag_sup.end(); ++it){

            int cluster1 = buscarClusterContieneElemento(get<0>((*it)));
            int cluster2 = buscarClusterContieneElemento(get<1>((*it)));

            if (get<2>((*it)) == 1 && cluster1 != cluster2 ){
                infeasibility++;
            }

            if (get<2>((*it)) == -1 && cluster1 == cluster2 ){
                infeasibility++;
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

        random_shuffle(indices.begin(),indices.end(),RandPositiveInt);
        random_shuffle(indices_clusters.begin(),indices_clusters.end(),RandPositiveInt);

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

// PRÁCTICA 2
vector<int> PAR::clustersToSolucion( vector<Cluster> clustersSol) {
    vector<int> sol;

    int tam_total = 0;

    for (int i = 0; i < clustersSol.size(); i++){
        tam_total += clustersSol[i].getPuntos().size();
    }

    sol.resize(tam_total);

    for (int i = 0; i < clustersSol.size(); i++){
        for (auto it = clustersSol[i].getPuntos().begin(); it != clustersSol[i].getPuntos().end(); ++it){
            sol[(*it)] = i;
        }
    }

    return sol;
}

vector<PAR::Cluster> PAR::solucionToClusters(const vector<int> & sol){
    vector<PAR::Cluster> solucion;

    for (int i = 0; i < num_clusters; i++){
        solucion.push_back(Cluster((*this)));
    }

    for (int i = 0; i < sol.size(); i++){
        solucion[sol[i]].aniadirElemento(i);
    }

    return solucion;
}

vector<pair<vector<int>,double>> PAR::operadorSeleccion(const vector<pair<vector<int>, double>> & poblacion, const int tam){

    // Vector dondee guardaremos la población intermedia
    vector<pair<vector<int>,double>> poblacion_intermedia;

    // Vamos a hacer torneos binarios, sacamos dos índices aleatorios
    // y metemos aquel cromosoma que sea el mejor
    while ((int) poblacion_intermedia.size() != tam){
        int primer_candidato = RandPositiveInt(poblacion.size());
        int segundo_candidato = RandPositiveInt(poblacion.size());

        if(poblacion[primer_candidato].second > poblacion[segundo_candidato].second){
            poblacion_intermedia.push_back(poblacion[segundo_candidato]);
        }
        else{
            poblacion_intermedia.push_back(poblacion[primer_candidato]);
        }
    }

    return poblacion_intermedia;    
}

int PAR::operadorCruceUniforme (vector<pair<vector<int>, double>> & poblacion, const double PROB_CRUCE){
    // Numero de cruces que se espera hacer por evaluación
    const int NUM_CRUCES = (poblacion.size()/2) * PROB_CRUCE;

    int evaluaciones = 0;

    set<int> valores;
    vector<int> cruce;
    int indice = 0;

    for (int i = 0; i < NUM_CRUCES*2; i++){
        valores.clear();

        // Cogemos la mitad de los genes del primer padre
        while (valores.size() < poblacion[indice].first.size()/2){
            int valor = RandPositiveInt(poblacion[indice].first.size());

            auto pos = valores.find(valor);
            if (pos == valores.end()){
                valores.insert(valor);
            }
        }

        cruce.resize(poblacion[indice].first.size());

        // Vamos a ir añadiendo los genes al cruce, buscamos el gen y si está
        // en el vector de genes del padre 1 lo añadimos, si no lo añadimos del gen del otro padre
        for (int j = 0; j < poblacion[indice].first.size(); j++){
            auto pos = valores.find(j);

            if (pos != valores.end()){
                cruce[j] = poblacion[indice].first[j];
            }
            else{
                cruce[j] = poblacion[indice+1].first[j];
            }
        }

        repararCruce(cruce);

        clusters = solucionToClusters(cruce);
        desviacionGeneralParticion();
        calcularAgregado();

        pair<vector<int>,double> genCruzado (cruce,getAgregado());
        evaluaciones++;

        poblacion.push_back(genCruzado);

        // Si hemos hecho los dos ccruces, borramos los dos primeros individuos de la poblacion
        if (i%2 == 1){
            poblacion.erase(poblacion.begin());
            poblacion.erase(poblacion.begin());
        }
    }

    return evaluaciones;
}

void PAR::repararCruce(vector<int> & reparado){
    vector<int> contador(clusters.size(),0);

    for (auto it = reparado.begin(); it != reparado.end(); ++it){
        contador[(*it)]++;
    }

    for (int i = 0; i < contador.size(); i++){
        if (contador[i] == 0){
            int ele_alea;

            do{
                ele_alea = RandPositiveInt(reparado.size());
            } while (contador[reparado[ele_alea]] - 1 == 0);

            contador[reparado[ele_alea]]--;
            reparado[ele_alea] = i;
            contador[i]++;
        }
    }
}

int PAR::operadorCruceSegmentoFijo(vector<pair<vector<int>,double>> & poblacion, const double PROB_CRUCE){

    const int NUM_CRUCES = poblacion.size()/2 * PROB_CRUCE;

    set<int> valores;
    int evaluaciones = 0;

    vector<int> cruce;

    int tam_segmento;
    int ini_segmento;
    int fin_segmento;

    int rango_fijo_low, rango_fijo_high;

    int indice_p1 = 0;
    int indice_p2 = 1;

    for (int i = 0; i < NUM_CRUCES*2; i++){

        indice_p1 = 0;
        indice_p2 = 1;

        if (poblacion[indice_p2].second < poblacion[indice_p1].second){
            indice_p1 = 1;
            indice_p2 = 0;
        }

        tam_segmento = RandPositiveInt(poblacion[i].first.size());
        ini_segmento = RandPositiveInt(poblacion[i].first.size());

        fin_segmento = ini_segmento + tam_segmento +1;

        cruce = vector<int>(poblacion[i].first.size(), -1);

        int j = ini_segmento;

        while (j <= ini_segmento + tam_segmento){
            cruce[j%poblacion[indice_p1].first.size()] = poblacion[indice_p1].first[j%poblacion[indice_p1].first.size()];
            j++;
        }

        if (fin_segmento >= (int) poblacion[indice_p1].first.size()){
            rango_fijo_low = (fin_segmento % poblacion[indice_p1].first.size());
            rango_fijo_high = ini_segmento;
        }
        else{
            rango_fijo_low = fin_segmento;
            rango_fijo_high = poblacion[indice_p1].first.size() + ini_segmento;
        }

        valores.clear();

        while( valores.size() < (int) (rango_fijo_high-rango_fijo_low+1)/2){
            int valor = Randint(rango_fijo_low, rango_fijo_high);

            auto pos = valores.find(valor);
            if (pos == valores.end()){
                valores.insert(valor % poblacion[indice_p1].first.size());
            }
        }

        for (int j = rango_fijo_low; j < rango_fijo_high; j++){
            int v = j % poblacion[indice_p1].first.size();
            auto pos = valores.find(v);

            if (pos != valores.end()){
                cruce[v] = poblacion[indice_p1].first[v];
            }
            else{
                cruce[v] = poblacion[indice_p2].first[v];
            }
        }

        repararCruce(cruce);

        clusters = solucionToClusters(cruce);
        desviacionGeneralParticion();
        calcularAgregado();

        pair<vector<int>,double> cruzado (cruce, getAgregado());
        evaluaciones++;

        poblacion.push_back(cruzado);

        if (i % 2 == 1){
            poblacion.erase(poblacion.begin());
            poblacion.erase(poblacion.begin());
        }
    }

    return evaluaciones;
}

int PAR::operadorMutacion(vector<pair<vector<int>, double>> & poblacion, const double PROB_MUT,tipo_generacion tipo_gen){

    vector<int> contador (clusters.size(),0);

    int evaluaciones = 0;

    int elemento_poblacion;
    int gen;
    int destino;
    int mut;

    // Si es estacionario nos sale que mutemos 0.2 genes por lo que vamos a mutar 1 gen solo
    if (tipo_gen == tipo_generacion::ESTACIONARIO){
        mut = 1;
    }
    else{
        mut = poblacion.size()*poblacion[0].first.size() * PROB_MUT;
    }

    const int NUM_MUTACIONES = mut;

    for (int i = 0; i < NUM_MUTACIONES; i++){
        elemento_poblacion = RandPositiveInt(poblacion.size());
        contador = vector<int>(clusters.size(),0);

        for (auto it = poblacion[elemento_poblacion].first.begin(); it != poblacion[elemento_poblacion].first.end();++it){
            contador[(*it)]++;
        }

        do{
            gen = RandPositiveInt(poblacion[elemento_poblacion].first.size());
        } while (contador[poblacion[elemento_poblacion].first[gen]] - 1 <= 0);

        destino = RandPositiveInt(clusters.size());

        contador[poblacion[elemento_poblacion].first[gen]]--;
        poblacion[elemento_poblacion].first[gen] = destino;
        contador[destino]++;

        clusters = solucionToClusters(poblacion[elemento_poblacion].first);
        desviacionGeneralParticion();
        calcularAgregado();
        poblacion[elemento_poblacion].second = getAgregado();
        evaluaciones++;
    }

    return evaluaciones;
}

vector<vector<int>> PAR::generarPoblacionInicial(const int TAM_POB){
    vector<vector<int>> poblacion;

    while(poblacion.size() < TAM_POB){
        poblacion.push_back(clustersToSolucion(generarSolucionAleatoria(num_clusters)));
    }

    return poblacion;
}

vector<PAR::Cluster> PAR::AlgGenetico(const int EV_MAX,const int TAM_POB,const double PROB_MUT, const double PROB_CRUCE, operador_cruce tipo_cruce, tipo_generacion tipo_gen ,bool elitismo){

    // Generamos la población inical
    vector<vector<int>> p = generarPoblacionInicial(TAM_POB);

    // Variables donde guardaremos la población actual y la anterior
    vector<pair<vector<int>, double>> poblacion;
    vector<pair<vector<int>,double>> poblacion_anterior;

    poblacion.resize(p.size());

    // Inicializamos la poblacion a la población inicial
    // y guardamos el valor de la función objetivo
    for (int i = 0; i < poblacion.size(); i++){
        poblacion[i].first = p[i];
        clusters = solucionToClusters(poblacion[i].first);
        desviacionGeneralParticion();
        calcularAgregado();
        poblacion[i].second = getAgregado();
    }

    poblacion_anterior = poblacion;
    pair<vector<int>,double> mejor = poblacion[0];

    int indice_peor = 0;

    // Guardamos el mejor cromosoma
    for (int i = 0; i < poblacion.size(); i++){
        if(poblacion[i].second < mejor.second){
            mejor = poblacion[i];
        }
    }

    int generacion = 0;
    int evaluaciones = 0;

    while (evaluaciones < EV_MAX){
        // Operador de seleccion si es estacionario usamos dos padres
        // si es generacional usamos la poblacion entera
        if (tipo_gen == tipo_generacion::ESTACIONARIO){
            poblacion = operadorSeleccion(poblacion_anterior,2);
        }
        else{
            poblacion = operadorSeleccion(poblacion_anterior, poblacion_anterior.size());
        }

        // OPERADOR DE CRUCE
        if (tipo_cruce == operador_cruce::SEGMENTO_FIJO){
            evaluaciones += operadorCruceSegmentoFijo(poblacion,PROB_CRUCE);
        }
        else{
            evaluaciones += operadorCruceUniforme(poblacion,PROB_CRUCE);
        }

        // OPERADOR DE MUTACIÓN
        evaluaciones += operadorMutacion(poblacion,PROB_MUT,tipo_gen);

        // REEMPLAZAMIENTO DE LA POBLACION
        if (tipo_gen != tipo_generacion::ESTACIONARIO){
            if (elitismo){
                // miramos si el mejor de la anterior población sigue estando
                auto pos_mejor = find(poblacion.begin(),poblacion.end(), mejor);

                // si no esta lo añadimos
                if (pos_mejor == poblacion.end()){
                    poblacion.pop_back();
                    poblacion.push_back(mejor);
                }
            }

            poblacion_anterior = poblacion;
        }
        else{

            int indice_peor = 0, indice_segundo_peor = 1;

            if (poblacion[0].second > poblacion[1].second){
                poblacion.push_back(poblacion[0]);
                poblacion.erase(poblacion.begin());
            }

            for (int i = 0; i < poblacion_anterior.size(); i++){
                if (poblacion_anterior[i].second > poblacion_anterior[indice_peor].second ){
                    indice_peor = i;    
                }

                if (poblacion_anterior[i].second > poblacion_anterior[indice_segundo_peor].second && indice_segundo_peor != indice_peor){
                    indice_segundo_peor = i;
                }
            }

            for (int i = 0; i < poblacion.size(); i++){
                if (indice_peor != -1 && poblacion[i].second < poblacion_anterior[indice_peor].second){
                    auto it = poblacion_anterior.begin();
                    advance(it,indice_peor);

                    poblacion_anterior.erase(it);
                    poblacion_anterior.push_back(poblacion[i]);

                    indice_peor = -1;
                }
                else if (indice_segundo_peor != -1 && poblacion[i].second < poblacion_anterior[indice_segundo_peor].second){
                    auto it = poblacion_anterior.begin();
                    advance(it,indice_segundo_peor);

                    poblacion_anterior.erase(it);
                    poblacion_anterior.push_back(poblacion[i]);

                    indice_segundo_peor = -1;
                }
            }
        }

        for (int i = 0; i < poblacion_anterior.size(); i++){
            if (poblacion_anterior[i].second < mejor.second){
                mejor = poblacion_anterior[i];
            }
        }

        generacion++;
    }

    clusters = solucionToClusters(mejor.first);
    desviacionGeneralParticion();
    calcularAgregado();

    return clusters;
}

int PAR::busquedaLocalSuave(pair<vector<int>,double> & solucion, const int num_fallos_permitidos){
    int fallos = 0;
    bool mejora = true;
    int contador = 0;
    int evaluaciones = 0;

    vector<int> indices;
    vector<int> contador_ele_clusters(clusters.size(),0);

    for (int i = 0; i < solucion.first.size(); i++){
        indices.push_back(i);
        contador_ele_clusters[solucion.first[i]]++;
    }

    random_shuffle(indices.begin(), indices.end(), RandPositiveInt);

    clusters = solucionToClusters(solucion.first);
    desviacionGeneralParticion();
    calcularAgregado();

    pair<vector<int>,double> solucion_intermedia;

    double val_mejor_cluster;
    int mejor_cluster;


    while ( (mejora || fallos < num_fallos_permitidos) && contador < solucion.first.size()){
        mejora = false;
        mejor_cluster = -1;

        val_mejor_cluster = solucion.second;
        solucion_intermedia = solucion;

        for (int i = 0; i < num_clusters; i++){
            if(solucion.first[indices[i]] != i && contador_ele_clusters[solucion.first[indices[i]]] -1 > 0){

                solucion_intermedia.first[indices[i]] = i;

                clusters = solucionToClusters(solucion_intermedia.first);
                desviacionGeneralParticion();
                calcularAgregado();
                solucion_intermedia.second = getAgregado();
                evaluaciones++;

                if (solucion_intermedia.second < val_mejor_cluster){
                    mejor_cluster = i;
                    val_mejor_cluster = solucion_intermedia.second;
                    mejora = true;
                }
            }
        }

        if (!mejora){
            fallos++;
        }
        else{
            contador_ele_clusters[solucion.first[indices[contador]]]--;
            solucion.first[indices[contador]] = mejor_cluster;
            solucion.second = val_mejor_cluster;
            contador_ele_clusters[mejor_cluster]++;
        }

        contador++;

    }

    return evaluaciones;

}

vector<PAR::Cluster> PAR::AlgMemetico(const int EV_MAX, const int TAM_POB,const double PROB_MUT, const double PROB_CRUCE,bool elitismo,tipo_memetico memetico){
    // Generamos la población inical
    vector<vector<int>> p = generarPoblacionInicial(TAM_POB);

    // Variables donde guardaremos la población actual y la anterior
    vector<pair<vector<int>, double>> poblacion;
    vector<pair<vector<int>,double>> poblacion_anterior;

    poblacion.resize(p.size());

    // Inicializamos la poblacion a la población inicial
    // y guardamos el valor de la función objetivo
    for (int i = 0; i < poblacion.size(); i++){
        poblacion[i].first = p[i];
        clusters = solucionToClusters(poblacion[i].first);
        desviacionGeneralParticion();
        calcularAgregado();
        poblacion[i].second = getAgregado();
    }

    poblacion_anterior = poblacion;
    pair<vector<int>,double> mejor = poblacion[0];

    int indice_peor = 0;

    // Guardamos el mejor cromosoma
    for (int i = 0; i < poblacion.size(); i++){
        if(poblacion[i].second < mejor.second){
            mejor = poblacion[i];
        }
    }

    int generacion = 0;
    int evaluaciones = 0;

    while (evaluaciones < EV_MAX){
        // Solo vamos a usar el modelo generacional
        poblacion = operadorSeleccion(poblacion_anterior, poblacion_anterior.size());

        // OPERADOR DE CRUCE SEGMENTO FIJO (el que mejor resultados nos ha dado)
        evaluaciones += operadorCruceSegmentoFijo(poblacion,PROB_CRUCE);

        // OPERADOR DE MUTACIÓN
        evaluaciones += operadorMutacion(poblacion,PROB_MUT,tipo_generacion::GENERACIONAL);

        // REEMPLAZAMIENTO DE LA POBLACION
        //Buscamos el mejor de la iteración pasada
        auto pos_mejor = find(poblacion.begin(),poblacion.end(), mejor);

        // si no esta lo añadimos
        if (pos_mejor == poblacion.end()){
            poblacion.pop_back();
            poblacion.push_back(mejor);
        }

        poblacion_anterior = poblacion;

        // Cada 10 generaciones aplicamos BL-Suave
        if (generacion % 10 == 0){
            const int EV_BL = 100000;
            if (memetico == tipo_memetico::M_110){
                for (int i = 0; i < poblacion_anterior.size(); i++){
                    evaluaciones += busquedaLocalSuave(poblacion_anterior[i],EV_BL);
                }
            }
            else if (memetico == tipo_memetico::M_101){
                const int NUM_BL = 0.1 * poblacion_anterior.size();

                for (int i = 0; i < NUM_BL; i++){
                    evaluaciones += busquedaLocalSuave(poblacion_anterior[i],0.1*poblacion_anterior[i].first.size());
                }
            }
            else{
                const int NUM_MEJORES = 0.1 * poblacion_anterior.size();
                set<int> indices_mejores;

                for (int i = 0; i < NUM_MEJORES; i++){
                    int indice_mejor = 0;
                    for (int j = 0; j < poblacion_anterior.size(); j++){
                        if (poblacion_anterior[indice_mejor].second > poblacion_anterior[j].second && indices_mejores.find(j) == indices_mejores.end()){
                            indice_mejor = j;
                        }
                    }

                    indices_mejores.insert(indice_mejor);
                }

                for (auto it = indices_mejores.begin(); it != indices_mejores.end(); ++it){
                    evaluaciones += busquedaLocalSuave(poblacion_anterior[(*it)], 0.1*poblacion_anterior[(*it)].first.size());
                }
            }
        }

        for (int i = 0; i < poblacion_anterior.size(); i++){
            if (poblacion_anterior[i].second < mejor.second){
                mejor = poblacion_anterior[i];
            }
        }

        generacion++;
    }

    clusters = solucionToClusters(mejor.first);
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