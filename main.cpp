#include <iostream>
#include <string>
#include <conio.h>
#include <fstream>

using namespace std;






int main(){

    ofstream out_admin("administradores.csv", ios::out);
    ifstream in_admin("administradores.csv", ios::in);

    if(!out_admin.is_open()){
        cout << "No se pudo abrir el archivo";
        return 1;
    }



    

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