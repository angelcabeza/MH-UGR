#include <datos.h>
#include <fstream>
#include <iostream>

using namespace std;

LeerDatos::LeerDatos(){}


vector<vector<double>> LeerDatos::leerDatos(const string path){

    ifstream fichero(path);

    vector<vector<double>> datos;
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

    return datos;
}

vector<vector<int>> LeerDatos::leerRestricciones(const string path){
    
    ifstream fichero(path);

    vector<vector<int>> datos;
    vector<int> fila;
    string dato;
    string linea;
    string dato_linea;
    size_t pos_coma;
    int valor;

    if (fichero.is_open()){

        while(!fichero.eof()){
            
            // Cojo una línea del fichero
            getline(fichero,linea);

            while (!linea.empty()){
                pos_coma = linea.find(',');

                if (pos_coma != linea.npos){
                    dato = linea.substr(0,pos_coma);
                    valor = stoi(dato);
                    fila.push_back(valor);
                    linea.erase(0,pos_coma+1);
                }
                else{
                    valor = stoi(linea);
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

    return datos;
}

list<pair<pair<vector<double>,vector<double>>,int>> LeerDatos::restriccionesEnLista (vector<vector<double>> datos, vector<vector<int>> restricciones){

    list<pair<pair<vector<double>,vector<double>>,int>> lista_restricciones;

    pair<vector<double>,vector<double>> par_puntos;
    pair<pair<vector<double>,vector<double>>,int> puntos_restriccion;

    for (int i = 0; i < restricciones.size(); i++){
        for (int j = 0; j < restricciones[i].size(); j++){
            par_puntos.first = datos[i];
            par_puntos.second = datos[j];

            puntos_restriccion.first = par_puntos;
            puntos_restriccion.second = restricciones[i][j];

            lista_restricciones.push_back(puntos_restriccion);
        }
    }

    return lista_restricciones;
}