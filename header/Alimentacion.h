#pragma once
#include "Servicio.h"
#include <string>
using namespace std;

class Alimentacion : public Servicio {
    private:
    string tipoComida;
public:
    Alimentacion();
    Alimentacion(string codigo, string nombre, string descripcion, double precio,bool activo, string tipoComida);

    string getTipoComida() const;
    string getUnidadCobro() const;
    string describir() const;
    double calcularCosto(int cantidad) const;
};
