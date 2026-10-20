#pragma once
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <techstore/dominio.h>

namespace techstore {
using namespace std;

template <class T>
class Registro {
    vector<unique_ptr<T>> items;
public:
    void agregar(unique_ptr<T> p) { items.push_back(move(p)); }
    const vector<unique_ptr<T>>& todos() const { return items; }
    template <class F> const T* buscar(F pred) const { // primer elemento que cumple la condicion
        for (const auto& i : items) if (pred(*i)) return i.get();
        return nullptr;
    }
};

// ---- Hash de claves ----
class IHasheadorClave {
public:
    virtual ~IHasheadorClave() = default;
    virtual string hash(const string& clave) const = 0;
};
// OJO: std::hash NO es criptografico. Para un sistema real cambia esta clase por bcrypt/argon2;
// como el resto depende de IHasheadorClave, no hay que tocar nada mas.
class HasheadorStd : public IHasheadorClave {
public:
    string hash(const string& clave) const override { return to_string(std::hash<string>{}(clave)); }
};

// ---- Tipos de comprobante ----
class ITipoComprobante {
public:
    virtual ~ITipoComprobante() = default;
    virtual string nombre() const = 0;            // BOLETA, FACTURA...
    virtual string serie() const = 0;             // prefijo del numero
    virtual string tipo_documento() const = 0;    // DNI o RUC
    virtual bool pide_apellidos() const = 0;      // personas naturales si, empresas no
    virtual string etiqueta_nombre() const = 0;   // "Nombre(s): " o "Razon Social: "
};
class Boleta : public ITipoComprobante {
public:
    string nombre() const override { return "BOLETA"; }
    string serie() const override { return "B001-"; }
    string tipo_documento() const override { return "DNI"; }
    bool pide_apellidos() const override { return true; }
    string etiqueta_nombre() const override { return "Nombre(s): "; }
};
class Factura : public ITipoComprobante {
public:
    string nombre() const override { return "FACTURA"; }
    string serie() const override { return "F001-"; }
    string tipo_documento() const override { return "RUC"; }
    bool pide_apellidos() const override { return false; }
    string etiqueta_nombre() const override { return "Razon Social: "; }
};

// ---- Medios de pago ----
class IMedioPago {
public:
    virtual ~IMedioPago() = default;
    virtual string nombre() const = 0;
    virtual bool es_efectivo() const = 0;   // si pide monto recibido y calcula vuelto
};
class MedioPagoSimple : public IMedioPago {
    string nom; bool efectivo;
public:
    MedioPagoSimple(const string& n, bool ef) : nom(n), efectivo(ef) {}
    string nombre() const override { return nom; }
    bool es_efectivo() const override { return efectivo; }
};

// ---- Politica de descuentos ----
class IPoliticaDescuento {
public:
    virtual ~IPoliticaDescuento() = default;
    virtual bool tiene_descuento_automatico(const Cliente& c) const = 0;
    virtual double descuento_automatico() const = 0;
    virtual double descuento_manual_maximo() const = 0;
};
class PoliticaDescuentoTienda : public IPoliticaDescuento {
public:
    bool tiene_descuento_automatico(const Cliente& c) const override { return c.get_categoria() == Config::CAT_FRECUENTE; }
    double descuento_automatico() const override { return Config::DESCUENTO_FRECUENTE; }
    double descuento_manual_maximo() const override { return Config::DESCUENTO_MAXIMO; }
};

// ---- Fabrica de productos: crear un tipo nuevo = registrar un creador ----
struct DatosBaseProducto { // campos comunes a todo producto
    string codigo, marca, modelo;
    double precio = 0, costo = 0;
    int stock = 0;
    bool activo = true;
};
class ICreadorProducto {
public:
    virtual ~ICreadorProducto() = default;
    virtual string tipo() const = 0;
    virtual char prefijo() const = 0; // letra inicial del codigo (L01, C02...)
    virtual unique_ptr<Producto> crear(const DatosBaseProducto& b, const vector<string>& a) const = 0; // a = atributos propios
};
class CreadorLaptop : public ICreadorProducto {
public:
    string tipo() const override { return "LAPTOP"; }
    char prefijo() const override { return 'L'; }
    unique_ptr<Producto> crear(const DatosBaseProducto& b, const vector<string>& a) const override {
        if (a.size() != 3) return nullptr; // registro incompleto
        return make_unique<Laptop>(b.codigo, b.marca, b.modelo, b.precio, b.costo, b.stock, a[0], stoi(a[1]), a[2], b.activo);
    }
};
class CreadorCelular : public ICreadorProducto {
public:
    string tipo() const override { return "CELULAR"; }
    char prefijo() const override { return 'C'; }
    unique_ptr<Producto> crear(const DatosBaseProducto& b, const vector<string>& a) const override {
        if (a.size() != 3) return nullptr;
        return make_unique<Celular>(b.codigo, b.marca, b.modelo, b.precio, b.costo, b.stock, stoi(a[0]), a[1], stoi(a[2]), b.activo);
    }
};
class FabricaProductos {
    Registro<ICreadorProducto> creadores;
public:
    void agregar(unique_ptr<ICreadorProducto> c) { creadores.agregar(move(c)); }
    const ICreadorProducto* buscar(const string& tipo) const { // creador segun el tipo, o nullptr
        return creadores.buscar([&](const ICreadorProducto& c) { return c.tipo() == tipo; });
    }
};


} // namespace techstore