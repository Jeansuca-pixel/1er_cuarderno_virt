#pragma once
#include <list>
#include <string>
#include <vector>
#include <techstore/config.h>
#include <techstore/fecha.h>
#include <techstore/utilidades.h>

namespace techstore {
using namespace std;

class Persona { // datos comunes de clientes y empleados
private:
    string Tipo_documento, Numero_documento, Materno, Paterno, Nombre, Telefono, Direccion;
public:
    Persona(const string& tipo_doc, const string& num_doc, const string& materno, const string& paterno,
            const string& nombre, const string& telefono, const string& direccion)
        : Tipo_documento(tipo_doc), Numero_documento(num_doc), Materno(materno),
          Paterno(paterno), Nombre(nombre), Telefono(telefono), Direccion(direccion) {}
    const string& get_tipo_documento() const { return Tipo_documento; }
    const string& get_numero_documento() const { return Numero_documento; }
    const string& get_materno() const { return Materno; }
    const string& get_paterno() const { return Paterno; }
    const string& get_nombre() const { return Nombre; }
    const string& get_telefono() const { return Telefono; }
    const string& get_direccion() const { return Direccion; }
    string nombre_completo() const { return recortar(Nombre + " " + Paterno); } // nombre + apellido paterno
};

class Cliente : public Persona {
private:
    string Categoria; // General o Frecuente
public:
    Cliente(const string& tipo_doc, const string& num_doc, const string& materno, const string& paterno,
            const string& nombre, const string& telefono, const string& direccion, const string& categoria)
        : Persona(tipo_doc, num_doc, materno, paterno, nombre, telefono, direccion), Categoria(categoria) {}
    const string& get_categoria() const { return Categoria; }
    void set_categoria(const string& c) { Categoria = c; }
};

class Empleado : public Persona {
private:
    string Codigo_empleado, Cargo, Rol;
    Fecha Fecha_ingreso;
    double Sueldo;
    bool Estado; // true = activo
    string Hash_clave; // el empleado solo guarda el hash; quien lo calcula es IHasheadorClave
public:
    Empleado(const string& tipo_doc, const string& num_doc, const string& materno, const string& paterno,
             const string& nombre, const string& telefono, const string& direccion, const string& codigo_emp,
             const string& cargo, const string& rol, const Fecha& fecha_ingreso, double sueldo, bool estado,
             const string& hash)
        : Persona(tipo_doc, num_doc, materno, paterno, nombre, telefono, direccion),
          Codigo_empleado(codigo_emp), Cargo(cargo), Rol(rol), Fecha_ingreso(fecha_ingreso),
          Sueldo(sueldo), Estado(estado), Hash_clave(hash) {}
    const string& get_codigo_empleado() const { return Codigo_empleado; }
    const string& get_cargo() const { return Cargo; }
    const string& get_rol() const { return Rol; }
    const Fecha& get_fecha_ingreso() const { return Fecha_ingreso; }
    double get_sueldo() const { return Sueldo; }
    bool get_estado() const { return Estado; }
    bool es_admin() const { return Rol == Config::ROL_ADMIN; }
    const string& get_hash_clave() const { return Hash_clave; }
    void set_estado(bool e) { Estado = e; }
    void set_hash_clave(const string& h) { Hash_clave = h; }
};

class Proveedor {
private:
    string RUC, Razon_social, Contacto_ventas, Telefono, Direccion_fiscal;
    bool Estado;
public:
    Proveedor(const string& ruc, const string& razon, const string& contacto,
              const string& tel, const string& dir, bool est)
        : RUC(ruc), Razon_social(razon), Contacto_ventas(contacto),
          Telefono(tel), Direccion_fiscal(dir), Estado(est) {}
    const string& get_ruc() const { return RUC; }
    const string& get_razon_social() const { return Razon_social; }
    const string& get_contacto() const { return Contacto_ventas; }
    const string& get_telefono() const { return Telefono; }
    const string& get_direccion_fiscal() const { return Direccion_fiscal; }
    bool get_estado() const { return Estado; }
};

// ---- Productos: la clase base no sabe nada de pantalla ni de archivos ----
class Producto {
protected:
    string Codigo, Marca, Modelo;
    double Precio, Costo;
    int Stock;
    bool Activo; // false = dado de baja
public:
    Producto(const string& cod, const string& marca, const string& modelo, double precio, double costo, int stock, bool activo = true)
        : Codigo(cod), Marca(marca), Modelo(modelo), Precio(precio), Costo(costo), Stock(stock), Activo(activo) {}
    virtual ~Producto() = default; // necesario para borrar por puntero base

    virtual string tipo() const = 0;                    // nombre de la categoria
    virtual string detalle() const = 0;                 // texto de especificaciones
    virtual vector<string> atributos() const = 0;       // atributos propios (para quien necesite guardarlos)

    const string& get_codigo() const { return Codigo; }
    const string& get_marca() const { return Marca; }
    const string& get_modelo() const { return Modelo; }
    double get_precio() const { return Precio; }
    double get_costo() const { return Costo; }
    int get_stock() const { return Stock; }
    bool esta_activo() const { return Activo; }
    bool stock_bajo() const { return Stock <= Config::STOCK_MINIMO; }

    bool reducir_stock(int cant) { if (cant > 0 && cant <= Stock) { Stock -= cant; return true; } return false; } // false si no alcanza
    void aumentar_stock(int cant) { if (cant > 0) Stock += cant; }
    void set_precio(double p) { if (p > 0) Precio = p; }
    void set_costo(double c) { if (c > 0) Costo = c; }
    void dar_de_baja() { Activo = false; }
};

class Laptop : public Producto {
private:
    string Procesador;
    int Ram_GB;
    string Almacenamiento;
public:
    Laptop(const string& cod, const string& marca, const string& modelo, double precio, double costo, int stock,
           const string& cpu, int ram, const string& disco, bool activo = true)
        : Producto(cod, marca, modelo, precio, costo, stock, activo),
          Procesador(cpu), Ram_GB(ram), Almacenamiento(disco) {}
    string tipo() const override { return "LAPTOP"; }
    string detalle() const override { return Procesador + " | " + to_string(Ram_GB) + "GB RAM | " + Almacenamiento; }
    vector<string> atributos() const override { return {Procesador, to_string(Ram_GB), Almacenamiento}; }
};

class Celular : public Producto {
private:
    int Almacenamiento_GB;
    string Camara;
    int Bateria_mAh;
public:
    Celular(const string& cod, const string& marca, const string& modelo, double precio, double costo, int stock,
            int rom, const string& camara, int bat, bool activo = true)
        : Producto(cod, marca, modelo, precio, costo, stock, activo),
          Almacenamiento_GB(rom), Camara(camara), Bateria_mAh(bat) {}
    string tipo() const override { return "CELULAR"; }
    string detalle() const override { return to_string(Almacenamiento_GB) + "GB | " + Camara + " | " + to_string(Bateria_mAh) + "mAh"; }
    vector<string> atributos() const override { return {to_string(Almacenamiento_GB), Camara, to_string(Bateria_mAh)}; }
};

// ---- Transacciones ----
struct ItemVenta { string codigo, descripcion; int cantidad; double precio_unitario, costo_unitario, subtotal; }; // costo guardado para calcular utilidad
struct ItemCompra { string codigo, descripcion; int cantidad; double costo_unitario, subtotal; };

inline double subtotal_items(const list<ItemVenta>& items) { // suma del carrito
    double t = 0;
    for (const auto& i : items) t += i.subtotal;
    return redondear2(t);
}
inline double calcular_descuento(double bruto, double pct) { return redondear2(bruto * pct / 100.0); }

class Transaccion { // datos comunes de ventas y compras
protected:
    string Id_transaccion;
    Fecha Fecha_transaccion;
    double Monto_total;
    string Estado; // Completada o Anulada
public:
    Transaccion(const string& id, const Fecha& fecha, double monto, const string& est)
        : Id_transaccion(id), Fecha_transaccion(fecha), Monto_total(monto), Estado(est) {}
    const string& get_id_transaccion() const { return Id_transaccion; }
    const Fecha& get_fecha_transaccion() const { return Fecha_transaccion; }
    double get_monto_total() const { return Monto_total; }
    const string& get_estado() const { return Estado; }
    void set_estado(const string& e) { Estado = e; }
};

class Venta : public Transaccion {
private:
    Cliente cliente;      // copia del cliente al momento de la venta (dato historico)
    Empleado empleado;    // copia del cajero al momento de la venta
    list<ItemVenta> Items;
    string Tipo_Comprobante, Medio_pago;
    double Descuento_pct, Descuento_monto, Monto_pagado, Vuelto;
public:
    Venta(const string& id, const Fecha& fecha, double monto, const string& est,
          const Cliente& cli, const Empleado& emp, const list<ItemVenta>& items, const string& tipo_comp,
          const string& medio, double desc_pct, double desc_monto, double pagado, double vuelto)
        : Transaccion(id, fecha, monto, est), cliente(cli), empleado(emp), Items(items),
          Tipo_Comprobante(tipo_comp), Medio_pago(medio), Descuento_pct(desc_pct),
          Descuento_monto(desc_monto), Monto_pagado(pagado), Vuelto(vuelto) {}
    const Cliente& get_cliente() const { return cliente; }
    const Empleado& get_empleado() const { return empleado; }
    const list<ItemVenta>& get_items() const { return Items; }
    const string& get_tipo_comprobante() const { return Tipo_Comprobante; }
    const string& get_medio_pago() const { return Medio_pago; }
    double get_descuento_pct() const { return Descuento_pct; }
    double get_descuento_monto() const { return Descuento_monto; }
    double get_monto_pagado() const { return Monto_pagado; }
    double get_vuelto() const { return Vuelto; }
    double get_subtotal_bruto() const { return redondear2(Monto_total + Descuento_monto); } // total antes del descuento
    double utilidad_estimada() const { // venta sin IGV menos costo
        if (Estado != Config::ESTADO_COMPLETADA) return 0.0; // las anuladas no dan utilidad
        double costo = 0.0;
        for (const auto& i : Items) costo += i.costo_unitario * i.cantidad;
        return redondear2(Monto_total / (1.0 + Config::IGV) - costo);
    }
};

class Compra : public Transaccion {
private:
    Proveedor proveedor;
    Empleado empleado;
    list<ItemCompra> Items_comprados;
    string Tipo_Comprobante, Numero_factura_proveedor;
public:
    Compra(const string& id, const Fecha& fecha, double monto, const string& est,
           const Proveedor& prov, const Empleado& emp, const list<ItemCompra>& items,
           const string& tipo_comp, const string& num_factura)
        : Transaccion(id, fecha, monto, est), proveedor(prov), empleado(emp),
          Items_comprados(items), Tipo_Comprobante(tipo_comp), Numero_factura_proveedor(num_factura) {}
    const Proveedor& get_proveedor() const { return proveedor; }
    const Empleado& get_empleado() const { return empleado; }
    const list<ItemCompra>& get_items_comprados() const { return Items_comprados; }
    const string& get_tipo_comprobante() const { return Tipo_Comprobante; }
    const string& get_numero_factura_proveedor() const { return Numero_factura_proveedor; }
};

} // namespace techstore