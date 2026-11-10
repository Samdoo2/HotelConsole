#include "persistencia.h"
bool crearArchivoAdministradores() {
    ofstream out_admin("administradores.csv", ios::out);
    ifstream in_admin("administradores.csv", ios::in);

    if(!out_admin.is_open()){
        cout << "No se pudo abrir el archivo";
        return false;
    }
}
