#pragma once
#include <string>
#include "Enumeraciones.h"
using namespace std;

class Factura {
protected:
    string codigo;
    string codigoReserva;
    string fechaEmision;
    double valorNoches;
    double subtotalServicios;
    double impuestos;
    double totalPagar;
    string estadoPago;
public:
    Factura();
    Factura(string codigo, string codigoReserva, string fechaEmision,
            double valorNoches, double subtotalServicios, double tasaImpuesto = 0);
    string getCodigo() const;
    string getCodigoReserva() const;
    string getFechaEmision() const;
    double getValorNoches() const;
    double getSubtotalServicios() const;
    double getImpuestos() const;
    double getTotalPagar() const;
    string getEstadoPago() const;
    EstadoPago getEstadoPagoEnum() const;
    void registrarPago();
    void setEstadoPago(string nuevoEstado);
};
