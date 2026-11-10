#include <iostream>
#include <string>
#include <conio.h>
#include "persistencia.h"

using namespace std;

int main() {
    // Verificamos o creamos la base de datos de admins antes de arrancar
    if (!inicializarArchivoAdministradores()) {
        return 1; // Terminamos el programa si hay un fallo de disco
    }

    string username, password;
    bool autenticado = false;

    cout << "--- SISTEMA HOTEL CONSOLE ---" << endl;

    while (!autenticado) {
        cout << "Usuario: ";
        cin >> username;
        
        cout << "Contrasena: ";
        password = ""; 
        char ch;
        
        while ((ch = _getch()) != '\r') {
            if (ch == '\b') { 
                if (!password.empty()) {
                    password.pop_back();
                    cout << "\b \b";
                }
            } else {
                password.push_back(ch);
                cout << '*'; 
            }
        }
        cout << endl;

        // AQUÍ USAMOS TU NUEVA FUNCIÓN ADAPTADA
        if (validarCredenciales(username, password)) {
            autenticado = true;
            cout << "\nIngreso exitoso. Bienvenido(a), " << username << "!" << endl;
        } else {
            cout << "Credenciales incorrectas. Intente de nuevo.\n" << endl;
        }
    }

    // Aquí arranca tu menú principal...
    return 0;
}