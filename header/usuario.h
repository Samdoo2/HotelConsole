    #pragma once

    #include <string>
    using namespace std;

    class Usuario{

    protected:
        string nombreUsuario;
        string contrasena;
        const string rol;
        
    public:
        Usuario();
        Usuario(string nombreUsuario, string contra, const string rol);
        virtual ~Usuario() = default;
        bool autenticar(string contrasena);
        virtual bool tienePermiso(string operacion) const = 0;
        virtual string descripcionRol() const = 0;
        void validar();
        string serializar();
        void deserializar(string linea);


        string getNombreUsuario();
        string getContra();
        string getRol();
    };