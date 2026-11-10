#include "../header/Servicio.h"

using namespace std;


Servicio::Servicio(string codigo, string nombre, string desc, double precio, bool activo){
    this->codigo=codigo;
    this->nombre=nombre;
    this->desc=desc;
    this->precio=precio;
    this->activo=activo;
}



