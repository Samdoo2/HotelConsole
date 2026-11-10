#include "Alimentacion.h"

Alimentacion::Alimentacion() : Servicio() {}
Alimentacion::Alimentacion(string codigo, string nombre, string descripcion, double precio,bool activo): 
Servicio(codigo, nombre, descripcion, precio, activo, "Alimentacion") {}
string Alimentacion::getTipo() const { return "Alimentacion"; }
string Alimentacion::getUnidadCobro() const { return "plato"; }
double Alimentacion::calcularCosto(int cantidad) const {
 return Servicio::calcularCosto(cantidad);
}
