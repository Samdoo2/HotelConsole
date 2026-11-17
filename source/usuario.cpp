#include "../header/usuario.h"


Usuario::Usuario(): nombreUsuario(""), contrasena(""), rol("") {}
Usuario::Usuario(string nombreUsuario, string contrasena, const string rol): 
nombreUsuario(nombreUsuario), contrasena(contrasena), rol(rol) {}


string Usuario::getNombreUsuario() const { return nombreUsuario;}
string Usuario::getContra() const { return contrasena;}
string Usuario::getRol() const {return rol;}


