#include "../header/habitacion.h"
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <vector>
using namespace std;

static bool codigoValido(const string& codigo) {
    return !codigo.empty() &&
           codigo.find_first_not_of(" \t") != string::npos &&
           codigo.find_first_of(",\r\n") == string::npos;
}

Habitacion::Habitacion()
    : codigo(""), tipo(TipoHabitacion::Sencilla), capacidad(0),
      precioPorNoche(0), estado(EstadoHabitacion::Disponible) {}

Habitacion::Habitacion(string codigo, TipoHabitacion tipo, int capacidad,
                       double precioPorNoche, EstadoHabitacion estado)
    : codigo(codigo), tipo(tipo), capacidad(capacidad),
      precioPorNoche(precioPorNoche), estado(estado) {
    if (!codigoValido(codigo))
        throw invalid_argument("El codigo de habitacion no es valido.");
    actualizarDatos(tipo, capacidad, precioPorNoche);
}

string Habitacion::getCodigo() const { return codigo; }
TipoHabitacion Habitacion::getTipo() const { return tipo; }
int Habitacion::getCapacidad() const { return capacidad; }
double Habitacion::getPrecioPorNoche() const { return precioPorNoche; }
EstadoHabitacion Habitacion::getEstado() const { return estado; }

void Habitacion::actualizarDatos(TipoHabitacion nuevoTipo, int nuevaCapacidad,
                                double nuevoPrecio) {
    if (nuevaCapacidad <= 0)
        throw invalid_argument("La capacidad debe ser positiva.");
    if (!isfinite(nuevoPrecio) || nuevoPrecio <= 0)
        throw invalid_argument("La tarifa por noche debe ser positiva.");
    tipo = nuevoTipo;
    capacidad = nuevaCapacidad;
    precioPorNoche = nuevoPrecio;
}

void Habitacion::cambiarEstado(EstadoHabitacion nuevoEstado) {
    estado = nuevoEstado;
}

// Esto NO revisa cruces de fechas: eso se agregara con Reserva.
bool Habitacion::estaOperativa() const {
    return estado == EstadoHabitacion::Disponible;
}

string Habitacion::nombreTipo(TipoHabitacion tipo) {
    switch (tipo) {
        case TipoHabitacion::Sencilla: return "Sencilla";
        case TipoHabitacion::Doble: return "Doble";
        case TipoHabitacion::Familiar: return "Familiar";
        case TipoHabitacion::Suite: return "Suite";
        case TipoHabitacion::Otra: return "Otra";
    }
    throw invalid_argument("Tipo de habitacion invalido.");
}

string Habitacion::nombreEstado(EstadoHabitacion estado) {
    switch (estado) {
        case EstadoHabitacion::Disponible: return "Disponible";
        case EstadoHabitacion::Mantenimiento: return "Mantenimiento";
        case EstadoHabitacion::FueraDeServicio: return "Fuera de servicio";
    }
    throw invalid_argument("Estado de habitacion invalido.");
}

string Habitacion::serializar() const {
    if (!codigoValido(codigo) || capacidad <= 0 ||
        !isfinite(precioPorNoche) || precioPorNoche <= 0)
        throw invalid_argument("No puedes guardar una habitacion incompleta.");
    ostringstream salida;
    salida.precision(17);
    salida << codigo << ',' << nombreTipo(tipo) << ',' << capacidad << ','
           << precioPorNoche << ',' << nombreEstado(estado);
    return salida.str();
}

Habitacion Habitacion::deserializar(const string& linea) {
    stringstream entrada(linea);
    vector<string> campos;
    string campo;
    while (getline(entrada, campo, ',')) campos.push_back(campo);
    if (campos.size() != 5)
        throw invalid_argument("La linea CSV debe tener 5 campos.");

    if (campos[1] != "Sencilla" && campos[1] != "Doble" &&
        campos[1] != "Familiar" && campos[1] != "Suite" &&
        campos[1] != "Otra")
        throw invalid_argument("Tipo desconocido en el archivo.");

    size_t usadosCapacidad = 0, usadosPrecio = 0;
    int capacidad = stoi(campos[2], &usadosCapacidad);
    double precio = stod(campos[3], &usadosPrecio);
    if (usadosCapacidad != campos[2].size() || usadosPrecio != campos[3].size())
        throw invalid_argument("Capacidad o precio con caracteres invalidos.");

    return Habitacion(campos[0], convertirTipoHabitacion(campos[1]),
                      capacidad, precio, convertirEstadoHabitacion(campos[4]));
}