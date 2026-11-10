#pragma once
#include <string>
#include <vector>
using namespace std;
class Reserva;
class ConsumoServicio;
class Huesped{
    private:
    string documento, telefono, nombre, correo;
    public:
    Huesped();
    Huesped(string documento, string telefono, string nombre, string correo);
    ~Huesped();
    void validar();
    vector<Reserva*> obtenerHistorialReservas();
    vector <ConsumoServicio*> obtenerServiciosUtilizados();
    string serializar();
    void deserializar(string linea);
    string Huesped::getDocumento() const;
string Huesped::getNombre() const;
string Huesped::getTelefono() const;
string Huesped::getCorreo() const;
void Huesped::actualizarDatos(string nuevoNombre, string nuevoTelefono, string nuevoCorreo);

};