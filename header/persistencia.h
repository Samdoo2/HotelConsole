

#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <string>

// Verifica si el archivo existe. Si no, lo crea con un admin por defecto.
bool inicializarArchivoAdministradores();

// Busca en el CSV si el usuario y la contraseña coinciden.
bool validarCredenciales(const std::string& username, const std::string& password);

#endif