#include "../header/huesped.h"
#include <stdexcept>

Huesped::Huesped() {}
Huesped::Huesped(string documento, string nombre, string telefono, string correo)
    : documento(documento) {
    if (documento.empty()) throw invalid_argument("El documento es obligatorio.");
    Huesped::actualizarDatos(nombre, telefono, correo);
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
    nombre = nuevoNombre;
    telefono = nuevoTelefono;
    correo = nuevoCorreo;
}
