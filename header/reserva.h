#pragma once
#include "fecha.h"
#include <string>
#include "Enumeraciones.h"
using namespace std;
class Fecha;
class Huesped;
class Habitacion;
class Reserva{
    private:
    string fecha, hora, estado;

    public:
    Reserva();
    Reserva(string codigo, string codigoGrupo, string documentoTitular,
            string codigoHabitacion, string fechaEntrada, string fechaSalida,
            int personas, double tarifaPorNoche,
            EstadoReserva estado = EstadoReserva::Confirmada);

    string getCodigo() const;
    string getCodigoGrupo() const;
    string getDocumentoTitular() const;
    string getCodigoHabitacion() const;
    string getFechaEntrada() const;
    string getFechaSalida() const;
    int getPersonas() const;
    double getTarifaPorNoche() const;
    EstadoReserva getEstado() const;
    bool estaActiva() const;

    int calcularNoches() const;
    double calcularValorEstimado() const;
    bool seSuperponeCon(const Reserva& otra) const;
    void cambiarFechas(const string& nuevaEntrada, const string& nuevaSalida);
    void cancelar();
    void validar() const;
    string serializar() const;
    static Reserva deserializar(const string& linea);
    static string nombreEstado(EstadoReserva estado);
};