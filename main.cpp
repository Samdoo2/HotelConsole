#include <iostream>
#include <string>
#include <conio.h>
#include "header/persistencia.h"

using namespace std;

int main() {
    // Verificamos o creamos la base de datos de admins antes de arrancar
    if (!inicializarArchivoAdministradores()) {
        return 1; // Terminamos el programa si hay un fallo de disco
    }

    string username, password, rol;
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

        if (validarCredenciales(username, password, rol)) {
            
            autenticado = true;
            cout << "\nIngreso exitoso. Bienvenido(a), " << username << "!" << endl;
        } else {
         cout << "Credenciales incorrectas. Intente de nuevo.\n" << endl;
        }
    }


int opcion = 0;


    // Aquí arranca el menu principal
    if (rol == "admin"){

        do {
        cout << "\n=== MENU PRINCIPAL - ADMIN ===" << endl;
        cout << "1. Gestion de Habitaciones" << endl;
        cout << "2. Gestion de Huespedes y Reservas" << endl;
        cout << "3. Registrar Consumos (Restaurante/Lavanderia)" << endl;
        cout << "4. Facturacion y Check-out" << endl;
        cout << "5. Salir del sistema" << endl;
        cout << "67. Salir del sistema" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "\nModulo de Habitaciones" << endl;
                break;
            case 2:
                cout << "\nModulo de Reservas" << endl;
                break;
            case 3:
                cout << "\nModulo de Consumos" << endl;
                break;
            case 4:
                cout << "\nModulo de Facturacion" << endl;
                break;
            case 5:
                cout << "\nGuardando cambios... Cerrando sesion." << endl;
                break;
            default:
                cout << "\nOpcion no valida. Intente de nuevo." << endl;
        }

    } while (opcion != 5);

    } else{
        do {
        cout << "\n=== MENU PRINCIPAL - RECEPCIONISTA ===" << endl;
        cout << "1. Gestion de Habitaciones" << endl;
        cout << "2. Gestion de Huespedes y Reservas" << endl;
        cout << "3. Registrar Consumos (Restaurante/Lavanderia)" << endl;
        cout << "4. Facturacion y Check-out" << endl;
        cout << "5. Salir del sistema" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "\nModulo de Habitaciones" << endl;
                break;
            case 2:
                cout << "\nModulo de Reservas" << endl;
                break;
            case 3:
                cout << "\nModulo de Consumos" << endl;
                break;
            case 4:
                cout << "\nModulo de Facturacion" << endl;
                break;
            case 5:
                cout << "\nGuardando cambios... Cerrando sesion." << endl;
                break;
            default:
                cout << "\nOpcion no valida. Intente de nuevo." << endl;
        }

    } while (opcion != 5);
    }
    

    

    return 0;
}
  