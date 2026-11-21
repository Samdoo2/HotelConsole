#include "../header/recepcionista.h"
#include <string>
using namespace std;

Recepcionista::Recepcionista() : Usuario("", "", "Recepcionista"), turno("Diurno") {}

Recepcionista::Recepcionista(string nombreUser, string contra, string turno)
    : Usuario(nombreUser, contra, "Recepcionista"), turno(turno) {}

Recepcionista::~Recepcionista() {}

bool Recepcionista::tienePermiso(string operacion) const {
    // Consultas y atencion, pero nunca administracion de usuarios o servicios.
    return operacion == "habitaciones_consultar" || operacion == "huespedes" ||
           operacion == "reservas" || operacion == "consumos" ||
           operacion == "facturacion";
}

string Recepcionista::descripcionRol() const { return "Recepcionista"; }

void Recepcionista::registrarReserva() {}  // Se implementara con la clase Reserva.
void Recepcionista::registrarConsumo() {} // Se implementara en el bloque Consumos.
