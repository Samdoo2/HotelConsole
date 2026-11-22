#include "../header/persistencia.h"
#include "../header/administrador.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unordered_set>

using namespace std;
namespace fs = filesystem;

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