#include <iostream>
#include <string>
#include <conio.h>
#include <fstream>
#include "../header/persistencia.h"


using namespace std;


int main(){

    crearArchivoAdministradores();

    string password = "";
    char ch;

    cout << "Introduce tu contrasena: ";

    while ((ch = _getch()) != '\r') { // '\r' es la tecla Enter en Windows
        if (ch == '\b') { // Si se presiona Backspace (Borrar)
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b"; // Borra el asterisco de la pantalla
            }
        } else {
            password.push_back(ch);
            std::cout << '*'; // Muestra el asterisco en lugar del carácter
        }
    }

    cout << "\nBienvenido.\n";


    return 0;
}