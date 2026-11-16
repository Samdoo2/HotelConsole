#include "../header/persistencia.h"
#include "../header/administrador.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

using namespace std;

bool inicializarArchivoAdministradores() {
    bool necesitaCreacion = false;

    // 1. Esto crea la carpeta donde va lo de persistencia
    filesystem::create_directories("../../../Persistencia");

    // 2. Intentamos abrirlo para ver si existe y si tiene contenido
    ifstream in_admin("../../../persistencia/usuarios.csv", ios::in);

    if (!in_admin.is_open()) {
        necesitaCreacion = true; // El archivo ni siquiera existe
    } else {
        // El archivo existe, pero ¿está vacío?
        // peek() mira el primer caracter. Si es el final del archivo (EOF), está vacío.
        if (in_admin.peek() == ifstream::traits_type::eof()) {
            necesitaCreacion = true; 
        }
        in_admin.close();
    }

    // 3. Si no existía o estaba vacío, lo reescribimos
    if (necesitaCreacion) {
        ofstream out_admin("../../../persistencia/usuarios.csv", ios::out); // Esto sobrescribe si estaba vacío
        if (!out_admin.is_open()) {
            cout << "Error critico: No se pudo escribir en usuarios.csv" << endl;
            return false;
        }


        Administrador admin0("admin", "12345", 1 );
        
        // Escribimos el administrador por defecto
        out_admin << admin0.getNombreUsuario() << ","
        << admin0.getContra() << ","
        << admin0.getRol() << ","
        << admin0.getNivelAcceso() << "\n";
        out_admin.close();
    }
    
    return true;
}


bool validarCredenciales(const string& username, const string& password, string& rolOut) {
    ifstream in_admin("../../../persistencia/usuarios.csv");

    if (!in_admin.is_open()) {
        return false;
    }

    string linea, csv_user, csv_pass, csv_rol;
    while (getline(in_admin, linea)) {
        stringstream ss(linea);
        getline(ss, csv_user, ',');
        getline(ss, csv_pass, ',');
        getline(ss, csv_rol, ',');

        // Limpieza del caracter de retorno de Windows (\r)
        /* if (!csv_pass.empty() && csv_pass.back() == '\r') {
            csv_pass.pop_back();
        } */

        if (csv_user == username && csv_pass == password) {
            rolOut = csv_rol;
            in_admin.close();
            return true; // Match encontrado
        }
    }

    in_admin.close();
    return false; // Terminó de leer y no hubo match
}