#pragma once
#include "usuario.h"
using namespace std;
class Recepcionista : public Usuario{
protected:
    string turno;
public:
    Recepcionista();
    Recepcionista(string nombreUser, string contra, string turno);
    ~Recepcionista();
    
    bool tienePermiso(string operacion) const override;
    string descripcionRol() const override;
    void registrarReserva();
    void registrarConsumo();
};