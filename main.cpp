#include <iostream>
#include <string>
#include <conio.h>
#include <fstream>
<<<<<<< HEAD
#include <sstream>
=======
#include "../header/persistencia.h"

>>>>>>> 614814c9993c36a942c12b600a7cd1f73eec0df6

using namespace std;

int main() {
    string username, password;
    bool autenticado = false;

<<<<<<< HEAD
    cout << "--- SISTEMA HOTEL CONSOLE ---" << endl;

    // Bucle infinito hasta que las credenciales sean correctas
    while (!autenticado) {
        cout << "Usuario: ";
        cin >> username;
        
        cout << "Contrasena: ";
        password = ""; // Se reinicia en cada intento
        char ch;
        
        // Captura de contraseña con asteriscos
        while ((ch = _getch()) != '\r') {
            if (ch == '\b') { 
                if (!password.empty()) {
                    password.pop_back();
                    cout << "\b \b";
                }
            } else {
                password.push_back(ch);
                cout << '*'; 
=======
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
>>>>>>> 614814c9993c36a942c12b600a7cd1f73eec0df6
            }
        }
        cout << endl;

        // Apertura y lectura del archivo
        ifstream in_admin("administradores.csv");
        if (!in_admin.is_open()) {
            cout << "Error: No se pudo abrir administradores.csv" << endl;
            return 1; // Termina si hay error con el archivo
        }

        string linea, csv_user, csv_pass;
        bool credencialesCorrectas = false;

        // Búsqueda en el archivo CSV
        while (getline(in_admin, linea)) {
            stringstream ss(linea);
            getline(ss, csv_user, ',');  // Extrae hasta la primera coma
            getline(ss, csv_pass, ',');  // Extrae hasta la segunda coma

            if (csv_user == username && csv_pass == password) {
                credencialesCorrectas = true;
                break; // Si coincide, detenemos la búsqueda
            }
        }
        
        in_admin.close(); // Cerramos el archivo

        // Verificación final del intento
        if (credencialesCorrectas) {
            autenticado = true;
            cout << "\nIngreso exitoso. Bienvenido(a), " << username << "!" << endl;
        } else {
            cout << "Credenciales incorrectas. Intente de nuevo.\n" << endl;
        }
    }

    // A partir de aquí puedes agregar el menú principal de tu programa
    // ...

    return 0;
}