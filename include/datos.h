#ifndef LEERDATOS_H_INCLUDED
#define LEERDATOS_H_INCLUDED

#include <iostream>
#include <vector>
#include <list>
using namespace std;

class LeerDatos{
public:

    LeerDatos();
    vector<vector<double>> leerDatos (const string path);
    vector<vector<int>> leerRestricciones (const string path);
    list<pair<pair<vector<double>,vector<double>>,int>> restriccionesEnLista (vector<vector<double>> datos, vector<vector<int>> restricciones);
};

#endif