#pragma once
#include <string>
using namespace std;

enum class Rol { Administrador, Recepcionista };
enum class TipoHabitacion { Sencilla, Doble, Suite, Familiar, Otra };
enum class EstadoHabitacion { Disponible, Mantenimiento, FueraDeServicio };
enum class EstadoReserva { Pendiente, Confirmada, Cancelada };
enum class EstadoPago { Pendiente, Pagada };

Rol convertirRol(string texto);
TipoHabitacion convertirTipoHabitacion(string texto);
EstadoHabitacion convertirEstadoHabitacion(string texto);
EstadoReserva convertirEstadoReserva(string texto);
EstadoPago convertirEstadoPago(string texto);
