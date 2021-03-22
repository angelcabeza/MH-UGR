#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<vector<double>> leerDatos(const string path){

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

vector<vector<int>> leerRestricciones(const string path){
    
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

int main (){
    vector<vector<double>> datos = leerDatos("../conjunto_datos_PAR_2020-21/bupa_set.dat");
    vector<vector<int>> restricciones = leerRestricciones("../conjunto_datos_PAR_2020-21/bupa_set_const_10.const");

    /*for (int i = 0; i < datos.size(); i++){
        if ( i != 0)
            cout << endl;
        for (int j = 0; j < datos[i].size(); j++){
            cout << datos[i][j] << " ";
        }
    }*/
}