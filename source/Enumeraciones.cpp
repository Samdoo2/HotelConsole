#include "Enumeraciones.h"
#include <stdexcept>

Rol convertirRol(string texto) {
    if (texto == "Administrador") return Rol::Administrador;
    if (texto == "Recepcionista") return Rol::Recepcionista;
    throw invalid_argument("Rol de usuario invalido.");
}
TipoHabitacion convertirTipoHabitacion(string texto) {
    if (texto == "Sencilla") return TipoHabitacion::Sencilla;
    if (texto == "Doble") return TipoHabitacion::Doble;
    if (texto == "Suite") return TipoHabitacion::Suite;
    if (texto == "Familiar") return TipoHabitacion::Familiar;
    return TipoHabitacion::Otra; // Se permite definir otros tipos de habitacion.
}
EstadoHabitacion convertirEstadoHabitacion(string texto) {
    if (texto == "Disponible") return EstadoHabitacion::Disponible;
    if (texto == "Mantenimiento") return EstadoHabitacion::Mantenimiento;
    if (texto == "Fuera de servicio") return EstadoHabitacion::FueraDeServicio;
    throw invalid_argument("Estado de habitacion invalido.");
}
EstadoReserva convertirEstadoReserva(string texto) {
    if (texto == "Pendiente") return EstadoReserva::Pendiente;
    if (texto == "Confirmada") return EstadoReserva::Confirmada;
    if (texto == "Cancelada") return EstadoReserva::Cancelada;
    throw invalid_argument("Estado de reserva invalido.");
}
EstadoPago convertirEstadoPago(string texto) {
    if (texto == "Pendiente") return EstadoPago::Pendiente;
    if (texto == "Pagada") return EstadoPago::Pagada;
    throw invalid_argument("Estado de pago invalido.");
}
