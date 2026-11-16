

#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <string>
using namespace std;

// Verifica si el archivo existe. Si no, lo crea con un admin por defecto.
bool inicializarArchivoAdministradores();

// Busca en el CSV si el usuario y la contraseña coinciden.
bool validarCredenciales(const string &username, const string &password, string& rolOut);

#endif