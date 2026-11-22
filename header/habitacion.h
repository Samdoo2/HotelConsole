#pragma once
#include <string>
#include "Enumeraciones.h"
using namespace std;

// Representa una habitacion fisica del hotel.
class Habitacion {
private:
    string codigo;
    TipoHabitacion tipo;
    int capacidad;
    double precioPorNoche;
    EstadoHabitacion estado;

public:
    Habitacion();
    Habitacion(string codigo, TipoHabitacion tipo, int capacidad,
              double precioPorNoche,
              EstadoHabitacion estado = EstadoHabitacion::Disponible);

    string getCodigo() const;
    TipoHabitacion getTipo() const;
    int getCapacidad() const;
    double getPrecioPorNoche() const;
    EstadoHabitacion getEstado() const;

    // Validaciones para impedir habitaciones con datos incorrectos.
    void actualizarDatos(TipoHabitacion tipo, int capacidad, double precioPorNoche);
    void cambiarEstado(EstadoHabitacion nuevoEstado);
    bool estaOperativa() const;

    // Convertir entre objeto y linea de texto para guardar en CSV.
    string serializar() const;
    static Habitacion deserializar(const string& linea);
    static string nombreTipo(TipoHabitacion tipo);
    static string nombreEstado(EstadoHabitacion estado);
};
