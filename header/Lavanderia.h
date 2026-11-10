#pragma once

#include "servicio.h"
#include <string>
using namespace std;

class Lavanderia : public Servicio{

    protected:
    double recargoUrgente;
    bool urgente;

    public:

    Lavanderia();
    Lavanderia(string codigo, string nombre, string desc, double precio, bool activo, double recargoUrgente, bool urgente);
    
    double calcularCosto();
    string getCategoria();
    string describir();


};