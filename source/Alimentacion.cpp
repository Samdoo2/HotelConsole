#include "../header/Alimentacion.h"
#include "../header/Servicio.h"


Alimentacion::Alimentacion() : Servicio() {}
Alimentacion::Alimentacion(string codigo, string nombre, string descripcion, double precio,bool activo, string tipoComida): Servicio(codigo, nombre, descripcion, precio, activo) 
{
    this->tipoComida=tipoComida;
}

string Alimentacion::getTipoComida() const { return "Alimentacion"; }
string Alimentacion::getUnidadCobro() const { return "plato"; }
string Alimentacion::describir() const{return "Servicio de alimentacion: " + tipoComida; }
double Alimentacion::calcularCosto(int cantidad) const {
 return Alimentacion::calcularCosto(cantidad);
}
