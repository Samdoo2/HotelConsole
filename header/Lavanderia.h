#pragma once

#include "servicio.h"
#include <string>
using namespace std;

class Lavanderia : public Servicio {
protected:
    double recargoUrgente;
    bool urgente;

public:
    Lavanderia();
    Lavanderia(string codigo, string nombre, string desc, double precio,
               bool activo, double recargoUrgente, bool urgente);

    double getRecargoUrgente() const;
    bool isUrgente() const;
    string getCategoria() const;

    string getTipoServicio() const override;
    double calcularCosto(int cantidad) const override;
    string describir() const override;
    string serializar() const override;
    void deserializar(const string& linea);
};