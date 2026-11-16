#include "../header/usuario.h"


Usuario::Usuario(): nombreUsuario(""), contrasena(""), rol("") {}
Usuario::Usuario(string nombreUsuario, string contrasena, const string rol): 
nombreUsuario(nombreUsuario), contrasena(contrasena), rol(rol) {}


string Usuario::getNombreUsuario(){ return nombreUsuario;}
string Usuario::getContra(){ return contrasena;}
string Usuario::getRol(){return rol;}


