#include "../header/menu.h"
#include "../header/administrador.h"
#include "../header/recepcionista.h"
#include "../header/persistencia.h"
#include "../header/Fecha.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

static int leerOpcion() {
    string texto;
    while (true) {
        cout << "Seleccione una opcion: ";
        if (!getline(cin, texto)) return 0;
        istringstream entrada(texto);
        int valor; char sobra;
        if ((entrada >> valor) && !(entrada >> sobra)) return valor;
        cout << "Escribe un numero valido.\n";
    }
}
static string leerCampo(const string& mensaje) {
    cout << mensaje;
    string valor;
    if (!getline(cin, valor)) throw runtime_error("Se interrumpio la entrada.");
    if (valor.empty()) throw invalid_argument("Este campo no puede estar vacio.");
    return valor;
}
static int leerEnteroEntre(const string& mensaje, int minimo, int maximo) {
    const string texto = leerCampo(mensaje);
    istringstream entrada(texto);
    int valor; char sobra;
    if (!(entrada >> valor) || (entrada >> sobra) || valor < minimo || valor > maximo)
        throw invalid_argument("Escribe un entero entre " + to_string(minimo) + " y " + to_string(maximo) + ".");
    return valor;
}
static bool confirmar(const string& mensaje) {
    const string texto = leerCampo(mensaje + " (S/N): ");
    return texto == "S" || texto == "s";
}
static void fechasValidasParaNuevaReserva(const string& entrada, const string& salida) {
    Fecha::diasEntre(entrada, salida);
    time_t ahora = time(nullptr);
    tm* fechaLocal = localtime(&ahora);
    if (!fechaLocal) throw runtime_error("No se pudo obtener la fecha del equipo.");
    char hoy[11] = {};
    if (strftime(hoy, sizeof(hoy), "%Y-%m-%d", fechaLocal) == 0)
        throw runtime_error("No se pudo interpretar la fecha del equipo.");
    if (entrada < string(hoy))
        throw invalid_argument("La fecha de entrada no puede ser anterior a hoy.");
}

static void anunciarModulo(const Usuario& usuario, const string& permiso, const string& titulo) {
    if (!usuario.tienePermiso(permiso)) {
        cout << "Acceso denegado para este rol.\n";
        return;
    }
    cout << "\nModulo " << titulo << " pendiente en los siguientes bloques.\n";
}

static void mostrarHabitacion(const Habitacion& h) {
    cout << "Habitacion " << h.getCodigo() << " | " << Habitacion::nombreTipo(h.getTipo())
         << " | Capacidad " << h.getCapacidad() << " | $"
         << fixed << setprecision(0) << h.getPrecioPorNoche() << "/noche"
         << " | " << Habitacion::nombreEstado(h.getEstado()) << '\n';
}
static void inventarioMenu() {
    const vector<Habitacion> lista = cargarHabitaciones();
    cout << "\n=== INVENTARIO DE HABITACIONES (" << lista.size() << ") ===\n";
    for (const Habitacion& h : lista) mostrarHabitacion(h);
    cout << "Nota: 'Disponible' indica operativa, no significa libre en cualquier fecha.\n";
}
static void disponibilidadMenu() {
    cout << "\n=== CONSULTAR DISPONIBILIDAD ===\n";
    const string entrada = leerCampo("Fecha de entrada (AAAA-MM-DD): ");
    const string salida = leerCampo("Fecha de salida (AAAA-MM-DD): ");
    fechasValidasParaNuevaReserva(entrada, salida);
    const int personas = leerEnteroEntre("Personas por habitacion: ", 1, 50);
    const vector<Habitacion> libres = buscarHabitacionesDisponibles(entrada, salida, personas);
    cout << "\nHay " << libres.size() << " habitacion(es) libres para "
         << Fecha::diasEntre(entrada, salida) << " noche(s).\n";
    for (const Habitacion& h : libres) mostrarHabitacion(h);
}

static void mostrarResumenGrupo(const vector<Reserva>& grupo) {
    if (grupo.empty()) return;
    const Reserva& primera = grupo[0];
    cout << "\n=== RESERVA " << primera.getCodigoGrupo() << " ===\n"
         << "Titular: " << primera.getDocumentoTitular()
         << " | Fechas: " << primera.getFechaEntrada() << " a " << primera.getFechaSalida()
         << " | Noches: " << primera.calcularNoches() << '\n';
    double total = 0;
    int totalPersonas = 0;
    for (const Reserva& r : grupo) {
        cout << "  Habitacion " << r.getCodigoHabitacion() << " | " << r.getPersonas()
             << " persona(s) | " << Reserva::nombreEstado(r.getEstado())
             << " | $" << fixed << setprecision(0) << r.calcularValorEstimado() << '\n';
        total += r.calcularValorEstimado();
        totalPersonas += r.getPersonas();
    }
    cout << "Habitaciones: " << grupo.size() << " | Personas: " << totalPersonas
         << " | Total estimado: $" << fixed << setprecision(0) << total << '\n';
    cout << "(Solo alojamiento; sin impuestos ni servicios adicionales.)\n";
}

static void registrarGrupoMenu() {
    cout << "\n=== REGISTRAR RESERVA DE 1 A 5 HABITACIONES ===\n";
    const string codigoGrupo = leerCampo("Codigo de reserva (ej. R001): ");
    const string documento = leerCampo("Documento de la persona titular: ");
    bool existe = false;
    for (const Huesped& h : cargarHuespedes()) {
        if (h.getDocumento() == documento) {
            existe = true;
            cout << "Titular encontrado: " << h.getNombre() << '\n';
            break;
        }
    }
    if (!existe) {
        cout << "No existe ese titular. Vamos a registrarlo ahora.\n";
        const string nombre = leerCampo("Nombre completo: ");
        const string telefono = leerCampo("Telefono: ");
        const string correo = leerCampo("Correo: ");
        registrarHuespedEnArchivo(Huesped(documento, nombre, telefono, correo));
        cout << "Titular registrado. Ahora continuamos con la reserva.\n";
    }

    const string entrada = leerCampo("Fecha de entrada (AAAA-MM-DD): ");
    const string salida = leerCampo("Fecha de salida (AAAA-MM-DD): ");
    fechasValidasParaNuevaReserva(entrada, salida);

    const int cantidad = leerEnteroEntre("Cuantas habitaciones va a reservar (1-5)?: ", 1, 5);
    vector<Reserva> nuevas;
    for (int i = 1; i <= cantidad; ++i) {
        cout << "\n--- Habitacion " << i << " de " << cantidad << " ---\n";
        const int personas = leerEnteroEntre("Cuantas personas se alojaran en esta habitacion?: ", 1, 50);
        const vector<Habitacion> disponibles = buscarHabitacionesDisponibles(entrada, salida, personas);
        vector<Habitacion> elegibles;
        for (const Habitacion& h : disponibles) {
            bool yaElegida = false;
            for (const Reserva& r : nuevas)
                if (r.getCodigoHabitacion() == h.getCodigo()) yaElegida = true;
            if (!yaElegida) elegibles.push_back(h);
        }
        if (elegibles.empty())
            throw invalid_argument("No hay suficientes habitaciones libres para completar el grupo.");
        cout << "Habitaciones que puedes elegir:\n";
        for (const Habitacion& h : elegibles) mostrarHabitacion(h);
        const string codigoH = leerCampo("Codigo de la habitacion elegida: ");
        bool encontrada = false;
        for (const Habitacion& h : elegibles) {
            if (h.getCodigo() == codigoH) {
                nuevas.push_back(Reserva(codigoGrupo + "-" + to_string(i), codigoGrupo,
                                         documento, h.getCodigo(), entrada, salida,
                                         personas, h.getPrecioPorNoche()));
                encontrada = true;
                break;
            }
        }
        if (!encontrada)
            throw invalid_argument("Ese codigo no esta disponible o ya fue seleccionado.");
    }
    mostrarResumenGrupo(nuevas);
    if (!confirmar("Confirmar y guardar TODAS las habitaciones")) {
        cout << "Operacion descartada. No se guardo ninguna habitacion.\n";
        return;
    }
    registrarGrupoReservas(nuevas); // Se valida y guarda todo de una sola vez.
    cout << "GRUPO GUARDADO: " << codigoGrupo << " | " << cantidad << " habitacion(es).\n";
}

static void listarGruposMenu() {
    const vector<Reserva> lista = cargarReservas();
    cout << "\n=== RESERVAS POR GRUPOS ===\n";
    if (lista.empty()) { cout << "No existen reservas.\n"; return; }
    vector<string> vistos;
    for (const Reserva& r : lista) {
        bool repetido = false;
        for (const string& c : vistos) if (c == r.getCodigoGrupo()) repetido = true;
        if (repetido) continue;
        vistos.push_back(r.getCodigoGrupo());
        vector<Reserva> grupo;
        for (const Reserva& otra : lista)
            if (otra.getCodigoGrupo() == r.getCodigoGrupo()) grupo.push_back(otra);
        mostrarResumenGrupo(grupo);
    }
}
static void buscarGrupoMenu() {
    const string codigo = leerCampo("Codigo de grupo (ej. R001): ");
    const vector<Reserva> lista = cargarReservas();
    vector<Reserva> grupo;
    for (const Reserva& r : lista) if (r.getCodigoGrupo() == codigo) grupo.push_back(r);
    if (grupo.empty()) cout << "No existe una reserva con ese codigo.\n";
    else mostrarResumenGrupo(grupo);
}
static void cambiarFechasMenu() {
    const string grupo = leerCampo("Codigo del grupo a modificar: ");
    const string entrada = leerCampo("NUEVA entrada (AAAA-MM-DD): ");
    const string salida = leerCampo("NUEVA salida (AAAA-MM-DD): ");
    fechasValidasParaNuevaReserva(entrada, salida);
    if (!confirmar("Cambiar las fechas para TODAS las habitaciones del grupo")) return;
    modificarFechasGrupo(grupo, entrada, salida);
    cout << "Fechas actualizadas y guardadas correctamente.\n";
}
static void cancelarGrupoMenu() {
    const string grupo = leerCampo("Codigo de grupo a cancelar: ");
    if (!confirmar("Cancelar TODAS las habitaciones de ese grupo")) return;
    cancelarGrupoReservas(grupo);
    cout << "Grupo cancelado. Se conserva el historial y se liberan esas fechas.\n";
}

static void menuTitulares() {
    int opcion;
    do {
        cout << "\n=== TITULARES DE RESERVA ===\n"
             << "1. Registrar titular\n2. Consultar titulares\n3. Buscar por documento\n0. Volver\n";
        opcion = leerOpcion();
        try {
            switch (opcion) {
                case 1: {
                    const string doc = leerCampo("Documento: ");
                    const string nombre = leerCampo("Nombre completo: ");
                    const string telefono = leerCampo("Telefono: ");
                    const string correo = leerCampo("Correo: ");
                    Huesped h(doc, nombre, telefono, correo);
                    registrarHuespedEnArchivo(h);
                    cout << "Titular guardado en huespedes.csv.\n";
                    break;
                }
                case 2: {
                    for (const Huesped& h : cargarHuespedes())
                        cout << h.getDocumento() << " | " << h.getNombre() << " | "
                             << h.getTelefono() << " | " << h.getCorreo() << '\n';
                    break;
                }
                case 3: {
                    const string doc = leerCampo("Documento: ");
                    bool encontrado = false;
                    for (const Huesped& h : cargarHuespedes()) {
                        if (h.getDocumento() == doc) {
                            cout << h.getNombre() << " | " << h.getTelefono() << " | " << h.getCorreo() << '\n';
                            encontrado = true;
                        }
                    }
                    if (!encontrado) cout << "No registrado.\n";
                    break;
                }
                case 0: break;
                default: cout << "Opcion no valida.\n";
            }
        } catch (const exception& e) { cout << "Error: " << e.what() << '\n'; }
    } while (opcion != 0);
}

// Agrupamos consultas para mantener pequeno el menu principal de reservas.
static void menuConsultasReservas() {
    int opcion;
    do {
        cout << "\n=== CONSULTAR RESERVAS Y TITULARES ===\n"
             << "1. Ver todas las reservas\n"
             << "2. Buscar reserva por codigo\n"
             << "3. Ver o registrar titulares\n"
             << "0. Volver\n";
        opcion = leerOpcion();
        try {
            switch (opcion) {
                case 1: listarGruposMenu(); break;
                case 2: buscarGrupoMenu(); break;
                case 3: menuTitulares(); break;
                case 0: break;
                default: cout << "Opcion no valida.\n"; break;
            }
        } catch (const exception& e) {
            cout << "No se pudo consultar: " << e.what() << '\n';
        }
    } while (opcion != 0);
}

// Solo dos acciones para editar un grupo; no afecta a otros grupos.
static void menuCambiosReservas() {
    cout << "\n=== MODIFICAR O CANCELAR RESERVA ===\n"
         << "1. Cambiar fechas de todo el grupo\n"
         << "2. Cancelar todo el grupo\n"
         << "0. Volver\n";
    const int opcion = leerOpcion();
    if (opcion == 1) cambiarFechasMenu();
    else if (opcion == 2) cancelarGrupoMenu();
    else if (opcion != 0) cout << "Opcion no valida.\n";
}

// Modulo 2: reservas y habitaciones, sin administracion del inventario.
static void menuReservasYHabitaciones(const Usuario& user) {
    if (!user.tienePermiso("reservas")) {
        cout << "Acceso denegado para este rol.\n";
        return;
    }
    int opcion;
    do {
        cout << "\n=== 2. RESERVAS Y HABITACIONES ===\n"
             << "1. Ver habitaciones y disponibilidad\n"
             << "2. Registrar reserva (1 a 5 habitaciones)\n"
             << "3. Consultar reservas y titulares\n"
             << "4. Modificar o cancelar una reserva\n"
             << "0. Volver\n";
        opcion = leerOpcion();
        try {
            switch (opcion) {
                case 1:
                    inventarioMenu();
                    if (confirmar("Consultar disponibilidad para fechas concretas"))
                        disponibilidadMenu();
                    break;
                case 2: registrarGrupoMenu(); break;
                case 3: menuConsultasReservas(); break;
                case 4: menuCambiosReservas(); break;
                case 0: break;
                default: cout << "Opcion no valida.\n";
            }
        } catch (const exception& e) {
            cout << "No se pudo completar la operacion: " << e.what() << '\n';
        }
    } while (opcion != 0);
}

void menuPrincipal(const Usuario& user) {
    cout << "\nBienvenido, " << user.descripcionRol() << endl;
    const bool admin = user.getRol() == "Administrador";
    const bool recep = user.getRol() == "Recepcionista";
    if (!admin && !recep) { cout << "Rol no reconocido.\n"; return; }

    // Inicializacion desde persistencia: nunca se duplican habitaciones.
    if (!inicializarArchivoHabitaciones() || !inicializarArchivoHuespedes() ||
        !inicializarArchivoReservas()) {
        cout << "No se pudo preparar la informacion del hotel.\n";
        return;
    }
    int opcion;
    do {
        cout << "\n=== MENU PRINCIPAL - " << (admin ? "ADMINISTRADOR" : "RECEPCIONISTA")
             << " ===\n";
        if (admin) cout << "1. Opciones de administrador\n";
        cout << "2. Reservas y Habitaciones\n"
             << "3. Gestion de Huespedes (pendiente)\n"
             << "4. Registrar Consumos (pendiente)\n"
             << "5. Facturacion y Check-out (pendiente)\n"
             << "0. Cerrar sesion\n";
        opcion = leerOpcion();
        switch (opcion) {
            case 1:
                if (admin && user.tienePermiso("usuarios")) menuAdmin(user);
                else cout << "Acceso denegado.\n";
                break;
            case 2:
                if (user.tienePermiso("reservas")) menuReservasYHabitaciones(user);
                else cout << "Acceso denegado.\n";
                break;
            case 3: anunciarModulo(user, "huespedes", "Huespedes"); break;
            case 4: anunciarModulo(user, "consumos", "Consumos"); break;
            case 5: anunciarModulo(user, "facturacion", "Facturacion y Check-out"); break;
            case 0: cout << "Cerrando sesion...\n"; break;
            default: cout << "Opcion no valida.\n";
        }
    } while (opcion != 0);
}

void menuAdmin(const Usuario& user) {
    if (!user.tienePermiso("usuarios")) { cout << "Acceso denegado.\n"; return; }
    int opcion;
    do {
        cout << "\n=== OPCIONES DE ADMINISTRADOR ===\n"
             << "1. Gestion de Usuarios\n"
             << "2. Reservas y Habitaciones\n"
             << "3. Gestion de Huespedes (pendiente)\n"
             << "4. Gestion de Servicios (pendiente)\n"
             << "0. Volver al menu principal\n";
        opcion = leerOpcion();
        switch (opcion) {
            case 1: menuGestionUsuarios(); break;
            case 2: menuReservasYHabitaciones(user); break;
            case 3: anunciarModulo(user, "huespedes", "Huespedes"); break;
            case 4: anunciarModulo(user, "servicios_gestion", "Servicios"); break;
            case 0: break;
            default: cout << "Opcion no valida.\n";
        }
    } while (opcion != 0);
}
void menuGestionUsuarios() {
    int opcion;
    do {
        cout << "\n=== GESTION DE USUARIOS ===\n"
             << "1. Crear Usuario\n"
             << "2. Eliminar Usuario (pendiente)\n"
             << "3. Editar Usuario (pendiente)\n"
             << "0. Volver\n";
        opcion = leerOpcion();
        switch (opcion) {
            case 1:
                try {
                    cout << "\n-- Crear Usuario --\n";
                    string nombre = leerCampo("Nombre de usuario: ");
                    string contrasena = leerCampo("Contrasena: ");
                    string tipo = leerCampo("Tipo (Administrador/Recepcionista): ");
                    string extra;
                    if (tipo == "Administrador") {
                        // No se solicita nivel: el rol determina los permisos.
                        Administrador usuarioNuevo(nombre, contrasena);
                    } else if (tipo == "Recepcionista") {
                        extra = leerCampo("Turno: ");
                        Recepcionista usuarioNuevo(nombre, contrasena, extra);
                    } else {
                        throw invalid_argument("Tipo no valido: usa Administrador o Recepcionista.");
                    }
                    registrarUsuario(nombre, contrasena, tipo, extra);
                    cout << "Usuario creado y guardado en usuarios.csv." << endl;
                } catch (const exception& e) {
                    cout << "No se pudo crear el usuario: " << e.what() << endl;
                }
                break;
            case 2: cout << "Eliminar usuarios se implementara en otro bloque." << endl; break;
            case 3: cout << "Editar usuarios se implementara en otro bloque." << endl; break;
            case 0: cout << "Volviendo..." << endl; break;
            default: cout << "Opcion no valida." << endl;
        }
    } while (opcion != 0);
}