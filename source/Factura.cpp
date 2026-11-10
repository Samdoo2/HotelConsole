#include "Factura.h"
#include "Fecha.h"
#include <stdexcept>
using namespace std;
Factura::Factura():valorNoches(0),subtotalServicios(0),impuestos(0),totalPagar(0),estadoPago("Pendiente") {}
Factura::Factura(string codigo, string codigoReserva, string fechaEmision,double valorNoches, double subtotalServicios, double tasaImpuesto)
:codigo(codigo), codigoReserva(codigoReserva), fechaEmision(fechaEmision),valorNoches(valorNoches),subtotalServicios(subtotalServicios),
impuestos(0), totalPagar(0), estadoPago("Pendiente") {
    if (codigo.empty() || codigoReserva.empty() || !Fecha::esValida(fechaEmision))
     throw invalid_argument("Datos de factura invalidos.");
    if (valorNoches < 0 || subtotalServicios < 0 || tasaImpuesto < 0 || tasaImpuesto > 1)
        throw invalid_argument("Los valores de factura o la tasa de impuesto son invalidos.");
    impuestos = (valorNoches + subtotalServicios) * tasaImpuesto;
    totalPagar = valorNoches + subtotalServicios + impuestos;
}
string Factura::getCodigo() const { return codigo; }
string Factura::getCodigoReserva() const { return codigoReserva; }
string Factura::getFechaEmision() const { return fechaEmision; }
double Factura::getValorNoches() const { return valorNoches; }
double Factura::getSubtotalServicios() const { return subtotalServicios; }
double Factura::getImpuestos() const { return impuestos; }
double Factura::getTotalPagar() const { return totalPagar; }
string Factura::getEstadoPago() const { return estadoPago; }
EstadoPago Factura::getEstadoPagoEnum() const { return convertirEstadoPago(estadoPago); }
void Factura::registrarPago() { estadoPago = "Pagada"; }
void Factura::setEstadoPago(string nuevoEstado) {
    convertirEstadoPago(nuevoEstado);
    estadoPago = nuevoEstado;
}
