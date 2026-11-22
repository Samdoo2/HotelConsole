#include "../header/Lavanderia.h"
#include <stdexcept>
#include <sstream>

using namespace std;

Lavanderia::Lavanderia() : Servicio(), recargoUrgente(0.0), urgente(false) {}

Lavanderia::Lavanderia(string codigo, string nombre, string desc, double precio,
                       bool activo, double recargoUrgente, bool urgente)
    : Servicio(codigo, nombre, desc, precio, activo),
      recargoUrgente(recargoUrgente), urgente(urgente) {
    if (recargoUrgente < 0)
        throw invalid_argument("El recargo urgente no puede ser negativo.");
}

double Lavanderia::getRecargoUrgente() const { return recargoUrgente; }
bool Lavanderia::isUrgente() const { return urgente; }

string Lavanderia::getCategoria() const {
    return urgente ? "Lavanderia Urgente" : "Lavanderia Estandar";
}

string Lavanderia::getTipoServicio() const { return "Lavanderia"; }

double Lavanderia::calcularCosto(int cantidad) const {
    if (cantidad <= 0) throw invalid_argument("La cantidad de prendas debe ser mayor a cero.");
    if (!activo) throw invalid_argument("El servicio de lavanderia esta inactivo.");
    double subtotal = precio * cantidad;
    if (urgente) {
        subtotal += recargoUrgente * cantidad;
    }
    return subtotal;
}

string Lavanderia::describir() const {
    string detalle = "[" + codigo + "] " + nombre + " (" + getCategoria() + ") - $" + to_string(precio);
    if (urgente) detalle += " + $" + to_string(recargoUrgente) + " recargo/prenda";
    return detalle;
}

string Lavanderia::serializar() const {
    return Servicio::serializar() + "," + to_string(recargoUrgente) + "," + (urgente ? "1" : "0");
}

void Lavanderia::deserializar(const string& linea) {
    stringstream ss(linea);
    string tipo, cod, nom, d, precStr, actStr, recStr, urgStr;
    if (!getline(ss, tipo, ',') || !getline(ss, cod, ',') ||
        !getline(ss, nom, ',') || !getline(ss, d, ',') ||
        !getline(ss, precStr, ',') || !getline(ss, actStr, ',') ||
        !getline(ss, recStr, ',') || !getline(ss, urgStr)) {
        throw invalid_argument("Formato CSV invalido para Lavanderia.");
    }
    if (!urgStr.empty() && urgStr.back() == '\r') urgStr.pop_back();
    if (tipo != "Lavanderia")
        throw invalid_argument("La linea no corresponde a un servicio de Lavanderia.");

    Lavanderia temp(cod, nom, d, stod(precStr), actStr == "1", stod(recStr), urgStr == "1");
    *this = temp;
}