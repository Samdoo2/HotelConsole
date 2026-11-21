#pragma once
#include <string>
using namespace std;

// Crea la carpeta y el CSV con un administrador si no existe o esta vacio.
bool inicializarArchivoAdministradores();

// Verifica credenciales y devuelve el rol guardado por referencia.
bool validarCredenciales(const string& username, const string& password,
                        string& rolOut, string& datoExtraOut);

// Crea un usuario persistente. Rechaza datos invalidos y nombres duplicados.
// El cuarto dato es el nivel (Administrador) o turno (Recepcionista).
void registrarUsuario(const string& username, const string& password,
                     const string& rol, const string& datoExtra);
