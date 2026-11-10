#include "../header/usuario.h"
#include "../header/administrador.h"
#include <string>
using namespace std;



    Administrador::Administrador(){}
    Administrador::Administrador(string nombreUser, string contra, string rol, int nivelAcceso) : Usuario( nombreUser, contra, rol){this->nivelAcceso=nivelAcceso;}


bool Administrador::tienePermiso(string operacion) const {
    return true; // El Administrador tiene acceso total a cualquier operación
}

string Administrador::descripcionRol() const {
    return "Administrador con nivel de acceso " + to_string(nivelAcceso);
}
