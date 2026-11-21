#include <iostream>
#include <string>
#include <exception>

#ifdef _WIN32
#include <conio.h> // _getch() existe en Windows.
#endif

#include "header/administrador.h"
#include "header/recepcionista.h"
#include "header/usuario.h"
#include "header/menu.h"
#include "header/persistencia.h"

using namespace std;

// En Windows oculta los caracteres de la contrasena.
// En otros sistemas usa getline para que tambien pueda compilarse.
string leerContrasena() {
#ifdef _WIN32
    string password;
    int tecla;
    while ((tecla = _getch()) != 13) { // Enter
        if (tecla == 8) {               // Retroceso
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        } else if (tecla >= 32 && tecla <= 126) {
            password += static_cast<char>(tecla);
            cout << '*';
        }
    }
    cout << endl;
    return password;
#else
    string password;
    getline(cin, password);
    return password;
#endif
}

int main() {
    try {
        if (!inicializarArchivoAdministradores()) return 1;

        cout << "--- SISTEMA HOTEL CONSOLE ---" << endl;
        string username, password, rol, datoExtra;

        while (true) {
            cout << "Usuario: ";
            if (!getline(cin, username)) return 0;
            cout << "Contrasena: ";
            password = leerContrasena();
            if (!cin) return 0;

            if (!validarCredenciales(username, password, rol, datoExtra)) {
                cout << "Credenciales incorrectas. Intente de nuevo.\n" << endl;
                continue;
            }

            // Un mismo rol en todas las clases. Compatibilidad con CSV antiguos: Admin.
            if (rol == "Admin") rol = "Administrador";

            if (rol == "Administrador") {
                Administrador usuarioActual(username, password, stoi(datoExtra));
                menuPrincipal(usuarioActual);
            } else if (rol == "Recepcionista") {
                Recepcionista usuarioActual(username, password, datoExtra);
                menuPrincipal(usuarioActual);
            } else {
                cout << "Error: el rol '" << rol
                     << "' no es reconocido. Consulta al administrador." << endl;
                return 1;
            }
            return 0; // Sesion cerrada correctamente.
        }
    } catch (const exception& e) {
        cerr << "Error del sistema: " << e.what() << endl;
        return 1;
    }
}




    // Aquí arranca el menu principal



/*     if (rol == "Admin"){

        do {
        cout << "\n=== MENU PRINCIPAL - ADMIN ===" << endl;
        cout << "1. Opciones de administrador" << endl;
        cout << "2. Gestion de Habitaciones" << endl;
        cout << "3. Gestion de Huespedes y Reservas" << endl;
        cout << "4. Registrar Consumos (Restaurante/Lavanderia)" << endl;
        cout << "5. Facturacion y Check-out" << endl;
        cout << "6. Salir del sistema" << endl;
        cout << "7. Salir del sistema" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                system("cls");
                cout << "\nOpciones de Administrador" << endl;
                opcion = 0;
            do {
                cout << "1. Gestion Usuarios" << endl;
                cout << "2. Gestion de Habitaciones" << endl;
                cout << "3. Gestion de Huespedes y Reservas" << endl;
                cout << "4. Registrar Consumos (Restaurante/Lavanderia)" << endl;
                cout << "5. Facturacion y Check-out" << endl;
                cout << "6. Volver al menu principal." << endl;
                cout << "Seleccione una opcion: ";
                cin >> opcion;

            } while (opcion != 5);

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
     */

    

   
  