#include "../header/administrador.h"
#include <stdexcept>
using namespace std;

Administrador::Administrador() : Usuario("", "", "Administrador"), nivelAcceso(1) {}

Administrador::Administrador(string nombreUser, string contra, int nivelAcceso)
    : Usuario(nombreUser, contra, "Administrador"), nivelAcceso(nivelAcceso) {
    if (nivelAcceso <= 0)
        throw invalid_argument("El nivel de acceso debe ser mayor a cero.");
}

bool Administrador::tienePermiso(string operacion) const {
    return true; // El administrador puede realizar todas las operaciones.
}

string Administrador::descripcionRol() const {
    return "Administrador con nivel de acceso " + to_string(nivelAcceso);
}

int Administrador::getNivelAcceso() { return nivelAcceso; }