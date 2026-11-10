#pragma once
#include "servicio.h"
using namespace std;

class Alimentacion : public Servicio {
    private:
    string tipoComida;
public:
    Alimentacion();
    Alimentacion(string codigo, string nombre, string descripcion, double precio,bool activo = true);
    string getTipoComida() const;
    string describir() const;
    double calcularCosto(int cantidad) const;
};
