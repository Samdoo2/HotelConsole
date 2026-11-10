#pragma once
#include "usuario.h"
using namespace std;
class Recepcionista : public Usuario{
protected:
    string turno;
public:
    Recepcionista();
    Recepcionista(string nombreUser, string contra, string rol, string turno):Usuario(nombreUser, contra, rol){this->turno=turno;};
    ~Recepcionista();
    bool tienePermiso(string operacion) const override;
    string descripcionRol() const override;
    void registrarReserva();
    void registrarConsumo();
};