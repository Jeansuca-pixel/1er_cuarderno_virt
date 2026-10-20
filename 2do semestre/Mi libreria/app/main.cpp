#include <techstore/datos_iniciales.h>
#include "interfaz.h"

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