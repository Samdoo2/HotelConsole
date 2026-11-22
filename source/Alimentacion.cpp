#include "../header/Alimentacion.h"
#include <stdexcept>
#include <sstream>

using namespace std;

Alimentacion::Alimentacion() : Servicio(), tipoComida("") {}

Alimentacion::Alimentacion(string codigo, string nombre, string descripcion,
                           double precio, bool activo, string tipoComida)
    : Servicio(codigo, nombre, descripcion, precio, activo), tipoComida(tipoComida) {
    if (tipoComida.empty() || tipoComida.find_first_of(",\r\n") != string::npos)
        throw invalid_argument("El tipo de comida es obligatorio y no puede contener comas.");
}

string Alimentacion::getTipoComida() const { return tipoComida; }
string Alimentacion::getUnidadCobro() const { return "plato"; }
string Alimentacion::getTipoServicio() const { return "Alimentacion"; }

string Alimentacion::describir() const {
    return "[" + codigo + "] " + nombre + " (" + tipoComida + ") - $" +
           to_string(precio) + " por " + getUnidadCobro();
}

double Alimentacion::calcularCosto(int cantidad) const {
    if (cantidad <= 0) throw invalid_argument("La cantidad debe ser mayor a cero.");
    if (!activo) throw invalid_argument("El servicio de alimentacion esta inactivo.");
    return precio * cantidad;
}

string Alimentacion::serializar() const {
    return Servicio::serializar() + "," + tipoComida;
}

void Alimentacion::deserializar(const string& linea) {
    stringstream ss(linea);
    string tipo, cod, nom, d, precStr, actStr, comida;
    if (!getline(ss, tipo, ',') || !getline(ss, cod, ',') ||
        !getline(ss, nom, ',') || !getline(ss, d, ',') ||
        !getline(ss, precStr, ',') || !getline(ss, actStr, ',') ||
        !getline(ss, comida)) {
        throw invalid_argument("Formato CSV invalido para Alimentacion.");
    }
    if (!comida.empty() && comida.back() == '\r') comida.pop_back();
    if (tipo != "Alimentacion")
        throw invalid_argument("La linea no corresponde a un servicio de Alimentacion.");

    Alimentacion temp(cod, nom, d, stod(precStr), actStr == "1", comida);
    *this = temp;
}