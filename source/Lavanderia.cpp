
#include "../header/servicio.h"
#include "../header/lavanderia.h"



Lavanderia::Lavanderia(string codigo, string nombre, string desc, double precio, bool activo, double recargoUrgente, bool urgente) : Servicio(codigo, nombre, desc, precio, activo){
        this->recargoUrgente=recargoUrgente;
        this->urgente=urgente;
}