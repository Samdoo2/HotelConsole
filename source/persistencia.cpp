#include "persistencia.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

#include "persistencia.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

bool inicializarArchivoAdministradores() {
    bool necesitaCreacion = false;

    // 1. Intentamos abrirlo para ver si existe y si tiene contenido
    ifstream in_admin("administradores.csv");
    if (!in_admin.is_open()) {
        necesitaCreacion = true; // El archivo ni siquiera existe
    } else {
        // El archivo existe, pero ¿está vacío?
        // peek() mira el primer caracter. Si es el final del archivo (EOF), está vacío.
        if (in_admin.peek() == std::ifstream::traits_type::eof()) {
            necesitaCreacion = true; 
        }
        in_admin.close();
    }

    // 2. Si no existía o estaba vacío, lo reescribimos
    if (necesitaCreacion) {
        ofstream out_admin("administradores.csv"); // Esto sobrescribe si estaba vacío
        if (!out_admin.is_open()) {
            cout << "Error critico: No se pudo escribir en administradores.csv" << endl;
            return false;
        }

        // Escribimos el administrador por defecto
        out_admin << "admin,12345\n";
        out_admin.close();
    }
    
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