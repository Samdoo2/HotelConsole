#include "persistencia.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

bool inicializarArchivoAdministradores() {
    // 1. Intentamos abrirlo en modo lectura para ver si ya existe
    ifstream in_admin("administradores.csv");
    if (in_admin.is_open()) {
        in_admin.close();
        return true; // Ya existe, no hacemos nada para no borrar datos
    }

    // 2. Si no existe, lo creamos (ios::out es por defecto en ofstream)
    ofstream out_admin("administradores.csv");
    if (!out_admin.is_open()) {
        cout << "Error critico: No se pudo crear administradores.csv" << endl;
        return false;
    }

    // 3. Escribimos un administrador por defecto para poder entrar la primera vez
    out_admin << "admin,12345\n";
    out_admin.close();
    
    return true;
}

bool validarCredenciales(const string& username, const string& password) {
    ifstream in_admin("administradores.csv");
    if (!in_admin.is_open()) {
        return false;
    }

    string linea, csv_user, csv_pass;
    while (getline(in_admin, linea)) {
        stringstream ss(linea);
        getline(ss, csv_user, ',');
        getline(ss, csv_pass, ',');

        // Limpieza del caracter de retorno de Windows (\r)
        if (!csv_pass.empty() && csv_pass.back() == '\r') {
            csv_pass.pop_back();
        }

        if (csv_user == username && csv_pass == password) {
            in_admin.close();
            return true; // Match encontrado
        }
    }

    in_admin.close();
    return false; // Terminó de leer y no hubo match
}