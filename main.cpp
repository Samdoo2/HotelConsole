#include <iostream>
#include <string>
#include <exception>

#include <conio.h> // Oculta la contrasena durante el inicio de sesion.

#include "header/administrador.h"
#include "header/recepcionista.h"
#include "header/usuario.h"
#include "header/menu.h"
#include "header/persistencia.h"

using namespace std;

// Lee la contrasena y muestra asteriscos en lugar de los caracteres.
string leerContrasena() {
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
                Administrador usuarioActual(username, password);
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