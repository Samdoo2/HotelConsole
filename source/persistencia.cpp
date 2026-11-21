#include "../header/persistencia.h"
#include "../header/administrador.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

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
        Administrador admin("admin", "12345", 1);
        salida << admin.getNombreUsuario() << ',' << admin.getContra() << ','
               << admin.getRol() << ',' << admin.getNivelAcceso() << '\n';
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
        if (!getline(ss, csvUser, ',') || !getline(ss, csvPass, ',') ||
            !getline(ss, csvRol, ',') || !getline(ss, csvDato)) continue;
        if (!csvDato.empty() && csvDato.back() == '\r') csvDato.pop_back();
        if (csvUser == username && csvPass == password) {
            rolOut = rolNormalizado(csvRol);
            datoExtraOut = csvDato;
            return true;
        }
    }
    return false;
}

void registrarUsuario(const string& username, const string& password,
                     const string& rol, const string& datoExtra) {
    if (!campoValido(username) || !campoValido(password) || !campoValido(datoExtra))
        throw invalid_argument("No se permiten campos vacios, comas ni saltos de linea.");
    if (rol != "Administrador" && rol != "Recepcionista")
        throw invalid_argument("Tipo de usuario invalido.");

    if (rol == "Administrador") {
        istringstream num(datoExtra);
        int nivel; char sobra;
        if (!(num >> nivel) || nivel <= 0 || (num >> sobra))
            throw invalid_argument("El nivel de acceso debe ser un entero mayor a 0.");
    }

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
    salida << username << ',' << password << ',' << rol << ',' << datoExtra << '\n';
    salida.flush();
    if (!salida) throw runtime_error("No se pudo guardar el usuario.");
}