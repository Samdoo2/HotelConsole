#pragma once
#include <string>
#include <vector>
#include "habitacion.h"
#include "huesped.h"
#include "reserva.h"
using namespace std;

// Crea la carpeta y el CSV con un administrador si no existe o esta vacio.
bool inicializarArchivoAdministradores();

// Verifica credenciales y devuelve el rol guardado por referencia.
bool validarCredenciales(const string& username, const string& password,
                        string& rolOut, string& datoExtraOut);

// Crea un usuario persistente. Rechaza datos invalidos y nombres duplicados.
// Administrador se guarda con 3 campos; Recepcionista usa un cuarto campo (turno).
// datoExtra se conserva en la firma para el turno de recepcionista.
void registrarUsuario(const string& username, const string& password,
                     const string& rol, const string& datoExtra);

// Inventario de habitaciones (sin clase gestionadora adicional).
// Solo crea las habitaciones predeterminadas si el archivo no existe o esta vacio.
bool inicializarArchivoHabitaciones();

// Carga de habitaciones.csv y devuelve la lista de habitaciones.
vector<Habitacion> cargarHabitaciones();

// Almacena cambios futuros sin duplicar codigos.
void guardarHabitaciones(const vector<Habitacion>& habitaciones);

// ---- Huesped titular: se guarda sin crear una clase gestora adicional ----
bool inicializarArchivoHuespedes();
vector<Huesped> cargarHuespedes();
void registrarHuespedEnArchivo(const Huesped& huesped);

// ---- Reservas: cada grupo puede contener de 1 a 5 habitaciones ----
bool inicializarArchivoReservas();
vector<Reserva> cargarReservas();
vector<Habitacion> buscarHabitacionesDisponibles(const string& entrada,
                                                const string& salida, int personas);
void registrarGrupoReservas(const vector<Reserva>& nuevas);
void cancelarGrupoReservas(const string& codigoGrupo);
void modificarFechasGrupo(const string& codigoGrupo, const string& entrada,
                         const string& salida);

// ---- Inventario: modificaciones para administradores desde el menu 2 ----
void agregarHabitacion(const Habitacion& habitacion);
void actualizarHabitacion(const string& codigo, TipoHabitacion tipo,
                         int capacidad, double precio);
void cambiarEstadoHabitacion(const string& codigo, EstadoHabitacion estado);
void eliminarHabitacion(const string& codigo);