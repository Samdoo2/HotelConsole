#pragma once
#include <string>
using namespace std;

class ConsumoServicio {
protected:
    string codigo;
    string codigoReserva;
    string codigoServicio;
    int cantidad;
    double precioAplicado;
    double valorTotal;

public:
    ConsumoServicio();
    ConsumoServicio(string codigo, string codigoReserva, string codigoServicio,int cantidad, double precioAplicado);
    string getCodigo() const;
    string getCodigoReserva() const;
    string getCodigoServicio() const;
    int getCantidad() const;
    double getPrecioAplicado() const;
    double getValorTotal() const;
    void actualizarCantidad(int nuevaCantidad);
};
