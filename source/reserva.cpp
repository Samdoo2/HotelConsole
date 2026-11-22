#include "../header/reserva.h"
#include "../header/Fecha.h"
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <vector>
using namespace std;

static bool textoValidoReserva(const string& s) {
    return !s.empty() && s.find_first_not_of(" \t") != string::npos &&
           s.find_first_of(",\r\n") == string::npos;
}

Reserva::Reserva() : personas(0), tarifaPorNoche(0), estado(EstadoReserva::Pendiente) {}
Reserva::Reserva(string codigo, string codigoGrupo, string documentoTitular,
                 string codigoHabitacion, string fechaEntrada, string fechaSalida,
                 int personas, double tarifaPorNoche, EstadoReserva estado)
    : codigo(codigo), codigoGrupo(codigoGrupo), documentoTitular(documentoTitular),
      codigoHabitacion(codigoHabitacion), fechaEntrada(fechaEntrada),
      fechaSalida(fechaSalida), personas(personas),
      tarifaPorNoche(tarifaPorNoche), estado(estado) { validar(); }

string Reserva::getCodigo() const { return codigo; }
string Reserva::getCodigoGrupo() const { return codigoGrupo; }
string Reserva::getDocumentoTitular() const { return documentoTitular; }
string Reserva::getCodigoHabitacion() const { return codigoHabitacion; }
string Reserva::getFechaEntrada() const { return fechaEntrada; }
string Reserva::getFechaSalida() const { return fechaSalida; }
int Reserva::getPersonas() const { return personas; }
double Reserva::getTarifaPorNoche() const { return tarifaPorNoche; }
EstadoReserva Reserva::getEstado() const { return estado; }
bool Reserva::estaActiva() const { return estado != EstadoReserva::Cancelada; }

void Reserva::validar() const {
    if (!textoValidoReserva(codigo) || !textoValidoReserva(codigoGrupo) ||
        !textoValidoReserva(documentoTitular) || !textoValidoReserva(codigoHabitacion))
        throw invalid_argument("Codigo, grupo, titular y habitacion son obligatorios y no pueden tener comas.");
    if (personas <= 0) throw invalid_argument("Cada habitacion debe tener al menos una persona.");
    if (!isfinite(tarifaPorNoche) || tarifaPorNoche <= 0)
        throw invalid_argument("La tarifa debe ser mayor que cero.");
    Fecha::diasEntre(fechaEntrada, fechaSalida);
    nombreEstado(estado);
}
int Reserva::calcularNoches() const { return Fecha::diasEntre(fechaEntrada, fechaSalida); }
double Reserva::calcularValorEstimado() const { return calcularNoches() * tarifaPorNoche; }
bool Reserva::seSuperponeCon(const Reserva& otra) const {
    return estaActiva() && otra.estaActiva() && codigoHabitacion == otra.codigoHabitacion &&
           Fecha::seSuperponen(fechaEntrada, fechaSalida, otra.fechaEntrada, otra.fechaSalida);
}
void Reserva::cambiarFechas(const string& entrada, const string& salida) {
    if (!estaActiva()) throw invalid_argument("No puedes editar una reserva cancelada.");
    Fecha::diasEntre(entrada, salida);
    fechaEntrada = entrada;
    fechaSalida = salida;
}
void Reserva::cancelar() {
    if (!estaActiva()) throw invalid_argument("Esta reserva ya estaba cancelada.");
    estado = EstadoReserva::Cancelada;
}
string Reserva::nombreEstado(EstadoReserva estado) {
    switch (estado) {
        case EstadoReserva::Pendiente: return "Pendiente";
        case EstadoReserva::Confirmada: return "Confirmada";
        case EstadoReserva::Cancelada: return "Cancelada";
    }
    throw invalid_argument("Estado de reserva desconocido.");
}
string Reserva::serializar() const {
    validar();
    ostringstream salida;
    salida.precision(17);
    salida << codigo << ',' << codigoGrupo << ',' << documentoTitular << ','
           << codigoHabitacion << ',' << fechaEntrada << ',' << fechaSalida << ','
           << personas << ',' << tarifaPorNoche << ',' << nombreEstado(estado);
    return salida.str();
}

static int enteroEstrictoReserva(const string& s) {
    size_t pos = 0;
    int valor = stoi(s, &pos);
    if (pos != s.size()) throw invalid_argument("Numero entero mal escrito.");
    return valor;
}
static double decimalEstrictoReserva(const string& s) {
    size_t pos = 0;
    double valor = stod(s, &pos);
    if (pos != s.size()) throw invalid_argument("Precio mal escrito.");
    return valor;
}
Reserva Reserva::deserializar(const string& linea) {
    istringstream entrada(linea);
    vector<string> campos;
    string campo;
    while (getline(entrada, campo, ',')) campos.push_back(campo);
    if (campos.size() == 9) {
        return Reserva(campos[0], campos[1], campos[2], campos[3], campos[4],
                       campos[5], enteroEstrictoReserva(campos[6]),
                       decimalEstrictoReserva(campos[7]), convertirEstadoReserva(campos[8]));
    }
    // Tambien permite leer el formato anterior de 8 columnas (una habitacion por reserva).
    if (campos.size() == 8) {
        return Reserva(campos[0], campos[0], campos[1], campos[2], campos[3],
                       campos[4], enteroEstrictoReserva(campos[5]),
                       decimalEstrictoReserva(campos[6]), convertirEstadoReserva(campos[7]));
    }
    throw invalid_argument("El archivo reservas.csv debe tener 9 campos (o 8 del formato anterior).");
}
