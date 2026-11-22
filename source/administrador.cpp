#include "../header/administrador.h"
using namespace std;

Administrador::Administrador() : Usuario("", "", "Administrador") {}

Administrador::Administrador(string nombreUser, string contra)
    : Usuario(nombreUser, contra, "Administrador") {}

bool Administrador::tienePermiso(string operacion) const {
    return true; // El rol Administrador tiene permisos completos.
}

string Administrador::descripcionRol() const {
    return "Administrador";
}