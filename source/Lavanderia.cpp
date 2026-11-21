#include "../header/Servicio.h"
#include "../header/Lavanderia.h"



Lavanderia::Lavanderia() : Servicio(), recargoUrgente(0), urgente(false) {}

Lavanderia::Lavanderia(string codigo, string nombre, string desc, double precio, bool activo, double recargoUrgente, bool urgente) : Servicio(codigo, nombre, desc, precio, activo){
        this->recargoUrgente=recargoUrgente;
        this->urgente=urgente;
}
