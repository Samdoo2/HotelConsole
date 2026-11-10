#include "../header/usuario.h"


Usuario::Usuario(){}
Usuario::Usuario(string nombreUser, string password, string rol){
    this->nombreUsuario=nombreUser;
    this->contrasena=password;
    this->rol=rol;
};

