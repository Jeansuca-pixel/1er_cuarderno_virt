#include <techstore/datos_iniciales.h>

namespace techstore {

void cargar_datos_iniciales(DatosTienda& d, const IHasheadorClave& h) {
    d.empleados.push_back(Empleado("DNI", "72341122", "Sucari", "Sucari", "Jean Andre", "951234567", "Av. Floral 120",
        "EMP01", "Administrador y Cajero", Config::ROL_ADMIN, Fecha(15, 3, 2024), 1500.0, true, h.hash("admin123")));
    d.empleados.push_back(Empleado("DNI", "70984433", "Quillimamani", "Huamantuma", "Franklin", "952987654", "Jr. Puno 340",
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

} // namespace techstore