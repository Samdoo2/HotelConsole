
#include "../header/Alimentacion.h"
#include <stdexcept>
using namespace std;

Alimentacion::Alimentacion() : Servicio(), tipoComida("") {}
Alimentacion::Alimentacion(string codigo, string nombre, string descripcion,
                           double precio, bool activo, string tipoComida)
    : Servicio(codigo, nombre, descripcion, precio, activo), tipoComida(tipoComida) {}

string Alimentacion::getTipoComida() const { return tipoComida; }
string Alimentacion::getUnidadCobro() const { return "plato"; }
string Alimentacion::describir() const { return "Servicio de alimentacion: " + tipoComida; }

double Alimentacion::calcularCosto(int cantidad) const {
    if (cantidad <= 0) throw invalid_argument("La cantidad debe ser mayor a cero.");
    if (!activo) throw invalid_argument("El servicio de alimentacion esta inactivo.");
    if (precio <= 0) throw invalid_argument("El precio debe ser mayor a cero.");
    return precio * cantidad; // No llamarse a si mismo: evita recursion infinita.
}
