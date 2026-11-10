#pragma once
#include <string>
using namespace std;
class Fecha;
class Huesped;
class Habitacion;
class Reserva{
    private:
    string fecha, hora, estado;

    public:
    Reserva();
    Reserva(string fecha, string hora, string estado);
    ~Reserva();
    int calcularNoches();
    double calcularValorEstimado();
    bool seSuperponeCon(Reserva* otra);
    void validar();
    void confirmar();
    void cancelar();
    string serializar();
    void deserializar(string linea);


};