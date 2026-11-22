#pragma once

#include "usuario.h"
using namespace std;

// El administrador tiene permisos completos por su rol,
// no necesita un nivel numerico de acceso.
class Administrador : public Usuario {
public:
    Administrador();
    Administrador(string nombreUser, string contra);
    virtual ~Administrador() = default;

    bool tienePermiso(string operacion) const override;
    string descripcionRol() const override;
    void gestionarUsuarios();
    void generarReportes();
};
