#include <iostream> // entrada y salida por consola
#include <string> // clase string
#include <vector> // contenedor vector
#include <cstdlib> // exit
#include <iomanip> // setw y setprecision
#include <sstream> // ostringstream
#include <algorithm> // all_of, find, transform
#include <cctype> // isdigit, tolower, toupper
#include <cmath> // floor, round, isfinite
#include <ctime> // fecha actual
#include <functional> // hash de cadenas
#ifdef _WIN32 // solo en Windows
#ifndef NOMINMAX // si aun no esta definida
#define NOMINMAX // evita las macros min y max de windows.h
#endif // fin del bloque NOMINMAX
#include <windows.h> // API de consola de Windows
#else // Linux y Mac
#include <unistd.h> // usleep
#endif // fin del bloque de plataforma
using namespace std; // evita escribir std::

namespace Config { // constantes del sistema
    const double IGV = 0.18; // tasa del IGV
    const int STOCK_MINIMO = 3; // stock igual o menor a este valor activa la alerta
    const double DESCUENTO_FRECUENTE = 5.0; // descuento automatico para clientes frecuentes
    const double DESCUENTO_MAXIMO = 30.0; // descuento manual maximo
    const int COMPRAS_PARA_FRECUENTE = 3; // compras necesarias para ser cliente frecuente
    const string RUC_EMPRESA = "20601234567"; // ruc de la tienda
    const string NOMBRE_EMPRESA = "TECH STORE S.A.C."; // razon social de la tienda
    const string DIRECCION_EMPRESA = "Av. Floral 450 - Puno / Cel: 951-000-111"; // direccion de la tienda
    const string SERIE_BOLETA = "B001-"; // prefijo de las boletas
    const string SERIE_FACTURA = "F001-"; // prefijo de las facturas
    const string ROL_ADMIN = "ADMIN"; // rol de administrador
    const string ROL_CAJERO = "CAJERO"; // rol de cajero
    const string ESTADO_COMPLETADA = "Completada"; // estado de venta o compra vigente
    const string ESTADO_ANULADA = "Anulada"; // estado de venta anulada
} // fin del namespace Config

namespace Color { // codigos de color ANSI
    const string ROJO = "\033[31m"; // texto rojo
    const string VERDE = "\033[32m"; // texto verde
    const string AMARILLO = "\033[33m"; // texto amarillo
    const string CIAN = "\033[36m"; // texto cian
    const string RESET = "\033[0m"; // vuelve al color normal
} // fin del namespace Color

inline void preparar_consola() { // activa los colores ANSI en Windows
#ifdef _WIN32 // solo en Windows
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); // obtiene la salida estandar
    DWORD modo = 0; // variable para el modo actual
    if (GetConsoleMode(h, &modo)) SetConsoleMode(h, modo | 0x0004); // activa el procesamiento ANSI
#endif // fin del bloque solo Windows
} // fin de la funcion

inline void esperar(int ms) { // pausa el programa
#ifdef _WIN32 // version para Windows
    Sleep(ms); // duerme los milisegundos indicados
#else // version para Linux y Mac
    usleep(ms * 1000); // duerme los microsegundos equivalentes
#endif // fin de la seleccion de plataforma
} // fin de la funcion

inline void limpiar() { // limpia la pantalla
    cout << "\033[2J\033[H" << flush; // borra la pantalla y mueve el cursor al inicio
} // fin de la funcion

inline void escribir(const string& texto, int ms = 15) { // escribe con efecto de maquina de escribir
    for (char c : texto) { // recorre cada caracter
        cout << c << flush; // imprime el caracter
        esperar(ms); // espera entre caracteres
    } // fin del bucle
} // fin de la funcion

inline void cargando(const string& mensaje) { // muestra una barra de carga
    cout << mensaje << endl << "["; // imprime el mensaje y abre la barra
    for (int i = 0; i < 20; i++) { // 20 pasos de carga
        cout << "#" << flush; // dibuja un bloque
        esperar(35); // espera entre bloques
    } // fin del bucle
    cout << "] 100%" << endl; // cierra la barra
    esperar(200); // pausa corta
} // fin de la funcion

inline void puntos(const string& mensaje) { // muestra un mensaje con tres puntos animados
    cout << mensaje; // imprime el mensaje
    for (int i = 0; i < 3; i++) { // tres puntos
        cout << "." << flush; // imprime un punto
        esperar(200); // espera entre puntos
    } // fin del bucle
    cout << endl; // salto de linea
} // fin de la funcion

inline string recortar(const string& s) { // quita espacios al inicio y al final
    size_t i = s.find_first_not_of(" \t\r\n"); // primera posicion no vacia
    if (i == string::npos) return ""; // si todo es espacio devuelve vacio
    size_t j = s.find_last_not_of(" \t\r\n"); // ultima posicion no vacia
    return s.substr(i, j - i + 1); // devuelve el texto recortado
} // fin de la funcion

inline string a_minuscula(string s) { // convierte a minusculas
    for (char& c : s) c = (char)tolower((unsigned char)c); // convierte cada caracter
    return s; // devuelve el resultado
} // fin de la funcion

inline string a_mayuscula(string s) { // convierte a mayusculas
    for (char& c : s) c = (char)toupper((unsigned char)c); // convierte cada caracter
    return s; // devuelve el resultado
} // fin de la funcion

inline double redondear2(double v) { // redondea a dos decimales
    return round(v * 100.0) / 100.0; // multiplica, redondea y divide
} // fin de la funcion

inline string dinero(double v) { // da formato de moneda a un monto
    ostringstream o; // flujo de texto temporal
    o << "S/ " << fixed << setprecision(2) << v; // escribe el monto con dos decimales
    return o.str(); // devuelve el texto
} // fin de la funcion

inline string hash_clave(const string& clave) { // convierte una clave en un hash
    return to_string(hash<string>{}(clave)); // aplica el hash estandar y lo pasa a texto
} // fin de la funcion

inline vector<string> dividir(const string& texto, char sep) { // separa un texto por un caracter
    vector<string> partes; // lista de partes
    string actual; // parte que se esta armando
    for (char c : texto) { // recorre cada caracter
        if (c == sep) { // si es el separador
            partes.push_back(actual); // guarda la parte
            actual.clear(); // reinicia la parte
        } else { // si no es el separador
            actual += c; // agrega el caracter a la parte
        } // fin del if
    } // fin del bucle
    partes.push_back(actual); // guarda la ultima parte
    return partes; // devuelve las partes
} // fin de la funcion

inline string limpiar_campo(string s) { // quita los separadores del archivo de datos
    for (char& c : s) { // recorre cada caracter
        if (c == '|' || c == '~' || c == '^') c = ' '; // reemplaza separadores por espacio
    } // fin del bucle
    return s; // devuelve el texto limpio
} // fin de la funcion

inline string leer_texto(const string& mensaje) { // lee una linea de texto
    cout << mensaje; // muestra el mensaje
    string s; // variable para la linea
    if (!getline(cin, s)) { // si la entrada se cerro (Ctrl+D o fin de archivo)
        cout << "\nEntrada cerrada. Saliendo del programa.\n"; // avisa al usuario
        exit(0); // termina el programa para evitar bucles infinitos
    } // fin del if
    return recortar(s); // devuelve la linea sin espacios sobrantes
} // fin de la funcion

inline string leer_texto_no_vacio(const string& mensaje) { // lee texto que no puede quedar vacio
    while (true) { // repite hasta recibir texto
        string s = leer_texto(mensaje); // lee el texto
        if (!s.empty()) return s; // si hay contenido lo devuelve
        cout << Color::ROJO << "Este campo no puede estar vacio." << Color::RESET << endl; // avisa el error
    } // fin del bucle
} // fin de la funcion

inline double leer_numero(const string& mensaje) { // lee un numero decimal valido
    while (true) { // repite hasta recibir un numero valido
        string s = leer_texto(mensaje); // lee el texto
        try { // intenta convertir
            size_t pos = 0; // cantidad de caracteres convertidos
            double v = stod(s, &pos); // convierte a double
            if (pos == s.size() && isfinite(v)) return v; // acepta solo si se uso todo el texto
        } catch (...) { // si falla la conversion
        } // fin del catch
        cout << Color::ROJO << "Numero invalido, intenta de nuevo." << Color::RESET << endl; // avisa el error
    } // fin del bucle
} // fin de la funcion

inline double leer_numero_rango(const string& mensaje, double minimo, double maximo) { // lee un numero dentro de un rango
    while (true) { // repite hasta recibir un valor valido
        double v = leer_numero(mensaje); // lee el numero
        if (v >= minimo && v <= maximo) return v; // si esta en rango lo devuelve
        cout << Color::ROJO << "Ingresa un valor entre " << minimo << " y " << maximo << "." << Color::RESET << endl; // avisa el error
    } // fin del bucle
} // fin de la funcion

inline double leer_numero_positivo(const string& mensaje) { // lee un numero mayor que cero
    while (true) { // repite hasta recibir un valor valido
        double v = leer_numero(mensaje); // lee el numero
        if (v > 0 && v <= 10000000) return v; // acepta valores positivos razonables
        cout << Color::ROJO << "El valor debe ser mayor que cero." << Color::RESET << endl; // avisa el error
    } // fin del bucle
} // fin de la funcion

inline int leer_entero_rango(const string& mensaje, int minimo, int maximo) { // lee un entero dentro de un rango
    while (true) { // repite hasta recibir un valor valido
        double v = leer_numero(mensaje); // lee el numero
        if (v == floor(v) && v >= minimo && v <= maximo) return (int)v; // acepta solo enteros en rango
        cout << Color::ROJO << "Ingresa un entero entre " << minimo << " y " << maximo << "." << Color::RESET << endl; // avisa el error
    } // fin del bucle
} // fin de la funcion

inline int leer_entero_positivo(const string& mensaje) { // lee un entero mayor que cero
    return leer_entero_rango(mensaje, 1, 100000); // reutiliza la lectura con rango
} // fin de la funcion

inline bool confirmar(const string& mensaje) { // pregunta si o no
    string r = a_minuscula(leer_texto(mensaje + " (s/n): ")); // lee la respuesta
    return r == "s" || r == "si"; // verdadero si responde si
} // fin de la funcion

inline void pausa() { // espera a que el usuario presione Enter
    leer_texto("\nPresiona Enter para continuar..."); // lee una linea vacia
} // fin de la funcion

inline bool solo_digitos(const string& s) { // verifica que todo sean digitos
    return !s.empty() && all_of(s.begin(), s.end(), [](unsigned char c) { return isdigit(c) != 0; }); // no vacio y todos digitos
} // fin de la funcion

inline bool dni_valido(const string& s) { // valida un DNI
    return s.size() == 8 && solo_digitos(s); // 8 digitos
} // fin de la funcion

inline bool ruc_valido(const string& s) { // valida un RUC
    return s.size() == 11 && solo_digitos(s) && (s.substr(0, 2) == "10" || s.substr(0, 2) == "20"); // 11 digitos que empiezan con 10 o 20
} // fin de la funcion

inline string leer_documento(const string& tipo) { // lee un DNI o un RUC valido
    while (true) { // repite hasta recibir un documento valido
        string d = leer_texto("Numero de " + tipo + ": "); // lee el documento
        if (tipo == "RUC" ? ruc_valido(d) : dni_valido(d)) return d; // valida segun el tipo
        if (tipo == "RUC") cout << Color::ROJO << "El RUC debe tener 11 digitos y empezar con 10 o 20." << Color::RESET << endl; // error de RUC
        else cout << Color::ROJO << "El DNI debe tener 8 digitos." << Color::RESET << endl; // error de DNI
    } // fin del bucle
} // fin de la funcion



class Fecha { // fecha con validacion
private: // datos internos
    int dia, mes, anio; // componentes de la fecha

public: // interfaz publica
    Fecha(int d, int m, int a) : dia(d), mes(m), anio(a) {} // constructor con valores
    Fecha() : dia(1), mes(1), anio(2000) { *this = hoy(); } // constructor por defecto con la fecha actual

    static Fecha hoy() { // devuelve la fecha del sistema
        time_t t = time(nullptr); // hora actual en segundos
        tm info{}; // estructura de fecha y hora
#ifdef _WIN32 // version para Windows
        tm* local = localtime(&t); // convierte a hora local
        if (local != nullptr) info = *local; // copia el resultado
#else // version para Linux y Mac
        localtime_r(&t, &info); // convierte a hora local
#endif // fin de la seleccion de plataforma
        return Fecha(info.tm_mday, info.tm_mon + 1, info.tm_year + 1900); // arma la fecha
    } // fin de la funcion

    static bool es_bisiesto(int a) { // indica si el anio es bisiesto
        return (a % 4 == 0 && a % 100 != 0) || a % 400 == 0; // regla del calendario gregoriano
    } // fin de la funcion

    static int dias_del_mes(int m, int a) { // cantidad de dias de un mes
        static const int dias[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // dias por mes
        if (m == 2 && es_bisiesto(a)) return 29; // febrero bisiesto
        return dias[m - 1]; // dias normales del mes
    } // fin de la funcion

    static bool es_valida(int d, int m, int a) { // valida una fecha completa
        if (a < 2000 || a > 2100) return false; // rango de anios permitido
        if (m < 1 || m > 12) return false; // rango de meses
        return d >= 1 && d <= dias_del_mes(m, a); // rango de dias segun el mes
    } // fin de la funcion

    static bool desde_texto(const string& s, Fecha& resultado) { // convierte dd/mm/aaaa a Fecha
        vector<string> p = dividir(s, '/'); // separa por la barra
        if (p.size() != 3) return false; // deben ser tres partes
        if (!solo_digitos(p[0]) || !solo_digitos(p[1]) || !solo_digitos(p[2])) return false; // solo digitos
        if (p[0].size() > 2 || p[1].size() > 2 || p[2].size() != 4) return false; // longitudes correctas
        int d = stoi(p[0]), m = stoi(p[1]), a = stoi(p[2]); // convierte a enteros
        if (!es_valida(d, m, a)) return false; // valida el calendario
        resultado = Fecha(d, m, a); // guarda el resultado
        return true; // conversion correcta
    } // fin de la funcion

    int clave() const { return anio * 10000 + mes * 100 + dia; } // numero para comparar fechas
    bool operator<(const Fecha& o) const { return clave() < o.clave(); } // menor que
    bool operator<=(const Fecha& o) const { return clave() <= o.clave(); } // menor o igual
    bool operator>=(const Fecha& o) const { return clave() >= o.clave(); } // mayor o igual
    bool operator==(const Fecha& o) const { return clave() == o.clave(); } // igual

    string texto() const { // devuelve dd/mm/aaaa
        ostringstream o; // flujo de texto temporal
        o << setfill('0') << setw(2) << dia << "/" << setw(2) << mes << "/" << setw(4) << anio; // arma el texto
        return o.str(); // devuelve el texto
    } // fin de la funcion
}; // fin de la clase Fecha

inline Fecha leer_fecha(const string& mensaje) { // lee una fecha valida
    while (true) { // repite hasta recibir una fecha valida
        string s = leer_texto(mensaje); // lee el texto
        if (s.empty()) return Fecha::hoy(); // Enter vacio usa la fecha actual
        Fecha f; // fecha de resultado
        if (Fecha::desde_texto(s, f)) return f; // si es valida la devuelve
        cout << Color::ROJO << "Fecha invalida. Usa el formato dd/mm/aaaa (ej. 05/03/2026)." << Color::RESET << endl; // avisa el error
    } // fin del bucle
} // fin de la funcion

class Persona { // datos comunes de una persona
private: // datos internos
    string Tipo_documento; // DNI o RUC
    string Numero_documento; // numero del documento
    string Materno; // apellido materno
    string Paterno; // apellido paterno
    string Nombre; // nombres
    string Telefono; // telefono
    string Direccion; // direccion

protected: // para las clases hijas
    string serializar_base() const { // une los datos comunes en una linea
        return limpiar_campo(Tipo_documento) + "|" + limpiar_campo(Numero_documento) + "|" + limpiar_campo(Materno) + "|" // primeros campos
             + limpiar_campo(Paterno) + "|" + limpiar_campo(Nombre) + "|" + limpiar_campo(Telefono) + "|" + limpiar_campo(Direccion); // campos restantes
    } // fin de la funcion

public: // interfaz publica
    Persona(const string& tipo_doc, const string& num_doc, const string& materno, const string& paterno, // parametros del constructor
            const string& nombre, const string& telefono, const string& direccion) // mas parametros
        : Tipo_documento(tipo_doc), Numero_documento(num_doc), Materno(materno), // inicializa campos
          Paterno(paterno), Nombre(nombre), Telefono(telefono), Direccion(direccion) {} // inicializa campos

    const string& get_tipo_documento() const { return Tipo_documento; } // devuelve el tipo de documento
    const string& get_numero_documento() const { return Numero_documento; } // devuelve el numero de documento
    const string& get_materno() const { return Materno; } // devuelve el apellido materno
    const string& get_paterno() const { return Paterno; } // devuelve el apellido paterno
    const string& get_nombre() const { return Nombre; } // devuelve los nombres
    const string& get_telefono() const { return Telefono; } // devuelve el telefono
    const string& get_direccion() const { return Direccion; } // devuelve la direccion
    string nombre_completo() const { return recortar(Nombre + " " + Paterno); } // nombre y paterno sin espacios sobrantes
}; // fin de la clase Persona

class Cliente : public Persona { // cliente de la tienda
private: // datos internos
    string Categoria; // General o Frecuente

public: // interfaz publica
    Cliente(const string& tipo_doc, const string& num_doc, const string& materno, const string& paterno, // parametros del constructor
            const string& nombre, const string& telefono, const string& direccion, const string& categoria) // mas parametros
        : Persona(tipo_doc, num_doc, materno, paterno, nombre, telefono, direccion), Categoria(categoria) {} // inicializa la base y la categoria

    const string& get_categoria() const { return Categoria; } // devuelve la categoria
    void set_categoria(const string& c) { Categoria = c; } // cambia la categoria
    string serializar() const { return serializar_base() + "|" + limpiar_campo(Categoria); } // linea para guardar en archivo
}; // fin de la clase Cliente

class Empleado : public Persona { // empleado de la tienda
private: // datos internos
    string Codigo_empleado; // codigo de acceso
    string Cargo; // cargo descriptivo
    string Rol; // ADMIN o CAJERO
    Fecha Fecha_ingreso; // fecha de ingreso
    double Sueldo; // sueldo mensual
    bool Estado; // activo o inactivo
    string Hash_clave; // clave guardada como hash

public: // interfaz publica
    Empleado(const string& tipo_doc, const string& num_doc, const string& materno, const string& paterno, // parametros del constructor
             const string& nombre, const string& telefono, const string& direccion, const string& codigo_emp, // mas parametros
             const string& cargo, const string& rol, const Fecha& fecha_ingreso, double sueldo, bool estado, // mas parametros
             const string& hash) // hash de la clave
        : Persona(tipo_doc, num_doc, materno, paterno, nombre, telefono, direccion), // inicializa la base
          Codigo_empleado(codigo_emp), Cargo(cargo), Rol(rol), Fecha_ingreso(fecha_ingreso), // inicializa campos
          Sueldo(sueldo), Estado(estado), Hash_clave(hash) {} // inicializa campos

    const string& get_codigo_empleado() const { return Codigo_empleado; } // devuelve el codigo
    const string& get_cargo() const { return Cargo; } // devuelve el cargo
    const string& get_rol() const { return Rol; } // devuelve el rol
    const Fecha& get_fecha_ingreso() const { return Fecha_ingreso; } // devuelve la fecha de ingreso
    double get_sueldo() const { return Sueldo; } // devuelve el sueldo
    bool get_estado() const { return Estado; } // devuelve si esta activo
    bool es_admin() const { return Rol == Config::ROL_ADMIN; } // indica si es administrador
    void set_estado(bool e) { Estado = e; } // activa o desactiva al empleado
    bool clave_correcta(const string& clave) const { return Hash_clave == hash_clave(clave); } // compara la clave ingresada
    void set_clave(const string& clave) { Hash_clave = hash_clave(clave); } // cambia la clave

    string serializar() const { // linea para guardar en archivo
        return serializar_base() + "|" + limpiar_campo(Codigo_empleado) + "|" + limpiar_campo(Cargo) + "|" + limpiar_campo(Rol) // datos de identidad
             + "|" + Fecha_ingreso.texto() + "|" + to_string(Sueldo) + "|" + (Estado ? "1" : "0") + "|" + Hash_clave; // fecha, sueldo, estado y hash
    } // fin de la funcion
}; // fin de la clase Empleado

class Proveedor { // proveedor comercial B2B
private: // datos internos
    string RUC; // ruc del proveedor
    string Razon_social; // razon social
    string Contacto_ventas; // persona de contacto
    string Telefono; // telefono
    string Direccion_fiscal; // direccion fiscal
    bool Estado; // activo o inactivo

public: // interfaz publica
    Proveedor(const string& ruc, const string& razon, const string& contacto, // parametros del constructor
              const string& tel, const string& dir, bool est) // mas parametros
        : RUC(ruc), Razon_social(razon), Contacto_ventas(contacto), // inicializa campos
          Telefono(tel), Direccion_fiscal(dir), Estado(est) {} // inicializa campos

    const string& get_ruc() const { return RUC; } // devuelve el ruc
    const string& get_razon_social() const { return Razon_social; } // devuelve la razon social
    const string& get_contacto() const { return Contacto_ventas; } // devuelve el contacto
    const string& get_telefono() const { return Telefono; } // devuelve el telefono
    const string& get_direccion_fiscal() const { return Direccion_fiscal; } // devuelve la direccion fiscal
    bool get_estado() const { return Estado; } // devuelve si esta activo

    string serializar() const { // linea para guardar en archivo
        return limpiar_campo(RUC) + "|" + limpiar_campo(Razon_social) + "|" + limpiar_campo(Contacto_ventas) + "|" // primeros campos
             + limpiar_campo(Telefono) + "|" + limpiar_campo(Direccion_fiscal) + "|" + (Estado ? "1" : "0"); // campos restantes
    } // fin de la funcion
}; // fin de la clase Proveedor



class Producto { // clase base abstracta de productos
protected: // visible para las clases hijas
    string Codigo; // codigo unico
    string Marca; // marca
    string Modelo; // modelo
    double Precio; // precio de venta
    double Costo; // ultimo costo de compra
    int Stock; // unidades disponibles
    bool Activo; // false si fue dado de baja

public: // interfaz publica
    Producto(const string& cod, const string& marca, const string& modelo, double precio, double costo, int stock, bool activo = true) // constructor
        : Codigo(cod), Marca(marca), Modelo(modelo), Precio(precio), Costo(costo), Stock(stock), Activo(activo) {} // inicializa campos

    virtual ~Producto() = default; // destructor virtual para borrar por puntero base

    virtual string tipo() const = 0; // nombre de la categoria
    virtual string detalle() const = 0; // especificaciones tecnicas para mostrar
    virtual string extras() const = 0; // campos propios para guardar en archivo

    const string& get_codigo() const { return Codigo; } // devuelve el codigo
    const string& get_marca() const { return Marca; } // devuelve la marca
    const string& get_modelo() const { return Modelo; } // devuelve el modelo
    double get_precio() const { return Precio; } // devuelve el precio de venta
    double get_costo() const { return Costo; } // devuelve el costo
    int get_stock() const { return Stock; } // devuelve el stock
    bool esta_activo() const { return Activo; } // indica si esta en catalogo
    bool stock_bajo() const { return Stock <= Config::STOCK_MINIMO; } // indica si el stock es bajo

    bool reducir_stock(int cant) { // descuenta unidades
        if (cant > 0 && cant <= Stock) { // cantidad positiva y suficiente
            Stock -= cant; // descuenta
            return true; // operacion correcta
        } // fin del if
        return false; // cantidad invalida o insuficiente
    } // fin de la funcion

    void aumentar_stock(int cant) { // suma unidades
        if (cant > 0) Stock += cant; // solo cantidades positivas
    } // fin de la funcion

    void set_precio(double p) { // cambia el precio de venta
        if (p > 0) Precio = p; // solo precios positivos
    } // fin de la funcion

    void set_costo(double c) { // cambia el costo
        if (c > 0) Costo = c; // solo costos positivos
    } // fin de la funcion

    void dar_de_baja() { Activo = false; } // retira el producto del catalogo

    virtual void mostrar_info() const { // muestra el producto en una linea
        cout << "[" << Codigo << "] " << tipo() << ": " << Marca << " " << Modelo // codigo, tipo y nombre
             << " | " << detalle() << " | " << dinero(Precio) << " | Stock: " << Stock; // detalle, precio y stock
        if (stock_bajo()) cout << Color::ROJO << "  (STOCK BAJO)" << Color::RESET; // alerta de stock bajo
        cout << endl; // salto de linea
    } // fin de la funcion

    string serializar() const { // linea para guardar en archivo
        return tipo() + "|" + Codigo + "|" + limpiar_campo(Marca) + "|" + limpiar_campo(Modelo) // tipo, codigo y nombre
             + "|" + to_string(Precio) + "|" + to_string(Costo) + "|" + to_string(Stock) // precio, costo y stock
             + "|" + (Activo ? "1" : "0") + "|" + extras(); // estado y campos propios
    } // fin de la funcion
}; // fin de la clase Producto

class Laptop : public Producto { // producto tipo laptop
private: // datos internos
    string Procesador; // modelo de procesador
    int Ram_GB; // memoria RAM en GB
    string Almacenamiento; // disco

public: // interfaz publica
    Laptop(const string& cod, const string& marca, const string& modelo, double precio, double costo, int stock, // parametros base
           const string& cpu, int ram, const string& disco, bool activo = true) // parametros propios
        : Producto(cod, marca, modelo, precio, costo, stock, activo), // inicializa la base
          Procesador(cpu), Ram_GB(ram), Almacenamiento(disco) {} // inicializa campos propios

    string tipo() const override { return "LAPTOP"; } // nombre de la categoria
    string detalle() const override { return Procesador + " | " + to_string(Ram_GB) + "GB RAM | " + Almacenamiento; } // especificaciones
    string extras() const override { return limpiar_campo(Procesador) + "|" + to_string(Ram_GB) + "|" + limpiar_campo(Almacenamiento); } // campos para guardar
}; // fin de la clase Laptop

class Celular : public Producto { // producto tipo celular
private: // datos internos
    int Almacenamiento_GB; // memoria interna en GB
    string Camara; // camara principal
    int Bateria_mAh; // bateria

public: // interfaz publica
    Celular(const string& cod, const string& marca, const string& modelo, double precio, double costo, int stock, // parametros base
            int rom, const string& camara, int bat, bool activo = true) // parametros propios
        : Producto(cod, marca, modelo, precio, costo, stock, activo), // inicializa la base
          Almacenamiento_GB(rom), Camara(camara), Bateria_mAh(bat) {} // inicializa campos propios

    string tipo() const override { return "CELULAR"; } // nombre de la categoria
    string detalle() const override { return to_string(Almacenamiento_GB) + "GB | " + Camara + " | " + to_string(Bateria_mAh) + "mAh"; } // especificaciones
    string extras() const override { return to_string(Almacenamiento_GB) + "|" + limpiar_campo(Camara) + "|" + to_string(Bateria_mAh); } // campos para guardar
}; // fin de la clase Celular


#include <list> // contenedor list

struct ItemVenta { // linea de una venta
    string codigo; // codigo del producto
    string descripcion; // marca y modelo
    int cantidad; // unidades vendidas
    double precio_unitario; // precio de venta
    double costo_unitario; // costo al momento de la venta
    double subtotal; // cantidad por precio
}; // fin de la estructura

struct ItemCompra { // linea de una compra
    string codigo; // codigo del producto
    string descripcion; // marca y modelo
    int cantidad; // unidades compradas
    double costo_unitario; // costo de compra
    double subtotal; // cantidad por costo
}; // fin de la estructura

inline double subtotal_items(const list<ItemVenta>& items) { // suma los subtotales del carrito
    double t = 0; // acumulador
    for (const auto& i : items) t += i.subtotal; // suma cada linea
    return redondear2(t); // devuelve el total redondeado
} // fin de la funcion

inline double calcular_descuento(double bruto, double pct) { // calcula el monto del descuento
    return redondear2(bruto * pct / 100.0); // aplica el porcentaje
} // fin de la funcion

class Transaccion { // datos comunes de ventas y compras
protected: // visible para las clases hijas
    string Id_transaccion; // identificador
    Fecha Fecha_transaccion; // fecha
    double Monto_total; // monto final
    string Estado; // Completada o Anulada

public: // interfaz publica
    Transaccion(const string& id, const Fecha& fecha, double monto, const string& est) // constructor
        : Id_transaccion(id), Fecha_transaccion(fecha), Monto_total(monto), Estado(est) {} // inicializa campos

    const string& get_id_transaccion() const { return Id_transaccion; } // devuelve el id
    const Fecha& get_fecha_transaccion() const { return Fecha_transaccion; } // devuelve la fecha
    double get_monto_total() const { return Monto_total; } // devuelve el monto
    const string& get_estado() const { return Estado; } // devuelve el estado
    void set_estado(const string& e) { Estado = e; } // cambia el estado
}; // fin de la clase Transaccion

class Venta : public Transaccion { // venta a un cliente
private: // datos internos
    Cliente cliente; // cliente de la venta
    Empleado empleado; // cajero de la venta
    list<ItemVenta> Items; // productos vendidos
    string Tipo_Comprobante; // BOLETA o FACTURA
    string Medio_pago; // efectivo, tarjeta, yape o plin
    double Descuento_pct; // porcentaje de descuento
    double Descuento_monto; // monto del descuento
    double Monto_pagado; // monto entregado por el cliente
    double Vuelto; // cambio devuelto

public: // interfaz publica
    Venta(const string& id, const Fecha& fecha, double monto, const string& est, // parametros de transaccion
          const Cliente& cli, const Empleado& emp, const list<ItemVenta>& items, const string& tipo_comp, // parametros de venta
          const string& medio, double desc_pct, double desc_monto, double pagado, double vuelto) // parametros de pago
        : Transaccion(id, fecha, monto, est), cliente(cli), empleado(emp), Items(items), // inicializa campos
          Tipo_Comprobante(tipo_comp), Medio_pago(medio), Descuento_pct(desc_pct), // inicializa campos
          Descuento_monto(desc_monto), Monto_pagado(pagado), Vuelto(vuelto) {} // inicializa campos

    const Cliente& get_cliente() const { return cliente; } // devuelve el cliente
    const Empleado& get_empleado() const { return empleado; } // devuelve el cajero
    const list<ItemVenta>& get_items() const { return Items; } // devuelve los items
    const string& get_tipo_comprobante() const { return Tipo_Comprobante; } // devuelve el tipo de comprobante
    const string& get_medio_pago() const { return Medio_pago; } // devuelve el medio de pago
    double get_descuento_pct() const { return Descuento_pct; } // devuelve el porcentaje de descuento
    double get_descuento_monto() const { return Descuento_monto; } // devuelve el monto de descuento
    double get_monto_pagado() const { return Monto_pagado; } // devuelve el monto pagado
    double get_vuelto() const { return Vuelto; } // devuelve el vuelto
    double get_subtotal_bruto() const { return redondear2(Monto_total + Descuento_monto); } // total antes del descuento

    double utilidad_estimada() const { // utilidad sin IGV menos el costo
        if (Estado != Config::ESTADO_COMPLETADA) return 0.0; // las anuladas no generan utilidad
        double costo = 0.0; // costo total de lo vendido
        for (const auto& i : Items) costo += i.costo_unitario * i.cantidad; // suma costo por cantidad
        return redondear2(Monto_total / (1.0 + Config::IGV) - costo); // base imponible menos costo
    } // fin de la funcion

    string serializar() const { // linea para guardar en archivo
        string items; // items unidos en texto
        for (const auto& i : Items) { // recorre los items
            if (!items.empty()) items += "~"; // separa items con tilde
            items += limpiar_campo(i.codigo) + "^" + limpiar_campo(i.descripcion) + "^" + to_string(i.cantidad) // codigo, descripcion y cantidad
                   + "^" + to_string(i.precio_unitario) + "^" + to_string(i.costo_unitario) + "^" + to_string(i.subtotal); // precio, costo y subtotal
        } // fin del bucle
        return Id_transaccion + "|" + Fecha_transaccion.texto() + "|" + to_string(Monto_total) + "|" + Estado // datos de transaccion
             + "|" + cliente.get_numero_documento() + "|" + empleado.get_codigo_empleado() + "|" + Tipo_Comprobante // cliente, cajero y comprobante
             + "|" + limpiar_campo(Medio_pago) + "|" + to_string(Descuento_pct) + "|" + to_string(Descuento_monto) // pago y descuento
             + "|" + to_string(Monto_pagado) + "|" + to_string(Vuelto) + "|" + items; // monto pagado, vuelto e items
    } // fin de la funcion
}; // fin de la clase Venta

class Compra : public Transaccion { // compra a un proveedor
private: // datos internos
    Proveedor proveedor; // proveedor de la compra
    Empleado empleado; // empleado que recibio
    list<ItemCompra> Items_comprados; // productos comprados
    string Tipo_Comprobante; // tipo de comprobante
    string Numero_factura_proveedor; // numero de factura del proveedor

public: // interfaz publica
    Compra(const string& id, const Fecha& fecha, double monto, const string& est, // parametros de transaccion
           const Proveedor& prov, const Empleado& emp, const list<ItemCompra>& items, // parametros de compra
           const string& tipo_comp, const string& num_factura) // comprobante y factura
        : Transaccion(id, fecha, monto, est), proveedor(prov), empleado(emp), // inicializa campos
          Items_comprados(items), Tipo_Comprobante(tipo_comp), Numero_factura_proveedor(num_factura) {} // inicializa campos

    const Proveedor& get_proveedor() const { return proveedor; } // devuelve el proveedor
    const Empleado& get_empleado() const { return empleado; } // devuelve el empleado
    const list<ItemCompra>& get_items_comprados() const { return Items_comprados; } // devuelve los items
    const string& get_tipo_comprobante() const { return Tipo_Comprobante; } // devuelve el tipo de comprobante
    const string& get_numero_factura_proveedor() const { return Numero_factura_proveedor; } // devuelve el numero de factura

    string serializar() const { // linea para guardar en archivo
        string items; // items unidos en texto
        for (const auto& i : Items_comprados) { // recorre los items
            if (!items.empty()) items += "~"; // separa items con tilde
            items += limpiar_campo(i.codigo) + "^" + limpiar_campo(i.descripcion) + "^" + to_string(i.cantidad) // codigo, descripcion y cantidad
                   + "^" + to_string(i.costo_unitario) + "^" + to_string(i.subtotal); // costo y subtotal
        } // fin del bucle
        return Id_transaccion + "|" + Fecha_transaccion.texto() + "|" + to_string(Monto_total) + "|" + Estado // datos de transaccion
             + "|" + proveedor.get_ruc() + "|" + empleado.get_codigo_empleado() + "|" + limpiar_campo(Tipo_Comprobante) // proveedor, empleado y comprobante
             + "|" + limpiar_campo(Numero_factura_proveedor) + "|" + items; // factura e items
    } // fin de la funcion
}; // fin de la clase Compra


#include <memory> // unique_ptr
#include <list> // contenedor list
#include <map> // contenedor map
#include <fstream> // lectura y escritura de archivos
#include <functional> // function

class Tienda { // guarda todos los datos y la logica de negocio
private: // datos internos
    vector<unique_ptr<Producto>> inventario; // productos de todas las categorias
    list<Empleado> empleados; // empleados registrados
    list<Proveedor> proveedores; // proveedores registrados
    list<Cliente> clientes; // clientes registrados
    list<Venta> ventas; // historial de ventas
    list<Compra> compras; // historial de compras
    int contador_laptop = 1; // siguiente numero de codigo de laptop
    int contador_celular = 1; // siguiente numero de codigo de celular
    int contador_boleta = 1; // siguiente numero de boleta
    int contador_factura = 1; // siguiente numero de factura

    const string ARCH_PRODUCTOS = "datos_productos.csv"; // archivo de productos
    const string ARCH_EMPLEADOS = "datos_empleados.csv"; // archivo de empleados
    const string ARCH_PROVEEDORES = "datos_proveedores.csv"; // archivo de proveedores
    const string ARCH_CLIENTES = "datos_clientes.csv"; // archivo de clientes
    const string ARCH_VENTAS = "datos_ventas.csv"; // archivo de ventas
    const string ARCH_COMPRAS = "datos_compras.csv"; // archivo de compras
    const string ARCH_CONFIG = "datos_config.csv"; // archivo de contadores

    static string codigo_con_prefijo(char prefijo, int n) { // arma codigos como L04 o C12
        ostringstream o; // flujo de texto temporal
        o << prefijo << setfill('0') << setw(2) << n; // prefijo y numero con dos digitos
        return o.str(); // devuelve el codigo
    } // fin de la funcion

    bool escribir_archivo(const string& nombre, const function<void(ostream&)>& escritor) const { // escribe un archivo
        ofstream archivo(nombre); // abre el archivo para escribir
        if (!archivo.is_open()) { // si no se pudo abrir
            cerr << "No se pudo escribir el archivo " << nombre << endl; // avisa el error
            return false; // indica fallo
        } // fin del if
        escritor(archivo); // escribe el contenido
        return true; // indica exito
    } // fin de la funcion

    bool leer_registros(const string& nombre, vector<vector<string>>& registros) const { // lee un archivo en registros
        ifstream archivo(nombre); // abre el archivo para leer
        if (!archivo.is_open()) return false; // si no existe devuelve falso
        string linea; // linea leida
        while (getline(archivo, linea)) { // lee linea por linea
            linea = recortar(linea); // quita espacios y retornos
            if (!linea.empty()) registros.push_back(dividir(linea, '|')); // guarda los campos de la linea
        } // fin del bucle
        return true; // lectura correcta
    } // fin de la funcion

    void reiniciar() { // borra todos los datos en memoria
        inventario.clear(); // vacia productos
        empleados.clear(); // vacia empleados
        proveedores.clear(); // vacia proveedores
        clientes.clear(); // vacia clientes
        ventas.clear(); // vacia ventas
        compras.clear(); // vacia compras
        contador_laptop = contador_celular = contador_boleta = contador_factura = 1; // reinicia contadores
    } // fin de la funcion

    void cargar_datos_iniciales() { // datos de ejemplo para el primer uso
        empleados.push_back(Empleado("DNI", "72341122", "Quispe", "Mamani", "Carlos", "951234567", "Av. Floral 120", // datos personales
                                     "EMP01", "Administrador y Cajero", Config::ROL_ADMIN, Fecha(15, 3, 2024), 1500.0, true, hash_clave("admin123"))); // datos laborales
        empleados.push_back(Empleado("DNI", "70984433", "Condori", "Flores", "Lucia", "952987654", "Jr. Puno 340", // datos personales
                                     "EMP02", "Asesora de Ventas", Config::ROL_CAJERO, Fecha(1, 8, 2025), 1600.0, true, hash_clave("caja123"))); // datos laborales
        proveedores.push_back(Proveedor("20100106963", "Grupo Deltron S.A.", "Ing. Marco Ramos", "01-4150100", "Av. Manuel Echeandia 904, Lima", true)); // proveedor 1
        proveedores.push_back(Proveedor("20254139943", "Ingram Micro Peru S.A.C.", "Lic. Elena Ruiz", "01-6169000", "Av. Rivera Navarrete 501, San Isidro", true)); // proveedor 2
        proveedores.push_back(Proveedor("20508565934", "Intcomex del Peru S.A.C.", "Sr. David Tello", "01-7128000", "Av. Los Ingenieros 148, Ate", true)); // proveedor 3
        inventario.push_back(make_unique<Laptop>("L01", "Lenovo", "ThinkPad E14", 3200.0, 2700.0, 6, "Core i7 13va", 16, "512GB SSD")); // laptop 1
        inventario.push_back(make_unique<Laptop>("L02", "ASUS", "TUF Gaming F15", 4100.0, 3500.0, 3, "Ryzen 7 7735HS", 16, "1TB SSD")); // laptop 2
        inventario.push_back(make_unique<Laptop>("L03", "HP", "Pavilion 15", 2650.0, 2200.0, 8, "Core i5 12va", 8, "512GB SSD")); // laptop 3
        inventario.push_back(make_unique<Celular>("C01", "Samsung", "Galaxy S24", 3800.0, 3300.0, 5, 256, "50MP", 4000)); // celular 1
        inventario.push_back(make_unique<Celular>("C02", "Xiaomi", "Redmi Note 13", 950.0, 750.0, 10, 128, "108MP", 5000)); // celular 2
        inventario.push_back(make_unique<Celular>("C03", "Apple", "iPhone 15", 4300.0, 3800.0, 4, 128, "48MP", 3349)); // celular 3
        contador_laptop = 4; // proxima laptop sera L04
        contador_celular = 4; // proximo celular sera C04
    } // fin de la funcion

    bool cargar() { // carga los datos desde los archivos
        vector<vector<string>> r; // registros leidos

        if (leer_registros(ARCH_EMPLEADOS, r)) { // lee empleados
            for (const auto& c : r) { // recorre cada registro
                try { // protege contra datos danados
                    Fecha f; // fecha de ingreso
                    if (c.size() != 14 || !Fecha::desde_texto(c[10], f)) continue; // salta registros invalidos
                    empleados.push_back(Empleado(c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8], c[9], f, stod(c[11]), c[12] == "1", c[13])); // crea el empleado
                } catch (...) { // si falla la conversion
                } // fin del catch
            } // fin del bucle
        } // fin del if

        r.clear(); // limpia los registros
        if (leer_registros(ARCH_PROVEEDORES, r)) { // lee proveedores
            for (const auto& c : r) { // recorre cada registro
                if (c.size() == 6) proveedores.push_back(Proveedor(c[0], c[1], c[2], c[3], c[4], c[5] == "1")); // crea el proveedor
            } // fin del bucle
        } // fin del if

        r.clear(); // limpia los registros
        if (leer_registros(ARCH_CLIENTES, r)) { // lee clientes
            for (const auto& c : r) { // recorre cada registro
                if (c.size() == 8) clientes.push_back(Cliente(c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7])); // crea el cliente
            } // fin del bucle
        } // fin del if

        r.clear(); // limpia los registros
        if (leer_registros(ARCH_PRODUCTOS, r)) { // lee productos
            for (const auto& c : r) { // recorre cada registro
                try { // protege contra datos danados
                    if (c.size() != 11) continue; // salta registros invalidos
                    double precio = stod(c[4]), costo = stod(c[5]); // precio y costo
                    int stock = stoi(c[6]); // stock
                    bool activo = (c[7] == "1"); // estado
                    int numero = stoi(c[1].substr(1)) + 1; // siguiente numero de codigo
                    if (c[0] == "LAPTOP") { // si es laptop
                        inventario.push_back(make_unique<Laptop>(c[1], c[2], c[3], precio, costo, stock, c[8], stoi(c[9]), c[10], activo)); // crea la laptop
                        contador_laptop = max(contador_laptop, numero); // actualiza el contador
                    } else if (c[0] == "CELULAR") { // si es celular
                        inventario.push_back(make_unique<Celular>(c[1], c[2], c[3], precio, costo, stock, stoi(c[8]), c[9], stoi(c[10]), activo)); // crea el celular
                        contador_celular = max(contador_celular, numero); // actualiza el contador
                    } // fin del if
                } catch (...) { // si falla la conversion
                } // fin del catch
            } // fin del bucle
        } // fin del if

        r.clear(); // limpia los registros
        if (leer_registros(ARCH_CONFIG, r)) { // lee contadores
            for (const auto& c : r) { // recorre cada registro
                try { // protege contra datos danados
                    if (c.size() != 2) continue; // salta registros invalidos
                    int v = stoi(c[1]); // valor del contador
                    if (c[0] == "boleta") contador_boleta = v; // contador de boletas
                    else if (c[0] == "factura") contador_factura = v; // contador de facturas
                    else if (c[0] == "laptop") contador_laptop = max(contador_laptop, v); // contador de laptops
                    else if (c[0] == "celular") contador_celular = max(contador_celular, v); // contador de celulares
                } catch (...) { // si falla la conversion
                } // fin del catch
            } // fin del bucle
        } // fin del if

        r.clear(); // limpia los registros
        if (leer_registros(ARCH_VENTAS, r)) { // lee ventas
            for (const auto& c : r) { // recorre cada registro
                try { // protege contra datos danados
                    Fecha f; // fecha de la venta
                    if (c.size() != 13 || !Fecha::desde_texto(c[1], f)) continue; // salta registros invalidos
                    Empleado* emp = buscar_empleado(c[5]); // busca al cajero
                    if (emp == nullptr) continue; // salta si no existe
                    Cliente* cli = buscar_cliente(c[4]); // busca al cliente
                    Cliente respaldo("DNI", c[4], "", "", "DESCONOCIDO", "", "", "General"); // cliente de respaldo
                    list<ItemVenta> items; // items de la venta
                    for (const string& it : dividir(c[12], '~')) { // recorre cada item
                        vector<string> p = dividir(it, '^'); // separa los campos del item
                        if (p.size() == 6) items.push_back(ItemVenta{p[0], p[1], stoi(p[2]), stod(p[3]), stod(p[4]), stod(p[5])}); // crea el item
                    } // fin del bucle
                    ventas.push_back(Venta(c[0], f, stod(c[2]), c[3], cli ? *cli : respaldo, *emp, items, c[6], c[7], stod(c[8]), stod(c[9]), stod(c[10]), stod(c[11]))); // crea la venta
                } catch (...) { // si falla la conversion
                } // fin del catch
            } // fin del bucle
        } // fin del if

        r.clear(); // limpia los registros
        if (leer_registros(ARCH_COMPRAS, r)) { // lee compras
            for (const auto& c : r) { // recorre cada registro
                try { // protege contra datos danados
                    Fecha f; // fecha de la compra
                    if (c.size() != 9 || !Fecha::desde_texto(c[1], f)) continue; // salta registros invalidos
                    Empleado* emp = buscar_empleado(c[5]); // busca al empleado
                    Proveedor* prov = buscar_proveedor(c[4]); // busca al proveedor
                    if (emp == nullptr || prov == nullptr) continue; // salta si no existen
                    list<ItemCompra> items; // items de la compra
                    for (const string& it : dividir(c[8], '~')) { // recorre cada item
                        vector<string> p = dividir(it, '^'); // separa los campos del item
                        if (p.size() == 5) items.push_back(ItemCompra{p[0], p[1], stoi(p[2]), stod(p[3]), stod(p[4])}); // crea el item
                    } // fin del bucle
                    compras.push_back(Compra(c[0], f, stod(c[2]), c[3], *prov, *emp, items, c[6], c[7])); // crea la compra
                } catch (...) { // si falla la conversion
                } // fin del catch
            } // fin del bucle
        } // fin del if

        return !inventario.empty() && !empleados.empty(); // hay datos usables
    } // fin de la funcion

public: // interfaz publica
    Tienda() { // constructor
        if (!cargar()) { // si no hay datos guardados validos
            reiniciar(); // limpia lo que se haya cargado a medias
            cargar_datos_iniciales(); // usa los datos de ejemplo
            guardar(); // crea los archivos
        } // fin del if
    } // fin del constructor

    bool guardar() const { // guarda todos los datos en archivos
        bool ok = true; // resultado general
        ok = escribir_archivo(ARCH_PRODUCTOS, [&](ostream& o) { for (const auto& p : inventario) o << p->serializar() << "\n"; }) && ok; // guarda productos
        ok = escribir_archivo(ARCH_EMPLEADOS, [&](ostream& o) { for (const auto& e : empleados) o << e.serializar() << "\n"; }) && ok; // guarda empleados
        ok = escribir_archivo(ARCH_PROVEEDORES, [&](ostream& o) { for (const auto& p : proveedores) o << p.serializar() << "\n"; }) && ok; // guarda proveedores
        ok = escribir_archivo(ARCH_CLIENTES, [&](ostream& o) { for (const auto& c : clientes) o << c.serializar() << "\n"; }) && ok; // guarda clientes
        ok = escribir_archivo(ARCH_VENTAS, [&](ostream& o) { for (const auto& v : ventas) o << v.serializar() << "\n"; }) && ok; // guarda ventas
        ok = escribir_archivo(ARCH_COMPRAS, [&](ostream& o) { for (const auto& c : compras) o << c.serializar() << "\n"; }) && ok; // guarda compras
        ok = escribir_archivo(ARCH_CONFIG, [&](ostream& o) { // guarda contadores
            o << "laptop|" << contador_laptop << "\n"; // contador de laptops
            o << "celular|" << contador_celular << "\n"; // contador de celulares
            o << "boleta|" << contador_boleta << "\n"; // contador de boletas
            o << "factura|" << contador_factura << "\n"; // contador de facturas
        }) && ok; // fin de la escritura de contadores
        return ok; // devuelve el resultado
    } // fin de la funcion

    const vector<unique_ptr<Producto>>& get_inventario() const { return inventario; } // devuelve el inventario
    const list<Empleado>& get_empleados() const { return empleados; } // devuelve los empleados
    const list<Proveedor>& get_proveedores() const { return proveedores; } // devuelve los proveedores
    const list<Venta>& get_ventas() const { return ventas; } // devuelve las ventas
    const list<Compra>& get_compras() const { return compras; } // devuelve las compras

    Producto* buscar_producto(const string& cod, bool solo_activos = true) { // busca un producto por codigo
        string c = a_mayuscula(cod); // codigo en mayusculas
        for (auto& p : inventario) { // recorre el inventario
            if (p->get_codigo() == c && (!solo_activos || p->esta_activo())) return p.get(); // devuelve si coincide
        } // fin del bucle
        return nullptr; // no encontrado
    } // fin de la funcion

    vector<Producto*> buscar_por_texto(const string& texto) { // busca por marca o modelo
        vector<Producto*> r; // resultados
        string t = a_minuscula(texto); // texto en minusculas
        for (auto& p : inventario) { // recorre el inventario
            string nombre = a_minuscula(p->get_marca() + " " + p->get_modelo()); // marca y modelo en minusculas
            if (p->esta_activo() && nombre.find(t) != string::npos) r.push_back(p.get()); // agrega si contiene el texto
        } // fin del bucle
        return r; // devuelve los resultados
    } // fin de la funcion

    string nuevo_codigo_laptop() { return codigo_con_prefijo('L', contador_laptop++); } // genera un codigo de laptop sin repetir
    string nuevo_codigo_celular() { return codigo_con_prefijo('C', contador_celular++); } // genera un codigo de celular sin repetir

    int cantidad_stock_bajo() const { // cuenta productos con stock bajo
        int n = 0; // contador
        for (const auto& p : inventario) { // recorre el inventario
            if (p->esta_activo() && p->stock_bajo()) n++; // cuenta si esta activo y con stock bajo
        } // fin del bucle
        return n; // devuelve la cantidad
    } // fin de la funcion

    double valor_inventario_costo() const { // valor del inventario al costo
        double t = 0; // acumulador
        for (const auto& p : inventario) { // recorre el inventario
            if (p->esta_activo()) t += p->get_costo() * p->get_stock(); // suma costo por stock
        } // fin del bucle
        return redondear2(t); // devuelve el total
    } // fin de la funcion

    double valor_inventario_venta() const { // valor del inventario al precio de venta
        double t = 0; // acumulador
        for (const auto& p : inventario) { // recorre el inventario
            if (p->esta_activo()) t += p->get_precio() * p->get_stock(); // suma precio por stock
        } // fin del bucle
        return redondear2(t); // devuelve el total
    } // fin de la funcion

    Empleado* buscar_empleado(const string& codigo) { // busca un empleado por codigo
        string c = a_mayuscula(codigo); // codigo en mayusculas
        for (auto& e : empleados) { // recorre los empleados
            if (e.get_codigo_empleado() == c) return &e; // devuelve si coincide
        } // fin del bucle
        return nullptr; // no encontrado
    } // fin de la funcion

    bool existe_empleado_documento(const string& doc) const { // verifica si ya hay un empleado con ese documento
        for (const auto& e : empleados) { // recorre los empleados
            if (e.get_numero_documento() == doc) return true; // coincide
        } // fin del bucle
        return false; // no existe
    } // fin de la funcion

    string nuevo_codigo_empleado() const { // genera un codigo de empleado nuevo
        int mayor = 0; // mayor numero encontrado
        for (const auto& e : empleados) { // recorre los empleados
            try { // protege contra codigos raros
                mayor = max(mayor, stoi(e.get_codigo_empleado().substr(3))); // toma el numero del codigo
            } catch (...) { // si falla la conversion
            } // fin del catch
        } // fin del bucle
        ostringstream o; // flujo de texto temporal
        o << "EMP" << setfill('0') << setw(2) << mayor + 1; // arma el codigo
        return o.str(); // devuelve el codigo
    } // fin de la funcion

    void agregar_empleado(const Empleado& e) { // registra un empleado nuevo
        empleados.push_back(e); // lo agrega a la lista
        guardar(); // guarda los cambios
    } // fin de la funcion

    int administradores_activos() const { // cuenta administradores activos
        int n = 0; // contador
        for (const auto& e : empleados) { // recorre los empleados
            if (e.get_estado() && e.es_admin()) n++; // cuenta si esta activo y es admin
        } // fin del bucle
        return n; // devuelve la cantidad
    } // fin de la funcion

    Cliente* buscar_cliente(const string& doc) { // busca un cliente por documento
        for (auto& c : clientes) { // recorre los clientes
            if (c.get_numero_documento() == doc) return &c; // devuelve si coincide
        } // fin del bucle
        return nullptr; // no encontrado
    } // fin de la funcion

    Cliente* registrar_cliente(const Cliente& c) { // registra un cliente nuevo
        clientes.push_back(c); // lo agrega a la lista
        guardar(); // guarda los cambios
        return &clientes.back(); // devuelve su direccion
    } // fin de la funcion

    Proveedor* buscar_proveedor(const string& ruc) { // busca un proveedor por ruc
        for (auto& p : proveedores) { // recorre los proveedores
            if (p.get_ruc() == ruc) return &p; // devuelve si coincide
        } // fin del bucle
        return nullptr; // no encontrado
    } // fin de la funcion

    Proveedor* proveedor_por_indice(int indice) { // devuelve el proveedor en una posicion (desde 1)
        if (indice < 1 || indice > (int)proveedores.size()) return nullptr; // fuera de rango
        auto it = proveedores.begin(); // inicio de la lista
        advance(it, indice - 1); // avanza hasta la posicion
        return &(*it); // devuelve su direccion
    } // fin de la funcion

    Proveedor* agregar_proveedor(const Proveedor& p) { // registra un proveedor nuevo
        proveedores.push_back(p); // lo agrega a la lista
        guardar(); // guarda los cambios
        return &proveedores.back(); // devuelve su direccion
    } // fin de la funcion

    const Venta* finalizar_venta(const string& tipo_comp, const Cliente& cli, const Empleado& emp, // registra y cobra una venta
                                 const list<ItemVenta>& carrito, const string& medio, double desc_pct, // carrito y descuento
                                 double monto_pagado, const Fecha& fecha) { // pago y fecha
        for (const auto& it : carrito) { // valida el stock de cada linea
            Producto* p = buscar_producto(it.codigo); // busca el producto
            if (p == nullptr || p->get_stock() < it.cantidad) return nullptr; // cancela si no alcanza el stock
        } // fin del bucle
        for (const auto& it : carrito) { // descuenta el stock recien ahora
            buscar_producto(it.codigo)->reducir_stock(it.cantidad); // descuenta las unidades
        } // fin del bucle
        double bruto = subtotal_items(carrito); // total antes del descuento
        double desc = calcular_descuento(bruto, desc_pct); // monto del descuento
        double total = redondear2(bruto - desc); // total a cobrar
        double vuelto = (monto_pagado > total) ? redondear2(monto_pagado - total) : 0.0; // cambio a devolver
        string id = (tipo_comp == "BOLETA") // elige la serie segun el comprobante
            ? Config::SERIE_BOLETA + to_string(1000 + contador_boleta++) // numero de boleta
            : Config::SERIE_FACTURA + to_string(1000 + contador_factura++); // numero de factura
        ventas.push_back(Venta(id, fecha, total, Config::ESTADO_COMPLETADA, cli, emp, carrito, tipo_comp, medio, desc_pct, desc, monto_pagado, vuelto)); // guarda la venta
        actualizar_categoria_cliente(cli.get_numero_documento()); // revisa si pasa a cliente frecuente
        guardar(); // guarda los cambios
        return &ventas.back(); // devuelve la venta creada
    } // fin de la funcion

    void actualizar_categoria_cliente(const string& doc) { // sube a Frecuente segun compras realizadas
        Cliente* c = buscar_cliente(doc); // busca al cliente
        if (c == nullptr || c->get_categoria() != "General") return; // solo actualiza clientes generales
        int n = 0; // compras completadas
        for (const auto& v : ventas) { // recorre las ventas
            if (v.get_estado() == Config::ESTADO_COMPLETADA && v.get_cliente().get_numero_documento() == doc) n++; // cuenta las del cliente
        } // fin del bucle
        if (n >= Config::COMPRAS_PARA_FRECUENTE) c->set_categoria("Frecuente"); // lo promueve
    } // fin de la funcion

    string anular_venta(const string& id) { // anula una venta y devuelve el stock
        string idm = a_mayuscula(id); // id en mayusculas
        for (auto& v : ventas) { // recorre las ventas
            if (v.get_id_transaccion() != idm) continue; // salta las que no coinciden
            if (v.get_estado() == Config::ESTADO_ANULADA) return "La venta ya estaba anulada."; // ya anulada
            for (const auto& it : v.get_items()) { // recorre los items
                Producto* p = buscar_producto(it.codigo, false); // busca el producto aunque este dado de baja
                if (p != nullptr) p->aumentar_stock(it.cantidad); // devuelve el stock
            } // fin del bucle
            v.set_estado(Config::ESTADO_ANULADA); // marca la venta como anulada
            guardar(); // guarda los cambios
            return ""; // vacio significa exito
        } // fin del bucle
        return "No existe una venta con ese numero."; // no encontrada
    } // fin de la funcion

    const Compra* registrar_compra(const Proveedor& prov, const Empleado& emp, const list<ItemCompra>& items, // registra una compra
                                   vector<unique_ptr<Producto>>& nuevos, const string& num_factura, const Fecha& fecha) { // productos nuevos y factura
        for (auto& n : nuevos) inventario.push_back(move(n)); // incorpora los productos nuevos al catalogo
        nuevos.clear(); // vacia la lista pendiente
        double total = 0.0; // costo total de la compra
        for (const auto& it : items) { // recorre los items
            Producto* p = buscar_producto(it.codigo, false); // busca el producto
            if (p != nullptr) { // si existe
                p->aumentar_stock(it.cantidad); // suma el stock
                p->set_costo(it.costo_unitario); // actualiza el costo
            } // fin del if
            total += it.subtotal; // suma al total
        } // fin del bucle
        string id = "CP001-" + to_string(2000 + compras.size() + 1); // numero interno de compra
        compras.push_back(Compra(id, fecha, redondear2(total), Config::ESTADO_COMPLETADA, prov, emp, items, "Factura Proveedor", num_factura)); // guarda la compra
        guardar(); // guarda los cambios
        return &compras.back(); // devuelve la compra creada
    } // fin de la funcion

    vector<const Venta*> ventas_en_rango(const Fecha& desde, const Fecha& hasta) const { // ventas completadas en un periodo
        vector<const Venta*> r; // resultados
        for (const auto& v : ventas) { // recorre las ventas
            bool vigente = v.get_estado() == Config::ESTADO_COMPLETADA; // solo ventas no anuladas
            if (vigente && v.get_fecha_transaccion() >= desde && v.get_fecha_transaccion() <= hasta) r.push_back(&v); // agrega si esta en rango
        } // fin del bucle
        return r; // devuelve los resultados
    } // fin de la funcion

    vector<pair<string, int>> ranking_productos() const { // unidades vendidas por producto
        map<string, int> acumulado; // unidades por producto
        for (const auto& v : ventas) { // recorre las ventas
            if (v.get_estado() != Config::ESTADO_COMPLETADA) continue; // ignora las anuladas
            for (const auto& i : v.get_items()) acumulado["[" + i.codigo + "] " + i.descripcion] += i.cantidad; // suma las unidades
        } // fin del bucle
        vector<pair<string, int>> r(acumulado.begin(), acumulado.end()); // pasa a vector
        sort(r.begin(), r.end(), [](const pair<string, int>& a, const pair<string, int>& b) { return a.second > b.second; }); // ordena de mayor a menor
        return r; // devuelve el ranking
    } // fin de la funcion

    map<string, pair<int, double>> ventas_por_cajero() const { // cantidad y monto vendido por cajero
        map<string, pair<int, double>> r; // resultado
        for (const auto& v : ventas) { // recorre las ventas
            if (v.get_estado() != Config::ESTADO_COMPLETADA) continue; // ignora las anuladas
            string clave = v.get_empleado().get_codigo_empleado() + " - " + v.get_empleado().nombre_completo(); // codigo y nombre
            r[clave].first += 1; // suma una venta
            r[clave].second += v.get_monto_total(); // suma el monto
        } // fin del bucle
        return r; // devuelve el resultado
    } // fin de la funcion

    double utilidad_en_rango(const Fecha& desde, const Fecha& hasta) const { // utilidad estimada en un periodo
        double t = 0; // acumulador
        for (const Venta* v : ventas_en_rango(desde, hasta)) t += v->utilidad_estimada(); // suma la utilidad de cada venta
        return redondear2(t); // devuelve el total
    } // fin de la funcion
}; // fin de la clase Tienda



class Aplicacion { // interfaz de consola del sistema
private: // datos internos
    Tienda tienda; // datos y logica del negocio
    string codigo_activo; // codigo del empleado con sesion iniciada

    const Empleado& usuario() { return *tienda.buscar_empleado(codigo_activo); } // empleado con sesion activa

    bool exigir_admin() { // verifica que el usuario sea administrador
        if (usuario().es_admin()) return true; // si es admin permite continuar
        cout << Color::ROJO << "Esta opcion es solo para administradores." << Color::RESET << endl; // avisa el bloqueo
        pausa(); // espera al usuario
        return false; // no permite continuar
    } // fin de la funcion

    static string centrar(const string& t, size_t ancho) { // centra un texto en un ancho dado
        if (t.size() >= ancho) return t; // si no cabe lo deja igual
        return string((ancho - t.size()) / 2, ' ') + t; // agrega espacios a la izquierda
    } // fin de la funcion

    bool iniciar_sesion() { // pide codigo y clave
        limpiar(); // limpia la pantalla
        cout << Color::CIAN << "========================================================\n"; // linea superior
        cout << "                CONTROL DE ACCESO (LOGIN)               \n"; // titulo
        cout << "========================================================" << Color::RESET << "\n"; // linea inferior
        cout << "Usuarios registrados:\n"; // encabezado de la lista
        cout << "Clave para EMP01: admin123\n";
        cout << "Clave para EMP02: caja123\n";
        for (const auto& e : tienda.get_empleados()) { // recorre los empleados
            if (e.get_estado()) cout << "  " << e.get_codigo_empleado() << " - " << e.nombre_completo() << " (" << e.get_cargo() << ")\n"; // muestra los activos
        } // fin del bucle
        cout << "\n"; // linea en blanco
        string codigo = a_mayuscula(leer_texto("Codigo de empleado (o SALIR): ")); // lee el codigo
        if (codigo == "SALIR") exit(0); // permite cerrar el programa
        Empleado* emp = tienda.buscar_empleado(codigo); // busca al empleado
        if (emp == nullptr) { // si no existe
            cout << Color::ROJO << "Codigo no registrado en el sistema." << Color::RESET << "\n"; // avisa el error
            pausa(); // espera al usuario
            return false; // login fallido
        } // fin del if
        if (!emp->get_estado()) { // si esta inactivo
            cout << Color::ROJO << "El usuario se encuentra inactivo." << Color::RESET << "\n"; // avisa el error
            pausa(); // espera al usuario
            return false; // login fallido
        } // fin del if
        string clave = leer_texto("Clave: "); // lee la clave
        if (!emp->clave_correcta(clave)) { // si la clave no coincide
            cout << Color::ROJO << "Clave incorrecta." << Color::RESET << "\n"; // avisa el error
            pausa(); // espera al usuario
            return false; // login fallido
        } // fin del if
        codigo_activo = emp->get_codigo_empleado(); // guarda la sesion
        cout << Color::VERDE << "Acceso autorizado: " << emp->nombre_completo() << " [" << emp->get_cargo() << "]" << Color::RESET << "\n"; // confirma el acceso
        esperar(1000); // pausa corta
        return true; // login correcto
    } // fin de la funcion

    void mostrar_catalogo() { // muestra el catalogo agrupado por categoria
        cout << "\n========================================================================\n"; // linea superior
        cout << "                          CATALOGO Y STOCK ACTUAL                       \n"; // titulo
        cout << "========================================================================\n"; // linea inferior
        vector<string> tipos; // categorias encontradas
        for (const auto& p : tienda.get_inventario()) { // recorre el inventario
            if (p->esta_activo() && find(tipos.begin(), tipos.end(), p->tipo()) == tipos.end()) tipos.push_back(p->tipo()); // registra la categoria
        } // fin del bucle
        for (const string& t : tipos) { // recorre cada categoria
            cout << Color::AMARILLO << "--- " << t << " ---" << Color::RESET << "\n"; // titulo de categoria
            for (const auto& p : tienda.get_inventario()) { // recorre el inventario
                if (p->esta_activo() && p->tipo() == t) p->mostrar_info(); // muestra el producto
            } // fin del bucle
        } // fin del bucle
        cout << "========================================================================\n"; // linea final
        int bajos = tienda.cantidad_stock_bajo(); // productos con stock bajo
        if (bajos > 0) cout << Color::ROJO << "ALERTA: " << bajos << " producto(s) con stock bajo (" << Config::STOCK_MINIMO << " o menos)." << Color::RESET << "\n"; // muestra la alerta
    } // fin de la funcion

    void buscar_y_mostrar() { // busca productos por marca o modelo
        string texto = leer_texto_no_vacio("Buscar por marca o modelo: "); // lee el texto
        vector<Producto*> r = tienda.buscar_por_texto(texto); // busca coincidencias
        if (r.empty()) { // si no hay resultados
            cout << Color::ROJO << "No se encontraron productos." << Color::RESET << "\n"; // avisa
            return; // termina
        } // fin del if
        for (const Producto* p : r) p->mostrar_info(); // muestra cada resultado
    } // fin de la funcion

    void mostrar_carrito(const list<ItemVenta>& carrito) { // muestra el carrito actual
        cout << "\n--- CARRITO ---\n"; // titulo
        if (carrito.empty()) { // si esta vacio
            cout << "(vacio)\n"; // avisa
            return; // termina
        } // fin del if
        cout << left << setw(8) << "CODIGO" << setw(32) << "DESCRIPCION" << setw(6) << "CANT" // encabezado izquierdo
             << right << setw(12) << "P.UNIT" << setw(12) << "IMPORTE" << "\n"; // encabezado derecho
        for (const auto& i : carrito) { // recorre las lineas
            cout << left << setw(8) << i.codigo << setw(32) << i.descripcion.substr(0, 30) << setw(6) << i.cantidad // codigo, descripcion y cantidad
                 << right << setw(12) << dinero(i.precio_unitario) << setw(12) << dinero(i.subtotal) << "\n"; // precio e importe
        } // fin del bucle
        cout << left; // restaura la alineacion
        cout << "Subtotal: " << dinero(subtotal_items(carrito)) << "\n"; // muestra el subtotal
    } // fin de la funcion

    void agregar_al_carrito(list<ItemVenta>& carrito, const string& cod) { // agrega un producto al carrito
        Producto* p = tienda.buscar_producto(cod); // busca el producto activo
        if (p == nullptr) { // si no existe
            cout << Color::ROJO << "Codigo inexistente o producto dado de baja." << Color::RESET << "\n"; // avisa
            return; // termina
        } // fin del if
        int en_carrito = 0; // unidades ya en el carrito
        for (const auto& i : carrito) { // recorre el carrito
            if (i.codigo == p->get_codigo()) en_carrito += i.cantidad; // suma las unidades del mismo producto
        } // fin del bucle
        int disponible = p->get_stock() - en_carrito; // unidades que aun se pueden agregar
        if (disponible <= 0) { // si no queda stock
            cout << Color::ROJO << "No hay mas stock disponible de este producto." << Color::RESET << "\n"; // avisa
            return; // termina
        } // fin del if
        int cant = leer_entero_positivo("Cantidad deseada: "); // lee la cantidad
        if (cant > disponible) { // si pide mas de lo disponible
            cout << Color::ROJO << "Stock insuficiente. Disponible: " << disponible << Color::RESET << "\n"; // avisa
            return; // termina
        } // fin del if
        for (auto& i : carrito) { // busca si el producto ya esta en el carrito
            if (i.codigo == p->get_codigo()) { // si ya esta
                i.cantidad += cant; // acumula la cantidad
                i.subtotal = redondear2(i.cantidad * i.precio_unitario); // recalcula el subtotal
                cout << Color::VERDE << "Cantidad actualizada: " << p->get_modelo() << " x" << i.cantidad << Color::RESET << "\n"; // confirma
                return; // termina
            } // fin del if
        } // fin del bucle
        ItemVenta nuevo{p->get_codigo(), p->get_marca() + " " + p->get_modelo(), cant, p->get_precio(), p->get_costo(), redondear2(cant * p->get_precio())}; // crea la linea
        carrito.push_back(nuevo); // la agrega al carrito
        cout << Color::VERDE << "Anadido: " << p->get_modelo() << " x" << cant << " | Subtotal: " << dinero(nuevo.subtotal) << Color::RESET << "\n"; // confirma
    } // fin de la funcion

    void quitar_del_carrito(list<ItemVenta>& carrito) { // quita una linea del carrito
        if (carrito.empty()) { // si esta vacio
            cout << "El carrito esta vacio.\n"; // avisa
            return; // termina
        } // fin del if
        mostrar_carrito(carrito); // muestra el carrito
        string cod = a_mayuscula(leer_texto("Codigo a quitar: ")); // lee el codigo
        for (auto it = carrito.begin(); it != carrito.end(); ++it) { // recorre el carrito
            if (it->codigo == cod) { // si coincide
                carrito.erase(it); // quita la linea
                cout << Color::VERDE << "Producto quitado del carrito." << Color::RESET << "\n"; // confirma
                return; // termina
            } // fin del if
        } // fin del bucle
        cout << Color::ROJO << "Ese codigo no esta en el carrito." << Color::RESET << "\n"; // avisa
    } // fin de la funcion

    void linea_monto(const string& etiqueta, double valor) { // imprime una linea de totales del ticket
        cout << right << setw(48) << etiqueta << setw(16) << dinero(valor) << "\n"; // etiqueta y monto alineados
    } // fin de la funcion

    void imprimir_ticket(const Venta& v) { // imprime el comprobante
        limpiar(); // limpia la pantalla
        cout << "================================================================\n"; // linea superior
        cout << centrar(Config::NOMBRE_EMPRESA, 64) << "\n"; // nombre de la empresa
        cout << centrar("R.U.C. No. " + Config::RUC_EMPRESA, 64) << "\n"; // ruc de la empresa
        cout << centrar(Config::DIRECCION_EMPRESA, 64) << "\n"; // direccion de la empresa
        cout << "================================================================\n"; // linea separadora
        cout << left << setw(20) << "COMPROBANTE:" << v.get_tipo_comprobante() << " ELECTRONICA\n"; // tipo de comprobante
        cout << left << setw(20) << "NUMERO DE SERIE:" << v.get_id_transaccion() << "\n"; // numero de serie
        cout << left << setw(20) << "FECHA DE EMISION:" << v.get_fecha_transaccion().texto() << "\n"; // fecha
        cout << left << setw(20) << "CAJERO EN TURNO:" << v.get_empleado().nombre_completo() << " (" << v.get_empleado().get_codigo_empleado() << ")\n"; // cajero
        cout << "----------------------------------------------------------------\n"; // linea separadora
        cout << "ADQUIRIENTE: " << v.get_cliente().nombre_completo() << "\n"; // cliente
        cout << v.get_cliente().get_tipo_documento() << ": " << v.get_cliente().get_numero_documento() << "\n"; // documento del cliente
        cout << "DIRECCION: " << v.get_cliente().get_direccion() << "\n"; // direccion del cliente
        cout << "================================================================\n"; // linea separadora
        cout << left << setw(5) << "CANT" << setw(33) << "DESCRIPCION" << right << setw(12) << "P.UNIT" << setw(14) << "IMPORTE" << "\n"; // encabezado de la tabla
        cout << "----------------------------------------------------------------\n"; // linea separadora
        for (const auto& item : v.get_items()) { // recorre los items
            string desc = item.descripcion; // descripcion del item
            if (desc.length() > 30) desc = desc.substr(0, 29) + "."; // recorta descripciones largas
            cout << left << setw(5) << item.cantidad << setw(33) << desc // cantidad y descripcion
                 << right << setw(12) << dinero(item.precio_unitario) << setw(14) << dinero(item.subtotal) << "\n"; // precio e importe
        } // fin del bucle
        double total = v.get_monto_total(); // total cobrado
        double gravada = redondear2(total / (1.0 + Config::IGV)); // base imponible
        double igv = redondear2(total - gravada); // monto del IGV
        cout << "================================================================\n"; // linea separadora
        linea_monto("SUBTOTAL:", v.get_subtotal_bruto()); // subtotal antes del descuento
        if (v.get_descuento_monto() > 0) linea_monto("DESCUENTO (" + to_string((int)v.get_descuento_pct()) + "%):", -v.get_descuento_monto()); // descuento aplicado
        linea_monto("OP. GRAVADA:", gravada); // base imponible
        linea_monto("I.G.V. (18%):", igv); // igv
        linea_monto("IMPORTE TOTAL:", total); // total final
        cout << "----------------------------------------------------------------\n"; // linea separadora
        cout << left << setw(20) << "MEDIO DE PAGO:" << v.get_medio_pago() << "\n"; // medio de pago
        if (v.get_medio_pago() == "Efectivo") { // si pago en efectivo
            linea_monto("RECIBIDO:", v.get_monto_pagado()); // monto recibido
            linea_monto("VUELTO:", v.get_vuelto()); // vuelto entregado
        } // fin del if
        cout << left; // restaura la alineacion
        cout << "================================================================\n"; // linea separadora
        cout << centrar("GRACIAS POR SU COMPRA EN TECH STORE", 64) << "\n"; // agradecimiento
        cout << centrar("Conserve este comprobante para validar la garantia tecnica.", 64) << "\n"; // aviso de garantia
        cout << "================================================================\n"; // linea final
    } // fin de la funcion

    void registrar_venta() { // modulo de venta
        limpiar(); // limpia la pantalla
        escribir("=== MODULO DE VENTA / PUNTO DE FACTURACION ===\n", 15); // titulo
        cout << "Cajero a cargo: " << usuario().nombre_completo() << "\n\n"; // cajero actual

        string tipo_comp; // tipo de comprobante elegido
        while (true) { // repite hasta elegir una opcion valida
            string op = a_minuscula(leer_texto("Tipo de Comprobante (1. Boleta / 2. Factura): ")); // lee la opcion
            if (op == "1" || op == "boleta") { tipo_comp = "BOLETA"; break; } // eligio boleta
            if (op == "2" || op == "factura") { tipo_comp = "FACTURA"; break; } // eligio factura
            cout << Color::ROJO << "Opcion no valida." << Color::RESET << "\n"; // avisa el error
        } // fin del bucle

        cout << "\n--- Datos del Cliente ---\n"; // titulo de la seccion
        string tipo_doc = (tipo_comp == "FACTURA") ? "RUC" : "DNI"; // tipo de documento segun comprobante
        string num_doc = leer_documento(tipo_doc); // lee y valida el documento
        Cliente* cli = tienda.buscar_cliente(num_doc); // busca si ya esta registrado
        if (cli != nullptr) { // si ya existe
            cout << Color::VERDE << "Cliente registrado: " << cli->nombre_completo() << " [" << cli->get_categoria() << "]" << Color::RESET << "\n"; // lo muestra
        } else { // si es un cliente nuevo
            string nombre = leer_texto_no_vacio((tipo_comp == "FACTURA") ? "Razon Social: " : "Nombre(s): "); // nombre o razon social
            string paterno = "", materno = ""; // apellidos vacios para facturas
            if (tipo_comp == "BOLETA") { // las boletas piden apellidos
                paterno = leer_texto_no_vacio("Apellido Paterno: "); // apellido paterno
                materno = leer_texto("Apellido Materno: "); // apellido materno
            } // fin del if
            string direccion = leer_texto_no_vacio("Direccion: "); // direccion
            string telefono = leer_texto("Telefono de contacto: "); // telefono
            cli = tienda.registrar_cliente(Cliente(tipo_doc, num_doc, materno, paterno, nombre, telefono, direccion, "General")); // registra al cliente
        } // fin del if

        list<ItemVenta> carrito; // productos de la venta
        mostrar_catalogo(); // muestra el catalogo
        cout << "\nEscribe un codigo para agregar. Comandos: buscar, ver, quitar, fin, cancelar\n"; // instrucciones
        bool cancelada = false; // indica si se cancelo la venta
        while (true) { // repite hasta terminar el carrito
            string entrada = leer_texto("Codigo o comando: "); // lee la entrada
            string cmd = a_minuscula(entrada); // comando en minusculas
            if (cmd == "fin") break; // termina el carrito
            if (cmd == "cancelar") { cancelada = true; break; } // cancela la venta
            if (cmd == "ver") { mostrar_carrito(carrito); continue; } // muestra el carrito
            if (cmd == "buscar") { buscar_y_mostrar(); continue; } // busca productos
            if (cmd == "quitar") { quitar_del_carrito(carrito); continue; } // quita un producto
            if (cmd.empty()) continue; // ignora entradas vacias
            agregar_al_carrito(carrito, a_mayuscula(entrada)); // agrega el producto
        } // fin del bucle

        if (cancelada || carrito.empty()) { // si no hay nada que facturar
            cout << "\nVenta descartada. El stock no fue modificado.\n"; // avisa
            pausa(); // espera al usuario
            return; // termina
        } // fin del if

        mostrar_carrito(carrito); // muestra el carrito final
        double bruto = subtotal_items(carrito); // total antes del descuento
        double desc_pct = 0.0; // porcentaje de descuento
        if (cli->get_categoria() == "Frecuente") { // si es cliente frecuente
            desc_pct = Config::DESCUENTO_FRECUENTE; // aplica descuento automatico
            cout << Color::VERDE << "Cliente frecuente: descuento automatico de " << desc_pct << "%." << Color::RESET << "\n"; // avisa
        } else { // si es cliente general
            desc_pct = leer_numero_rango("Descuento % (0 a " + to_string((int)Config::DESCUENTO_MAXIMO) + ", 0 si no aplica): ", 0.0, Config::DESCUENTO_MAXIMO); // lee el descuento
        } // fin del if
        double total = redondear2(bruto - calcular_descuento(bruto, desc_pct)); // total a cobrar
        cout << "TOTAL A COBRAR: " << dinero(total) << "\n"; // muestra el total

        cout << "\nMedio de pago:\n1. Efectivo\n2. Tarjeta\n3. Yape\n4. Plin\n"; // opciones de pago
        int mp = leer_entero_rango("Elige medio de pago: ", 1, 4); // lee la opcion
        string medio = (mp == 1) ? "Efectivo" : (mp == 2) ? "Tarjeta" : (mp == 3) ? "Yape" : "Plin"; // nombre del medio
        double pagado = total; // monto pagado
        if (mp == 1) { // si es efectivo
            while (true) { // repite hasta recibir suficiente dinero
                pagado = leer_numero_positivo("Monto recibido: S/ "); // lee el monto
                if (pagado >= total - 0.0001) break; // acepta si cubre el total
                cout << Color::ROJO << "El monto recibido no cubre el total." << Color::RESET << "\n"; // avisa el error
            } // fin del bucle
        } // fin del if

        cout << "\nFecha de emision del comprobante:\n"; // titulo de la fecha
        Fecha fecha = leer_fecha("Fecha (dd/mm/aaaa, Enter = hoy): "); // lee la fecha

        if (!confirmar("\nConfirmar venta")) { // pide confirmacion final
            cout << "Venta descartada. El stock no fue modificado.\n"; // avisa
            pausa(); // espera al usuario
            return; // termina
        } // fin del if

        const Venta* v = tienda.finalizar_venta(tipo_comp, *cli, usuario(), carrito, medio, desc_pct, pagado, fecha); // registra la venta
        if (v == nullptr) { // si el stock cambio y ya no alcanza
            cout << Color::ROJO << "No se pudo completar la venta por falta de stock." << Color::RESET << "\n"; // avisa
            pausa(); // espera al usuario
            return; // termina
        } // fin del if
        puntos("Generando comprobante fiscal"); // animacion
        imprimir_ticket(*v); // imprime el comprobante
        pausa(); // espera al usuario
    } // fin de la funcion

    void registrar_compra() { // modulo de compras a proveedores
        limpiar(); // limpia la pantalla
        escribir("=== MODULO DE COMPRAS (ABASTECIMIENTO B2B) ===\n", 15); // titulo
        cout << "Responsable de recepcion: " << usuario().nombre_completo() << "\n\n"; // responsable

        cout << "--- DISTRIBUIDORES MAYORISTAS ASOCIADOS ---\n"; // titulo de la lista
        int idx = 1; // numero de opcion
        for (const auto& p : tienda.get_proveedores()) { // recorre los proveedores
            cout << idx++ << ". RUC: " << p.get_ruc() << " | " << p.get_razon_social() << " | Contacto: " << p.get_contacto() << "\n"; // muestra el proveedor
        } // fin del bucle
        cout << idx << ". Registrar otro proveedor\n"; // opcion de nuevo proveedor
        int opc = leer_entero_rango("Seleccione el proveedor: ", 1, idx); // lee la opcion
        Proveedor* prov = tienda.proveedor_por_indice(opc); // busca el proveedor elegido
        if (prov == nullptr) { // si eligio registrar uno nuevo
            cout << "\n--- Registro de Nuevo Proveedor ---\n"; // titulo
            string ruc = leer_documento("RUC"); // lee y valida el ruc
            prov = tienda.buscar_proveedor(ruc); // revisa si ya existe
            if (prov != nullptr) { // si ya existia
                cout << Color::AMARILLO << "Ese RUC ya estaba registrado: " << prov->get_razon_social() << Color::RESET << "\n"; // avisa
            } else { // si es nuevo
                string razon = leer_texto_no_vacio("Razon Social: "); // razon social
                string cont = leer_texto_no_vacio("Contacto comercial: "); // contacto
                string tel = leer_texto("Telefono: "); // telefono
                string dir = leer_texto_no_vacio("Direccion fiscal: "); // direccion
                prov = tienda.agregar_proveedor(Proveedor(ruc, razon, cont, tel, dir, true)); // registra el proveedor
            } // fin del if
        } // fin del if

        string num_factura = leer_texto_no_vacio("\nNumero de Factura del Proveedor (ej. F102-4521): "); // numero de factura
        cout << "Fecha de ingreso al almacen:\n"; // titulo de la fecha
        Fecha fecha = leer_fecha("Fecha (dd/mm/aaaa, Enter = hoy): "); // lee la fecha

        list<ItemCompra> articulos; // items de la compra
        vector<unique_ptr<Producto>> nuevos; // productos nuevos pendientes
        bool cancelada = false; // indica si se cancelo la compra
        while (true) { // repite hasta finalizar
            cout << "\n--- Operacion de Almacen ---\n"; // titulo
            cout << "1. Reabastecer stock de un producto existente\n"; // opcion 1
            cout << "2. Dar de alta una Laptop nueva en el catalogo\n"; // opcion 2
            cout << "3. Dar de alta un Celular nuevo en el catalogo\n"; // opcion 3
            cout << "4. Finalizar ingreso de mercaderia\n"; // opcion 4
            cout << "5. Cancelar compra\n"; // opcion 5
            int accion = leer_entero_rango("Elige opcion: ", 1, 5); // lee la opcion
            if (accion == 4) break; // finaliza
            if (accion == 5) { cancelada = true; break; } // cancela

            if (accion == 1) { // reabastecer
                mostrar_catalogo(); // muestra el catalogo
                string cod = a_mayuscula(leer_texto("Codigo del producto a reabastecer: ")); // lee el codigo
                Producto* p = tienda.buscar_producto(cod); // busca el producto
                if (p == nullptr) { // si no existe
                    cout << Color::ROJO << "Codigo no existe o esta dado de baja." << Color::RESET << "\n"; // avisa
                    continue; // vuelve al menu
                } // fin del if
                int cant = leer_entero_positivo("Cantidad recibida: "); // lee la cantidad
                double costo = leer_numero_positivo("Costo unitario de compra: S/ "); // lee el costo
                if (costo >= p->get_precio()) cout << Color::AMARILLO << "Aviso: el costo iguala o supera el precio de venta actual." << Color::RESET << "\n"; // advierte
                articulos.push_back(ItemCompra{p->get_codigo(), p->get_marca() + " " + p->get_modelo(), cant, costo, redondear2(cant * costo)}); // agrega el item
                cout << Color::VERDE << "Item agregado. El stock se actualiza al finalizar la compra." << Color::RESET << "\n"; // confirma
            } else if (accion == 2) { // alta de laptop
                cout << "\n--- Alta de Nueva Laptop ---\n"; // titulo
                string cod = tienda.nuevo_codigo_laptop(); // codigo nuevo
                string marca = leer_texto_no_vacio("Marca: "); // marca
                string modelo = leer_texto_no_vacio("Modelo: "); // modelo
                string cpu = leer_texto_no_vacio("Procesador: "); // procesador
                int ram = leer_entero_positivo("RAM (GB): "); // ram
                string disco = leer_texto_no_vacio("Almacenamiento (ej. 1TB SSD): "); // disco
                int cant = leer_entero_positivo("Cantidad comprada: "); // cantidad
                double costo = leer_numero_positivo("Costo unitario de compra: S/ "); // costo
                double p_venta = leer_numero_positivo("Precio de venta al publico: S/ "); // precio de venta
                if (p_venta <= costo) cout << Color::AMARILLO << "Aviso: el precio de venta no supera el costo." << Color::RESET << "\n"; // advierte
                nuevos.push_back(make_unique<Laptop>(cod, marca, modelo, p_venta, costo, 0, cpu, ram, disco)); // crea el producto con stock 0
                articulos.push_back(ItemCompra{cod, marca + " " + modelo, cant, costo, redondear2(cant * costo)}); // agrega el item
                cout << Color::VERDE << "Laptop preparada con codigo [" << cod << "]. Se incorpora al finalizar la compra." << Color::RESET << "\n"; // confirma
            } else if (accion == 3) { // alta de celular
                cout << "\n--- Alta de Nuevo Celular ---\n"; // titulo
                string cod = tienda.nuevo_codigo_celular(); // codigo nuevo
                string marca = leer_texto_no_vacio("Marca: "); // marca
                string modelo = leer_texto_no_vacio("Modelo: "); // modelo
                int rom = leer_entero_positivo("Almacenamiento (GB): "); // memoria interna
                string cam = leer_texto_no_vacio("Camara (ej. 64MP): "); // camara
                int bat = leer_entero_positivo("Bateria (mAh): "); // bateria
                int cant = leer_entero_positivo("Cantidad comprada: "); // cantidad
                double costo = leer_numero_positivo("Costo unitario de compra: S/ "); // costo
                double p_venta = leer_numero_positivo("Precio de venta al publico: S/ "); // precio de venta
                if (p_venta <= costo) cout << Color::AMARILLO << "Aviso: el precio de venta no supera el costo." << Color::RESET << "\n"; // advierte
                nuevos.push_back(make_unique<Celular>(cod, marca, modelo, p_venta, costo, 0, rom, cam, bat)); // crea el producto con stock 0
                articulos.push_back(ItemCompra{cod, marca + " " + modelo, cant, costo, redondear2(cant * costo)}); // agrega el item
                cout << Color::VERDE << "Celular preparado con codigo [" << cod << "]. Se incorpora al finalizar la compra." << Color::RESET << "\n"; // confirma
            } // fin del if
        } // fin del bucle

        if (cancelada || articulos.empty()) { // si no hay nada que registrar
            cout << "\nCompra cancelada. No se modifico el inventario.\n"; // avisa
            pausa(); // espera al usuario
            return; // termina
        } // fin del if

        const Compra* c = tienda.registrar_compra(*prov, usuario(), articulos, nuevos, num_factura, fecha); // registra la compra
        puntos("Actualizando kardex e inventario de tienda"); // animacion
        cout << Color::VERDE << "Entrada de mercaderia " << c->get_id_transaccion() << " registrada por " << dinero(c->get_monto_total()) << "." << Color::RESET << "\n"; // confirma
        pausa(); // espera al usuario
    } // fin de la funcion

    void ver_ventas() { // historial de ventas
        limpiar(); // limpia la pantalla
        escribir("=== HISTORIAL DE TRANSACCIONES / VENTAS ===\n", 15); // titulo
        if (tienda.get_ventas().empty()) cout << "\nNo hay ventas registradas.\n"; // avisa si no hay ventas
        for (const Venta& v : tienda.get_ventas()) { // recorre las ventas
            bool anulada = v.get_estado() == Config::ESTADO_ANULADA; // indica si esta anulada
            cout << "\n----------------------------------------------------------------\n"; // linea separadora
            cout << "Comprobante: " << v.get_tipo_comprobante() << " No. " << v.get_id_transaccion() << " | Fecha: " << v.get_fecha_transaccion().texto(); // comprobante y fecha
            if (anulada) cout << Color::ROJO << " | ANULADA" << Color::RESET; // marca las anuladas
            cout << "\nCliente: " << v.get_cliente().nombre_completo() << " (" << v.get_cliente().get_tipo_documento() << ": " << v.get_cliente().get_numero_documento() << ")\n"; // cliente
            cout << "Cajero: " << v.get_empleado().nombre_completo() << " | Pago: " << v.get_medio_pago() << "\n"; // cajero y pago
            cout << "Articulos facturados:\n"; // titulo de items
            for (const auto& item : v.get_items()) { // recorre los items
                cout << "  * [" << item.codigo << "] " << item.descripcion << " x" << item.cantidad // codigo, descripcion y cantidad
                     << " @ " << dinero(item.precio_unitario) << " = " << dinero(item.subtotal) << "\n"; // precio y subtotal
            } // fin del bucle
            if (v.get_descuento_monto() > 0) cout << "Descuento: " << dinero(v.get_descuento_monto()) << " (" << v.get_descuento_pct() << "%)\n"; // muestra el descuento
            cout << "TOTAL: " << dinero(v.get_monto_total()) << "\n"; // total
        } // fin del bucle
        pausa(); // espera al usuario
    } // fin de la funcion

    void ver_compras() { // historial de compras
        limpiar(); // limpia la pantalla
        escribir("=== HISTORIAL DE COMPRAS (ENTRADAS DE ALMACEN) ===\n", 15); // titulo
        if (tienda.get_compras().empty()) cout << "\nNo hay compras registradas.\n"; // avisa si no hay compras
        for (const Compra& c : tienda.get_compras()) { // recorre las compras
            cout << "\n----------------------------------------------------------------\n"; // linea separadora
            cout << "Registro Interno: " << c.get_id_transaccion() << " | Fecha: " << c.get_fecha_transaccion().texto() << "\n"; // registro y fecha
            cout << "Proveedor: " << c.get_proveedor().get_razon_social() << " (RUC: " << c.get_proveedor().get_ruc() << ")\n"; // proveedor
            cout << "Factura Proveedor: " << c.get_numero_factura_proveedor() << "\n"; // factura
            cout << "Recibido por: " << c.get_empleado().nombre_completo() << "\n"; // responsable
            cout << "Mercaderia ingresada:\n"; // titulo de items
            for (const auto& item : c.get_items_comprados()) { // recorre los items
                cout << "  + [" << item.codigo << "] " << item.descripcion << " x" << item.cantidad // codigo, descripcion y cantidad
                     << " @ Costo " << dinero(item.costo_unitario) << " = " << dinero(item.subtotal) << "\n"; // costo y subtotal
            } // fin del bucle
            cout << "TOTAL INVERTIDO: " << dinero(c.get_monto_total()) << "\n"; // total
        } // fin del bucle
        pausa(); // espera al usuario
    } // fin de la funcion

    void anular_venta() { // anula una venta y devuelve el stock
        limpiar(); // limpia la pantalla
        escribir("=== ANULACION DE VENTAS ===\n", 15); // titulo
        if (tienda.get_ventas().empty()) { // si no hay ventas
            cout << "\nNo hay ventas registradas.\n"; // avisa
            pausa(); // espera al usuario
            return; // termina
        } // fin del if
        for (const Venta& v : tienda.get_ventas()) { // lista las ventas
            cout << v.get_id_transaccion() << " | " << v.get_fecha_transaccion().texto() << " | " << dinero(v.get_monto_total()) << " | " << v.get_estado() << "\n"; // resumen de la venta
        } // fin del bucle
        string id = leer_texto_no_vacio("\nNumero de venta a anular (ej. B001-1001): "); // lee el numero
        if (!confirmar("Se devolvera el stock y la venta quedara anulada. Continuar")) { // pide confirmacion
            cout << "Operacion cancelada.\n"; // avisa
            pausa(); // espera al usuario
            return; // termina
        } // fin del if
        string error = tienda.anular_venta(id); // intenta anular
        if (error.empty()) cout << Color::VERDE << "Venta anulada y stock devuelto." << Color::RESET << "\n"; // exito
        else cout << Color::ROJO << error << Color::RESET << "\n"; // error
        pausa(); // espera al usuario
    } // fin de la funcion

    void editar_precio() { // cambia el precio de venta de un producto
        limpiar(); // limpia la pantalla
        mostrar_catalogo(); // muestra el catalogo
        string cod = a_mayuscula(leer_texto("\nCodigo del producto a modificar: ")); // lee el codigo
        Producto* p = tienda.buscar_producto(cod); // busca el producto
        if (p == nullptr) { // si no existe
            cout << Color::ROJO << "Codigo inexistente o dado de baja." << Color::RESET << "\n"; // avisa
            pausa(); // espera al usuario
            return; // termina
        } // fin del if
        cout << "Precio actual: " << dinero(p->get_precio()) << " | Costo: " << dinero(p->get_costo()) << "\n"; // muestra precio y costo
        double nuevo = leer_numero_positivo("Nuevo precio de venta: S/ "); // lee el nuevo precio
        if (nuevo <= p->get_costo()) cout << Color::AMARILLO << "Aviso: el precio no supera el costo." << Color::RESET << "\n"; // advierte
        p->set_precio(nuevo); // aplica el precio
        tienda.guardar(); // guarda los cambios
        cout << Color::VERDE << "Precio actualizado." << Color::RESET << "\n"; // confirma
        pausa(); // espera al usuario
    } // fin de la funcion

    void dar_de_baja_producto() { // retira un producto del catalogo
        limpiar(); // limpia la pantalla
        mostrar_catalogo(); // muestra el catalogo
        string cod = a_mayuscula(leer_texto("\nCodigo del producto a dar de baja: ")); // lee el codigo
        Producto* p = tienda.buscar_producto(cod); // busca el producto
        if (p == nullptr) { // si no existe
            cout << Color::ROJO << "Codigo inexistente o ya dado de baja." << Color::RESET << "\n"; // avisa
            pausa(); // espera al usuario
            return; // termina
        } // fin del if
        if (p->get_stock() > 0) cout << Color::AMARILLO << "Aviso: el producto aun tiene " << p->get_stock() << " unidades en stock." << Color::RESET << "\n"; // advierte
        if (confirmar("Dar de baja " + p->get_marca() + " " + p->get_modelo())) { // pide confirmacion
            p->dar_de_baja(); // lo retira del catalogo
            tienda.guardar(); // guarda los cambios
            cout << Color::VERDE << "Producto dado de baja. El historial se conserva." << Color::RESET << "\n"; // confirma
        } else { // si no confirma
            cout << "Operacion cancelada.\n"; // avisa
        } // fin del if
        pausa(); // espera al usuario
    } // fin de la funcion

    void leer_rango(Fecha& desde, Fecha& hasta) { // lee un rango de fechas valido
        while (true) { // repite hasta recibir un rango valido
            desde = leer_fecha("Fecha inicial (dd/mm/aaaa, Enter = hoy): "); // fecha inicial
            hasta = leer_fecha("Fecha final (dd/mm/aaaa, Enter = hoy): "); // fecha final
            if (desde <= hasta) return; // acepta si el orden es correcto
            cout << Color::ROJO << "La fecha inicial no puede ser mayor que la final." << Color::RESET << "\n"; // avisa el error
        } // fin del bucle
    } // fin de la funcion

    void listar_ventas_rango(const Fecha& desde, const Fecha& hasta) { // lista las ventas de un periodo
        vector<const Venta*> lista = tienda.ventas_en_rango(desde, hasta); // ventas del periodo
        if (lista.empty()) { // si no hay ventas
            cout << "No hay ventas completadas en ese periodo.\n"; // avisa
            return; // termina
        } // fin del if
        double total = 0; // total vendido
        for (const Venta* v : lista) { // recorre las ventas
            cout << v->get_id_transaccion() << " | " << v->get_fecha_transaccion().texto() << " | " // numero y fecha
                 << left << setw(28) << v->get_cliente().nombre_completo().substr(0, 27) << " | " << dinero(v->get_monto_total()) << "\n"; // cliente y monto
            total += v->get_monto_total(); // suma al total
        } // fin del bucle
        cout << "Ventas: " << lista.size() << " | Total vendido: " << dinero(redondear2(total)) << "\n"; // muestra el resumen
    } // fin de la funcion

    void menu_reportes() { // submenu de reportes
        int op = 0; // opcion elegida
        while (op != 7) { // repite hasta volver
            limpiar(); // limpia la pantalla
            cout << Color::CIAN << "=============== REPORTES ===============" << Color::RESET << "\n"; // titulo
            cout << "1. Ventas del dia\n2. Ventas por rango de fechas\n3. Productos mas vendidos\n"; // opciones 1 a 3
            cout << "4. Ventas por cajero\n5. Utilidad estimada por rango\n6. Valor del inventario\n7. Volver\n"; // opciones 4 a 7
            op = leer_entero_rango("Elige una opcion: ", 1, 7); // lee la opcion
            if (op == 1) { // ventas del dia
                cout << "\n"; // linea en blanco
                listar_ventas_rango(Fecha::hoy(), Fecha::hoy()); // lista las de hoy
                pausa(); // espera al usuario
            } else if (op == 2) { // ventas por rango
                Fecha d, h; // fechas del rango
                leer_rango(d, h); // lee el rango
                cout << "\n"; // linea en blanco
                listar_ventas_rango(d, h); // lista las ventas
                pausa(); // espera al usuario
            } else if (op == 3) { // productos mas vendidos
                auto ranking = tienda.ranking_productos(); // calcula el ranking
                cout << "\n"; // linea en blanco
                if (ranking.empty()) cout << "Aun no hay ventas.\n"; // avisa si no hay ventas
                for (size_t i = 0; i < ranking.size() && i < 10; i++) cout << (i + 1) << ". " << ranking[i].first << " - " << ranking[i].second << " unidades\n"; // muestra el top 10
                pausa(); // espera al usuario
            } else if (op == 4) { // ventas por cajero
                auto por_cajero = tienda.ventas_por_cajero(); // calcula las ventas por cajero
                cout << "\n"; // linea en blanco
                if (por_cajero.empty()) cout << "Aun no hay ventas.\n"; // avisa si no hay ventas
                for (const auto& par : por_cajero) cout << par.first << " | " << par.second.first << " ventas | " << dinero(redondear2(par.second.second)) << "\n"; // muestra cada cajero
                pausa(); // espera al usuario
            } else if (op == 5) { // utilidad estimada
                Fecha d, h; // fechas del rango
                leer_rango(d, h); // lee el rango
                cout << "\nUtilidad estimada (venta sin IGV menos costo): " << dinero(tienda.utilidad_en_rango(d, h)) << "\n"; // muestra la utilidad
                pausa(); // espera al usuario
            } else if (op == 6) { // valor del inventario
                cout << "\nValor del inventario al costo: " << dinero(tienda.valor_inventario_costo()) << "\n"; // valor al costo
                cout << "Valor del inventario a precio de venta: " << dinero(tienda.valor_inventario_venta()) << "\n"; // valor a precio de venta
                pausa(); // espera al usuario
            } // fin del if
        } // fin del bucle
    } // fin de la funcion

    void menu_empleados() { // submenu de gestion de empleados
        int op = 0; // opcion elegida
        while (op != 4) { // repite hasta volver
            limpiar(); // limpia la pantalla
            cout << Color::CIAN << "========== GESTION DE EMPLEADOS ==========" << Color::RESET << "\n"; // titulo
            cout << "1. Listar empleados\n2. Registrar empleado\n3. Activar / desactivar empleado\n4. Volver\n"; // opciones
            op = leer_entero_rango("Elige una opcion: ", 1, 4); // lee la opcion
            if (op == 1) { // listar
                cout << "\n"; // linea en blanco
                for (const auto& e : tienda.get_empleados()) { // recorre los empleados
                    cout << e.get_codigo_empleado() << " | " << e.nombre_completo() << " " << e.get_materno() << " | " << e.get_cargo() // codigo, nombre y cargo
                         << " | " << e.get_rol() << " | Ingreso: " << e.get_fecha_ingreso().texto() // rol e ingreso
                         << " | " << dinero(e.get_sueldo()) << " | " << (e.get_estado() ? "Activo" : "Inactivo") << "\n"; // sueldo y estado
                } // fin del bucle
                pausa(); // espera al usuario
            } else if (op == 2) { // registrar
                cout << "\n--- Registro de Empleado ---\n"; // titulo
                string dni = leer_documento("DNI"); // lee el dni
                if (tienda.existe_empleado_documento(dni)) { // si ya existe
                    cout << Color::ROJO << "Ya existe un empleado con ese DNI." << Color::RESET << "\n"; // avisa
                    pausa(); // espera al usuario
                    continue; // vuelve al submenu
                } // fin del if
                string nombre = leer_texto_no_vacio("Nombre(s): "); // nombres
                string paterno = leer_texto_no_vacio("Apellido Paterno: "); // apellido paterno
                string materno = leer_texto("Apellido Materno: "); // apellido materno
                string tel = leer_texto("Telefono: "); // telefono
                string dir = leer_texto_no_vacio("Direccion: "); // direccion
                string cargo = leer_texto_no_vacio("Cargo: "); // cargo
                int rol = leer_entero_rango("Rol (1. Administrador / 2. Cajero): ", 1, 2); // rol
                double sueldo = leer_numero_positivo("Sueldo: S/ "); // sueldo
                string clave; // clave inicial
                while (true) { // repite hasta recibir una clave valida
                    clave = leer_texto("Clave inicial (minimo 4 caracteres): "); // lee la clave
                    if (clave.size() >= 4) break; // acepta si es suficientemente larga
                    cout << Color::ROJO << "La clave es muy corta." << Color::RESET << "\n"; // avisa el error
                } // fin del bucle
                string codigo = tienda.nuevo_codigo_empleado(); // codigo nuevo
                tienda.agregar_empleado(Empleado("DNI", dni, materno, paterno, nombre, tel, dir, codigo, cargo, // datos personales y cargo
                                                 rol == 1 ? Config::ROL_ADMIN : Config::ROL_CAJERO, Fecha::hoy(), sueldo, true, hash_clave(clave))); // rol, ingreso, sueldo y clave
                cout << Color::VERDE << "Empleado registrado con codigo " << codigo << "." << Color::RESET << "\n"; // confirma
                pausa(); // espera al usuario
            } else if (op == 3) { // activar o desactivar
                string codigo = a_mayuscula(leer_texto("\nCodigo del empleado: ")); // lee el codigo
                Empleado* e = tienda.buscar_empleado(codigo); // busca al empleado
                if (e == nullptr) { // si no existe
                    cout << Color::ROJO << "Empleado no encontrado." << Color::RESET << "\n"; // avisa
                } else if (e->get_estado() && e->get_codigo_empleado() == codigo_activo) { // si intenta desactivarse a si mismo
                    cout << Color::ROJO << "No puedes desactivar tu propio usuario." << Color::RESET << "\n"; // avisa
                } else if (e->get_estado() && e->es_admin() && tienda.administradores_activos() <= 1) { // si es el ultimo admin
                    cout << Color::ROJO << "No se puede desactivar al ultimo administrador." << Color::RESET << "\n"; // avisa
                } else { // si es valido
                    e->set_estado(!e->get_estado()); // invierte el estado
                    tienda.guardar(); // guarda los cambios
                    cout << Color::VERDE << "Empleado ahora " << (e->get_estado() ? "ACTIVO" : "INACTIVO") << "." << Color::RESET << "\n"; // confirma
                } // fin del if
                pausa(); // espera al usuario
            } // fin del if
        } // fin del bucle
    } // fin de la funcion

    void cambiar_clave() { // cambia la clave del usuario actual
        limpiar(); // limpia la pantalla
        Empleado* e = tienda.buscar_empleado(codigo_activo); // empleado actual
        string actual = leer_texto("Clave actual: "); // lee la clave actual
        if (!e->clave_correcta(actual)) { // si no coincide
            cout << Color::ROJO << "Clave incorrecta." << Color::RESET << "\n"; // avisa
            pausa(); // espera al usuario
            return; // termina
        } // fin del if
        string nueva = leer_texto("Nueva clave (minimo 4 caracteres): "); // lee la nueva clave
        if (nueva.size() < 4) { // si es muy corta
            cout << Color::ROJO << "La clave es muy corta." << Color::RESET << "\n"; // avisa
            pausa(); // espera al usuario
            return; // termina
        } // fin del if
        if (leer_texto("Repite la nueva clave: ") != nueva) { // si la repeticion no coincide
            cout << Color::ROJO << "Las claves no coinciden." << Color::RESET << "\n"; // avisa
            pausa(); // espera al usuario
            return; // termina
        } // fin del if
        e->set_clave(nueva); // aplica la nueva clave
        tienda.guardar(); // guarda los cambios
        cout << Color::VERDE << "Clave actualizada." << Color::RESET << "\n"; // confirma
        pausa(); // espera al usuario
    } // fin de la funcion

    void mostrar_menu() { // dibuja el menu principal
        limpiar(); // limpia la pantalla
        cout << Color::CIAN << "========================================================\n"; // linea superior
        cout << "             SISTEMA DE GESTION TECH STORE              \n"; // titulo
        cout << "========================================================" << Color::RESET << "\n"; // linea inferior
        cout << Color::AMARILLO << "Usuario activo: " << usuario().nombre_completo() << " (" << usuario().get_cargo() << ") - " << usuario().get_rol() << Color::RESET << "\n"; // usuario actual
        int bajos = tienda.cantidad_stock_bajo(); // productos con stock bajo
        if (bajos > 0) cout << Color::ROJO << "ALERTA: " << bajos << " producto(s) con stock bajo." << Color::RESET << "\n"; // muestra la alerta
        cout << "--------------------------------------------------------\n"; // linea separadora
        cout << " 1. Registrar Venta (Facturacion con Ticket Fiscal)\n"; // opcion 1
        cout << " 2. Registrar Compra a Mayoristas [ADMIN]\n"; // opcion 2
        cout << " 3. Ver Catalogo e Inventario Actual\n"; // opcion 3
        cout << " 4. Buscar Producto por Marca o Modelo\n"; // opcion 4
        cout << " 5. Ver Historial de Ventas\n"; // opcion 5
        cout << " 6. Ver Historial de Compras de Almacen [ADMIN]\n"; // opcion 6
        cout << " 7. Anular Venta [ADMIN]\n"; // opcion 7
        cout << " 8. Editar Precio de Producto [ADMIN]\n"; // opcion 8
        cout << " 9. Dar de Baja un Producto [ADMIN]\n"; // opcion 9
        cout << "10. Reportes [ADMIN]\n"; // opcion 10
        cout << "11. Gestion de Empleados [ADMIN]\n"; // opcion 11
        cout << "12. Cambiar mi Clave\n"; // opcion 12
        cout << "13. Cambiar de Usuario / Cajero\n"; // opcion 13
        cout << "14. Salir\n"; // opcion 14
        cout << "\nElige una opcion: "; // prompt
    } // fin de la funcion

public: // interfaz publica
    Aplicacion() { preparar_consola(); } // activa los colores en Windows

    void ejecutar() { // bucle principal del programa
        cargando("Iniciando Sistema de Ventas Tech Store"); // animacion de inicio
        while (codigo_activo.empty()) iniciar_sesion(); // pide login hasta tener sesion
        int opcion = 0; // opcion elegida
        while (opcion != 14) { // repite hasta salir
            mostrar_menu(); // dibuja el menu
            string entrada = leer_texto(""); // lee la opcion
            try { // intenta convertir a numero
                opcion = stoi(entrada); // convierte la entrada
            } catch (...) { // si no es numero
                opcion = 0; // opcion invalida
            } // fin del try
            switch (opcion) { // ejecuta la opcion
                case 1: puntos("Abriendo terminal de venta"); registrar_venta(); break; // venta
                case 2: if (exigir_admin()) { puntos("Abriendo recepcion de mercaderia"); registrar_compra(); } break; // compra
                case 3: limpiar(); mostrar_catalogo(); pausa(); break; // catalogo
                case 4: limpiar(); buscar_y_mostrar(); pausa(); break; // busqueda
                case 5: puntos("Consultando kardex de ventas"); ver_ventas(); break; // historial de ventas
                case 6: if (exigir_admin()) { puntos("Consultando kardex de compras"); ver_compras(); } break; // historial de compras
                case 7: if (exigir_admin()) anular_venta(); break; // anular venta
                case 8: if (exigir_admin()) editar_precio(); break; // editar precio
                case 9: if (exigir_admin()) dar_de_baja_producto(); break; // dar de baja
                case 10: if (exigir_admin()) menu_reportes(); break; // reportes
                case 11: if (exigir_admin()) menu_empleados(); break; // empleados
                case 12: cambiar_clave(); break; // cambiar clave
                case 13: codigo_activo.clear(); while (codigo_activo.empty()) iniciar_sesion(); break; // cambiar de usuario
                case 14: escribir("Cerrando estacion de trabajo y caja. Hasta luego!\n", 25); break; // salir
                default: cout << Color::ROJO << "Opcion invalida" << Color::RESET << endl; esperar(800); break; // opcion invalida
            } // fin del switch
        } // fin del bucle
        tienda.guardar(); // guarda todo antes de salir
    } // fin de la funcion
}; // fin de la clase Aplicacion



int main() { // punto de entrada del programa
    Aplicacion app; // crea la aplicacion y carga los datos
    app.ejecutar(); // inicia el bucle principal
    return 0; // termina sin errores
} // fin de main