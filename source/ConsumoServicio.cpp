#include "ConsumoServicio.h"
#include <stdexcept>

ConsumoServicio::ConsumoServicio() : cantidad(0), precioAplicado(0), valorTotal(0) {}
ConsumoServicio::ConsumoServicio(string codigo, string codigoReserva, string codigoServicio,int cantidad, double precioAplicado): codigo(codigo), codigoReserva(codigoReserva), codigoServicio(codigoServicio),
cantidad(0), precioAplicado(precioAplicado), valorTotal(0) {
if (codigo.empty() || codigoReserva.empty() || codigoServicio.empty() || precioAplicado <= 0)
   throw invalid_argument("Datos de consumo de servicio invalidos.");
    actualizarCantidad(cantidad);
}
string ConsumoServicio::getCodigo() const { return codigo; }
string ConsumoServicio::getCodigoReserva() const { return codigoReserva; }
string ConsumoServicio::getCodigoServicio() const { return codigoServicio; }
int ConsumoServicio::getCantidad() const { return cantidad; }
double ConsumoServicio::getPrecioAplicado() const { return precioAplicado; }
double ConsumoServicio::getValorTotal() const { return valorTotal; }
void ConsumoServicio::actualizarCantidad(int nuevaCantidad) {
    if (nuevaCantidad <= 0) throw invalid_argument("La cantidad debe ser positiva.");
    cantidad = nuevaCantidad;
    valorTotal = precioAplicado * cantidad;
}
