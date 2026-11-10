#pragma once

#include "usuario.h"

using namespace std;


class Administrador : public Usuario{

protected:
    int nivelAcceso;
public:
    Administrador();
    Administrador(string nombreUser, string contra, string rol, int nivelAcceso);
    ~Administrador();
    bool tienePermiso(string operacion) const override;
    string descripcionRol() const override;
    void gestionarUsuarios();
    void generarReportes();

};