#ifndef LEERDATOS_H_INCLUDED
#define LEERDATOS_H_INCLUDED

#include <iostream>
#include <vector>
using namespace std;

class LeerDatos{
public:

    LeerDatos();
    vector<vector<double>> leerDatos (const string path);
    vector<vector<int>> leerRestricciones (const string path);
};

#endif