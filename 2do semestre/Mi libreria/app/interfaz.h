#pragma once
#include <functional>
#include <iomanip>
#include <techstore/servicios.h>
#include <techstore/config.h>
#include "consola.h"

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
