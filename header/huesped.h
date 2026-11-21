#pragma once
#include <string>
using namespace std;

class Huesped {
private:
    string documento, telefono, nombre, correo;
public:
    Huesped();
    // Orden fijo: documento, nombre, telefono, correo.
    Huesped(string documento, string nombre, string telefono, string correo);
    ~Huesped();
    void validar();
    string serializar();
    void deserializar(string linea);
    string getDocumento() const;
    string getNombre() const;
    string getTelefono() const;
    string getCorreo() const;
    void actualizarDatos(string nuevoNombre, string nuevoTelefono, string nuevoCorreo);
    // Historiales se agregaran cuando Reserva y Consumo esten conectados.
};
