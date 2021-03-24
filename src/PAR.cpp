#include <iostream>
#include <vector>
#include "datos.h"

using namespace std;

int main (){

    LeerDatos lectura_datos;
    vector<vector<double>> datos = lectura_datos.leerDatos("../conjunto_datos_PAR_2020-21/bupa_set.dat");
    vector<vector<int>> restricciones_matriz = lectura_datos.leerRestricciones("../conjunto_datos_PAR_2020-21/bupa_set_const_20.const");
    list<pair<pair<vector<double>,vector<double>>,int>> lista_restricciones = lectura_datos.restriccionesEnLista(datos,restricciones_matriz);

    cout << datos.size() << endl;
    cout << restricciones_matriz.size() << endl;
    cout << lista_restricciones.size() << endl;

    cout << "Este es el primer punto almacenado en las restricciones forma lista" << endl;

    list<pair<pair<vector<double>,vector<double>>,int>>::iterator it = lista_restricciones.begin();
    it++;

    for (int i = 0; i < (*lista_restricciones.begin()).first.first.size(); i++){
        cout << (*(it)).first.first[i] << ", ";
    }

    for (int i = 0; i < (*lista_restricciones.begin()).first.second.size(); i++){
        cout << (*it).first.second[i] << ", ";
    }

    cout << (*it).second << endl;

    // K = número de clusters
    const int K = 16;
}
