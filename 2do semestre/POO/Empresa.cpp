#include <iostream>
#include <string>
#include <list>
using namespace std;

class Persona {
private:
    string Tipo_documento; // ej. DNI, Pasaporte
    string Numero_documento;
    string Materno;
    string Paterno;
    string Nombre;
    string Genero; // ej. Masculino, Femenino
    string Correo_electronico;
    string Telefono;
    string Direccion;

public:
    Persona(string tipo_doc, string num_doc, string materno, string paterno, string nombre, string genero,
            string correo, string telefono, string direccion) {
        Tipo_documento = tipo_doc;
        Numero_documento = num_doc;
        Materno = materno;
        Paterno = paterno;
        Nombre = nombre;
        Genero = genero;
        Correo_electronico = correo;
        Telefono = telefono;
        Direccion = direccion;
    }

//getters
    string get_tipo_documento() {
        return Tipo_documento;
    }

    string get_numero_documento() {
        return Numero_documento;
    }

    string get_materno() {
        return Materno;
    }

    string get_paterno() {
        return Paterno;
    }

    string get_nombre() {
        return Nombre;
    }

    string get_genero() {
        return Genero;
    }

    string get_correo_electronico() {
        return Correo_electronico;
    }

    string get_telefono() {
        return Telefono;
    }

    string get_direccion() {
        return Direccion;
    }
//setters
    void set_materno(string materno) {
        Materno = materno;
    }

    void set_paterno(string paterno) {
        Paterno = paterno;
    }

    void set_nombre(string nombre) {
        Nombre = nombre;
    }

    void set_genero(string genero) {
        Genero = genero;
    }

    void set_correo_electronico(string correo) {
        Correo_electronico = correo;
    }

    void set_telefono(string telefono) {
        Telefono = telefono;
    }

    void set_direccion(string direccion) {
        Direccion = direccion;
    }
};
class Cliente : public Persona {
private:
    string Categoria; // ej. Regular, Premium, VIP
public:
    Cliente(string tipo_doc, string num_doc, string materno, string paterno, string nombre, string genero,
            string correo, string telefono, string direccion, string categoria)
        : Persona(tipo_doc, num_doc, materno, paterno, nombre, genero, correo, telefono, direccion) {
        Categoria = categoria;
    }

    // Getter
    string get_categoria() {
        return Categoria;
    }

    // Setter
    void set_categoria(string categoria) {
        Categoria = categoria;
    }
};
class Proveedor : public Persona {
private:
    string RUC; // Registro Único de Contribuyentes
    string Razon_social;
    string Pagina_web;
    bool Estado; // Activo o Inactivo
public:
    Proveedor(string tipo_doc, string num_doc, string materno, string paterno, string nombre, string genero,
              string correo, string telefono, string direccion, string ruc, string razon_social,
              string pagina_web, bool estado)
        : Persona(tipo_doc, num_doc, materno, paterno, nombre, genero, correo, telefono, direccion) {
        RUC = ruc;
        Razon_social = razon_social;
        Pagina_web = pagina_web;
        Estado = estado;
    }

    // Getters
    string get_ruc() {
        return RUC;
    }

    string get_razon_social() {
        return Razon_social;
    }

    string get_pagina_web() {
        return Pagina_web;
    }

    bool get_estado() {
        return Estado;
    }

    // Setters
    void set_razon_social(string razon) {
        Razon_social = razon;
    }

    void set_pagina_web(string pagina) {
        Pagina_web = pagina;
    }

    void set_estado(bool est) {
        Estado = est;
    }
};
class Empleado : public Persona {
private:
    string Codigo_empleado;
    string Cargo; // ej. Gerente, Técnico, Administrativo
    Fecha Fecha_ingreso;
    float Sueldo;
    string Horario_trabajo; // ej. 9:00-18:00
    bool Estado; // Activo o Inactivo
public:
    Empleado(string tipo_doc, string num_doc, string materno, string paterno, string nombre, string genero,
             string correo, string telefono, string direccion, string codigo_emp, string cargo,
             Fecha fecha_ingreso, float sueldo, string horario, bool estado)
        : Persona(tipo_doc, num_doc, materno, paterno, nombre, genero, correo, telefono, direccion) {
        Codigo_empleado = codigo_emp;
        Cargo = cargo;
        Fecha_ingreso = fecha_ingreso;
        Sueldo = sueldo;
        Horario_trabajo = horario;
        Estado = estado;
    }

    // Getters
    string get_codigo_empleado() {
        return Codigo_empleado;
    }

    string get_cargo() {
        return Cargo;
    }

    Fecha get_fecha_ingreso() {
        return Fecha_ingreso;
    }

    float get_sueldo() {
        return Sueldo;
    }

    string get_horario_trabajo() {
        return Horario_trabajo;
    }

    bool get_estado() {
        return Estado;
    }

    // Setters
    void set_cargo(string cargo) {
        Cargo = cargo;
    }

    void set_fecha_ingreso(Fecha fecha) {
        Fecha_ingreso = fecha;
    }

    void set_sueldo(float sueldo) {
        Sueldo = sueldo;
    }

    void set_horario_trabajo(string horario) {
        Horario_trabajo = horario;
    }

    void set_estado(bool est) {
        Estado = est;
    }
};
// Clase Fecha
class Fecha {
private:
    int dia;
    int mes;
    int anio;

public:
    Fecha() {
        dia = 1;
        mes = 1;
        anio = 2026;
    }

    Fecha(int d, int m, int a) {
        dia = d;
        mes = m;
        anio = a;
    }

    void imprimir() {
        cout << dia << "/" << mes << "/" << anio;
    }
};
// Class Transaccion
class Transaccion {
private:
    string Id_transaccion;
    Fecha Fecha_transaccion;
    float Monto_total;
    string Estado; // ej. Completada, Pendiente, Cancelada

public:
    Transaccion(string id, Fecha fecha, float monto, string est) {
        Id_transaccion = id;
        Fecha_transaccion = fecha;
        Monto_total = monto;
        Estado = est;
    }

    // Getters
    string get_id_transaccion() {
        return Id_transaccion;
    }

    Fecha get_fecha_transaccion() {
        return Fecha_transaccion;
    }

    float get_monto_total() {
        return Monto_total;
    }

    string get_estado() {
        return Estado;
    }

    // Setters
    void set_monto_total(float monto) {
        Monto_total = monto;
    }

    void set_estado(string est) {
        Estado = est;
    }
};
class Venta : public Transaccion {
private:
    Cliente cliente;
    Empleado empleado;
    list<string> Productos_vendidos; // Lista de productos vendidos
    string Tipo_Comprobante; // ej. Factura, Boleta
public:
   Venta(string id, Fecha fecha, float monto, string est,
          Cliente cli, Empleado emp, list<string> productos,
          string tipo_comp)
        : Transaccion(id, fecha, monto, est) {

        cliente = cli;
        empleado = emp;
        Productos_vendidos = productos;
        Tipo_Comprobante = tipo_comp;

    }
    // Getters
    Cliente get_cliente() {
        return cliente;
    }

    Empleado get_empleado() {
        return empleado;
    }

    list<string> get_productos_vendidos() {
        return Productos_vendidos;
    }

    string get_tipo_comprobante() {
        return Tipo_Comprobante;
    }

    // Setters
    void set_cliente(Cliente cli) {
        cliente = cli;
    }

    void set_empleado(Empleado emp) {
        empleado = emp;
    }

    void set_productos_vendidos(list<string> productos) {
        Productos_vendidos = productos;
    }

    void set_tipo_comprobante(string tipo_comp) {
        Tipo_Comprobante = tipo_comp;
    }
};
class Compra : public Transaccion {
private:
    Proveedor proveedor;
    Empleado empleado;
    list<string> Productos_comprados; // Lista de productos comprados
    string Tipo_Comprobante; // ej. Factura, Boleta
    string Numero_factura_proveedor; // Número de factura o boleta del proveedor

public:
    Compra(string id, Fecha fecha, float monto, string est,
           Proveedor prov, Empleado emp, list<string> productos,
           string tipo_comp, string num_factura)
        : Transaccion(id, fecha, monto, est) {

        proveedor = prov;
        empleado = emp;
        Productos_comprados = productos;
        Tipo_Comprobante = tipo_comp;
        Numero_factura_proveedor = num_factura;
    }
    
    // Getters
    Proveedor get_proveedor() {
        return proveedor;
    }   

    Empleado get_empleado() {
        return empleado;
    }

    list<string> get_productos_comprados() {
        return Productos_comprados;
    }

    string get_tipo_comprobante() {
        return Tipo_Comprobante;
    }

    string get_numero_factura_proveedor() {
        return Numero_factura_proveedor;
    }

    // Setters
    void set_proveedor(Proveedor prov) {
        proveedor = prov;
    }

    void set_empleado(Empleado emp) {
        empleado = emp;
    }

    void set_productos_comprados(list<string> productos) {
        Productos_comprados = productos;
    }

    void set_tipo_comprobante(string tipo_comp) {
        Tipo_Comprobante = tipo_comp;
    }

    void set_numero_factura_proveedor(string num_factura) {
        Numero_factura_proveedor = num_factura;
    }
};
class Metodo_pago {
private:
    string Id_pago;
    float Monto;
    string Moneda;
    Fecha Fecha_pago;
    string Estado; // ej. Completado, Pendiente, Cancelado

public:
    Metodo_pago(string id, float monto, string moneda, Fecha fecha, string est) {
        Id_pago = id;
        Monto = monto;
        Moneda = moneda;
        Fecha_pago = fecha;
        Estado = est;
    }
    
    // Getters
    string get_id_pago() {
        return Id_pago;
    }

    float get_monto() {
        return Monto;
    }

    string get_moneda() {
        return Moneda;
    }

    Fecha get_fecha_pago() {
        return Fecha_pago;
    }

    string get_estado() {
        return Estado;
    }
    // Setters
    void set_estado(string est) {
        Estado = est;
    }
};
class Efectivo : public Metodo_pago {
private:

    float Monto_recibido;
    float Cambio;

    public:
    Efectivo(string id, float monto, string moneda, Fecha fecha, string est,
             float monto_recibido, float cambio)
        : Metodo_pago(id, monto, moneda, fecha, est) {
        Monto_recibido = monto_recibido;
        Cambio = cambio;
    }
    
    // Getters
    float get_monto_recibido() {
        return Monto_recibido;
    }

    float get_cambio() {
        return Cambio;
    }

    //setters
    void set_monto_recibido(float monto_recibido) {
        Monto_recibido = monto_recibido;
    }
    void set_cambio(float cambio) {
        Cambio = cambio;
    }
};
class Transferencia_bancaria : public Metodo_pago {
private:
    string Banco_origen;
    string Banco_destino;
    string Numero_operacion;
    string Cuenta_origen;
    string Cuenta_destino;

public:
    Transferencia_bancaria(string id, float monto, string moneda, Fecha fecha, string est,
                           string banco_origen, string banco_destino,
                           string num_operacion, string cuenta_origen, string cuenta_destino)
        : Metodo_pago(id, monto, moneda, fecha, est) {
        Banco_origen = banco_origen;
        Banco_destino = banco_destino;
        Numero_operacion = num_operacion;
        Cuenta_origen = cuenta_origen;
        Cuenta_destino = cuenta_destino;
    }

    // Getters
    string get_banco_origen() {
        return Banco_origen;
    }

    string get_banco_destino() {
        return Banco_destino;
    }

    string get_numero_operacion() {
        return Numero_operacion;
    }

    string get_cuenta_origen() {
        return Cuenta_origen;
    }

    string get_cuenta_destino() {
        return Cuenta_destino;
    }

    // Setters
    void set_banco_origen(string banco) {
        Banco_origen = banco;
    }

    void set_banco_destino(string banco) {
        Banco_destino = banco;
    }

    void set_numero_operacion(string num) {
        Numero_operacion = num;
    }

    void set_cuenta_origen(string cuenta) {
        Cuenta_origen = cuenta;
    }

    void set_cuenta_destino(string cuenta) {
        Cuenta_destino = cuenta;
    }
};

class Pago_digital : public Metodo_pago {
private:
    string Plataforma; // ej. Yape, Plin
    string Numero_telefono;
    string Correo_cuenta;
    string Codigo_transaccion;

public:
    Pago_digital(string id, float monto, string moneda, Fecha fecha, string est,
                 string plataforma, string num_telefono, string correo_cuenta, string cod_transaccion)
        : Metodo_pago(id, monto, moneda, fecha, est) {
        Plataforma = plataforma;
        Numero_telefono = num_telefono;
        Correo_cuenta = correo_cuenta;
        Codigo_transaccion = cod_transaccion;
    }

    // Getters
    string get_plataforma() {
        return Plataforma;
    }

    string get_numero_telefono() {
        return Numero_telefono;
    }

    string get_correo_cuenta() {
        return Correo_cuenta;
    }

    string get_codigo_transaccion() {
        return Codigo_transaccion;
    }
    // Setters
    void set_plataforma(string plataforma) {
        Plataforma = plataforma;
    }

    void set_numero_telefono(string num) {
        Numero_telefono = num;
    }

    void set_correo_cuenta(string correo) {
        Correo_cuenta = correo;
    }

};
// Clase Base: Servicio
class Servicio {
private:
    string Id_servicio;
    string Nombre;
    string Descripcion;
    float Costo_base;
    Fecha Fecha_solicitud;
    string Estado; // ej. Pendiente, En Proceso, Completado, Cancelado

public:
    Servicio(string id, string nom, string desc, float costo, Fecha fecha, string est) {
        Id_servicio = id;
        Nombre = nom;
        Descripcion = desc;
        Costo_base = costo;
        Fecha_solicitud = fecha;
        Estado = est;
    }

    // Getters
    string get_id_servicio() {
        return Id_servicio;
    }

    string get_nombre() {
        return Nombre;
    }

    string get_descripcion() {
        return Descripcion;
    }

    float get_costo_base() {
        return Costo_base;
    }

    Fecha get_fecha_solicitud() {
        return Fecha_solicitud;
    }

    string get_estado() {
        return Estado;
    }

    // Setters
    void set_nombre(string nom) {
        Nombre = nom;
    }

    void set_descripcion(string desc) {
        Descripcion = desc;
    }

    void set_costo_base(float costo) {
        Costo_base = costo;
    }

    void set_estado(string est) {
        Estado = est;
    }
};

// Clase Derivada: Mantenimiento
class Mantenimiento : public Servicio {

private:

    string Tipo_mantenimiento;
    string Equipo_afectado;
    string Diagnostico;

    list<string> Repuestos_utilizados;

    float Horas_trabajadas;

public:

    Mantenimiento(string id, string nom, string desc, float costo,
                  Fecha fecha, string est,
                  string tipo_manten, string equipo,
                  string diag, float horas)

        : Servicio(id, nom, desc, costo, fecha, est) {

        Tipo_mantenimiento = tipo_manten;
        Equipo_afectado = equipo;
        Diagnostico = diag;
        Horas_trabajadas = horas;
    }


    // Getters

    string get_tipo_mantenimiento() {
        return Tipo_mantenimiento;
    }

    string get_equipo_afectado() {
        return Equipo_afectado;
    }

    string get_diagnostico() {
        return Diagnostico;
    }

    float get_horas_trabajadas() {
        return Horas_trabajadas;
    }


    
    
    // Setters
    
    void set_tipo_mantenimiento(string tipo) {
        
        Tipo_mantenimiento = tipo;
    }
    
    void set_equipo_afectado(string equipo) {

        Equipo_afectado = equipo;
    }

    void set_diagnostico(string diag) {

        Diagnostico = diag;
    }
    
    void set_horas_trabajadas(float horas) {
        
        Horas_trabajadas = horas;
    }
    
    // Mostrar repuestos

    void imprimir_repuestos() {

        for (string repuesto : Repuestos_utilizados) {
            cout << "- " << repuesto << endl;
        }
    }
    
    // Agregar un repuesto

    void agregar_repuesto(string repuesto) {

        Repuestos_utilizados.push_back(repuesto);
    }
};

// Clase Derivada: Reclamo
class Reclamo : public Servicio {
private:
    string Numero_reclamo;
    string Motivo;
    string Prioridad; // ej. Alta, Media, Baja
    string Solucion_propuesta;
    Fecha Fecha_resolucion;

public:
    Reclamo(string id, string nom, string desc, float costo, Fecha fecha, string est,
            string num_reclamo, string mot, string prior, string sol_propuesta, Fecha fecha_res)
        : Servicio(id, nom, desc, costo, fecha, est) {
        Numero_reclamo = num_reclamo;
        Motivo = mot;
        Prioridad = prior;
        Solucion_propuesta = sol_propuesta;
        Fecha_resolucion = fecha_res;
    }

    // Getters
    string get_numero_reclamo() {
        return Numero_reclamo;
    }

    string get_motivo() {
        return Motivo;
    }

    string get_prioridad() {
        return Prioridad;
    }

    string get_solucion_propuesta() {
        return Solucion_propuesta;
    }

    Fecha get_fecha_resolucion() {
        return Fecha_resolucion;
    }

    // Setters
    void set_motivo(string mot) {
        Motivo = mot;
    }

    void set_prioridad(string prior) {
        Prioridad = prior;
    }

    void set_solucion_propuesta(string sol) {
        Solucion_propuesta = sol;
    }

    void set_fecha_resolucion(Fecha fecha_res) {
        Fecha_resolucion = fecha_res;
    }
};

// Clase Derivada: Garantia
class Garantia : public Servicio {
private:
    string Codigo_garantia;
    string Numero_comprobante_origen; // ej. Factura o Boleta de compra
    Fecha Fecha_vencimiento_garantia;
    string Motivo_falla;
    bool Cobertura_aprobada;          // Aplica o no aplica la garantia

public:
    Garantia(string id, string nom, string desc, float costo, Fecha fecha, string est,
             string cod_garantia, string num_comp, Fecha fecha_venc, string mot_falla, bool cobertura)
        : Servicio(id, nom, desc, costo, fecha, est) {
        Codigo_garantia = cod_garantia;
        Numero_comprobante_origen = num_comp;
        Fecha_vencimiento_garantia = fecha_venc;
        Motivo_falla = mot_falla;
        Cobertura_aprobada = cobertura;
    }

    // Getters
    string get_codigo_garantia() {
        return Codigo_garantia;
    }

    string get_numero_comprobante_origen() {
        return Numero_comprobante_origen;
    }

    Fecha get_fecha_vencimiento_garantia() {
        return Fecha_vencimiento_garantia;
    }

    string get_motivo_falla() {
        return Motivo_falla;
    }

    bool get_cobertura_aprobada() {
        return Cobertura_aprobada;
    }

    // Setters
    void set_fecha_vencimiento_garantia(Fecha fecha_venc) {
        Fecha_vencimiento_garantia = fecha_venc;
    }

    void set_motivo_falla(string mot) {
        Motivo_falla = mot;
    }

    void set_cobertura_aprobada(bool cob) {
        Cobertura_aprobada = cob;
    }
};

int main() {
    return 0;
}