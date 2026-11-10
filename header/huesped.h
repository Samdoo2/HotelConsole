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
    string getDocumento() const;
string getNombre() const;
string getTelefono() const;
string getCorreo() const;
void actualizarDatos(string nuevoNombre, string nuevoTelefono, string nuevoCorreo);

};