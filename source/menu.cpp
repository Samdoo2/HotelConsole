#include <iostream>
#include <stdexcept>
#include <string>

#include "../header/menu.h"
#include "../header/usuario.h"
#include "../header/administrador.h"
#include "../header/recepcionista.h"



using namespace std;




void menuPrincipal(const Usuario &user){

    int opcion = -1;

    cout << "Bienvenido, " << user.descripcionRol() << endl;

    

    if(user.getRol() == "Admin"){

        do {
        cout << "\n=== MENU PRINCIPAL - ADMIN ===" << endl;
        cout << "1. Opciones de administrador" << endl;
        cout << "2. Gestion de Habitaciones" << endl;
        cout << "3. Gestion de Huespedes y Reservas" << endl;
        cout << "4. Registrar Consumos (Restaurante/Lavanderia)" << endl;
        cout << "5. Facturacion y Check-out" << endl;
        cout << "0. Salir del sistema" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                menuAdmin();

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
            case 0:
                cout << "\nGuardando cambios... Cerrando sesion." << endl;
                break;
            default:
                cout << "\nOpcion no valida. Intente de nuevo." << endl;
        }

    } while (opcion != 0);



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
            case 0:
                cout << "\nGuardando cambios... Cerrando Sesion" << endl;
                break;
            default:
                cout << "\nOpcion no valida. Intente de nuevo." << endl;
        }

    } while (opcion != 0);
    


    }
}


void menuAdmin(){
    system("cls");
    int opcion = -1;

    cout << "\nOpciones de Administrador" << endl;
    
            do {
                cout << "1. Gestion Usuarios." << endl;
                cout << "2. Gestion de Habitaciones." << endl;
                cout << "3. Gestion de Huespedes y Reservas." << endl;
                cout << "4. Gestion de Servicios." << endl;
                cout << "0. Volver al menu principal." << endl;
                cout << "Seleccione una opcion: ";
                cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "\nGestion de usuarios: " << endl;
                void menuGestionUsuarios();
                break;
            case 2:
                cout << "\nGestion de habitaciones" << endl;
                break;
            case 3:
                cout << "\nGestion de Huespedes y Reservas" << endl;
                break;
            case 4:
                cout << "\nGestion de Servicios." << endl;
                break;
            case 0:
                cout << "\nGuardando cambios...." << endl;
                break;
            default:
                cout << "\nOpcion no valida. Intente de nuevo." << endl;
        }


            } while (opcion != 0);



    
}




void menuGestionUsuario(){
    system("cls");
    int opcion = -1;

    cout << "\nOpciones de Administrador" << endl;
    
            do {
                cout << "1. Crear Usuario." << endl;
                cout << "2. Eliminar Usuario." << endl;
                cout << "3. Editar Usuario." << endl;
                cout << "0. Volver al menu principal." << endl;
                cout << "Seleccione una opcion: ";
                cin >> opcion;

        switch (opcion) {
            case 1: {
                string nombreUsuario, contrasena, tipo, turno = "";
                int nivelAcceso;

                cout << "\n-- Crear Usuario -- " << endl;

                cout << "Nombre de usuario: ";
                cin >> nombreUsuario;
                cout << endl;

                cout << "Contrasena: ";
                cin >> contrasena;
                cout << endl;

                cout << "Tipo de usuario (Administrador/Recepcionista):" << endl;
                cin >> tipo;
                cout << endl;

                if(tipo == "Administrador"){
                    cout << "Nivel de acceso: ";
                    cin >> nivelAcceso;
                    cout << endl;

                    if(cin.fail() || nivelAcceso <= 0){
                        cin.clear();
                        cin.ignore(10000, '\n');
                        throw invalid_argument("El nivel de acceso debe ser un numero entero mayor a 0.");

                    }

                Administrador nuevoAdmin(nombreUsuario, contrasena, nivelAcceso);
                

                }else if(tipo == "Recepcionista"){
                    cout << "Turno: ";
                    cin >> turno;
                    cout << endl;
                }


        
                break;

            }
                
            case 2:
                cout << "\n-- Eliminar Usuario -- " << endl;
                break;
            case 3:
                cout << "\n-- Editar Usuario -- " << endl;
                break;
            case 0:
                cout << "\nGuardando cambios...." << endl;
                break;
            default:
                cout << "\nOpcion no valida. Intente de nuevo." << endl;
        }


            } while (opcion != 0);



    
}