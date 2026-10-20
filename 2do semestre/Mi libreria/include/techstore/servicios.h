#pragma once
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <techstore/repositorio.h>

namespace techstore {
using namespace std;

class Catalogo {
    DatosTienda& d;
    IRepositorio& repo;
public:
    Catalogo(DatosTienda& datos, IRepositorio& r) : d(datos), repo(r) {}

    Producto* buscar(const string& cod, bool solo_activos = true) { // por codigo exacto
        string c = a_mayuscula(cod);
        for (auto& p : d.inventario)
            if (p->get_codigo() == c && (!solo_activos || p->esta_activo())) return p.get();
        return nullptr;
    }
    vector<Producto*> buscar_por_texto(const string& texto) { // coincidencia parcial en marca o modelo
        vector<Producto*> r;
        string t = a_minuscula(texto);
        for (auto& p : d.inventario) {
            string nombre = a_minuscula(p->get_marca() + " " + p->get_modelo());
            if (p->esta_activo() && nombre.find(t) != string::npos) r.push_back(p.get());
        }
        return r;
    }
    vector<const Producto*> activos_de_tipo(const string& tipo) const {
        vector<const Producto*> r;
        for (const auto& p : d.inventario) if (p->esta_activo() && p->tipo() == tipo) r.push_back(p.get());
        return r;
    }
    vector<string> tipos_activos() const { // categorias que tienen productos activos
        vector<string> tipos;
        for (const auto& p : d.inventario)
            if (p->esta_activo() && find(tipos.begin(), tipos.end(), p->tipo()) == tipos.end()) tipos.push_back(p->tipo());
        return tipos;
    }
    int cantidad_stock_bajo() const {
        int n = 0;
        for (const auto& p : d.inventario) if (p->esta_activo() && p->stock_bajo()) n++;
        return n;
    }
    double valor_al_costo() const {
        double t = 0;
        for (const auto& p : d.inventario) if (p->esta_activo()) t += p->get_costo() * p->get_stock();
        return redondear2(t);
    }
    double valor_a_venta() const {
        double t = 0;
        for (const auto& p : d.inventario) if (p->esta_activo()) t += p->get_precio() * p->get_stock();
        return redondear2(t);
    }
    void cambiar_precio(Producto& p, double nuevo) { p.set_precio(nuevo); repo.guardar(d); }
    void dar_de_baja(Producto& p) { p.dar_de_baja(); repo.guardar(d); }
};

// ---- Generador de codigos y numeros correlativos ----
class GeneradorCodigos {
    DatosTienda& d;
    const FabricaProductos& fabrica;
    int siguiente(const string& clave) { int& v = d.contadores[clave]; if (v < 1) v = 1; return v++; } // devuelve el numero y avanza el contador
public:
    GeneradorCodigos(DatosTienda& datos, const FabricaProductos& f) : d(datos), fabrica(f) {}

    string codigo_producto(const string& tipo) { // ej. L04, C12
        const ICreadorProducto* c = fabrica.buscar(tipo);
        char prefijo = c ? c->prefijo() : 'P';
        ostringstream o;
        o << prefijo << setfill('0') << setw(2) << siguiente(tipo);
        return o.str();
    }
    string numero_comprobante(const ITipoComprobante& tc) { return tc.serie() + to_string(1000 + siguiente(tc.nombre())); } // ej. B001-1001
    string codigo_empleado() const { // ej. EMP03: mayor existente + 1
        int mayor = 0;
        for (const auto& e : d.empleados) {
            try { mayor = max(mayor, stoi(e.get_codigo_empleado().substr(3))); } catch (...) {}
        }
        ostringstream o;
        o << "EMP" << setfill('0') << setw(2) << mayor + 1;
        return o.str();
    }
};

// ---- Gestion de empleados ----
class GestorEmpleados {
    DatosTienda& d;
    IRepositorio& repo;
public:
    GestorEmpleados(DatosTienda& datos, IRepositorio& r) : d(datos), repo(r) {}
    const list<Empleado>& todos() const { return d.empleados; }
    Empleado* buscar(const string& codigo) {
        string c = a_mayuscula(codigo);
        for (auto& e : d.empleados) if (e.get_codigo_empleado() == c) return &e;
        return nullptr;
    }
    bool existe_documento(const string& doc) const {
        for (const auto& e : d.empleados) if (e.get_numero_documento() == doc) return true;
        return false;
    }
    int administradores_activos() const {
        int n = 0;
        for (const auto& e : d.empleados) if (e.get_estado() && e.es_admin()) n++;
        return n;
    }
    void agregar(const Empleado& e) { d.empleados.push_back(e); repo.guardar(d); }
    void cambiar_hash_clave(Empleado& e, const string& hash) { e.set_hash_clave(hash); repo.guardar(d); }
    // Devuelve "" si todo salio bien, o el motivo del rechazo.
    string alternar_estado(const string& codigo, const string& codigo_sesion) { // activa o desactiva
        Empleado* e = buscar(codigo);
        if (e == nullptr) return "Empleado no encontrado.";
        if (e->get_estado() && e->get_codigo_empleado() == codigo_sesion) return "No puedes desactivar tu propio usuario.";
        if (e->get_estado() && e->es_admin() && administradores_activos() <= 1) return "No se puede desactivar al ultimo administrador.";
        e->set_estado(!e->get_estado());
        repo.guardar(d);
        return "";
    }
};

// ---- Gestion de clientes ----
class GestorClientes {
    DatosTienda& d;
    IRepositorio& repo;
public:
    GestorClientes(DatosTienda& datos, IRepositorio& r) : d(datos), repo(r) {}
    Cliente* buscar(const string& doc) {
        for (auto& c : d.clientes) if (c.get_numero_documento() == doc) return &c;
        return nullptr;
    }
    Cliente* registrar(const Cliente& c) { d.clientes.push_back(c); repo.guardar(d); return &d.clientes.back(); }
};

// ---- Gestion de proveedores ----
class GestorProveedores {
    DatosTienda& d;
    IRepositorio& repo;
public:
    GestorProveedores(DatosTienda& datos, IRepositorio& r) : d(datos), repo(r) {}
    const list<Proveedor>& todos() const { return d.proveedores; }
    Proveedor* buscar(const string& ruc) {
        for (auto& p : d.proveedores) if (p.get_ruc() == ruc) return &p;
        return nullptr;
    }
    Proveedor* por_indice(int indice) {   // posicion desde 1
        if (indice < 1 || indice > (int)d.proveedores.size()) return nullptr;
        auto it = d.proveedores.begin();
        advance(it, indice - 1);
        return &(*it);
    }
    Proveedor* agregar(const Proveedor& p) { d.proveedores.push_back(p); repo.guardar(d); return &d.proveedores.back(); }
};

// ---- Autenticacion ----
class ServicioAutenticacion {
    const IHasheadorClave& hasheador;
public:
    explicit ServicioAutenticacion(const IHasheadorClave& h) : hasheador(h) {}
    bool clave_correcta(const Empleado& e, const string& clave) const { return e.get_hash_clave() == hasheador.hash(clave); }
    string hash(const string& clave) const { return hasheador.hash(clave); }
};

// ---- Ventas ----
class ServicioVentas {
    DatosTienda& d;
    IRepositorio& repo;
    Catalogo& catalogo;
    GeneradorCodigos& generador;
    const Registro<ITipoComprobante>& comprobantes;

    void actualizar_categoria_cliente(const string& doc) { // sube a Frecuente al llegar a N compras
        Cliente* c = nullptr;
        for (auto& x : d.clientes) if (x.get_numero_documento() == doc) { c = &x; break; }
        if (c == nullptr || c->get_categoria() != Config::CAT_GENERAL) return;
        int n = 0;
        for (const auto& v : d.ventas)
            if (v.get_estado() == Config::ESTADO_COMPLETADA && v.get_cliente().get_numero_documento() == doc) n++;
        if (n >= Config::COMPRAS_PARA_FRECUENTE) c->set_categoria(Config::CAT_FRECUENTE);
    }
public:
    ServicioVentas(DatosTienda& datos, IRepositorio& r, Catalogo& cat, GeneradorCodigos& gen, const Registro<ITipoComprobante>& comp)
        : d(datos), repo(r), catalogo(cat), generador(gen), comprobantes(comp) {}

    const list<Venta>& historial() const { return d.ventas; }

    // Registra la venta y descuenta stock. Devuelve nullptr si el stock no alcanza.
    const Venta* finalizar(const string& tipo_comp, const Cliente& cli, const Empleado& emp, const list<ItemVenta>& carrito,
                           const string& medio, double desc_pct, double monto_pagado, const Fecha& fecha) {
        const ITipoComprobante* tc = comprobantes.buscar([&](const ITipoComprobante& t) { return t.nombre() == tipo_comp; });
        if (tc == nullptr) return nullptr;
        for (const auto& it : carrito) {                       // primero valida todo el stock
            Producto* p = catalogo.buscar(it.codigo);
            if (p == nullptr || p->get_stock() < it.cantidad) return nullptr;
        }
        for (const auto& it : carrito) catalogo.buscar(it.codigo)->reducir_stock(it.cantidad); // recien ahora descuenta
        double bruto = subtotal_items(carrito);
        double desc = calcular_descuento(bruto, desc_pct);
        double total = redondear2(bruto - desc);
        double vuelto = (monto_pagado > total) ? redondear2(monto_pagado - total) : 0.0;
        string id = generador.numero_comprobante(*tc);
        d.ventas.push_back(Venta(id, fecha, total, Config::ESTADO_COMPLETADA, cli, emp, carrito, tipo_comp, medio, desc_pct, desc, monto_pagado, vuelto));
        actualizar_categoria_cliente(cli.get_numero_documento());
        repo.guardar(d);
        return &d.ventas.back();
    }

    // Devuelve "" si se anulo, o el mensaje de error.
    string anular(const string& id) { // marca la venta como anulada y devuelve el stock
        string idm = a_mayuscula(id);
        for (auto& v : d.ventas) {
            if (v.get_id_transaccion() != idm) continue;
            if (v.get_estado() == Config::ESTADO_ANULADA) return "La venta ya estaba anulada.";
            for (const auto& it : v.get_items()) {
                Producto* p = catalogo.buscar(it.codigo, false); // incluye productos dados de baja
                if (p != nullptr) p->aumentar_stock(it.cantidad);
            }
            v.set_estado(Config::ESTADO_ANULADA);
            repo.guardar(d);
            return "";
        }
        return "No existe una venta con ese numero.";
    }
};

// ---- Compras ----
class ServicioCompras {
    DatosTienda& d;
    IRepositorio& repo;
    Catalogo& catalogo;
public:
    ServicioCompras(DatosTienda& datos, IRepositorio& r, Catalogo& cat) : d(datos), repo(r), catalogo(cat) {}
    const list<Compra>& historial() const { return d.compras; }

    // Incorpora productos nuevos, suma stock, actualiza costos y guarda la compra.
    const Compra* registrar(const Proveedor& prov, const Empleado& emp, const list<ItemCompra>& items,
                            vector<unique_ptr<Producto>>& nuevos, const string& num_factura, const Fecha& fecha) {
        for (auto& n : nuevos) d.inventario.push_back(move(n)); // pasa los productos nuevos al inventario
        nuevos.clear();
        double total = 0.0;
        for (const auto& it : items) {
            Producto* p = catalogo.buscar(it.codigo, false);
            if (p != nullptr) { p->aumentar_stock(it.cantidad); p->set_costo(it.costo_unitario); }
            total += it.subtotal;
        }
        string id = "CP001-" + to_string(2000 + d.compras.size() + 1); // numero interno de compra
        d.compras.push_back(Compra(id, fecha, redondear2(total), Config::ESTADO_COMPLETADA, prov, emp, items, "Factura Proveedor", num_factura));
        repo.guardar(d);
        return &d.compras.back();
    }
};

// ---- Reportes (solo lectura) ----
class ServicioReportes {
    const DatosTienda& d;
public:
    explicit ServicioReportes(const DatosTienda& datos) : d(datos) {}

    vector<const Venta*> ventas_en_rango(const Fecha& desde, const Fecha& hasta) const { // solo ventas no anuladas
        vector<const Venta*> r;
        for (const auto& v : d.ventas) {
            bool vigente = v.get_estado() == Config::ESTADO_COMPLETADA;
            if (vigente && v.get_fecha_transaccion() >= desde && v.get_fecha_transaccion() <= hasta) r.push_back(&v);
        }
        return r;
    }
    vector<pair<string, int>> ranking_productos() const { // unidades vendidas, de mayor a menor
        map<string, int> acumulado;
        for (const auto& v : d.ventas) {
            if (v.get_estado() != Config::ESTADO_COMPLETADA) continue;
            for (const auto& i : v.get_items()) acumulado["[" + i.codigo + "] " + i.descripcion] += i.cantidad;
        }
        vector<pair<string, int>> r(acumulado.begin(), acumulado.end());
        sort(r.begin(), r.end(), [](const pair<string, int>& a, const pair<string, int>& b) { return a.second > b.second; });
        return r;
    }
    map<string, pair<int, double>> ventas_por_cajero() const { // cajero -> (cantidad, monto)
        map<string, pair<int, double>> r;
        for (const auto& v : d.ventas) {
            if (v.get_estado() != Config::ESTADO_COMPLETADA) continue;
            string clave = v.get_empleado().get_codigo_empleado() + " - " + v.get_empleado().nombre_completo();
            r[clave].first += 1;
            r[clave].second += v.get_monto_total();
        }
        return r;
    }
    double utilidad_en_rango(const Fecha& desde, const Fecha& hasta) const {
        double t = 0;
        for (const Venta* v : ventas_en_rango(desde, hasta)) t += v->utilidad_estimada();
        return redondear2(t);
    }
};


} // namespace techstore