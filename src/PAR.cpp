#include "datos.h"
#include <iostream>
#include <vector>

using namespace std;

int main (){

    LeerDatos lectura_datos;
    vector<vector<double>> datos = lectura_datos.leerDatos("../conjunto_datos_PAR_2020-21/bupa_set.dat");
    vector<vector<int>> restricciones = lectura_datos.leerRestricciones("../conjunto_datos_PAR_2020-21/bupa_set_const_10.const");

    cout << datos.size() << endl;
    cout << restricciones.size() << endl;
}
