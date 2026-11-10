#pragma once

#include <string>

using namespace std;

class Servicio {

protected:
    string codigo;
    string nombre;
    string desc;
    double precio;
    bool activo;

public:
    Servicio();
    Servicio(string codigo, string nombre, string desc, double precio, bool activo );
    
    double calcularCosto(double cantidad);
    string describir();
    void activar();
    void desactivar();
    void validar();
    string serializar();
    void deserializar(string linea);

};

