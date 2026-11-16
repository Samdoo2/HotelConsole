#include "../header/recepcionista.h"
#include "../header/usuario.h"

#include <string>
using namespace std;


    Recepcionista::Recepcionista() : Usuario("", "", "Recepcionista") {}

    Recepcionista::Recepcionista(string nombreUser, string contra, string turno)
        : Usuario(nombreUser, contra, "Recepcionista") {
        this->turno = turno;
    }

    Recepcionista::~Recepcionista() {}

    bool Recepcionista::tienePermiso(string operacion) const {
        return false;
    }

    string Recepcionista::descripcionRol() const {
        return "Recepcionista";
    }

    void Recepcionista::registrarReserva() {}
    void Recepcionista::registrarConsumo() {}

