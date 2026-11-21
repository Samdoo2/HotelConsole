#include "../header/huesped.h"
#include <stdexcept>
#include <sstream>
using namespace std;

Huesped::Huesped() {}
Huesped::Huesped(string documento, string nombre, string telefono, string correo)
    : documento(documento) {
    actualizarDatos(nombre, telefono, correo);
    validar();
}
Huesped::~Huesped() {}

void Huesped::validar() {
    if (documento.empty() || nombre.empty() || telefono.empty() || correo.empty())
        throw invalid_argument("Todos los datos del huesped son obligatorios.");
    if (correo.find('@') == string::npos)
        throw invalid_argument("El correo del huesped no es valido.");
    if (documento.find_first_of(",\r\n") != string::npos ||
        nombre.find_first_of(",\r\n") != string::npos ||
        telefono.find_first_of(",\r\n") != string::npos ||
        correo.find_first_of(",\r\n") != string::npos)
        throw invalid_argument("Los datos del huesped no pueden contener comas ni saltos de linea.");
}

string Huesped::serializar() {
    validar();
    return documento + "," + nombre + "," + telefono + "," + correo;
}

void Huesped::deserializar(string linea) {
    stringstream ss(linea);
    string d, n, t, c;
    if (!getline(ss, d, ',') || !getline(ss, n, ',') ||
        !getline(ss, t, ',') || !getline(ss, c) ||
        c.find(',') != string::npos)
        throw invalid_argument("La linea del huesped tiene un formato incorrecto.");
    if (!c.empty() && c.back() == '\r') c.pop_back();
    Huesped temporal(d, n, t, c); // Valida primero: no modifica el objeto si falla.
    documento = temporal.documento;
    nombre = temporal.nombre;
    telefono = temporal.telefono;
    correo = temporal.correo;
}

string Huesped::getDocumento() const { return documento; }
string Huesped::getNombre() const { return nombre; }
string Huesped::getTelefono() const { return telefono; }
string Huesped::getCorreo() const { return correo; }

void Huesped::actualizarDatos(string nuevoNombre, string nuevoTelefono, string nuevoCorreo) {
    if (nuevoNombre.empty() || nuevoTelefono.empty() || nuevoCorreo.empty())
        throw invalid_argument("Todos los datos del huesped son obligatorios.");
    if (nuevoCorreo.find('@') == string::npos)
        throw invalid_argument("El correo del huesped no es valido.");
    if (nuevoNombre.find_first_of(",\r\n") != string::npos ||
        nuevoTelefono.find_first_of(",\r\n") != string::npos ||
        nuevoCorreo.find_first_of(",\r\n") != string::npos)
        throw invalid_argument("Los datos no pueden contener comas ni saltos de linea.");
    nombre = nuevoNombre;
    telefono = nuevoTelefono;
    correo = nuevoCorreo;
}