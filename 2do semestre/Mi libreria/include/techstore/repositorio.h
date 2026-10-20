#pragma once
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <techstore/dominio.h>
#include <techstore/abstracciones.h>

namespace techstore {
using namespace std;

struct DatosTienda {
    vector<unique_ptr<Producto>> inventario;
    list<Empleado> empleados;
    list<Proveedor> proveedores;
    list<Cliente> clientes;
    list<Venta> ventas;
    list<Compra> compras;
    map<string, int> contadores;
};

class IRepositorio {
public:
    virtual ~IRepositorio() = default;
    virtual bool cargar(DatosTienda& datos) = 0;
    virtual bool guardar(const DatosTienda& datos) = 0;
};

class RepositorioCSV : public IRepositorio {
    const FabricaProductos& fabrica;
    string carpeta;
public:
    explicit RepositorioCSV(const FabricaProductos& f, const string& carpeta_datos = "");
    bool guardar(const DatosTienda& d) override;
    bool cargar(DatosTienda& d) override;
};

} // namespace techstore