#include "../header/fecha.h"
#include <stdexcept>

bool Fecha::esBisiesto(int anio) {
    if (anio % 400 == 0) return true;
    if (anio % 100 == 0) return false;
    if (anio % 4 == 0) return true;
    return false;
}

int Fecha::diasDelMes(int mes, int anio) {
    if (mes == 2) {
        if (esBisiesto(anio)) return 29;
        return 28;
    }

    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
    return 30;
    }

    return 31;
}

bool Fecha::esValida(string fecha) {
    if (fecha.length() != 10) return false;
    if (fecha[4] != '-' || fecha[7] != '-') return false;

    for (int i = 0; i < 10; i++) {
        if (i != 4 && i != 7) {
            if (fecha[i] < '0' || fecha[i] > '9') return false;
        }
    }

    int anio = stoi(fecha.substr(0, 4));
    int mes = stoi(fecha.substr(5, 2));
    int dia = stoi(fecha.substr(8, 2));

    if (anio < 1) return false;
    if (mes < 1 || mes > 12) return false;
    if (dia < 1 || dia > diasDelMes(mes, anio)) return false;

    return true;
}

int Fecha::contarDias(string fecha) {
    int anio = stoi(fecha.substr(0, 4));
    int mes = stoi(fecha.substr(5, 2));
    int dia = stoi(fecha.substr(8, 2));

    int total = dia;

  
    for (int a = 1; a < anio; a++) {
        total = total + 365;
        if (esBisiesto(a)) total = total + 1;
    }
    for (int m = 1; m < mes; m++) {
        total = total + diasDelMes(m, anio);
    }

    return total;
}
int Fecha::diasEntre(string fechaEntrada, string fechaSalida) {
    if (!esValida(fechaEntrada) || !esValida(fechaSalida)) {
        throw invalid_argument("La fecha no es valida.");
    }

    int noches = contarDias(fechaSalida) - contarDias(fechaEntrada);

    if (noches <= 0) {
        throw invalid_argument("La salida debe ser posterior a la entrada.");
    }

    return noches;
}

bool Fecha::seSuperponen(string entrada1, string salida1,string entrada2, string salida2) {
    diasEntre(entrada1, salida1);
    diasEntre(entrada2, salida2);

    if (entrada1 < salida2 && entrada2 < salida1) {
        return true;
    }

    return false;
}
