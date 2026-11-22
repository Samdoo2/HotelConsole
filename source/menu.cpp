#include "../header/menu.h"
#include "../header/administrador.h"
#include "../header/recepcionista.h"
#include "../header/persistencia.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

// Lee una opcion sin dejar cin bloqueado al escribir letras.
// Si se acaba la entrada (EOF), sale del menu.
static int leerOpcion() {
    string texto;
    while (true) {
        cout << "Seleccione una opcion: ";
        if (!getline(cin, texto)) return 0;
        istringstream entrada(texto);
        int numero; char sobra;
        if ((entrada >> numero) && !(entrada >> sobra)) return numero;
        cout << "Entrada invalida: escribe un numero de la lista." << endl;
    }
}

static string leerCampo(const string& mensaje) {
    cout << mensaje;
    string valor;
    if (!getline(cin, valor)) throw runtime_error("Se interrumpio la entrada.");
    if (valor.empty()) throw invalid_argument("El campo no puede estar vacio.");
    return valor;
}

static void anunciarModulo(const Usuario& user, const string& permiso,
                          const string& modulo) {
    if (!user.tienePermiso(permiso)) {
        cout << "Acceso denegado: tu rol no permite esta operacion." << endl;
        return;
    }
    cout << "\nModulo de " << modulo << "." << endl;
    cout << "Esta funcion se conectara en los siguientes bloques." << endl;
}

void menuPrincipal(const Usuario& user) {
    cout << "\nBienvenido, " << user.descripcionRol() << endl;
    const bool administrador = user.getRol() == "Administrador";
    const bool recepcionista = user.getRol() == "Recepcionista";
    if (!administrador && !recepcionista) {
        cout << "Rol no reconocido: no se puede abrir el menu." << endl;
        return;
    }

    int opcion;
    do {
        cout << "\n=== MENU PRINCIPAL - "
             << (administrador ? "ADMINISTRADOR" : "RECEPCIONISTA") << " ===\n";
        if (administrador) cout << "1. Opciones de administrador\n";
        cout << "2. Reservas y Habitaciones (pendiente)\n"
             << "3. Gestion de Huespedes (pendiente)\n"
             << "4. Registrar Consumos (Restaurante/Lavanderia)\n"
             << "5. Facturacion y Check-out\n"
             << "0. Cerrar sesion\n";
        opcion = leerOpcion();
        switch (opcion) {
            case 1:
                if (administrador && user.tienePermiso("usuarios")) menuAdmin(user);
                else cout << "Opcion no permitida." << endl;
                break;
            case 2: anunciarModulo(user, "reservas", "Reservas y Habitaciones"); break;
            case 3: anunciarModulo(user, "huespedes", "Huespedes"); break;
            case 4: anunciarModulo(user, "consumos", "Consumos"); break;
            case 5: anunciarModulo(user, "facturacion", "Facturacion y Check-out"); break;
            case 0: cout << "Cerrando sesion..." << endl; break;
            default: cout << "Opcion no valida." << endl;
        }
    } while (opcion != 0);
}

void menuAdmin(const Usuario& user) {
    if (!user.tienePermiso("usuarios")) {
        cout << "Acceso denegado." << endl; return;
    }
    int opcion;
    do {
        cout << "\n=== OPCIONES DE ADMINISTRADOR ===\n"
             << "1. Gestion de Usuarios\n"
             << "2. Reservas y Habitaciones (pendiente)\n"
             << "3. Gestion de Huespedes (pendiente)\n"
             << "4. Gestion de Servicios\n"
             << "0. Volver al menu principal\n";
        opcion = leerOpcion();
        switch (opcion) {
            case 1: menuGestionUsuarios(); break; // Ahora SI se llama la funcion.
            case 2: anunciarModulo(user, "reservas", "Reservas y Habitaciones"); break;
            case 3: anunciarModulo(user, "huespedes", "Huespedes"); break;
            case 4: anunciarModulo(user, "servicios_gestion", "Servicios"); break;
            case 0: cout << "Regresando al menu principal." << endl; break;
            default: cout << "Opcion no valida." << endl;
        }
    } while (opcion != 0);
}

void menuGestionUsuarios() {
    int opcion;
    do {
        cout << "\n=== GESTION DE USUARIOS ===\n"
             << "1. Crear Usuario\n"
             << "2. Eliminar Usuario (pendiente)\n"
             << "3. Editar Usuario (pendiente)\n"
             << "0. Volver\n";
        opcion = leerOpcion();
        switch (opcion) {
            case 1:
                try {
                    cout << "\n-- Crear Usuario --\n";
                    string nombre = leerCampo("Nombre de usuario: ");
                    string contrasena = leerCampo("Contrasena: ");
                    string tipo = leerCampo("Tipo (Administrador/Recepcionista): ");
                    string extra;
                    if (tipo == "Administrador") {
                        // No se solicita nivel: el rol determina los permisos.
                        Administrador usuarioNuevo(nombre, contrasena);
                    } else if (tipo == "Recepcionista") {
                        extra = leerCampo("Turno: ");
                        Recepcionista usuarioNuevo(nombre, contrasena, extra);
                    } else {
                        throw invalid_argument("Tipo no valido: usa Administrador o Recepcionista.");
                    }
                    registrarUsuario(nombre, contrasena, tipo, extra);
                    cout << "Usuario creado y guardado en usuarios.csv." << endl;
                } catch (const exception& e) {
                    cout << "No se pudo crear el usuario: " << e.what() << endl;
                }
                break;
            case 2: cout << "Eliminar usuarios se implementara en otro bloque." << endl; break;
            case 3: cout << "Editar usuarios se implementara en otro bloque." << endl; break;
            case 0: cout << "Volviendo..." << endl; break;
            default: cout << "Opcion no valida." << endl;
        }
    } while (opcion != 0);
}
