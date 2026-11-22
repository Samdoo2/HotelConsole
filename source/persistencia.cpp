#include "../header/persistencia.h"
#include "../header/administrador.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include "../header/Fecha.h"

using namespace std;
namespace fs = std::filesystem;

// Resuelve siempre el archivo en la carpeta Persistencia del proyecto.
// Con CMake se conoce la ruta real incluso desde la carpeta build/.
static fs::path rutaUsuarios() {
#ifdef HOTEL_PROJECT_ROOT
    return fs::path(HOTEL_PROJECT_ROOT) / "Persistencia" / "usuarios.csv";
#else
    // Para compilacion manual: ejecutar desde la carpeta HotelConsole.
    return fs::current_path() / "Persistencia" / "usuarios.csv";
#endif
}

static string rolNormalizado(const string& rol) {
    if (rol == "Admin") return "Administrador"; // Compatibilidad hacia atras.
    return rol;
}

// CSV sencillo: los campos no deben introducir comas ni saltos de linea.
static bool campoValido(const string& campo) {
    return !campo.empty() && campo.find_first_of(",\r\n") == string::npos;
}

bool inicializarArchivoAdministradores() {
    try {
        const fs::path archivo = rutaUsuarios();
        fs::create_directories(archivo.parent_path());

        ifstream entrada(archivo);
        if (entrada.is_open() && entrada.peek() != ifstream::traits_type::eof())
            return true; // No sobrescribir usuarios existentes.

        ofstream salida(archivo, ios::trunc);
        if (!salida.is_open()) {
            cerr << "Error critico: no se pudo crear usuarios.csv" << endl;
            return false;
        }
        Administrador admin("admin", "12345");
        salida << admin.getNombreUsuario() << ',' << admin.getContra() << ','
               << admin.getRol() << '\n';
        salida.flush();
        if (!salida) {
            cerr << "Error critico: no se pudo escribir usuarios.csv" << endl;
            return false;
        }
        return true;
    } catch (const fs::filesystem_error& e) {
        cerr << "Error de persistencia: " << e.what() << endl;
        return false;
    }
}

bool validarCredenciales(const string& username, const string& password,
                        string& rolOut, string& datoExtraOut) {
    ifstream entrada(rutaUsuarios());
    if (!entrada.is_open()) return false;

    string linea;
    while (getline(entrada, linea)) {
        stringstream ss(linea);
        string csvUser, csvPass, csvRol, csvDato;
        // Acepta 3 campos para Administrador y 4 para Recepcionista.
        // Tambien puede leer administradores antiguos con 4 campos (nivel).
        if (!getline(ss, csvUser, ',') || !getline(ss, csvPass, ',') ||
            !getline(ss, csvRol, ',')) continue;
        getline(ss, csvDato); // Puede no existir en el formato nuevo de admin.
        if (!csvRol.empty() && csvRol.back() == '\r') csvRol.pop_back();
        if (!csvDato.empty() && csvDato.back() == '\r') csvDato.pop_back();
        const string rolLeido = rolNormalizado(csvRol);
        if (csvUser == username && csvPass == password &&
            (rolLeido == "Administrador" || rolLeido == "Recepcionista")) {
            rolOut = rolLeido;
            datoExtraOut = rolLeido == "Recepcionista" ? csvDato : "";
            return true;
        }
    }
    return false;
}

void registrarUsuario(const string& username, const string& password,
                     const string& rol, const string& datoExtra) {
    if (!campoValido(username) || !campoValido(password))
        throw invalid_argument("Nombre y contrasena no pueden estar vacios ni tener comas.");
    if (rol != "Administrador" && rol != "Recepcionista")
        throw invalid_argument("Tipo de usuario invalido.");
    if (rol == "Recepcionista" && !campoValido(datoExtra))
        throw invalid_argument("El turno es obligatorio y no debe tener comas.");

    ifstream entrada(rutaUsuarios());
    if (!entrada.is_open()) throw runtime_error("No se puede leer usuarios.csv.");
    string linea;
    while (getline(entrada, linea)) {
        stringstream ss(linea);
        string existente;
        if (getline(ss, existente, ',') && existente == username)
            throw invalid_argument("Ya existe un usuario con ese nombre.");
    }
    entrada.close();

    ofstream salida(rutaUsuarios(), ios::app);
    if (!salida.is_open()) throw runtime_error("No se puede abrir usuarios.csv para guardar.");
    // Se elimina el cuarto campo para administradores.
    salida << username << ',' << password << ',' << rol;
    if (rol == "Recepcionista") salida << ',' << datoExtra;
    salida << '\n';
    salida.flush();
    if (!salida) throw runtime_error("No se pudo guardar el usuario.");
}


// Mismo directorio que usuarios.csv. Asi Code Runner y CMake
// respetan las rutas ya corregidas en el Bloque 0.
static fs::path rutaHabitaciones() {
    return rutaUsuarios().parent_path() / "habitaciones.csv";
}

void guardarHabitaciones(const vector<Habitacion>& habitaciones) {
    const fs::path ruta = rutaHabitaciones();
    fs::create_directories(ruta.parent_path());

    // Revisar todos los registros antes de truncar el archivo existente.
    unordered_set<string> codigos;
    for (const Habitacion& h : habitaciones) {
        h.serializar(); // Verifica sus atributos antes de guardar.
        if (!codigos.insert(h.getCodigo()).second)
            throw invalid_argument("Hay codigos de habitacion duplicados.");
    }

    ofstream archivo(ruta, ios::trunc);
    if (!archivo.is_open())
        throw runtime_error("No se pudo abrir habitaciones.csv para escribir.");

    for (const Habitacion& h : habitaciones)
        archivo << h.serializar() << '\n';

    archivo.flush();
    if (!archivo)
        throw runtime_error("No se pudo guardar habitaciones.csv.");
}

vector<Habitacion> cargarHabitaciones() {
    ifstream archivo(rutaHabitaciones());
    if (!archivo.is_open())
        throw runtime_error("No se pudo abrir habitaciones.csv para leer.");

    vector<Habitacion> habitaciones;
    unordered_set<string> codigos;
    string linea;
    int numeroLinea = 0;
    while (getline(archivo, linea)) {
        ++numeroLinea;
        if (!linea.empty() && linea.back() == '\r') linea.pop_back();
        if (linea.empty()) continue;
        try {
            Habitacion h = Habitacion::deserializar(linea);
            if (!codigos.insert(h.getCodigo()).second)
                throw invalid_argument("Codigo de habitacion repetido.");
            habitaciones.push_back(h);
        } catch (const exception& e) {
            throw runtime_error("habitaciones.csv, linea " +
                                to_string(numeroLinea) + ": " + e.what());
        }
    }
    if (archivo.bad())
        throw runtime_error("Error al leer habitaciones.csv.");
    return habitaciones;
}

bool inicializarArchivoHabitaciones() {
    try {
        const fs::path ruta = rutaHabitaciones();
        fs::create_directories(ruta.parent_path());

        // No reemplazar datos creados ni cambios guardados.
        if (fs::exists(ruta) && fs::file_size(ruta) > 0) {
            cargarHabitaciones(); // Comprueba que el CSV existente es valido.
            return true;
        }

        // Datos predeterminados del hotel para las primeras pruebas.
        vector<Habitacion> habitaciones = {
            Habitacion("101", TipoHabitacion::Sencilla, 1, 100000),
            Habitacion("102", TipoHabitacion::Sencilla, 1, 100000),
            Habitacion("103", TipoHabitacion::Sencilla, 1, 105000),
            Habitacion("104", TipoHabitacion::Sencilla, 1, 105000),
            Habitacion("105", TipoHabitacion::Doble, 2, 155000),
            Habitacion("106", TipoHabitacion::Doble, 2, 155000),
            Habitacion("107", TipoHabitacion::Doble, 2, 165000),
            Habitacion("108", TipoHabitacion::Doble, 2, 165000),
            Habitacion("201", TipoHabitacion::Familiar, 4, 250000),
            Habitacion("202", TipoHabitacion::Familiar, 4, 250000,
                      EstadoHabitacion::FueraDeServicio),
            Habitacion("203", TipoHabitacion::Suite, 2, 350000),
            Habitacion("204", TipoHabitacion::Suite, 2, 350000,
                      EstadoHabitacion::Mantenimiento)
        };
        guardarHabitaciones(habitaciones);
        return true;
    } catch (const exception& e) {
        cerr << "Error al inicializar habitaciones: " << e.what() << endl;
        return false;
    }
}

// ===============================================================
// BLOQUE RESERVAS: todo el almacenamiento queda en Persistencia.
// No se crean clases gestionadoras de habitaciones ni de reservas.
// ===============================================================

static fs::path rutaHuespedes() {
    return rutaUsuarios().parent_path() / "huespedes.csv";
}
static fs::path rutaReservas() {
    return rutaUsuarios().parent_path() / "reservas.csv";
}
static void crearCsvVacioSiNoExiste(const fs::path& ruta) {
    fs::create_directories(ruta.parent_path());
    if (!fs::exists(ruta)) {
        ofstream salida(ruta);
        if (!salida.is_open() || !salida) throw runtime_error("No se pudo crear " + ruta.filename().string());
    }
}

vector<Huesped> cargarHuespedes() {
    ifstream archivo(rutaHuespedes());
    if (!archivo.is_open()) throw runtime_error("No se pudo abrir huespedes.csv.");
    vector<Huesped> lista;
    unordered_set<string> documentos;
    string linea;
    int numero = 0;
    while (getline(archivo, linea)) {
        ++numero;
        if (!linea.empty() && linea.back() == '\r') linea.pop_back();
        if (linea.empty()) continue;
        try {
            Huesped h;
            h.deserializar(linea);
            if (!documentos.insert(h.getDocumento()).second)
                throw invalid_argument("Documento duplicado.");
            lista.push_back(h);
        } catch (const exception& e) {
            throw runtime_error("huespedes.csv linea " + to_string(numero) + ": " + e.what());
        }
    }
    if (archivo.bad()) throw runtime_error("Error al leer huespedes.csv.");
    return lista;
}

bool inicializarArchivoHuespedes() {
    try {
        crearCsvVacioSiNoExiste(rutaHuespedes());
        cargarHuespedes();
        return true;
    } catch (const exception& e) {
        cerr << "Error al preparar huespedes: " << e.what() << endl;
        return false;
    }
}

void registrarHuespedEnArchivo(const Huesped& huesped) {
    Huesped copia = huesped;
    const string linea = copia.serializar(); // Primero valida, luego escribe.
    for (const Huesped& h : cargarHuespedes()) {
        if (h.getDocumento() == huesped.getDocumento())
            throw invalid_argument("Ese documento ya existe. Usa el titular registrado.");
    }
    ofstream salida(rutaHuespedes(), ios::app);
    if (!salida.is_open()) throw runtime_error("No se pudo abrir huespedes.csv para guardar.");
    salida << linea << '\n';
    salida.flush();
    if (!salida) throw runtime_error("No se pudo guardar el huesped.");
}

// Valida codigos, grupos, conflictos de ocupacion y el limite de 5
// habitaciones SIMULTANEAS por documento (no 5 en toda la vida).
static void validarReservasConjuntas(const vector<Reserva>& lista) {
    unordered_set<string> codigosDetalle;
    // Datos compartidos por las habitaciones del mismo grupo.
    struct DatosGrupo {
        string documento, entrada, salida;
        bool activa;
        int cantidad;
    };
    unordered_map<string, DatosGrupo> grupos;
    unordered_map<string, vector<pair<string, int>>> movimientosPorTitular;

    for (const Reserva& r : lista) {
        r.validar();
        if (!codigosDetalle.insert(r.getCodigo()).second)
            throw invalid_argument("Hay codigos individuales de reserva duplicados.");

        const string grupo = r.getCodigoGrupo();
        if (grupos.count(grupo) == 0) {
            grupos[grupo] = {r.getDocumentoTitular(), r.getFechaEntrada(),
                             r.getFechaSalida(), r.estaActiva(), 0};
        }
        DatosGrupo& datos = grupos[grupo];
        if (datos.documento != r.getDocumentoTitular() ||
            datos.entrada != r.getFechaEntrada() ||
            datos.salida != r.getFechaSalida() || datos.activa != r.estaActiva())
            throw invalid_argument("Un mismo grupo debe tener titular, fechas y estado iguales.");
        ++datos.cantidad;
        if (datos.cantidad > 5) throw invalid_argument("Un grupo no puede superar 5 habitaciones.");

        if (r.estaActiva()) {
            // La fecha de salida se considera liberada; salida antes que entrada.
            movimientosPorTitular[r.getDocumentoTitular()].push_back({r.getFechaEntrada(), +1});
            movimientosPorTitular[r.getDocumentoTitular()].push_back({r.getFechaSalida(), -1});
        }
    }

    for (size_t i = 0; i < lista.size(); ++i) {
        for (size_t j = 0; j < i; ++j) {
            if (lista[i].seSuperponeCon(lista[j]))
                throw invalid_argument("La habitacion " + lista[i].getCodigoHabitacion() +
                                       " ya esta reservada en esas fechas.");
        }
    }
    for (auto& par : movimientosPorTitular) {
        auto& movimientos = par.second;
        // std::pair ordena por fecha, luego -1 (salida) antes de +1 (entrada).
        sort(movimientos.begin(), movimientos.end());
        int ocupadas = 0;
        for (const auto& evento : movimientos) {
            ocupadas += evento.second;
            if (ocupadas > 5)
                throw invalid_argument("El titular " + par.first +
                                       " superaria el limite de 5 habitaciones en fechas coincidentes.");
        }
    }
}

vector<Reserva> cargarReservas() {
    ifstream archivo(rutaReservas());
    if (!archivo.is_open()) throw runtime_error("No se pudo leer reservas.csv.");
    vector<Reserva> lista;
    string linea;
    int numero = 0;
    while (getline(archivo, linea)) {
        ++numero;
        if (!linea.empty() && linea.back() == '\r') linea.pop_back();
        if (linea.empty()) continue;
        try { lista.push_back(Reserva::deserializar(linea)); }
        catch (const exception& e) {
            throw runtime_error("reservas.csv linea " + to_string(numero) + ": " + e.what());
        }
    }
    if (archivo.bad()) throw runtime_error("Error al leer reservas.csv.");
    validarReservasConjuntas(lista);
    return lista;
}

bool inicializarArchivoReservas() {
    try {
        // En caso de interrupcion mientras se reemplazaba, se recupera el respaldo.
        const fs::path ruta = rutaReservas();
        const fs::path respaldo = ruta.string() + ".bak";
        if (!fs::exists(ruta) && fs::exists(respaldo))
            fs::copy_file(respaldo, ruta);
        crearCsvVacioSiNoExiste(ruta);
        cargarReservas();
        return true;
    } catch (const exception& e) {
        cerr << "Error al preparar reservas: " << e.what() << endl;
        return false;
    }
}

static void guardarReservasCompletas(const vector<Reserva>& lista) {
    validarReservasConjuntas(lista);
    const fs::path destino = rutaReservas();
    const fs::path temporal = destino.string() + ".tmp";
    const fs::path respaldo = destino.string() + ".bak";
    {
        ofstream salida(temporal, ios::trunc);
        if (!salida.is_open()) throw runtime_error("No se pudo crear el CSV temporal.");
        for (const Reserva& r : lista) salida << r.serializar() << '\n';
        salida.flush();
        if (!salida) throw runtime_error("No se pudieron escribir todas las reservas.");
    }
    // Se sustituye el archivo una sola vez, despues de validar TODO el grupo.
    try {
        if (fs::exists(destino)) fs::copy_file(destino, respaldo, fs::copy_options::overwrite_existing);
        fs::remove(destino); // Necesario para renombrar sobre un archivo en Windows.
        fs::rename(temporal, destino);
    } catch (...) {
        if (!fs::exists(destino) && fs::exists(respaldo)) {
            try { fs::copy_file(respaldo, destino, fs::copy_options::overwrite_existing); }
            catch (...) { /* Se conserva el .bak para recuperacion manual. */ }
        }
        throw;
    }
}

vector<Habitacion> buscarHabitacionesDisponibles(const string& entrada,
                                                const string& salida, int personas) {
    Fecha::diasEntre(entrada, salida);
    if (personas <= 0) throw invalid_argument("El numero de personas debe ser positivo.");
    const vector<Habitacion> habitaciones = cargarHabitaciones();
    const vector<Reserva> reservas = cargarReservas();
    vector<Habitacion> libres;
    for (const Habitacion& h : habitaciones) {
        if (!h.estaOperativa() || h.getCapacidad() < personas) continue;
        bool ocupada = false;
        for (const Reserva& r : reservas) {
            if (r.estaActiva() && r.getCodigoHabitacion() == h.getCodigo() &&
                Fecha::seSuperponen(entrada, salida, r.getFechaEntrada(), r.getFechaSalida())) {
                ocupada = true;
                break;
            }
        }
        if (!ocupada) libres.push_back(h);
    }
    return libres;
}

void registrarGrupoReservas(const vector<Reserva>& nuevas) {
    if (nuevas.empty() || nuevas.size() > 5)
        throw invalid_argument("Se permiten de 1 a 5 habitaciones por reserva.");
    const string grupo = nuevas[0].getCodigoGrupo();
    const string doc = nuevas[0].getDocumentoTitular();
    for (const Reserva& r : nuevas) {
        r.validar();
        if (r.getCodigoGrupo() != grupo || r.getDocumentoTitular() != doc || !r.estaActiva())
            throw invalid_argument("Todas las habitaciones deben ser del mismo titular y grupo activo.");
    }
    bool titularExiste = false;
    for (const Huesped& h : cargarHuespedes())
        if (h.getDocumento() == doc) titularExiste = true;
    if (!titularExiste) throw invalid_argument("Registra al titular antes de reservar.");

    vector<Reserva> existentes = cargarReservas();
    for (const Reserva& r : existentes)
        if (r.getCodigoGrupo() == grupo)
            throw invalid_argument("El codigo de grupo ya existe (incluso si esta cancelado).");

    const vector<Habitacion> habitaciones = cargarHabitaciones();
    for (const Reserva& r : nuevas) {
        bool encontrada = false;
        for (const Habitacion& h : habitaciones) {
            if (h.getCodigo() != r.getCodigoHabitacion()) continue;
            encontrada = true;
            if (!h.estaOperativa())
                throw invalid_argument("Habitacion " + h.getCodigo() + " no operativa.");
            if (h.getCapacidad() < r.getPersonas())
                throw invalid_argument("La habitacion " + h.getCodigo() + " no tiene capacidad suficiente.");
            if (h.getPrecioPorNoche() != r.getTarifaPorNoche())
                throw invalid_argument("La tarifa no coincide con la habitacion " + h.getCodigo() + ".");
            break;
        }
        if (!encontrada) throw invalid_argument("Habitacion inexistente: " + r.getCodigoHabitacion());
    }
    existentes.insert(existentes.end(), nuevas.begin(), nuevas.end());
    guardarReservasCompletas(existentes);
}

void cancelarGrupoReservas(const string& codigoGrupo) {
    vector<Reserva> lista = cargarReservas();
    bool encontrado = false;
    for (Reserva& r : lista) {
        if (r.getCodigoGrupo() == codigoGrupo) {
            encontrado = true;
            if (!r.estaActiva()) throw invalid_argument("El grupo ya estaba cancelado.");
            r.cancelar();
        }
    }
    if (!encontrado) throw invalid_argument("No existe ese codigo de grupo.");
    guardarReservasCompletas(lista);
}

void modificarFechasGrupo(const string& codigoGrupo, const string& entrada,
                         const string& salida) {
    Fecha::diasEntre(entrada, salida);
    vector<Reserva> lista = cargarReservas();
    bool encontrado = false;
    for (Reserva& r : lista) {
        if (r.getCodigoGrupo() == codigoGrupo) {
            encontrado = true;
            r.cambiarFechas(entrada, salida);
        }
    }
    if (!encontrado) throw invalid_argument("El grupo de reserva no existe.");
    // Comprueba que los cuartos sigan operativos y disponibles en las nuevas fechas.
    const vector<Habitacion> habitaciones = cargarHabitaciones();
    for (const Reserva& r : lista) {
        if (r.getCodigoGrupo() != codigoGrupo) continue;
        bool existe = false;
        for (const Habitacion& h : habitaciones) {
            if (h.getCodigo() == r.getCodigoHabitacion()) {
                existe = true;
                if (!h.estaOperativa())
                    throw invalid_argument("Una habitacion del grupo esta en mantenimiento o fuera de servicio.");
                if (h.getCapacidad() < r.getPersonas())
                    throw invalid_argument("La capacidad actual no permite esa reserva.");
                break;
            }
        }
        if (!existe) throw invalid_argument("Una habitacion del grupo ya no existe.");
    }
    guardarReservasCompletas(lista); // Revalida cruces y maximo 5 habitaciones del titular.
}

void agregarHabitacion(const Habitacion& nueva) {
    vector<Habitacion> habitaciones = cargarHabitaciones();
    for (const Habitacion& h : habitaciones)
        if (h.getCodigo() == nueva.getCodigo())
            throw invalid_argument("Ese codigo de habitacion ya esta registrado.");
    habitaciones.push_back(nueva);
    guardarHabitaciones(habitaciones);
}
void actualizarHabitacion(const string& codigo, TipoHabitacion tipo,
                         int capacidad, double precio) {
    vector<Habitacion> habitaciones = cargarHabitaciones();
    bool encontrado = false;
    for (Habitacion& h : habitaciones) {
        if (h.getCodigo() == codigo) {
            encontrado = true;
            // No dejar con capacidad inferior al numero de personas de reservas activas.
            for (const Reserva& r : cargarReservas()) {
                if (r.estaActiva() && r.getCodigoHabitacion() == codigo && capacidad < r.getPersonas())
                    throw invalid_argument("La nueva capacidad afectaria una reserva activa.");
            }
            h.actualizarDatos(tipo, capacidad, precio);
        }
    }
    if (!encontrado) throw invalid_argument("No existe esa habitacion.");
    guardarHabitaciones(habitaciones);
}
void cambiarEstadoHabitacion(const string& codigo, EstadoHabitacion estado) {
    vector<Habitacion> habitaciones = cargarHabitaciones();
    bool encontrado = false;
    for (Habitacion& h : habitaciones) {
        if (h.getCodigo() == codigo) {
            encontrado = true;
            if (estado != EstadoHabitacion::Disponible) {
                for (const Reserva& r : cargarReservas()) {
                    if (r.estaActiva() && r.getCodigoHabitacion() == codigo)
                        throw invalid_argument("No puedes bloquear una habitacion con reservas activas. Cancela o reubica primero.");
                }
            }
            h.cambiarEstado(estado);
        }
    }
    if (!encontrado) throw invalid_argument("No existe esa habitacion.");
    guardarHabitaciones(habitaciones);
}
void eliminarHabitacion(const string& codigo) {
    // No borrar habitaciones con historial; eso rompería el registro de reservas.
    for (const Reserva& r : cargarReservas())
        if (r.getCodigoHabitacion() == codigo)
            throw invalid_argument("Esta habitacion tiene historial de reservas; ponla fuera de servicio.");
    vector<Habitacion> habitaciones = cargarHabitaciones();
    vector<Habitacion> restantes;
    bool encontrado = false;
    for (const Habitacion& h : habitaciones) {
        if (h.getCodigo() == codigo) encontrado = true;
        else restantes.push_back(h);
    }
    if (!encontrado) throw invalid_argument("No existe esa habitacion.");
    guardarHabitaciones(restantes);
}