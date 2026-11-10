#include "../header/usuario.h"
#include "../header/admininistrador.h"
#include <string>
using namespace std;



    Administrador::Administrador(){}
    Administrador::Administrador(string nombreUser, string contra, string rol, int nivelAcceso) : Usuario( nombreUser, contra, rol){this->nivelAcceso=nivelAcceso;}



