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
    Servicio(string codigo, string nombre, string desc, double precio, bool activo);
    virtual ~Servicio() = default;

    virtual double calcularCosto(int cantidad) const = 0;
    virtual string describir() const = 0;
    virtual string getTipoServicio() const = 0;
    virtual string serializar() const;

    void activar();
    void desactivar();
    void validar() const;

    string getCodigo() const;
    string getNombre() const;
    string getDesc() const;
    double getPrecio() const;
    bool isActivo() const;
};