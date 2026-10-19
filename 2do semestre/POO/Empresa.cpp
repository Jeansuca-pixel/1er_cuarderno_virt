// ============================================================================
//  TECH STORE - version refactorizada con principios SOLID
//
//  Capas del programa (de adentro hacia afuera):
//    1. Utilidades de consola y validacion
//    2. Dominio: Fecha, Persona, Cliente, Empleado, Proveedor, Producto, Venta, Compra
//    3. Abstracciones (interfaces): IRepositorio, IHasheadorClave, ITipoComprobante,
//       IMedioPago, IPoliticaDescuento, ICreadorProducto, IFormularioProducto
//    4. Persistencia: RepositorioCSV (implementa IRepositorio)
//    5. Servicios de negocio: Catalogo, Gestores, ServicioVentas, ServicioCompras...
//    6. Interfaz de consola: Vistas y Controladores
//    7. main(): raiz de composicion (aqui se arman y se conectan todas las piezas)
// ============================================================================
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <memory>
#include <fstream>
#include <functional>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <ctime>
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX // evita conflicto con min/max de windows.h
#endif
#include <windows.h>
#else
#include <unistd.h>
#endif
using namespace std;

// ============================================================================
//  1. CONFIGURACION Y UTILIDADES
// ============================================================================
namespace Config {
    const double IGV = 0.18;
    const int STOCK_MINIMO = 3; // a este valor o menos se alerta stock bajo
    const double DESCUENTO_FRECUENTE = 5.0; // porcentaje automatico
    const double DESCUENTO_MAXIMO = 30.0; // tope del descuento manual
    const int COMPRAS_PARA_FRECUENTE = 3;
    const string RUC_EMPRESA = "20601234567";
    const string NOMBRE_EMPRESA = "TECH STORE S.A.C.";
    const string DIRECCION_EMPRESA = "Av. Floral 450 - Puno / Cel: 951-000-111";
    const string ROL_ADMIN = "ADMIN";
    const string ROL_CAJERO = "CAJERO";
    const string ESTADO_COMPLETADA = "Completada";
    const string ESTADO_ANULADA = "Anulada";
    const string CAT_GENERAL = "General";
    const string CAT_FRECUENTE = "Frecuente";
}

namespace Color { // codigos ANSI para colorear la consola
    const string ROJO = "\033[31m";
    const string VERDE = "\033[32m";
    const string AMARILLO = "\033[33m";
    const string CIAN = "\033[36m";
    const string RESET = "\033[0m";
}

// Excepcion para cerrar el programa de forma ordenada (reemplaza a exit()).
struct SalirPrograma { string mensaje; };

inline void preparar_consola() { // activa colores ANSI en Windows
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo = 0;
    if (GetConsoleMode(h, &modo)) SetConsoleMode(h, modo | 0x0004);
#endif
}

inline void esperar(int ms) { // pausa en milisegundos
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

inline void limpiar() { cout << "\033[2J\033[H" << flush; } // borra la pantalla

inline void escribir(const string& texto, int ms = 15) { // efecto maquina de escribir
    for (char c : texto) { cout << c << flush; esperar(ms); }
}

inline void cargando(const string& mensaje) { // barra de carga animada
    cout << mensaje << endl << "[";
    for (int i = 0; i < 20; i++) { cout << "#" << flush; esperar(35); }
    cout << "] 100%" << endl;
    esperar(200);
}

inline void puntos(const string& mensaje) { // mensaje con tres puntos animados
    cout << mensaje;
    for (int i = 0; i < 3; i++) { cout << "." << flush; esperar(200); }
    cout << endl;
}

inline string recortar(const string& s) { // quita espacios al inicio y final
    size_t i = s.find_first_not_of(" \t\r\n");
    if (i == string::npos) return "";
    size_t j = s.find_last_not_of(" \t\r\n");
    return s.substr(i, j - i + 1);
}

inline string a_minuscula(string s) { for (char& c : s) c = (char)tolower((unsigned char)c); return s; }
inline string a_mayuscula(string s) { for (char& c : s) c = (char)toupper((unsigned char)c); return s; }
inline double redondear2(double v) { return round(v * 100.0) / 100.0; }

inline string dinero(double v) { // formato "S/ 12.50"
    ostringstream o;
    o << "S/ " << fixed << setprecision(2) << v;
    return o.str();
}

inline vector<string> dividir(const string& texto, char sep) { // separa un texto por un caracter
    vector<string> partes;
    string actual;
    for (char c : texto) {
        if (c == sep) { partes.push_back(actual); actual.clear(); }
        else actual += c;
    }
    partes.push_back(actual);
    return partes;
}

inline string centrar(const string& t, size_t ancho) { // centra el texto con espacios a la izquierda
    if (t.size() >= ancho) return t;
    return string((ancho - t.size()) / 2, ' ') + t;
}

// ---- Lectura de datos por consola ----
inline string leer_texto(const string& mensaje) {
    cout << mensaje;
    string s;
    if (!getline(cin, s)) throw SalirPrograma{"\nEntrada cerrada. Saliendo del programa."}; // fin de entrada (Ctrl+D)
    return recortar(s);
}

inline string leer_texto_no_vacio(const string& mensaje) {
    while (true) {
        string s = leer_texto(mensaje);
        if (!s.empty()) return s;
        cout << Color::ROJO << "Este campo no puede estar vacio." << Color::RESET << endl;
    }
}

inline double leer_numero(const string& mensaje) {
    while (true) {
        string s = leer_texto(mensaje);
        try {
            size_t pos = 0;
            double v = stod(s, &pos);
            if (pos == s.size() && isfinite(v)) return v; // acepta solo si todo el texto es numero
        } catch (...) {}
        cout << Color::ROJO << "Numero invalido, intenta de nuevo." << Color::RESET << endl;
    }
}

inline double leer_numero_rango(const string& mensaje, double minimo, double maximo) {
    while (true) {
        double v = leer_numero(mensaje);
        if (v >= minimo && v <= maximo) return v;
        cout << Color::ROJO << "Ingresa un valor entre " << minimo << " y " << maximo << "." << Color::RESET << endl;
    }
}

inline double leer_numero_positivo(const string& mensaje) {
    while (true) {
        double v = leer_numero(mensaje);
        if (v > 0 && v <= 10000000) return v; // tope para evitar valores absurdos
        cout << Color::ROJO << "El valor debe ser mayor que cero." << Color::RESET << endl;
    }
}

inline int leer_entero_rango(const string& mensaje, int minimo, int maximo) {
    while (true) {
        double v = leer_numero(mensaje);
        if (v == floor(v) && v >= minimo && v <= maximo) return (int)v; // solo enteros
        cout << Color::ROJO << "Ingresa un entero entre " << minimo << " y " << maximo << "." << Color::RESET << endl;
    }
}

inline int leer_entero_positivo(const string& mensaje) { return leer_entero_rango(mensaje, 1, 100000); }

inline bool confirmar(const string& mensaje) {
    string r = a_minuscula(leer_texto(mensaje + " (s/n): "));
    return r == "s" || r == "si";
}

inline void pausa() { leer_texto("\nPresiona Enter para continuar..."); }

inline bool solo_digitos(const string& s) {
    return !s.empty() && all_of(s.begin(), s.end(), [](unsigned char c) { return isdigit(c) != 0; });
}
inline bool dni_valido(const string& s) { return s.size() == 8 && solo_digitos(s); }
inline bool ruc_valido(const string& s) { // 11 digitos que empiezan con 10 o 20
    return s.size() == 11 && solo_digitos(s) && (s.substr(0, 2) == "10" || s.substr(0, 2) == "20");
}

inline string leer_documento(const string& tipo) { // pide un DNI o RUC hasta que sea valido
    while (true) {
        string d = leer_texto("Numero de " + tipo + ": ");
        if (tipo == "RUC" ? ruc_valido(d) : dni_valido(d)) return d;
        if (tipo == "RUC") cout << Color::ROJO << "El RUC debe tener 11 digitos y empezar con 10 o 20." << Color::RESET << endl;
        else cout << Color::ROJO << "El DNI debe tener 8 digitos." << Color::RESET << endl;
    }
}

// ============================================================================
//  2. DOMINIO
// ============================================================================
class Fecha {
private:
    int dia, mes, anio;
public:
    Fecha(int d, int m, int a) : dia(d), mes(m), anio(a) {}
    Fecha() : dia(1), mes(1), anio(2000) { *this = hoy(); } // por defecto: fecha actual

    static Fecha hoy() {
        time_t t = time(nullptr);
        tm info{};
#ifdef _WIN32
        tm* local = localtime(&t);
        if (local != nullptr) info = *local;
#else
        localtime_r(&t, &info); // version segura para hilos
#endif
        return Fecha(info.tm_mday, info.tm_mon + 1, info.tm_year + 1900);
    }
    static bool es_bisiesto(int a) { return (a % 4 == 0 && a % 100 != 0) || a % 400 == 0; }
    static int dias_del_mes(int m, int a) {
        static const int dias[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (m == 2 && es_bisiesto(a)) return 29;
        return dias[m - 1];
    }
    static bool es_valida(int d, int m, int a) {
        if (a < 2000 || a > 2100) return false; // rango de anios permitido
        if (m < 1 || m > 12) return false;
        return d >= 1 && d <= dias_del_mes(m, a);
    }
    static bool desde_texto(const string& s, Fecha& resultado) { // convierte "dd/mm/aaaa"
        vector<string> p = dividir(s, '/');
        if (p.size() != 3) return false;
        if (!solo_digitos(p[0]) || !solo_digitos(p[1]) || !solo_digitos(p[2])) return false;
        if (p[0].size() > 2 || p[1].size() > 2 || p[2].size() != 4) return false;
        int d = stoi(p[0]), m = stoi(p[1]), a = stoi(p[2]);
        if (!es_valida(d, m, a)) return false;
        resultado = Fecha(d, m, a);
        return true;
    }
    int clave() const { return anio * 10000 + mes * 100 + dia; } // numero comparable (aaaammdd)
    bool operator<(const Fecha& o) const { return clave() < o.clave(); }
    bool operator<=(const Fecha& o) const { return clave() <= o.clave(); }
    bool operator>=(const Fecha& o) const { return clave() >= o.clave(); }
    bool operator==(const Fecha& o) const { return clave() == o.clave(); }
    string texto() const { // devuelve "dd/mm/aaaa"
        ostringstream o;
        o << setfill('0') << setw(2) << dia << "/" << setw(2) << mes << "/" << setw(4) << anio;
        return o.str();
    }
};

inline Fecha leer_fecha(const string& mensaje) {
    while (true) {
        string s = leer_texto(mensaje);
        if (s.empty()) return Fecha::hoy(); // Enter vacio = hoy
        Fecha f;
        if (Fecha::desde_texto(s, f)) return f;
        cout << Color::ROJO << "Fecha invalida. Usa el formato dd/mm/aaaa (ej. 05/03/2026)." << Color::RESET << endl;
    }
}

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

// ============================================================================
//  3. ABSTRACCIONES (interfaces) - base de los principios O, I y D
// ============================================================================

// Contenedor generico de estrategias: agregar una nueva = registrarla, sin tocar el resto.
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

// ============================================================================
//  4. PERSISTENCIA: los servicios dependen de IRepositorio, no de archivos
// ============================================================================
struct DatosTienda { // todos los datos en memoria
    vector<unique_ptr<Producto>> inventario;
    list<Empleado> empleados;   // list: las direcciones no cambian al agregar
    list<Proveedor> proveedores;
    list<Cliente> clientes;
    list<Venta> ventas;
    list<Compra> compras;
    map<string, int> contadores;   // siguiente numero por clave (LAPTOP, CELULAR, BOLETA, FACTURA...)
};

class IRepositorio {
public:
    virtual ~IRepositorio() = default;
    virtual bool cargar(DatosTienda& datos) = 0;
    virtual bool guardar(const DatosTienda& datos) = 0;
};

class RepositorioCSV : public IRepositorio { // guarda en archivos de texto separados por '|'
private:
    const FabricaProductos& fabrica;
    string carpeta;

    string ruta(const string& nombre) const { return carpeta + nombre; }

    static string lc(string s) {   // quita los separadores del formato para no romper el archivo
        for (char& c : s) if (c == '|' || c == '~' || c == '^') c = ' ';
        return s;
    }
    static string b(bool v) { return v ? "1" : "0"; }

    bool escribir_archivo(const string& nombre, const function<void(ostream&)>& escritor) const {
        ofstream archivo(ruta(nombre));
        if (!archivo.is_open()) { cerr << "No se pudo escribir el archivo " << nombre << endl; return false; }
        escritor(archivo);
        return true;
    }
    bool leer_registros(const string& nombre, vector<vector<string>>& registros) const { // cada linea -> lista de campos
        ifstream archivo(ruta(nombre));
        if (!archivo.is_open()) return false;
        string linea;
        while (getline(archivo, linea)) {
            linea = recortar(linea);
            if (!linea.empty()) registros.push_back(dividir(linea, '|'));
        }
        return true;
    }

    // ---- serializacion de cada entidad (objeto -> linea de texto) ----
    static string ser(const Persona& p) {
        return lc(p.get_tipo_documento()) + "|" + lc(p.get_numero_documento()) + "|" + lc(p.get_materno()) + "|" +
               lc(p.get_paterno()) + "|" + lc(p.get_nombre()) + "|" + lc(p.get_telefono()) + "|" + lc(p.get_direccion());
    }
    static string ser(const Cliente& c) { return ser(static_cast<const Persona&>(c)) + "|" + lc(c.get_categoria()); }
    static string ser(const Empleado& e) {
        return ser(static_cast<const Persona&>(e)) + "|" + lc(e.get_codigo_empleado()) + "|" + lc(e.get_cargo()) + "|" +
               lc(e.get_rol()) + "|" + e.get_fecha_ingreso().texto() + "|" + to_string(e.get_sueldo()) + "|" +
               b(e.get_estado()) + "|" + e.get_hash_clave();
    }
    static string ser(const Proveedor& p) {
        return lc(p.get_ruc()) + "|" + lc(p.get_razon_social()) + "|" + lc(p.get_contacto()) + "|" +
               lc(p.get_telefono()) + "|" + lc(p.get_direccion_fiscal()) + "|" + b(p.get_estado());
    }
    static string ser(const Producto& p) {
        string s = p.tipo() + "|" + p.get_codigo() + "|" + lc(p.get_marca()) + "|" + lc(p.get_modelo()) + "|" +
                   to_string(p.get_precio()) + "|" + to_string(p.get_costo()) + "|" + to_string(p.get_stock()) + "|" + b(p.esta_activo());
        for (const string& a : p.atributos()) s += "|" + lc(a); // atributos propios al final
        return s;
    }
    static string ser(const Venta& v) {
        string items;
        for (const auto& i : v.get_items()) {
            if (!items.empty()) items += "~"; // '~' separa items, '^' separa campos del item
            items += lc(i.codigo) + "^" + lc(i.descripcion) + "^" + to_string(i.cantidad) + "^" +
                     to_string(i.precio_unitario) + "^" + to_string(i.costo_unitario) + "^" + to_string(i.subtotal);
        }
        return v.get_id_transaccion() + "|" + v.get_fecha_transaccion().texto() + "|" + to_string(v.get_monto_total()) + "|" +
               v.get_estado() + "|" + v.get_cliente().get_numero_documento() + "|" + v.get_empleado().get_codigo_empleado() + "|" +
               v.get_tipo_comprobante() + "|" + lc(v.get_medio_pago()) + "|" + to_string(v.get_descuento_pct()) + "|" +
               to_string(v.get_descuento_monto()) + "|" + to_string(v.get_monto_pagado()) + "|" + to_string(v.get_vuelto()) + "|" + items;
    }
    static string ser(const Compra& c) {
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

public:
    explicit RepositorioCSV(const FabricaProductos& f, const string& carpeta_datos = "") : fabrica(f), carpeta(carpeta_datos) {}

    bool guardar(const DatosTienda& d) override { // escribe todos los archivos
        bool ok = true;
        ok = escribir_archivo("datos_productos.csv", [&](ostream& o) { for (const auto& p : d.inventario) o << ser(*p) << "\n"; }) && ok;
        ok = escribir_archivo("datos_empleados.csv", [&](ostream& o) { for (const auto& e : d.empleados) o << ser(e) << "\n"; }) && ok;
        ok = escribir_archivo("datos_proveedores.csv", [&](ostream& o) { for (const auto& p : d.proveedores) o << ser(p) << "\n"; }) && ok;
        ok = escribir_archivo("datos_clientes.csv", [&](ostream& o) { for (const auto& c : d.clientes) o << ser(c) << "\n"; }) && ok;
        ok = escribir_archivo("datos_ventas.csv", [&](ostream& o) { for (const auto& v : d.ventas) o << ser(v) << "\n"; }) && ok;
        ok = escribir_archivo("datos_compras.csv", [&](ostream& o) { for (const auto& c : d.compras) o << ser(c) << "\n"; }) && ok;
        ok = escribir_archivo("datos_config.csv", [&](ostream& o) { for (const auto& kv : d.contadores) o << kv.first << "|" << kv.second << "\n"; }) && ok;
        return ok;
    }

    bool cargar(DatosTienda& d) override { // lee los archivos; los registros danados se ignoran
        vector<vector<string>> r;

        if (leer_registros("datos_empleados.csv", r)) {
            for (const auto& c : r) {
                try {
                    Fecha f;
                    if (c.size() != 14 || !Fecha::desde_texto(c[10], f)) continue;
                    d.empleados.push_back(Empleado(c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8], c[9], f, stod(c[11]), c[12] == "1", c[13]));
                } catch (...) {}
            }
        }
        r.clear();
        if (leer_registros("datos_proveedores.csv", r))
            for (const auto& c : r) if (c.size() == 6) d.proveedores.push_back(Proveedor(c[0], c[1], c[2], c[3], c[4], c[5] == "1"));
        r.clear();
        if (leer_registros("datos_clientes.csv", r))
            for (const auto& c : r) if (c.size() == 8) d.clientes.push_back(Cliente(c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7]));

        r.clear();
        if (leer_registros("datos_productos.csv", r)) {
            for (const auto& c : r) {
                try {
                    if (c.size() < 9) continue;
                    const ICreadorProducto* creador = fabrica.buscar(c[0]);
                    if (creador == nullptr) continue;   // tipo desconocido: se ignora
                    DatosBaseProducto base;
                    base.codigo = c[1]; base.marca = c[2]; base.modelo = c[3];
                    base.precio = stod(c[4]); base.costo = stod(c[5]); base.stock = stoi(c[6]); base.activo = (c[7] == "1");
                    unique_ptr<Producto> p = creador->crear(base, vector<string>(c.begin() + 8, c.end())); // del campo 8 en adelante: atributos propios
                    if (!p) continue;
                    d.inventario.push_back(move(p));
                    int siguiente = stoi(c[1].substr(1)) + 1; // numero del codigo + 1
                    d.contadores[c[0]] = max(d.contadores[c[0]], siguiente);
                } catch (...) {}
            }
        }
        r.clear();
        if (leer_registros("datos_config.csv", r)) {
            for (const auto& c : r) {
                try {
                    if (c.size() != 2) continue;
                    string clave = a_mayuscula(c[0]); // acepta claves en minuscula de versiones anteriores
                    int v = stoi(c[1]);
                    bool es_serie = (clave == "BOLETA" || clave == "FACTURA");
                    d.contadores[clave] = es_serie ? v : max(d.contadores[clave], v);
                } catch (...) {}
            }
        }
        r.clear();
        if (leer_registros("datos_ventas.csv", r)) {
            for (const auto& c : r) {
                try {
                    Fecha f;
                    if (c.size() != 13 || !Fecha::desde_texto(c[1], f)) continue;
                    const Empleado* emp = nullptr;
                    for (const auto& e : d.empleados) if (e.get_codigo_empleado() == c[5]) { emp = &e; break; }
                    if (emp == nullptr) continue; // sin cajero no se puede reconstruir la venta
                    Cliente cli("DNI", c[4], "", "", "DESCONOCIDO", "", "", Config::CAT_GENERAL); // cliente de respaldo
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
        if (leer_registros("datos_compras.csv", r)) {
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
        return !d.inventario.empty() && !d.empleados.empty(); // false = no hay datos usables
    }
};

// Datos de ejemplo para el primer uso.
inline void cargar_datos_iniciales(DatosTienda& d, const IHasheadorClave& h) {
    d.empleados.push_back(Empleado("DNI", "72341122", "Quispe", "Mamani", "Carlos", "951234567", "Av. Floral 120",
        "EMP01", "Administrador y Cajero", Config::ROL_ADMIN, Fecha(15, 3, 2024), 1500.0, true, h.hash("admin123")));
    d.empleados.push_back(Empleado("DNI", "70984433", "Condori", "Flores", "Lucia", "952987654", "Jr. Puno 340",
        "EMP02", "Asesora de Ventas", Config::ROL_CAJERO, Fecha(1, 8, 2025), 1600.0, true, h.hash("caja123")));
    d.proveedores.push_back(Proveedor("20100106963", "Grupo Deltron S.A.", "Ing. Marco Ramos", "01-4150100", "Av. Manuel Echeandia 904, Lima", true));
    d.proveedores.push_back(Proveedor("20254139943", "Ingram Micro Peru S.A.C.", "Lic. Elena Ruiz", "01-6169000", "Av. Rivera Navarrete 501, San Isidro", true));
    d.proveedores.push_back(Proveedor("20508565934", "Intcomex del Peru S.A.C.", "Sr. David Tello", "01-7128000", "Av. Los Ingenieros 148, Ate", true));
    d.inventario.push_back(make_unique<Laptop>("L01", "Lenovo", "ThinkPad E14", 3200.0, 2700.0, 6, "Core i7 13va", 16, "512GB SSD"));
    d.inventario.push_back(make_unique<Laptop>("L02", "ASUS", "TUF Gaming F15", 4100.0, 3500.0, 3, "Ryzen 7 7735HS", 16, "1TB SSD"));
    d.inventario.push_back(make_unique<Laptop>("L03", "HP", "Pavilion 15", 2650.0, 2200.0, 8, "Core i5 12va", 8, "512GB SSD"));
    d.inventario.push_back(make_unique<Celular>("C01", "Samsung", "Galaxy S24", 3800.0, 3300.0, 5, 256, "50MP", 4000));
    d.inventario.push_back(make_unique<Celular>("C02", "Xiaomi", "Redmi Note 13", 950.0, 750.0, 10, 128, "108MP", 5000));
    d.inventario.push_back(make_unique<Celular>("C03", "Apple", "iPhone 15", 4300.0, 3800.0, 4, 128, "48MP", 3349));
    d.contadores["LAPTOP"] = 4;  // proximo codigo: L04
    d.contadores["CELULAR"] = 4; // proximo codigo: C04
}

// ============================================================================
//  5. SERVICIOS DE NEGOCIO (cada uno con UNA responsabilidad)
// ============================================================================

// ---- Catalogo: consulta y modifica productos ----
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

// ============================================================================
//  6. INTERFAZ DE CONSOLA
// ============================================================================

// ---- Sesion: quien esta usando el sistema ----
class Sesion {
    GestorEmpleados& empleados;
    string codigo;
public:
    explicit Sesion(GestorEmpleados& g) : empleados(g) {}
    bool activa() const { return !codigo.empty(); }
    void iniciar(const string& cod) { codigo = cod; }
    void cerrar() { codigo.clear(); }
    const string& codigo_activo() const { return codigo; }
    const Empleado& usuario() { return *empleados.buscar(codigo); }
    bool es_admin() { return usuario().es_admin(); }
};

// ---- Vista del catalogo ----
class VistaCatalogo {
    Catalogo& catalogo;
public:
    explicit VistaCatalogo(Catalogo& c) : catalogo(c) {}

    void mostrar_producto(const Producto& p) const { // una linea por producto
        cout << "[" << p.get_codigo() << "] " << p.tipo() << ": " << p.get_marca() << " " << p.get_modelo()
             << " | " << p.detalle() << " | " << dinero(p.get_precio()) << " | Stock: " << p.get_stock();
        if (p.stock_bajo()) cout << Color::ROJO << "  (STOCK BAJO)" << Color::RESET;
        cout << endl;
    }
    void mostrar_catalogo() const { // productos agrupados por categoria
        cout << "\n========================================================================\n";
        cout << "                          CATALOGO Y STOCK ACTUAL                       \n";
        cout << "========================================================================\n";
        for (const string& t : catalogo.tipos_activos()) {
            cout << Color::AMARILLO << "--- " << t << " ---" << Color::RESET << "\n";
            for (const Producto* p : catalogo.activos_de_tipo(t)) mostrar_producto(*p);
        }
        cout << "========================================================================\n";
        int bajos = catalogo.cantidad_stock_bajo();
        if (bajos > 0) cout << Color::ROJO << "ALERTA: " << bajos << " producto(s) con stock bajo (" << Config::STOCK_MINIMO << " o menos)." << Color::RESET << "\n";
    }
    void buscar_y_mostrar() const {
        string texto = leer_texto_no_vacio("Buscar por marca o modelo: ");
        vector<Producto*> r = catalogo.buscar_por_texto(texto);
        if (r.empty()) { cout << Color::ROJO << "No se encontraron productos." << Color::RESET << "\n"; return; }
        for (const Producto* p : r) mostrar_producto(*p);
    }
};

// ---- Impresion del comprobante ----
class ImpresorTicket {
    const Registro<IMedioPago>& medios;
    static void linea_monto(const string& etiqueta, double valor) { // linea de totales alineada a la derecha
        cout << right << setw(48) << etiqueta << setw(16) << dinero(valor) << "\n";
    }
public:
    explicit ImpresorTicket(const Registro<IMedioPago>& m) : medios(m) {}

    void imprimir(const Venta& v) const {
        const string sep = "================================================================\n";
        const string sep2 = "----------------------------------------------------------------\n";
        limpiar();
        cout << sep << centrar(Config::NOMBRE_EMPRESA, 64) << "\n" << centrar("R.U.C. No. " + Config::RUC_EMPRESA, 64) << "\n"
             << centrar(Config::DIRECCION_EMPRESA, 64) << "\n" << sep;
        cout << left << setw(20) << "COMPROBANTE:" << v.get_tipo_comprobante() << " ELECTRONICA\n";
        cout << left << setw(20) << "NUMERO DE SERIE:" << v.get_id_transaccion() << "\n";
        cout << left << setw(20) << "FECHA DE EMISION:" << v.get_fecha_transaccion().texto() << "\n";
        cout << left << setw(20) << "CAJERO EN TURNO:" << v.get_empleado().nombre_completo() << " (" << v.get_empleado().get_codigo_empleado() << ")\n";
        cout << sep2;
        cout << "ADQUIRIENTE: " << v.get_cliente().nombre_completo() << "\n";
        cout << v.get_cliente().get_tipo_documento() << ": " << v.get_cliente().get_numero_documento() << "\n";
        cout << "DIRECCION: " << v.get_cliente().get_direccion() << "\n" << sep;
        cout << left << setw(5) << "CANT" << setw(33) << "DESCRIPCION" << right << setw(12) << "P.UNIT" << setw(14) << "IMPORTE" << "\n" << sep2;
        for (const auto& item : v.get_items()) {
            string desc = item.descripcion;
            if (desc.length() > 30) desc = desc.substr(0, 29) + "."; // recorta descripciones largas
            cout << left << setw(5) << item.cantidad << setw(33) << desc
                 << right << setw(12) << dinero(item.precio_unitario) << setw(14) << dinero(item.subtotal) << "\n";
        }
        double total = v.get_monto_total();
        double gravada = redondear2(total / (1.0 + Config::IGV)); // base imponible (sin IGV)
        double igv = redondear2(total - gravada);
        cout << sep;
        linea_monto("SUBTOTAL:", v.get_subtotal_bruto());
        if (v.get_descuento_monto() > 0) linea_monto("DESCUENTO (" + to_string((int)v.get_descuento_pct()) + "%):", -v.get_descuento_monto());
        linea_monto("OP. GRAVADA:", gravada);
        linea_monto("I.G.V. (" + to_string((int)round(Config::IGV * 100)) + "%):", igv);
        linea_monto("IMPORTE TOTAL:", total);
        cout << sep2;
        cout << left << setw(20) << "MEDIO DE PAGO:" << v.get_medio_pago() << "\n";
        const IMedioPago* mp = medios.buscar([&](const IMedioPago& m) { return m.nombre() == v.get_medio_pago(); });
        if (mp != nullptr && mp->es_efectivo()) { // recibido y vuelto solo en efectivo
            linea_monto("RECIBIDO:", v.get_monto_pagado());
            linea_monto("VUELTO:", v.get_vuelto());
        }
        cout << left << sep << centrar("GRACIAS POR SU COMPRA EN TECH STORE", 64) << "\n"
             << centrar("Conserve este comprobante para validar la garantia tecnica.", 64) << "\n" << sep;
    }
};

// ---- Pantalla de login ----
class PantallaLogin {
    Sesion& sesion;
    GestorEmpleados& empleados;
    const ServicioAutenticacion& auth;
public:
    PantallaLogin(Sesion& s, GestorEmpleados& g, const ServicioAutenticacion& a) : sesion(s), empleados(g), auth(a) {}

    bool intentar() { // un intento de login; true si entro
        limpiar();
        cout << Color::CIAN << "========================================================\n"
             << "                CONTROL DE ACCESO (LOGIN)               \n"
             << "========================================================" << Color::RESET << "\n";
        cout << "Usuarios registrados:\n";
        cout << "Clave para EMP01: admin123\n"; // claves de prueba (solo validas si no fueron cambiadas)
        cout << "Clave para EMP02: caja123\n";
        for (const auto& e : empleados.todos())
            if (e.get_estado()) cout << "  " << e.get_codigo_empleado() << " - " << e.nombre_completo() << " (" << e.get_cargo() << ")\n";
        cout << "\n";
        string codigo = a_mayuscula(leer_texto("Codigo de empleado (o SALIR): "));
        if (codigo == "SALIR") throw SalirPrograma{""}; // cierra el programa sin mensaje
        Empleado* emp = empleados.buscar(codigo);
        if (emp == nullptr) { cout << Color::ROJO << "Codigo no registrado en el sistema." << Color::RESET << "\n"; pausa(); return false; }
        if (!emp->get_estado()) { cout << Color::ROJO << "El usuario se encuentra inactivo." << Color::RESET << "\n"; pausa(); return false; }
        if (!auth.clave_correcta(*emp, leer_texto("Clave: "))) { cout << Color::ROJO << "Clave incorrecta." << Color::RESET << "\n"; pausa(); return false; }
        sesion.iniciar(emp->get_codigo_empleado());
        cout << Color::VERDE << "Acceso autorizado: " << emp->nombre_completo() << " [" << emp->get_cargo() << "]" << Color::RESET << "\n";
        esperar(1000);
        return true;
    }
    void exigir() { while (!sesion.activa()) intentar(); } // repite hasta iniciar sesion
};

// ---- Formularios de alta de producto (uno por tipo) ----
class IFormularioProducto {
public:
    virtual ~IFormularioProducto() = default;
    virtual string tipo() const = 0;
    virtual string etiqueta() const = 0; // texto para el menu
    virtual unique_ptr<Producto> pedir(const DatosBaseProducto& base) const = 0;   // pregunta solo lo propio del tipo
};
class FormularioLaptop : public IFormularioProducto {
public:
    string tipo() const override { return "LAPTOP"; }
    string etiqueta() const override { return "Laptop"; }
    unique_ptr<Producto> pedir(const DatosBaseProducto& b) const override {
        string cpu = leer_texto_no_vacio("Procesador: ");
        int ram = leer_entero_positivo("RAM (GB): ");
        string disco = leer_texto_no_vacio("Almacenamiento (ej. 1TB SSD): ");
        return make_unique<Laptop>(b.codigo, b.marca, b.modelo, b.precio, b.costo, b.stock, cpu, ram, disco);
    }
};
class FormularioCelular : public IFormularioProducto {
public:
    string tipo() const override { return "CELULAR"; }
    string etiqueta() const override { return "Celular"; }
    unique_ptr<Producto> pedir(const DatosBaseProducto& b) const override {
        int rom = leer_entero_positivo("Almacenamiento (GB): ");
        string cam = leer_texto_no_vacio("Camara (ej. 64MP): ");
        int bat = leer_entero_positivo("Bateria (mAh): ");
        return make_unique<Celular>(b.codigo, b.marca, b.modelo, b.precio, b.costo, b.stock, rom, cam, bat);
    }
};

// ---- Controlador de ventas ----
class ControladorVentas {
    ServicioVentas& ventas;
    Catalogo& catalogo;
    GestorClientes& clientes;
    Sesion& sesion;
    VistaCatalogo& vista;
    ImpresorTicket& ticket;
    const Registro<ITipoComprobante>& comprobantes;
    const Registro<IMedioPago>& medios;
    const IPoliticaDescuento& politica;

    struct Cobro { double desc_pct; string medio; double pagado; }; // resultado de la etapa de cobro

    const ITipoComprobante& elegir_comprobante() {
        const auto& lista = comprobantes.todos();
        cout << "Tipos de comprobante:\n";
        for (size_t i = 0; i < lista.size(); i++) cout << i + 1 << ". " << lista[i]->nombre() << "\n";
        int op = leer_entero_rango("Elige el comprobante: ", 1, (int)lista.size());
        return *lista[op - 1];
    }

    Cliente& identificar_cliente(const ITipoComprobante& tc) { // busca al cliente o lo registra
        cout << "\n--- Datos del Cliente ---\n";
        string num_doc = leer_documento(tc.tipo_documento());
        Cliente* cli = clientes.buscar(num_doc);
        if (cli != nullptr) {
            cout << Color::VERDE << "Cliente registrado: " << cli->nombre_completo() << " [" << cli->get_categoria() << "]" << Color::RESET << "\n";
            return *cli;
        }
        string nombre = leer_texto_no_vacio(tc.etiqueta_nombre());
        string paterno, materno;
        if (tc.pide_apellidos()) {
            paterno = leer_texto_no_vacio("Apellido Paterno: ");
            materno = leer_texto("Apellido Materno: ");
        }
        string direccion = leer_texto_no_vacio("Direccion: ");
        string telefono = leer_texto("Telefono de contacto: ");
        return *clientes.registrar(Cliente(tc.tipo_documento(), num_doc, materno, paterno, nombre, telefono, direccion, Config::CAT_GENERAL));
    }

    void mostrar_carrito(const list<ItemVenta>& carrito) {
        cout << "\n--- CARRITO ---\n";
        if (carrito.empty()) { cout << "(vacio)\n"; return; }
        cout << left << setw(8) << "CODIGO" << setw(32) << "DESCRIPCION" << setw(6) << "CANT"
             << right << setw(12) << "P.UNIT" << setw(12) << "IMPORTE" << "\n";
        for (const auto& i : carrito)
            cout << left << setw(8) << i.codigo << setw(32) << i.descripcion.substr(0, 30) << setw(6) << i.cantidad
                 << right << setw(12) << dinero(i.precio_unitario) << setw(12) << dinero(i.subtotal) << "\n";
        cout << left << "Subtotal: " << dinero(subtotal_items(carrito)) << "\n";
    }

    void agregar_al_carrito(list<ItemVenta>& carrito, const string& cod) {
        Producto* p = catalogo.buscar(cod);
        if (p == nullptr) { cout << Color::ROJO << "Codigo inexistente o producto dado de baja." << Color::RESET << "\n"; return; }
        int en_carrito = 0;
        for (const auto& i : carrito) if (i.codigo == p->get_codigo()) en_carrito += i.cantidad;
        int disponible = p->get_stock() - en_carrito; // stock menos lo ya reservado en el carrito
        if (disponible <= 0) { cout << Color::ROJO << "No hay mas stock disponible de este producto." << Color::RESET << "\n"; return; }
        int cant = leer_entero_positivo("Cantidad deseada: ");
        if (cant > disponible) { cout << Color::ROJO << "Stock insuficiente. Disponible: " << disponible << Color::RESET << "\n"; return; }
        for (auto& i : carrito) {
            if (i.codigo == p->get_codigo()) { // si ya esta, acumula la cantidad
                i.cantidad += cant;
                i.subtotal = redondear2(i.cantidad * i.precio_unitario);
                cout << Color::VERDE << "Cantidad actualizada: " << p->get_modelo() << " x" << i.cantidad << Color::RESET << "\n";
                return;
            }
        }
        ItemVenta nuevo{p->get_codigo(), p->get_marca() + " " + p->get_modelo(), cant, p->get_precio(), p->get_costo(), redondear2(cant * p->get_precio())};
        carrito.push_back(nuevo);
        cout << Color::VERDE << "Anadido: " << p->get_modelo() << " x" << cant << " | Subtotal: " << dinero(nuevo.subtotal) << Color::RESET << "\n";
    }

    void quitar_del_carrito(list<ItemVenta>& carrito) {
        if (carrito.empty()) { cout << "El carrito esta vacio.\n"; return; }
        mostrar_carrito(carrito);
        string cod = a_mayuscula(leer_texto("Codigo a quitar: "));
        for (auto it = carrito.begin(); it != carrito.end(); ++it) {
            if (it->codigo == cod) { carrito.erase(it); cout << Color::VERDE << "Producto quitado del carrito." << Color::RESET << "\n"; return; }
        }
        cout << Color::ROJO << "Ese codigo no esta en el carrito." << Color::RESET << "\n";
    }

    // Devuelve false si el usuario cancelo.
    bool llenar_carrito(list<ItemVenta>& carrito) { // bucle de comandos del carrito
        vista.mostrar_catalogo();
        cout << "\nEscribe un codigo para agregar. Comandos: buscar, ver, quitar, fin, cancelar\n";
        while (true) {
            string entrada = leer_texto("Codigo o comando: ");
            string cmd = a_minuscula(entrada);
            if (cmd == "fin") return true;
            if (cmd == "cancelar") return false;
            if (cmd == "ver") { mostrar_carrito(carrito); continue; }
            if (cmd == "buscar") { vista.buscar_y_mostrar(); continue; }
            if (cmd == "quitar") { quitar_del_carrito(carrito); continue; }
            if (cmd.empty()) continue;
            agregar_al_carrito(carrito, a_mayuscula(entrada)); // si no es comando, se toma como codigo
        }
    }

    Cobro cobrar(const Cliente& cli, double bruto) { // descuento, medio de pago y monto recibido
        Cobro c{0.0, "", 0.0};
        if (politica.tiene_descuento_automatico(cli)) {
            c.desc_pct = politica.descuento_automatico();
            cout << Color::VERDE << "Cliente frecuente: descuento automatico de " << c.desc_pct << "%." << Color::RESET << "\n";
        } else {
            double max_desc = politica.descuento_manual_maximo();
            c.desc_pct = leer_numero_rango("Descuento % (0 a " + to_string((int)max_desc) + ", 0 si no aplica): ", 0.0, max_desc);
        }
        double total = redondear2(bruto - calcular_descuento(bruto, c.desc_pct));
        cout << "TOTAL A COBRAR: " << dinero(total) << "\n";

        const auto& lista = medios.todos();
        cout << "\nMedio de pago:\n";
        for (size_t i = 0; i < lista.size(); i++) cout << i + 1 << ". " << lista[i]->nombre() << "\n";
        const IMedioPago& mp = *lista[leer_entero_rango("Elige medio de pago: ", 1, (int)lista.size()) - 1];
        c.medio = mp.nombre();
        c.pagado = total;
        if (mp.es_efectivo()) { // en efectivo pide cuanto entrego el cliente
            while (true) {
                c.pagado = leer_numero_positivo("Monto recibido: S/ ");
                if (c.pagado >= total - 0.0001) break; // tolerancia por decimales
                cout << Color::ROJO << "El monto recibido no cubre el total." << Color::RESET << "\n";
            }
        }
        return c;
    }

public:
    ControladorVentas(ServicioVentas& v, Catalogo& cat, GestorClientes& cl, Sesion& s, VistaCatalogo& vi, ImpresorTicket& t,
                      const Registro<ITipoComprobante>& comp, const Registro<IMedioPago>& med, const IPoliticaDescuento& pol)
        : ventas(v), catalogo(cat), clientes(cl), sesion(s), vista(vi), ticket(t), comprobantes(comp), medios(med), politica(pol) {}

    void registrar_venta() { // flujo completo: comprobante -> cliente -> carrito -> cobro -> ticket
        limpiar();
        escribir("=== MODULO DE VENTA / PUNTO DE FACTURACION ===\n", 15);
        cout << "Cajero a cargo: " << sesion.usuario().nombre_completo() << "\n\n";

        const ITipoComprobante& tc = elegir_comprobante();
        Cliente& cli = identificar_cliente(tc);

        list<ItemVenta> carrito;
        bool continuar = llenar_carrito(carrito);
        if (!continuar || carrito.empty()) {
            cout << "\nVenta descartada. El stock no fue modificado.\n";
            pausa();
            return;
        }
        mostrar_carrito(carrito);
        Cobro cobro = cobrar(cli, subtotal_items(carrito));

        cout << "\nFecha de emision del comprobante:\n";
        Fecha fecha = leer_fecha("Fecha (dd/mm/aaaa, Enter = hoy): ");
        if (!confirmar("\nConfirmar venta")) {
            cout << "Venta descartada. El stock no fue modificado.\n";
            pausa();
            return;
        }
        const Venta* v = ventas.finalizar(tc.nombre(), cli, sesion.usuario(), carrito, cobro.medio, cobro.desc_pct, cobro.pagado, fecha);
        if (v == nullptr) { // el stock pudo cambiar mientras se armaba el carrito
            cout << Color::ROJO << "No se pudo completar la venta por falta de stock." << Color::RESET << "\n";
            pausa();
            return;
        }
        puntos("Generando comprobante fiscal");
        ticket.imprimir(*v);
        pausa();
    }

    void ver_ventas() {
        limpiar();
        escribir("=== HISTORIAL DE TRANSACCIONES / VENTAS ===\n", 15);
        if (ventas.historial().empty()) cout << "\nNo hay ventas registradas.\n";
        for (const Venta& v : ventas.historial()) {
            bool anulada = v.get_estado() == Config::ESTADO_ANULADA;
            cout << "\n----------------------------------------------------------------\n";
            cout << "Comprobante: " << v.get_tipo_comprobante() << " No. " << v.get_id_transaccion() << " | Fecha: " << v.get_fecha_transaccion().texto();
            if (anulada) cout << Color::ROJO << " | ANULADA" << Color::RESET;
            cout << "\nCliente: " << v.get_cliente().nombre_completo() << " (" << v.get_cliente().get_tipo_documento() << ": " << v.get_cliente().get_numero_documento() << ")\n";
            cout << "Cajero: " << v.get_empleado().nombre_completo() << " | Pago: " << v.get_medio_pago() << "\n";
            cout << "Articulos facturados:\n";
            for (const auto& item : v.get_items())
                cout << "  * [" << item.codigo << "] " << item.descripcion << " x" << item.cantidad
                     << " @ " << dinero(item.precio_unitario) << " = " << dinero(item.subtotal) << "\n";
            if (v.get_descuento_monto() > 0) cout << "Descuento: " << dinero(v.get_descuento_monto()) << " (" << v.get_descuento_pct() << "%)\n";
            cout << "TOTAL: " << dinero(v.get_monto_total()) << "\n";
        }
        pausa();
    }

    void anular_venta() {
        limpiar();
        escribir("=== ANULACION DE VENTAS ===\n", 15);
        if (ventas.historial().empty()) { cout << "\nNo hay ventas registradas.\n"; pausa(); return; }
        for (const Venta& v : ventas.historial())
            cout << v.get_id_transaccion() << " | " << v.get_fecha_transaccion().texto() << " | " << dinero(v.get_monto_total()) << " | " << v.get_estado() << "\n";
        string id = leer_texto_no_vacio("\nNumero de venta a anular (ej. B001-1001): ");
        if (!confirmar("Se devolvera el stock y la venta quedara anulada. Continuar")) { cout << "Operacion cancelada.\n"; pausa(); return; }
        string error = ventas.anular(id);
        if (error.empty()) cout << Color::VERDE << "Venta anulada y stock devuelto." << Color::RESET << "\n";
        else cout << Color::ROJO << error << Color::RESET << "\n";
        pausa();
    }
};

// ---- Controlador de compras ----
class ControladorCompras {
    ServicioCompras& compras;
    Catalogo& catalogo;
    GestorProveedores& proveedores;
    GeneradorCodigos& generador;
    Sesion& sesion;
    VistaCatalogo& vista;
    const vector<unique_ptr<IFormularioProducto>>& formularios;

    Proveedor* elegir_proveedor() { // elige uno de la lista o registra uno nuevo
        cout << "--- DISTRIBUIDORES MAYORISTAS ASOCIADOS ---\n";
        int idx = 1;
        for (const auto& p : proveedores.todos())
            cout << idx++ << ". RUC: " << p.get_ruc() << " | " << p.get_razon_social() << " | Contacto: " << p.get_contacto() << "\n";
        cout << idx << ". Registrar otro proveedor\n"; // ultima opcion = nuevo proveedor
        int opc = leer_entero_rango("Seleccione el proveedor: ", 1, idx);
        Proveedor* prov = proveedores.por_indice(opc);
        if (prov != nullptr) return prov;

        cout << "\n--- Registro de Nuevo Proveedor ---\n";
        string ruc = leer_documento("RUC");
        prov = proveedores.buscar(ruc);
        if (prov != nullptr) { // evita duplicados
            cout << Color::AMARILLO << "Ese RUC ya estaba registrado: " << prov->get_razon_social() << Color::RESET << "\n";
            return prov;
        }
        string razon = leer_texto_no_vacio("Razon Social: ");
        string cont = leer_texto_no_vacio("Contacto comercial: ");
        string tel = leer_texto("Telefono: ");
        string dir = leer_texto_no_vacio("Direccion fiscal: ");
        return proveedores.agregar(Proveedor(ruc, razon, cont, tel, dir, true));
    }

    void reabastecer(list<ItemCompra>& articulos) { // suma stock a un producto existente
        vista.mostrar_catalogo();
        string cod = a_mayuscula(leer_texto("Codigo del producto a reabastecer: "));
        Producto* p = catalogo.buscar(cod);
        if (p == nullptr) { cout << Color::ROJO << "Codigo no existe o esta dado de baja." << Color::RESET << "\n"; return; }
        int cant = leer_entero_positivo("Cantidad recibida: ");
        double costo = leer_numero_positivo("Costo unitario de compra: S/ ");
        if (costo >= p->get_precio()) cout << Color::AMARILLO << "Aviso: el costo iguala o supera el precio de venta actual." << Color::RESET << "\n";
        articulos.push_back(ItemCompra{p->get_codigo(), p->get_marca() + " " + p->get_modelo(), cant, costo, redondear2(cant * costo)});
        cout << Color::VERDE << "Item agregado. El stock se actualiza al finalizar la compra." << Color::RESET << "\n";
    }

    void alta_producto(const IFormularioProducto& form, list<ItemCompra>& articulos, vector<unique_ptr<Producto>>& nuevos) {
        cout << "\n--- Alta de Nuevo Producto: " << form.etiqueta() << " ---\n";
        DatosBaseProducto b;
        b.codigo = generador.codigo_producto(form.tipo());
        b.marca = leer_texto_no_vacio("Marca: ");
        b.modelo = leer_texto_no_vacio("Modelo: ");
        int cant = leer_entero_positivo("Cantidad comprada: ");
        b.costo = leer_numero_positivo("Costo unitario de compra: S/ ");
        b.precio = leer_numero_positivo("Precio de venta al publico: S/ ");
        b.stock = 0;   // el stock entra al finalizar la compra
        if (b.precio <= b.costo) cout << Color::AMARILLO << "Aviso: el precio de venta no supera el costo." << Color::RESET << "\n";
        unique_ptr<Producto> p = form.pedir(b); // pide los datos propios del tipo
        articulos.push_back(ItemCompra{b.codigo, b.marca + " " + b.modelo, cant, b.costo, redondear2(cant * b.costo)});
        nuevos.push_back(move(p));
        cout << Color::VERDE << form.etiqueta() << " preparado con codigo [" << b.codigo << "]. Se incorpora al finalizar la compra." << Color::RESET << "\n";
    }

public:
    ControladorCompras(ServicioCompras& c, Catalogo& cat, GestorProveedores& pr, GeneradorCodigos& g, Sesion& s, VistaCatalogo& v,
                       const vector<unique_ptr<IFormularioProducto>>& f)
        : compras(c), catalogo(cat), proveedores(pr), generador(g), sesion(s), vista(v), formularios(f) {}

    void registrar_compra() {
        limpiar();
        escribir("=== MODULO DE COMPRAS (ABASTECIMIENTO B2B) ===\n", 15);
        cout << "Responsable de recepcion: " << sesion.usuario().nombre_completo() << "\n\n";

        Proveedor* prov = elegir_proveedor();
        string num_factura = leer_texto_no_vacio("\nNumero de Factura del Proveedor (ej. F102-4521): ");
        cout << "Fecha de ingreso al almacen:\n";
        Fecha fecha = leer_fecha("Fecha (dd/mm/aaaa, Enter = hoy): ");

        list<ItemCompra> articulos;
        vector<unique_ptr<Producto>> nuevos; // productos nuevos pendientes de incorporar
        const int n = (int)formularios.size();
        bool cancelada = false;
        while (true) {
            cout << "\n--- Operacion de Almacen ---\n1. Reabastecer stock de un producto existente\n";
            for (int i = 0; i < n; i++) cout << i + 2 << ". Dar de alta: " << formularios[i]->etiqueta() << " nuevo en el catalogo\n"; // una opcion por tipo de producto
            cout << n + 2 << ". Finalizar ingreso de mercaderia\n" << n + 3 << ". Cancelar compra\n";
            int accion = leer_entero_rango("Elige opcion: ", 1, n + 3);
            if (accion == n + 2) break;
            if (accion == n + 3) { cancelada = true; break; }
            if (accion == 1) reabastecer(articulos);
            else alta_producto(*formularios[accion - 2], articulos, nuevos);
        }
        if (cancelada || articulos.empty()) { cout << "\nCompra cancelada. No se modifico el inventario.\n"; pausa(); return; }

        const Compra* c = compras.registrar(*prov, sesion.usuario(), articulos, nuevos, num_factura, fecha);
        puntos("Actualizando kardex e inventario de tienda");
        cout << Color::VERDE << "Entrada de mercaderia " << c->get_id_transaccion() << " registrada por " << dinero(c->get_monto_total()) << "." << Color::RESET << "\n";
        pausa();
    }

    void ver_compras() {
        limpiar();
        escribir("=== HISTORIAL DE COMPRAS (ENTRADAS DE ALMACEN) ===\n", 15);
        if (compras.historial().empty()) cout << "\nNo hay compras registradas.\n";
        for (const Compra& c : compras.historial()) {
            cout << "\n----------------------------------------------------------------\n";
            cout << "Registro Interno: " << c.get_id_transaccion() << " | Fecha: " << c.get_fecha_transaccion().texto() << "\n";
            cout << "Proveedor: " << c.get_proveedor().get_razon_social() << " (RUC: " << c.get_proveedor().get_ruc() << ")\n";
            cout << "Factura Proveedor: " << c.get_numero_factura_proveedor() << "\n";
            cout << "Recibido por: " << c.get_empleado().nombre_completo() << "\nMercaderia ingresada:\n";
            for (const auto& item : c.get_items_comprados())
                cout << "  + [" << item.codigo << "] " << item.descripcion << " x" << item.cantidad
                     << " @ Costo " << dinero(item.costo_unitario) << " = " << dinero(item.subtotal) << "\n";
            cout << "TOTAL INVERTIDO: " << dinero(c.get_monto_total()) << "\n";
        }
        pausa();
    }
};

// ---- Controlador de inventario (editar precio, dar de baja, ver/buscar) ----
class ControladorInventario {
    Catalogo& catalogo;
    VistaCatalogo& vista;
public:
    ControladorInventario(Catalogo& c, VistaCatalogo& v) : catalogo(c), vista(v) {}

    void ver_catalogo() { limpiar(); vista.mostrar_catalogo(); pausa(); }
    void buscar() { limpiar(); vista.buscar_y_mostrar(); pausa(); }

    void editar_precio() {
        limpiar();
        vista.mostrar_catalogo();
        string cod = a_mayuscula(leer_texto("\nCodigo del producto a modificar: "));
        Producto* p = catalogo.buscar(cod);
        if (p == nullptr) { cout << Color::ROJO << "Codigo inexistente o dado de baja." << Color::RESET << "\n"; pausa(); return; }
        cout << "Precio actual: " << dinero(p->get_precio()) << " | Costo: " << dinero(p->get_costo()) << "\n";
        double nuevo = leer_numero_positivo("Nuevo precio de venta: S/ ");
        if (nuevo <= p->get_costo()) cout << Color::AMARILLO << "Aviso: el precio no supera el costo." << Color::RESET << "\n";
        catalogo.cambiar_precio(*p, nuevo);
        cout << Color::VERDE << "Precio actualizado." << Color::RESET << "\n";
        pausa();
    }

    void dar_de_baja() { // retira del catalogo pero conserva el historial
        limpiar();
        vista.mostrar_catalogo();
        string cod = a_mayuscula(leer_texto("\nCodigo del producto a dar de baja: "));
        Producto* p = catalogo.buscar(cod);
        if (p == nullptr) { cout << Color::ROJO << "Codigo inexistente o ya dado de baja." << Color::RESET << "\n"; pausa(); return; }
        if (p->get_stock() > 0) cout << Color::AMARILLO << "Aviso: el producto aun tiene " << p->get_stock() << " unidades en stock." << Color::RESET << "\n";
        if (confirmar("Dar de baja " + p->get_marca() + " " + p->get_modelo())) {
            catalogo.dar_de_baja(*p);
            cout << Color::VERDE << "Producto dado de baja. El historial se conserva." << Color::RESET << "\n";
        } else cout << "Operacion cancelada.\n";
        pausa();
    }
};

// ---- Controlador de reportes ----
class ControladorReportes {
    ServicioReportes& reportes;
    Catalogo& catalogo;

    void leer_rango(Fecha& desde, Fecha& hasta) { // pide dos fechas validas y en orden
        while (true) {
            desde = leer_fecha("Fecha inicial (dd/mm/aaaa, Enter = hoy): ");
            hasta = leer_fecha("Fecha final (dd/mm/aaaa, Enter = hoy): ");
            if (desde <= hasta) return;
            cout << Color::ROJO << "La fecha inicial no puede ser mayor que la final." << Color::RESET << "\n";
        }
    }
    void listar_ventas_rango(const Fecha& desde, const Fecha& hasta) {
        vector<const Venta*> lista = reportes.ventas_en_rango(desde, hasta);
        if (lista.empty()) { cout << "No hay ventas completadas en ese periodo.\n"; return; }
        double total = 0;
        for (const Venta* v : lista) {
            cout << v->get_id_transaccion() << " | " << v->get_fecha_transaccion().texto() << " | "
                 << left << setw(28) << v->get_cliente().nombre_completo().substr(0, 27) << " | " << dinero(v->get_monto_total()) << "\n";
            total += v->get_monto_total();
        }
        cout << "Ventas: " << lista.size() << " | Total vendido: " << dinero(redondear2(total)) << "\n";
    }
public:
    ControladorReportes(ServicioReportes& r, Catalogo& c) : reportes(r), catalogo(c) {}

    void menu() {
        int op = 0;
        while (op != 7) {
            limpiar();
            cout << Color::CIAN << "=============== REPORTES ===============" << Color::RESET << "\n";
            cout << "1. Ventas del dia\n2. Ventas por rango de fechas\n3. Productos mas vendidos\n"
                 << "4. Ventas por cajero\n5. Utilidad estimada por rango\n6. Valor del inventario\n7. Volver\n";
            op = leer_entero_rango("Elige una opcion: ", 1, 7);
            if (op == 1) {
                cout << "\n"; listar_ventas_rango(Fecha::hoy(), Fecha::hoy()); pausa();
            } else if (op == 2) {
                Fecha d, h; leer_rango(d, h); cout << "\n"; listar_ventas_rango(d, h); pausa();
            } else if (op == 3) {
                auto ranking = reportes.ranking_productos();
                cout << "\n";
                if (ranking.empty()) cout << "Aun no hay ventas.\n";
                for (size_t i = 0; i < ranking.size() && i < 10; i++) cout << (i + 1) << ". " << ranking[i].first << " - " << ranking[i].second << " unidades\n"; // top 10
                pausa();
            } else if (op == 4) {
                auto por_cajero = reportes.ventas_por_cajero();
                cout << "\n";
                if (por_cajero.empty()) cout << "Aun no hay ventas.\n";
                for (const auto& par : por_cajero) cout << par.first << " | " << par.second.first << " ventas | " << dinero(redondear2(par.second.second)) << "\n";
                pausa();
            } else if (op == 5) {
                Fecha d, h; leer_rango(d, h);
                cout << "\nUtilidad estimada (venta sin IGV menos costo): " << dinero(reportes.utilidad_en_rango(d, h)) << "\n";
                pausa();
            } else if (op == 6) {
                cout << "\nValor del inventario al costo: " << dinero(catalogo.valor_al_costo()) << "\n";
                cout << "Valor del inventario a precio de venta: " << dinero(catalogo.valor_a_venta()) << "\n";
                pausa();
            }
        }
    }
};

// ---- Controlador de empleados y cuenta propia ----
class ControladorEmpleados {
    GestorEmpleados& empleados;
    GeneradorCodigos& generador;
    const ServicioAutenticacion& auth;
    Sesion& sesion;

    string pedir_clave_nueva() { // exige minimo 4 caracteres
        while (true) {
            string clave = leer_texto("Clave inicial (minimo 4 caracteres): ");
            if (clave.size() >= 4) return clave;
            cout << Color::ROJO << "La clave es muy corta." << Color::RESET << "\n";
        }
    }
    void listar() {
        cout << "\n";
        for (const auto& e : empleados.todos())
            cout << e.get_codigo_empleado() << " | " << e.nombre_completo() << " " << e.get_materno() << " | " << e.get_cargo()
                 << " | " << e.get_rol() << " | Ingreso: " << e.get_fecha_ingreso().texto()
                 << " | " << dinero(e.get_sueldo()) << " | " << (e.get_estado() ? "Activo" : "Inactivo") << "\n";
        pausa();
    }
    void registrar() {
        cout << "\n--- Registro de Empleado ---\n";
        string dni = leer_documento("DNI");
        if (empleados.existe_documento(dni)) { cout << Color::ROJO << "Ya existe un empleado con ese DNI." << Color::RESET << "\n"; pausa(); return; }
        string nombre = leer_texto_no_vacio("Nombre(s): ");
        string paterno = leer_texto_no_vacio("Apellido Paterno: ");
        string materno = leer_texto("Apellido Materno: ");
        string tel = leer_texto("Telefono: ");
        string dir = leer_texto_no_vacio("Direccion: ");
        string cargo = leer_texto_no_vacio("Cargo: ");
        int rol = leer_entero_rango("Rol (1. Administrador / 2. Cajero): ", 1, 2);
        double sueldo = leer_numero_positivo("Sueldo: S/ ");
        string clave = pedir_clave_nueva();
        string codigo = generador.codigo_empleado();
        empleados.agregar(Empleado("DNI", dni, materno, paterno, nombre, tel, dir, codigo, cargo,
                                   rol == 1 ? Config::ROL_ADMIN : Config::ROL_CAJERO, Fecha::hoy(), sueldo, true, auth.hash(clave)));
        cout << Color::VERDE << "Empleado registrado con codigo " << codigo << "." << Color::RESET << "\n";
        pausa();
    }
    void alternar() { // activa o desactiva a un empleado
        string codigo = a_mayuscula(leer_texto("\nCodigo del empleado: "));
        string error = empleados.alternar_estado(codigo, sesion.codigo_activo());
        if (!error.empty()) cout << Color::ROJO << error << Color::RESET << "\n";
        else cout << Color::VERDE << "Empleado ahora " << (empleados.buscar(codigo)->get_estado() ? "ACTIVO" : "INACTIVO") << "." << Color::RESET << "\n";
        pausa();
    }
public:
    ControladorEmpleados(GestorEmpleados& e, GeneradorCodigos& g, const ServicioAutenticacion& a, Sesion& s)
        : empleados(e), generador(g), auth(a), sesion(s) {}

    void menu() {
        int op = 0;
        while (op != 4) {
            limpiar();
            cout << Color::CIAN << "========== GESTION DE EMPLEADOS ==========" << Color::RESET << "\n";
            cout << "1. Listar empleados\n2. Registrar empleado\n3. Activar / desactivar empleado\n4. Volver\n";
            op = leer_entero_rango("Elige una opcion: ", 1, 4);
            if (op == 1) listar();
            else if (op == 2) registrar();
            else if (op == 3) alternar();
        }
    }

    void cambiar_mi_clave() { // pide la clave actual, la nueva y su repeticion
        limpiar();
        Empleado* e = empleados.buscar(sesion.codigo_activo());
        if (!auth.clave_correcta(*e, leer_texto("Clave actual: "))) { cout << Color::ROJO << "Clave incorrecta." << Color::RESET << "\n"; pausa(); return; }
        string nueva = leer_texto("Nueva clave (minimo 4 caracteres): ");
        if (nueva.size() < 4) { cout << Color::ROJO << "La clave es muy corta." << Color::RESET << "\n"; pausa(); return; }
        if (leer_texto("Repite la nueva clave: ") != nueva) { cout << Color::ROJO << "Las claves no coinciden." << Color::RESET << "\n"; pausa(); return; }
        empleados.cambiar_hash_clave(*e, auth.hash(nueva));
        cout << Color::VERDE << "Clave actualizada." << Color::RESET << "\n";
        pausa();
    }
};

// ---- Aplicacion: menu principal basado en una lista de opciones ----
// Agregar una opcion nueva = agregar un elemento a la lista (en main), sin tocar esta clase.
struct OpcionMenu {
    string texto;
    bool solo_admin;
    function<void()> accion;
    bool termina = false; // true = al ejecutarla se cierra el programa
};

class Aplicacion {
    Sesion& sesion;
    PantallaLogin& login;
    Catalogo& catalogo;
    vector<OpcionMenu> opciones;

    void mostrar_menu() { // dibuja el menu segun la lista de opciones
        limpiar();
        cout << Color::CIAN << "========================================================\n"
             << "             SISTEMA DE GESTION TECH STORE              \n"
             << "========================================================" << Color::RESET << "\n";
        const Empleado& u = sesion.usuario();
        cout << Color::AMARILLO << "Usuario activo: " << u.nombre_completo() << " (" << u.get_cargo() << ") - " << u.get_rol() << Color::RESET << "\n";
        int bajos = catalogo.cantidad_stock_bajo();
        if (bajos > 0) cout << Color::ROJO << "ALERTA: " << bajos << " producto(s) con stock bajo." << Color::RESET << "\n";
        cout << "--------------------------------------------------------\n";
        for (size_t i = 0; i < opciones.size(); i++)
            cout << setw(2) << i + 1 << ". " << opciones[i].texto << (opciones[i].solo_admin ? " [ADMIN]" : "") << "\n";
        cout << "\nElige una opcion: ";
    }
    bool exigir_admin() { // bloquea la opcion si el usuario no es administrador
        if (sesion.es_admin()) return true;
        cout << Color::ROJO << "Esta opcion es solo para administradores." << Color::RESET << endl;
        pausa();
        return false;
    }
public:
    Aplicacion(Sesion& s, PantallaLogin& l, Catalogo& c, vector<OpcionMenu> ops)
        : sesion(s), login(l), catalogo(c), opciones(move(ops)) {}

    void ejecutar() { // bucle principal del programa
        cargando("Iniciando Sistema de Ventas Tech Store");
        login.exigir();
        bool salir = false;
        while (!salir) {
            mostrar_menu();
            int op;
            try { op = stoi(leer_texto("")); } catch (...) { op = 0; } // texto no numerico = opcion invalida
            if (op < 1 || op > (int)opciones.size()) {
                cout << Color::ROJO << "Opcion invalida" << Color::RESET << endl;
                esperar(800);
                continue;
            }
            const OpcionMenu& o = opciones[op - 1];
            if (o.solo_admin && !exigir_admin()) continue;
            o.accion();
            if (o.termina) salir = true;
        }
    }
};

// ============================================================================
//  7. RAIZ DE COMPOSICION: aqui (y solo aqui) se eligen las clases concretas
// ============================================================================
int main() {
    preparar_consola();
    try {
        // --- Tipos de producto, comprobantes, medios de pago y politicas ---
        FabricaProductos fabrica;
        fabrica.agregar(make_unique<CreadorLaptop>()); // para un tipo nuevo: registrarlo aqui
        fabrica.agregar(make_unique<CreadorCelular>());

        vector<unique_ptr<IFormularioProducto>> formularios;
        formularios.push_back(make_unique<FormularioLaptop>());
        formularios.push_back(make_unique<FormularioCelular>());

        Registro<ITipoComprobante> comprobantes;
        comprobantes.agregar(make_unique<Boleta>());
        comprobantes.agregar(make_unique<Factura>());

        Registro<IMedioPago> medios;
        medios.agregar(make_unique<MedioPagoSimple>("Efectivo", true));
        medios.agregar(make_unique<MedioPagoSimple>("Tarjeta", false));
        medios.agregar(make_unique<MedioPagoSimple>("Yape", false));
        medios.agregar(make_unique<MedioPagoSimple>("Plin", false));

        PoliticaDescuentoTienda politica;
        HasheadorStd hasheador;

        // --- Persistencia y datos ---
        RepositorioCSV repo(fabrica);
        DatosTienda datos;
        if (!repo.cargar(datos)) { // sin datos validos: usa los de ejemplo
            datos = DatosTienda();
            cargar_datos_iniciales(datos, hasheador);
            repo.guardar(datos);
        }

        // --- Servicios ---
        Catalogo catalogo(datos, repo);
        GeneradorCodigos generador(datos, fabrica);
        GestorEmpleados empleados(datos, repo);
        GestorClientes clientes(datos, repo);
        GestorProveedores proveedores(datos, repo);
        ServicioAutenticacion auth(hasheador);
        ServicioVentas servicio_ventas(datos, repo, catalogo, generador, comprobantes);
        ServicioCompras servicio_compras(datos, repo, catalogo);
        ServicioReportes servicio_reportes(datos);

        // --- Interfaz ---
        Sesion sesion(empleados);
        PantallaLogin login(sesion, empleados, auth);
        VistaCatalogo vista(catalogo);
        ImpresorTicket ticket(medios);
        ControladorVentas c_ventas(servicio_ventas, catalogo, clientes, sesion, vista, ticket, comprobantes, medios, politica);
        ControladorCompras c_compras(servicio_compras, catalogo, proveedores, generador, sesion, vista, formularios);
        ControladorInventario c_inventario(catalogo, vista);
        ControladorReportes c_reportes(servicio_reportes, catalogo);
        ControladorEmpleados c_empleados(empleados, generador, auth, sesion);

        // --- Menu principal ---
        vector<OpcionMenu> menu = { // {texto, solo_admin, accion}
            {"Registrar Venta (Facturacion con Ticket Fiscal)", false, [&] { puntos("Abriendo terminal de venta"); c_ventas.registrar_venta(); }},
            {"Registrar Compra a Mayoristas", true, [&] { puntos("Abriendo recepcion de mercaderia"); c_compras.registrar_compra(); }},
            {"Ver Catalogo e Inventario Actual", false, [&] { c_inventario.ver_catalogo(); }},
            {"Buscar Producto por Marca o Modelo", false, [&] { c_inventario.buscar(); }},
            {"Ver Historial de Ventas", false, [&] { puntos("Consultando kardex de ventas"); c_ventas.ver_ventas(); }},
            {"Ver Historial de Compras de Almacen", true, [&] { puntos("Consultando kardex de compras"); c_compras.ver_compras(); }},
            {"Anular Venta", true, [&] { c_ventas.anular_venta(); }},
            {"Editar Precio de Producto", true, [&] { c_inventario.editar_precio(); }},
            {"Dar de Baja un Producto", true, [&] { c_inventario.dar_de_baja(); }},
            {"Reportes", true, [&] { c_reportes.menu(); }},
            {"Gestion de Empleados", true, [&] { c_empleados.menu(); }},
            {"Cambiar mi Clave", false, [&] { c_empleados.cambiar_mi_clave(); }},
            {"Cambiar de Usuario / Cajero", false, [&] { sesion.cerrar(); login.exigir(); }},
            {"Salir", false, [&] { escribir("Cerrando estacion de trabajo y caja. Hasta luego!\n", 25); }, true}, // true = cierra el programa
        };

        Aplicacion app(sesion, login, catalogo, move(menu));
        app.ejecutar();
        repo.guardar(datos); // guarda todo antes de salir
    } catch (const SalirPrograma& s) { // salida ordenada (SALIR en el login o fin de entrada)
        if (!s.mensaje.empty()) cout << s.mensaje << endl;
    }
    return 0;
}