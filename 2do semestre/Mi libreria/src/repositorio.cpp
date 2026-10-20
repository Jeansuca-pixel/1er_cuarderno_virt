#include <techstore/repositorio.h>
#include <fstream>
#include <iostream>
#include <functional>

namespace techstore {

namespace {   // funciones auxiliares privadas de este archivo

string lc(string s) {
    for (char& c : s) if (c == '|' || c == '~' || c == '^') c = ' ';
    return s;
}
string b(bool v) { return v ? "1" : "0"; }

bool escribir_archivo(const string& carpeta, const string& nombre, const function<void(ostream&)>& escritor) {
    ofstream archivo(carpeta + nombre);
    if (!archivo.is_open()) { cerr << "No se pudo escribir el archivo " << nombre << endl; return false; }
    escritor(archivo);
    return true;
}

bool leer_registros(const string& carpeta, const string& nombre, vector<vector<string>>& registros) {
    ifstream archivo(carpeta + nombre);
    if (!archivo.is_open()) return false;
    string linea;
    while (getline(archivo, linea)) {
        linea = recortar(linea);
        if (!linea.empty()) registros.push_back(dividir(linea, '|'));
    }
    return true;
}

string ser(const Persona& p) {
    return lc(p.get_tipo_documento()) + "|" + lc(p.get_numero_documento()) + "|" + lc(p.get_materno()) + "|" +
           lc(p.get_paterno()) + "|" + lc(p.get_nombre()) + "|" + lc(p.get_telefono()) + "|" + lc(p.get_direccion());
}
string ser(const Cliente& c) { return ser(static_cast<const Persona&>(c)) + "|" + lc(c.get_categoria()); }
string ser(const Empleado& e) {
    return ser(static_cast<const Persona&>(e)) + "|" + lc(e.get_codigo_empleado()) + "|" + lc(e.get_cargo()) + "|" +
           lc(e.get_rol()) + "|" + e.get_fecha_ingreso().texto() + "|" + to_string(e.get_sueldo()) + "|" +
           b(e.get_estado()) + "|" + e.get_hash_clave();
}
string ser(const Proveedor& p) {
    return lc(p.get_ruc()) + "|" + lc(p.get_razon_social()) + "|" + lc(p.get_contacto()) + "|" +
           lc(p.get_telefono()) + "|" + lc(p.get_direccion_fiscal()) + "|" + b(p.get_estado());
}
string ser(const Producto& p) {
    string s = p.tipo() + "|" + p.get_codigo() + "|" + lc(p.get_marca()) + "|" + lc(p.get_modelo()) + "|" +
               to_string(p.get_precio()) + "|" + to_string(p.get_costo()) + "|" + to_string(p.get_stock()) + "|" + b(p.esta_activo());
    for (const string& a : p.atributos()) s += "|" + lc(a);
    return s;
}
string ser(const Venta& v) {
    string items;
    for (const auto& i : v.get_items()) {
        if (!items.empty()) items += "~";
        items += lc(i.codigo) + "^" + lc(i.descripcion) + "^" + to_string(i.cantidad) + "^" +
                 to_string(i.precio_unitario) + "^" + to_string(i.costo_unitario) + "^" + to_string(i.subtotal);
    }
    return v.get_id_transaccion() + "|" + v.get_fecha_transaccion().texto() + "|" + to_string(v.get_monto_total()) + "|" +
           v.get_estado() + "|" + v.get_cliente().get_numero_documento() + "|" + v.get_empleado().get_codigo_empleado() + "|" +
           v.get_tipo_comprobante() + "|" + lc(v.get_medio_pago()) + "|" + to_string(v.get_descuento_pct()) + "|" +
           to_string(v.get_descuento_monto()) + "|" + to_string(v.get_monto_pagado()) + "|" + to_string(v.get_vuelto()) + "|" + items;
}
string ser(const Compra& c) {
    string items;
    for (const auto& i : c.get_items_comprados()) {
        if (!items.empty()) items += "~";
        items += lc(i.codigo) + "^" + lc(i.descripcion) + "^" + to_string(i.cantidad) + "^" +
                 to_string(i.costo_unitario) + "^" + to_string(i.subtotal);
    }
    return c.get_id_transaccion() + "|" + c.get_fecha_transaccion().texto() + "|" + to_string(c.get_monto_total()) + "|" +
           c.get_estado() + "|" + c.get_proveedor().get_ruc() + "|" + c.get_empleado().get_codigo_empleado() + "|" +
           lc(c.get_tipo_comprobante()) + "|" + lc(c.get_numero_factura_proveedor()) + "|" + items;
}

} // fin del namespace anonimo

RepositorioCSV::RepositorioCSV(const FabricaProductos& f, const string& carpeta_datos)
    : fabrica(f), carpeta(carpeta_datos) {}

bool RepositorioCSV::guardar(const DatosTienda& d) {
    bool ok = true;
    ok = escribir_archivo(carpeta, "datos_productos.csv", [&](ostream& o) { for (const auto& p : d.inventario) o << ser(*p) << "\n"; }) && ok;
    ok = escribir_archivo(carpeta, "datos_empleados.csv", [&](ostream& o) { for (const auto& e : d.empleados) o << ser(e) << "\n"; }) && ok;
    ok = escribir_archivo(carpeta, "datos_proveedores.csv", [&](ostream& o) { for (const auto& p : d.proveedores) o << ser(p) << "\n"; }) && ok;
    ok = escribir_archivo(carpeta, "datos_clientes.csv", [&](ostream& o) { for (const auto& c : d.clientes) o << ser(c) << "\n"; }) && ok;
    ok = escribir_archivo(carpeta, "datos_ventas.csv", [&](ostream& o) { for (const auto& v : d.ventas) o << ser(v) << "\n"; }) && ok;
    ok = escribir_archivo(carpeta, "datos_compras.csv", [&](ostream& o) { for (const auto& c : d.compras) o << ser(c) << "\n"; }) && ok;
    ok = escribir_archivo(carpeta, "datos_config.csv", [&](ostream& o) { for (const auto& kv : d.contadores) o << kv.first << "|" << kv.second << "\n"; }) && ok;
    return ok;
}

bool RepositorioCSV::cargar(DatosTienda& d) {
    vector<vector<string>> r;

    if (leer_registros(carpeta, "datos_empleados.csv", r)) {
        for (const auto& c : r) {
            try {
                Fecha f;
                if (c.size() != 14 || !Fecha::desde_texto(c[10], f)) continue;
                d.empleados.push_back(Empleado(c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8], c[9], f, stod(c[11]), c[12] == "1", c[13]));
            } catch (...) {}
        }
    }
    r.clear();
    if (leer_registros(carpeta, "datos_proveedores.csv", r))
        for (const auto& c : r) if (c.size() == 6) d.proveedores.push_back(Proveedor(c[0], c[1], c[2], c[3], c[4], c[5] == "1"));
    r.clear();
    if (leer_registros(carpeta, "datos_clientes.csv", r))
        for (const auto& c : r) if (c.size() == 8) d.clientes.push_back(Cliente(c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7]));

    r.clear();
    if (leer_registros(carpeta, "datos_productos.csv", r)) {
        for (const auto& c : r) {
            try {
                if (c.size() < 9) continue;
                const ICreadorProducto* creador = fabrica.buscar(c[0]);
                if (creador == nullptr) continue;
                DatosBaseProducto base;
                base.codigo = c[1]; base.marca = c[2]; base.modelo = c[3];
                base.precio = stod(c[4]); base.costo = stod(c[5]); base.stock = stoi(c[6]); base.activo = (c[7] == "1");
                unique_ptr<Producto> p = creador->crear(base, vector<string>(c.begin() + 8, c.end()));
                if (!p) continue;
                d.inventario.push_back(move(p));
                int siguiente = stoi(c[1].substr(1)) + 1;
                d.contadores[c[0]] = max(d.contadores[c[0]], siguiente);
            } catch (...) {}
        }
    }
    r.clear();
    if (leer_registros(carpeta, "datos_config.csv", r)) {
        for (const auto& c : r) {
            try {
                if (c.size() != 2) continue;
                string clave = a_mayuscula(c[0]);
                int v = stoi(c[1]);
                bool es_serie = (clave == "BOLETA" || clave == "FACTURA");
                d.contadores[clave] = es_serie ? v : max(d.contadores[clave], v);
            } catch (...) {}
        }
    }
    r.clear();
    if (leer_registros(carpeta, "datos_ventas.csv", r)) {
        for (const auto& c : r) {
            try {
                Fecha f;
                if (c.size() != 13 || !Fecha::desde_texto(c[1], f)) continue;
                const Empleado* emp = nullptr;
                for (const auto& e : d.empleados) if (e.get_codigo_empleado() == c[5]) { emp = &e; break; }
                if (emp == nullptr) continue;
                Cliente cli("DNI", c[4], "", "", "DESCONOCIDO", "", "", Config::CAT_GENERAL);
                for (const auto& x : d.clientes) if (x.get_numero_documento() == c[4]) { cli = x; break; }
                list<ItemVenta> items;
                for (const string& it : dividir(c[12], '~')) {
                    vector<string> p = dividir(it, '^');
                    if (p.size() == 6) items.push_back(ItemVenta{p[0], p[1], stoi(p[2]), stod(p[3]), stod(p[4]), stod(p[5])});
                }
                d.ventas.push_back(Venta(c[0], f, stod(c[2]), c[3], cli, *emp, items, c[6], c[7], stod(c[8]), stod(c[9]), stod(c[10]), stod(c[11])));
            } catch (...) {}
        }
    }
    r.clear();
    if (leer_registros(carpeta, "datos_compras.csv", r)) {
        for (const auto& c : r) {
            try {
                Fecha f;
                if (c.size() != 9 || !Fecha::desde_texto(c[1], f)) continue;
                const Empleado* emp = nullptr;
                const Proveedor* prov = nullptr;
                for (const auto& e : d.empleados) if (e.get_codigo_empleado() == c[5]) { emp = &e; break; }
                for (const auto& p : d.proveedores) if (p.get_ruc() == c[4]) { prov = &p; break; }
                if (emp == nullptr || prov == nullptr) continue;
                list<ItemCompra> items;
                for (const string& it : dividir(c[8], '~')) {
                    vector<string> p = dividir(it, '^');
                    if (p.size() == 5) items.push_back(ItemCompra{p[0], p[1], stoi(p[2]), stod(p[3]), stod(p[4])});
                }
                d.compras.push_back(Compra(c[0], f, stod(c[2]), c[3], *prov, *emp, items, c[6], c[7]));
            } catch (...) {}
        }
    }
    return !d.inventario.empty() && !d.empleados.empty();
}

} // namespace techstore